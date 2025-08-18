#pragma once

#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include "enemy.hpp"

namespace mario::entity {
    #define FILE_PATH "../../asset/sprites/"

    enum class GoombaType {
        Brown,
        Blue,
        Grey
    };

    class Goomba : public Enemy {
    protected:
        enum class GoombaState {
            Walking,
            Dead,
            DeadSpecial
        };

        GoombaType goombaType;
        std::string typePrefix;
        GoombaState currentState;
        GoombaState lastState;
        float verticalVelocity = 0.f;
        const float jumpForce = -450.f;
        const float gravity = 600.f;
        
        std::string getTypePrefix() const {
            switch (goombaType) {
                case GoombaType::Brown: return "goomba-new-brown";
                case GoombaType::Blue: return "goomba-new-blue";
                case GoombaType::Grey: return "goomba-new-grey";
                default: return "goomba-new-brown";
            }
        }

        void loadWalkingAnimations() {
            p_animation->clearAnimationStep();
            for(int i = 0; i < 4; ++i) {
                std::string spriteID = getTypePrefix() + ".walking[" + std::to_string(i) + "]";
                try {
                    p_animation->addAnimationStep(spriteID);
                } catch (const std::out_of_range& e) {
                    std::cerr << "Error loading sprite: " << spriteID << " - " << e.what() << "\n";
                }
            }
            p_animation->setLoop(true);
        }

        void loadDeadAnimations() {
            p_animation->clearAnimationStep();
            for(int i = 0; i < 3; ++i) {
                std::string spriteID = getTypePrefix() + ".dead[" + std::to_string(i) + "]";
                try {
                    p_animation->addAnimationStep(spriteID);
                } catch (const std::out_of_range& e) {
                    std::cerr << "Error loading sprite: " << spriteID << " - " << e.what() << "\n";
                }
            }
            p_animation->setLoop(false);
        }

        void loadDeadSpecialAnimations() {
            p_animation->clearAnimationStep();
            std::string spriteID = getTypePrefix() + ".dead-special[0]";
            try {
                p_animation->addAnimationStep(spriteID);
            } catch (const std::out_of_range& e) {
                std::cerr << "Error loading sprite: " << spriteID << " - " << e.what() << "\n";
            }
            p_animation->setLoop(false);
        }

        void initializeAnimations(const std::string& jsonPath, const std::string& texturePath,
                                    sf::Vector2f scale) override {
            p_animation->loadSheet(jsonPath, texturePath);
            currentState = GoombaState::Walking;
            lastState = GoombaState::Walking;
            loadWalkingAnimations();
            try {
                p_animation->setSpriteAnimation(getTypePrefix() + ".walking[0]");
            } catch (const std::out_of_range& e) {
                std::cerr << "Error setting sprite: " << getTypePrefix() + ".walking[0] - " << e.what() << "\n";
            }
            p_animation->setAnimationState(true);
        }

        std::string getDeadSpriteID() const override {
            return getTypePrefix() + ".dead[0]";
        }

    public:
        Goomba(sf::Vector2f startPosition, GoombaType type)
            : Enemy(FILE_PATH"enemy.json", FILE_PATH"enemy.png", {2.3f, 2.3f}, 
                ((type == GoombaType::Brown) ? "goomba-new-brown" : 
                     (type == GoombaType::Blue) ? "goomba-new-blue" : "goomba-new-grey") + std::string(".walking[0]"),
                     startPosition, {44.f, 54.f}, "Patrol"),
                goombaType(type),
                currentState(GoombaState::Walking),
                lastState(GoombaState::Walking) {
            initializeAnimations(FILE_PATH"enemy.json", FILE_PATH"enemy.png", {2.3f, 2.3f});
        }

        void hitByPlayer() {
            if(currentState == GoombaState::Walking) {
                currentState = GoombaState::Dead;
                p_animation->setAnimationState(true);
                loadDeadAnimations();
                try{
                    p_animation->setSpriteAnimation(getTypePrefix() + ".dead[0]");
                } catch (const std::out_of_range& e) {
                    std::cerr << "Error setting sprite: " << getTypePrefix() + ".dead[0] - " << e.what() << "\n";
                }

                lastState = GoombaState::Walking;
            }
        }

        void reactCollision(int side, const Collision& collision) override {
            if(collision.isWithPlayer() && (side == SideCollision::Top)) {
                hitByPlayer();
            } else if(collision.isWithWall() && (side == SideCollision::Left || side == SideCollision::Right)) {
                DynamicBox* body = dynamic_cast<DynamicBox*>(p_body);
                if(body) {
                    if(side == SideCollision::Left) {
                        // Move right if hit from left
                        body->moveLeft(true);
                        body->moveRight(false);
                        // body->move(true, false); 
                        body->setIsFaceForward(true);
                    } else if(side == SideCollision::Right) {
                        // Move left if hit from right
                        body->moveRight(true);
                        body->moveLeft(false);
                        // body->move(false, false); 
                        body->setIsFaceForward(false);
                    }
                    if(p_animation->isFaceForward() != body->isFaceForward()) {
                        p_animation->rotate();
                    }
                }
            } else if(collision.isWithFireball()) {
                currentState = GoombaState::DeadSpecial;
                loadDeadSpecialAnimations();
                try {
                    p_animation->setSpriteAnimation(getTypePrefix() + ".dead-special[0]");
                } catch (const std::out_of_range& e) {
                    std::cerr << "Error setting sprite: " << getTypePrefix() + ".dead-special[0] - " << e.what() << "\n";
                }
                p_animation->setAnimationState(false);
                verticalVelocity = jumpForce;
                lastState = GoombaState::DeadSpecial;
                setActive(true);
                setIsCheckCollisionWithEnemy(false);
                setIsCheckCollisionWithPlayer(false);
            } else if(collision.isWithInvinciblePlayer()) {
                currentState = GoombaState::DeadSpecial;
                loadDeadSpecialAnimations();
                try {
                    p_animation->setSpriteAnimation(getTypePrefix() + ".dead-special[0]");
                } catch (const std::out_of_range& e) {
                    std::cerr << "Error setting sprite: " << getTypePrefix() + ".dead-special[0] - " << e.what() << "\n";
                }
                p_animation->setAnimationState(false);
                verticalVelocity = jumpForce;
                lastState = GoombaState::DeadSpecial;
                setActive(true);
                setIsCheckCollisionWithEnemy(false);
                setIsCheckCollisionWithPlayer(false);
            } else if(collision.isWithEnemy()) {
                currentState = GoombaState::DeadSpecial;
                loadDeadSpecialAnimations();
                try {
                    p_animation->setSpriteAnimation(getTypePrefix() + ".dead-special[0]");
                } catch (const std::out_of_range& e) {
                    std::cerr << "Error setting sprite: " << getTypePrefix() + ".dead-special[0] - " << e.what() << "\n";
                }
                p_animation->setAnimationState(false);
                verticalVelocity = jumpForce;
                lastState = GoombaState::DeadSpecial;
                setActive(true);
                setIsCheckCollisionWithEnemy(false);
            }
            else {
                // continue
                if(p_body->isFaceForward()) {
                    p_body->moveLeft(true);
                    p_body->moveRight(false);
                } else {
                    p_body->moveRight(true);
                    p_body->moveLeft(false);
                }
                // p_body->move(p_body->isFaceForward(), false); 
            }
        }

        void update(const sf::RenderWindow* window, float dt) override {
            if (shouldBeDeleted) {
                return;
            }
            if(p_body->getPosition().y > 1000.f) {
                if(currentState != GoombaState::Dead) {
                    currentState = GoombaState::Dead;
                    p_animation->setAnimationState(false);
                    loadDeadAnimations();
                    try {
                        p_animation->setSpriteAnimation(getDeadSpriteID());
                    } catch (const std::out_of_range& e) {
                        std::cerr << "Error setting sprite: " << getDeadSpriteID() << " - " << e.what() << "\n";
                    }
                    shouldBeDeleted = true;
                    lastState = GoombaState::Dead;
                }
            } else {
                if(currentState == GoombaState::Walking && lastState != GoombaState::Walking) {
                    loadWalkingAnimations();
                    try {
                        p_animation->setSpriteAnimation(getTypePrefix() + ".walking[0]");
                    } catch (const std::out_of_range& e) {
                        std::cerr << "Error setting sprite: " << getTypePrefix() + ".walking[0] - " << e.what() << "\n";
                    }
                    p_animation->setAnimationState(true);
                    lastState = GoombaState::Walking;
                } else if (currentState == GoombaState::Dead && lastState != GoombaState::Dead) {
                    loadDeadAnimations();
                    try {
                        p_animation->setSpriteAnimation(getTypePrefix() + ".dead[0]");
                    } catch (const std::out_of_range& e) {
                        std::cerr << "Error setting sprite: " << getTypePrefix() + ".dead[0] - " << e.what() << "\n";
                    }
                    p_animation->setAnimationState(true);
                    lastState = GoombaState::Dead;
                } else if (currentState == GoombaState::Dead && lastState == GoombaState::Dead) {
                    if (p_animation->getStep() >= 2) {
                        p_animation->setAnimationState(false);
                        shouldBeDeleted = true;
                    }
                } 
                if (currentState == GoombaState::DeadSpecial) {
                    verticalVelocity += gravity * dt;
                    p_body->setPosition(sf::Vector2f(p_body->getPosition().x, p_body->getPosition().y + verticalVelocity * dt));
                    setIsCheckCollisionWithBlock(false);
                } 
            }
            if (!shouldBeDeleted) {
                updateBehavior(dt);
                p_animation->update(window, dt);
                p_body->updateSize(p_animation);
                p_body->update(dt);
            }
        }
    };
}