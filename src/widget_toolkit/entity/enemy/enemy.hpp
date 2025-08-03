#pragma once

#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include "../entity.hpp"
#include "../box/dynamic-box.hpp"
#include "../player.hpp"

namespace mario::entity {
    #define FILE_PATH "../../asset/sprites/"

    struct Collision {
        enum class Type { Player, Wall, Bullet, Brick };
        Type type;
        sf::Vector2f direction; // x: -1 (left), 1 (right); y: -1 (up), 1 (down)
        Collision(Type t, sf::Vector2f dir) : type(t), direction(dir) {}
        bool isWithPlayer() const { return type == Type::Player; }
        bool isWithWall() const { return type == Type::Wall; }
        bool isWithBullet() const { return type == Type::Bullet; }
        bool isWithBrick() const { return type == Type::Brick; }
    };

    enum class EnemyBehavior {
        Patrol,
        Chase
    };

    class Enemy : public Entity {
    private:
        EnemyBehavior behavior;
        float patrolRange;
        sf::Vector2f initialPosition;
        float detectionRange;
        bool isActive;

        void patrol(float dt) {
            DynamicBox* body = dynamic_cast<DynamicBox*>(p_body);
            if(body) {
                float currentX = body->getPosition().x;
                if(currentX < initialPosition.x - patrolRange) {
                    body->move(true, false); // move right
                } else if (currentX > initialPosition.x + patrolRange) {
                    body->move(false, false); // move left
                } else {
                    body->move(body->isFaceForward(), false); // continue
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
                if(distance <= detectionRange) {
                    bool moveRight = player->getPosition().x > body->getPosition().x;
                    body->move(moveRight, false);
                } else {
                    body->move(body->isFaceForward(), true); // Stop move if player is go out of the range
                }

                if (p_animation->isFaceForward() == body->isFaceForward()) {
                    p_animation->rotate();
                }
            }
        }

        virtual void initializeAnimations(const std::string& jsonPath, const std::string& texturePath, sf::Vector2f scale) = 0;
        virtual std::string getDeadSpriteID() const = 0;
    public:
        Enemy(const std::string& jsonPath, const std::string& texturePath, sf::Vector2f scale, const std::string& spriteID,
            sf::Vector2f startPosition, sf::Vector2f size, const std::string& behaviorStr)
                : Entity(jsonPath, texturePath, scale, spriteID, new DynamicBox(startPosition, size, 70.f, 200.f, -300.f, 1)) {
            if(behaviorStr == "Patrol") {
                behavior = EnemyBehavior::Patrol;
                patrolRange = 100.f;
            } else {
                behavior = EnemyBehavior::Chase;
                detectionRange = 200.f;
            }

            initialPosition = startPosition;
            isActive = true;
            p_animation->setAnimationState(true);
        }
            
        virtual ~Enemy() = default;

        virtual void reactCollision(const Collision& collision) {
            if(collision.isWithWall()) {
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
            }
        }

        void update(const sf::RenderWindow* window, float dt) override {
            if(!p_body->isOnGround()) {
                p_animation->setAnimationState(false);
                p_animation->setSpriteAnimation(getDeadSpriteID());
            } else {
                p_animation->setAnimationState(true);
            }
            updateBehavior(dt);
            p_animation->update(window, dt);
            p_body->update(dt);
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
    };
    
    #undef FILE_PATH
};