#pragma once

#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include "../entity.hpp"
#include "../box/dynamic-box.hpp"
#include "../player/player.hpp"
#include "../blocks/Block.hpp"

namespace mario::entity {
    #define FILE_PATH "../../asset/sprites/"

    enum class EnemyBehavior {
        Patrol,
        Chase,
        Piranha
    };

    enum class EnemyType {
        Goomba,
        Koopa,
        Piranha,
        Lakitu,
        Ball // released from Lakitu
    };

    class Enemy : public Entity {
    private:
        EnemyBehavior behavior;
        float detectionRange;
        bool isActive;
        bool isDeadFlag = false;
        
        bool isPlayerDeadWhenCollisionLF = true;
        bool isPlayerDeadWhenCollisionT = false;
        bool isCheckCollisionWithBlock = true;
        bool isCheckCollisionWithEnemy = true;
        bool isCheckCollisionWithPlayer = true;

        bool hasNetworkAuthority = false;  // Does this client control this enemy?
        bool isNetworkControlled = false;  // Is this enemy controlled by network?
        bool isNetworked = false;
        bool networkNotified = false;
        int networkId = -1;

        sf::Vector2f networkTargetPos;     
        sf::Vector2f networkTargetVel;  
        sf::Vector2f lastNetworkPos;
        sf::Vector2f lastNetworkVel;
        float networkStateAge = 0.0f;
        uint64_t networkTimestamp = 0; 
        float networkInterpFactor = 0.15f;

        static constexpr float MAX_NETWORK_STATE_AGE = 0.5f; // 500ms timeout
        const float CHASE_LIMIT = 7439.0f;

        void patrol(float dt) {
            DynamicBox* body = dynamic_cast<DynamicBox*>(p_body);
            if(body) {
                float currentX = body->getPosition().x;
                if((currentX < initialPosition.x - patrolRange)) {
                    body->moveLeft(true);
                    body->moveRight(false);
                    // body->move(true, false);
                } else if ((currentX > initialPosition.x + patrolRange)) {
                    body->moveRight(true);
                    body->moveLeft(false);
                    // body->move(false, false);
                }
                if (p_animation->isFaceForward() == body->isFaceForward()) {
                    p_animation->rotate();
                }
            }
        }

        void chase(float dt, const Player* player) {
            DynamicBox* body = dynamic_cast<DynamicBox*>(p_body);
            if(body && player) {
                float distance = std::abs(player->getPosition().x - body->getPosition().x); 
                float currentX = body->getPosition().x;
                
                if(distance <= detectionRange) {
                    bool moveRight = player->getPosition().x > body->getPosition().x;
                    body->moveLeft(true);
                    body->moveRight(false);
                    // body->move(true, false);
                } else {
                    // Stop move if player is go out of the range
                    if(body->isFaceForward()) {
                        body->moveRight(true);
                    } else {
                        body->moveLeft(true);
                    }
                    // body->move(body->isFaceForward(), true); 
                }

                if (p_animation->isFaceForward() == body->isFaceForward()) {
                    p_animation->rotate();
                }
            }
        }

        virtual void initializeAnimations(const std::string& jsonPath, const std::string& texturePath, sf::Vector2f scale) = 0;
        virtual std::string getDeadSpriteID() const = 0;
    protected:
        bool shouldBeDeleted;
        sf::Vector2f initialPosition;
        float patrolRange;
    public:
        Enemy(const std::string& jsonPath, const std::string& texturePath, sf::Vector2f scale, const std::string& spriteID,
            sf::Vector2f startPosition, sf::Vector2f size, const std::string& behaviorStr)
                : Entity(jsonPath, texturePath, scale, spriteID, new DynamicBox(startPosition, size, 70.f, 200.f, -300.f, 1)),
                 shouldBeDeleted(false) {
            if(behaviorStr == "Patrol") {
                behavior = EnemyBehavior::Patrol;
                patrolRange = 100.f;
            } else if(behaviorStr == "Chase"){
                behavior = EnemyBehavior::Chase;
                detectionRange = 200.f;
            } else if(behaviorStr == "Piranha") {
                behavior = EnemyBehavior::Piranha;
            }

            initialPosition = startPosition;
            isActive = true;
            p_animation->setAnimationState(true);
        }
            
        virtual ~Enemy() = default;

        bool shouldDelete() const {
            return shouldBeDeleted;
        }

        void setShouldDelete(bool sD) {
            shouldBeDeleted = sD;
        }

        virtual void reactCollision(int side, const Collision& collision) {
            if(collision.isWithWall() && (side == SideCollision::Left || side == SideCollision::Right)) {
                DynamicBox* body = dynamic_cast<DynamicBox*>(p_body);
                if(body) {
                    body->setVelocity({-body->getVelocity().x, body->getVelocity().y});
                    if(p_animation->isFaceForward() == body->isFaceForward()) {
                        p_animation->rotate();
                    }
                }
            }
        }

        void updateBehavior(float dt, Player* player = nullptr) override {
            if(!isActive) return;

            switch(behavior) {
                case EnemyBehavior::Patrol:
                    patrol(dt);
                    break;
                case EnemyBehavior::Chase:
                    chase(dt, player);
                    break;
                case EnemyBehavior::Piranha:
                    // Piranha behavior is implemented in Derived Class
                    break;
            }
        }

        void setNetworkAuthority(bool hasAuth) { 
            hasNetworkAuthority = hasAuth;
            isNetworkControlled = !hasAuth;
        }
        
        bool hasAuthority() const { return hasNetworkAuthority; }
        bool isNetworkedControlled() const { return isNetworkControlled; }

        void update(const sf::RenderWindow* window, float dt) override {
            networkStateAge += dt;

            if (hasNetworkAuthority) {
                if(!p_body->isOnGround()) {
                    p_animation->setAnimationState(false);
                    p_animation->setSpriteAnimation(getDeadSpriteID());
                } else {
                    p_animation->setAnimationState(true);
                }
                updateBehavior(dt);
                p_body->update(dt);
            } else {
                updateNetworkInterpolation(dt);
                if (networkStateAge > MAX_NETWORK_STATE_AGE) {
                    p_body->update(dt);
                }
            } 
            p_animation->update(window, dt);
        }

        void handleEvent(const sf::RenderWindow* window, const sf::Event& event) override {

        }

        void setActive(bool active) {
            isActive = active;
            if(!active) {
                DynamicBox* body = dynamic_cast<DynamicBox*>(p_body);
                if(body) body->setVelocity({0.f, 0.f});
                p_animation->setAnimationState(false);
                p_animation->setSpriteAnimation(getDeadSpriteID());
            }
        }

        bool getActive() const {
            return isActive;
        }

        void setOnGround(bool onGround) {
            DynamicBox* body = dynamic_cast<DynamicBox*>(p_body);
            if (body) {
                body->setOnGround(onGround);
            }
        }

        sf::Vector2f getVelocity() const {
            DynamicBox* body = dynamic_cast<DynamicBox*>(p_body);
            if (body) {
                return body->getVelocity();
            }
            return sf::Vector2f(0.f, 0.f);
        }

        void setVelocity(const sf::Vector2f& velocity) {
            DynamicBox* body = dynamic_cast<DynamicBox*>(p_body);
            if (body) {
                body->setVelocity(velocity);
            }
        }

        std::string getCurrentSpriteId() const {
            if (p_animation) {
                return p_animation->getSpriteId(); 
            }
            return ""; 
        }

        bool isFaceForward() const {
            if (p_animation) {
                return p_animation->isFaceForward();
            }
            return true;
        }

        // Network methods
        void setNetworkId(int id) { networkId = id; }
        int getNetworkId() const { return networkId; }
        void setNetworkNotified(bool notified) { networkNotified = notified; }
        bool isNetworkNotified() const { return networkNotified; }
        void setDead(bool dead) { isDeadFlag = dead; }
        bool isDead() const { return isDeadFlag; }
        void setNetwork(bool network) { isNetworked = network; }
        bool getIsNetwork() const { return isNetworked; }
        void setIsPlayerDeadWhenCollisionLF(bool check) {
            isPlayerDeadWhenCollisionLF = check;
        }

        bool getIsPlayerDeadWhenCollisionLF() {
            return isPlayerDeadWhenCollisionLF;
        }

        void setIsPlayerDeadWhenCollisionT(bool check) {
            isPlayerDeadWhenCollisionT = check;
        }

        bool getIsPlayerDeadWhenCollisionT() {
            return isPlayerDeadWhenCollisionT;
        }

        void setIsCheckCollisionWithBlock(bool check) {
            isCheckCollisionWithBlock = check;
        }

        bool getIsCheckCollisionWithBlock() {
            return isCheckCollisionWithBlock;
        }

        void setIsCheckCollisionWithEnemy(bool check) {
            isCheckCollisionWithEnemy = check;
        }

        bool getIsCheckCollisionWithEnemy() {
            return isCheckCollisionWithEnemy;
        }

        void setIsCheckCollisionWithPlayer(bool check) {
            isCheckCollisionWithPlayer = check;
        }

        bool getIsCheckCollisionWithPlayer() {
            return isCheckCollisionWithPlayer;
        }

        void syncNetworkState(const sf::Vector2f& pos, const sf::Vector2f& vel, 
                             bool alive, bool active, const std::string& spriteId, 
                             bool faceForward, uint64_t timestamp) 
        {
            if (hasNetworkAuthority) return; // Ignore if we're the host or owner

            isNetworked = true;
            networkTimestamp = timestamp;
            networkStateAge = 0.0f;

            DynamicBox* body = dynamic_cast<DynamicBox*>(p_body);
            if (body) {
                sf::Vector2f currentPos = body->getPosition();

                // Snap if far away, otherwise smooth
                float dx = std::abs(currentPos.x - pos.x);
                float dy = std::abs(currentPos.y - pos.y);
                if (dx > 50.f || dy > 50.f) {
                    body->setPosition(pos); // Hard snap for big errors
                } else {
                    body->setPosition(currentPos + (pos - currentPos) * 0.4f); // Smooth catch-up
                }

                // Always overwrite velocity
                body->setVelocity(vel);
            }

            // Sync alive/active state
            isDeadFlag = !alive;
            setActive(active);

            // Update animation state immediately
            if (!spriteId.empty() && p_animation) {
                p_animation->setSpriteAnimation(spriteId);
                if (p_animation->isFaceForward() != faceForward) {
                    p_animation->rotate();
                }
            }
        }

        void updateNetworkInterpolation(float dt) {
            if (!isNetworkControlled || networkStateAge > MAX_NETWORK_STATE_AGE) return;

            DynamicBox* body = dynamic_cast<DynamicBox*>(p_body);
            if (!body) return;

            sf::Vector2f currentPos = body->getPosition();
            float distance = std::sqrt(
                std::pow(networkTargetPos.x - currentPos.x, 2) + 
                std::pow(networkTargetPos.y - currentPos.y, 2)
            );

            if (distance > 100.0f) {
                // Large desync → snap
                body->setPosition(networkTargetPos);
                body->setVelocity(networkTargetVel);
            } else {
                // Smooth interpolation
                float interpFactor = std::min(1.0f, networkInterpFactor * (dt * 60.0f));
                body->setPosition(currentPos + (networkTargetPos - currentPos) * interpFactor);
                body->setVelocity(networkTargetVel);
            }
        }
    };
    
    #undef FILE_PATH
};