#pragma once

#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include "enemy.hpp"

namespace mario::entity {
    #define FILE_PATH "../../asset/sprites/"

    class Ball : public Enemy {
    protected:
        enum class BallState {
            FreeFall,
            Walking,
            DeadSpecial
        };

        BallState currentState;
        BallState lastState;
        bool moveRight;
        float verticalVelocity = 0.f;
        const float jumpForce = -450.f;
        const float gravity = 600.f;

        void loadDeadSpecialAnimations() {
            p_animation->clearAnimationStep();
            std::string spriteID = "ball.dead-special[0]";
            try {
                p_animation->addAnimationStep(spriteID);
            } catch (const std::out_of_range& e) {
                std::cerr << "Error loading sprite: " << spriteID << " - " << e.what() << "\n";
            }
            p_animation->setLoop(false);
        }

        void initializeAnimations(const std::string& jsonPath, const std::string& texturePath, sf::Vector2f scale) override {
            p_animation->loadSheet(jsonPath, texturePath);
            p_animation->clearAnimationStep();

            try {
                if (currentState == BallState::FreeFall) {
                    p_animation->addAnimationStep("ball.free-fall[0]");
                    p_animation->addAnimationStep("ball.free-fall[1]");
                    p_animation->setSpriteAnimation("ball.free-fall[0]");
                } else {
                    p_animation->addAnimationStep("ball.walking[0]");
                    p_animation->addAnimationStep("ball.walking[1]");
                    p_animation->setSpriteAnimation("ball.walking[0]");
                }
            } catch (const std::out_of_range& e) {
                std::cerr << "Error setting ball animation - " << e.what() << "\n";
            }

            p_animation->setLoop(true);
            p_animation->setAnimationState(true);
        }

        std::string getDeadSpriteID() const override {
            return ""; // ball không có trạng thái chết
        }

    public:
        Ball(sf::Vector2f startPosition)
            : Enemy(FILE_PATH"enemy.json", FILE_PATH"enemy.png", {2.5f, 2.5f}, "ball.free-fall[0]",
                    startPosition, {32.f, 32.f}, ""), 
              currentState(BallState::FreeFall), lastState(BallState::FreeFall), moveRight(false) {
            
            if(auto body = dynamic_cast<DynamicBox*>(p_body)) {
                body->setGravityEnabled(true);
                body->setAcceleration({700.f, 920.f}); 
            }

            setIsCheckCollisionWithBlock(true);
            setIsPlayerDeadWhenCollisionT(true);
            initializeAnimations(FILE_PATH"enemy.json", FILE_PATH"enemy.png", {2.5f, 2.5f});
        }

        void reactCollision(int side, const Collision& collision) override {
            if(collision.isWithWall() && (side == SideCollision::Bottom) && currentState == BallState::FreeFall) {
                currentState = BallState::Walking;
                initializeAnimations(FILE_PATH"enemy.json", FILE_PATH"enemy.png", {2.5f, 2.5f});

            } else if(collision.isWithWall() && (side == SideCollision::Left || side == SideCollision::Right) && currentState == BallState::Walking) {
                DynamicBox* body = dynamic_cast<DynamicBox*>(p_body);
                if(body) {
                    if(side == SideCollision::Left) {
                        moveRight = true; // Move right
                        body->setVelocity({150.f, 0.f});
                        body->setIsFaceForward(true);
                    } else if(side == SideCollision::Right) {
                        moveRight = false; // Move left
                        body->setVelocity({-150.f, 0.f});
                        body->setIsFaceForward(false);
                    }
                    if(p_animation->isFaceForward() != body->isFaceForward()) {
                        p_animation->rotate();
                    }
                }
            } else if(collision.isWithEnemy()) {
                currentState = BallState::DeadSpecial;
                loadDeadSpecialAnimations();
                try {
                    p_animation->setSpriteAnimation("ball.dead-special[0]");
                } catch (const std::out_of_range& e) {
                    std::cerr << "Error setting sprite: ball.dead-special[0] - " << e.what() << "\n";
                }
                p_animation->setAnimationState(false);
                verticalVelocity = jumpForce;
                lastState = BallState::DeadSpecial;
                setActive(true);
                setIsCheckCollisionWithEnemy(false);
            } else if(collision.isWithFireball()) {
                currentState = BallState::DeadSpecial;
                loadDeadSpecialAnimations();
                try {
                    p_animation->setSpriteAnimation("ball.dead-special[0]");
                } catch (const std::out_of_range& e) {
                    std::cerr << "Error setting sprite: ball.dead-special[0] - " << e.what() << "\n";
                }
                p_animation->setAnimationState(false);
                verticalVelocity = jumpForce;
                lastState = BallState::DeadSpecial;
                setActive(true);
                setIsCheckCollisionWithEnemy(false);
            } else if(collision.isWithInvinciblePlayer()) {
                currentState = BallState::DeadSpecial;
                loadDeadSpecialAnimations();
                try {
                    p_animation->setSpriteAnimation("ball.dead-special[0]");
                } catch (const std::out_of_range& e) {
                    std::cerr << "Error setting sprite: ball.dead-special[0] - " << e.what() << "\n";
                }
                p_animation->setAnimationState(false);
                verticalVelocity = jumpForce;
                lastState = BallState::DeadSpecial;
                setActive(true);
                setIsCheckCollisionWithEnemy(false);
            }
        }

        void updateBehavior(float dt, const Player* player) {
            if(!getActive() || !player) return;
            
            if(p_body->getPosition().y > 1000.f) {
                if(currentState != BallState::DeadSpecial) {
                    currentState = BallState::DeadSpecial;
                    p_animation->setAnimationState(false);
                    loadDeadSpecialAnimations();
                    try {
                        p_animation->setSpriteAnimation(getDeadSpriteID());
                    } catch (const std::out_of_range& e) {
                        std::cerr << "Error setting sprite: " << getDeadSpriteID() << " - " << e.what() << "\n";
                    }
                    shouldBeDeleted = true;
                    lastState = BallState::DeadSpecial;
                }
            } else if(currentState == BallState::Walking && lastState != BallState::Walking) {
                if(auto body = dynamic_cast<DynamicBox*>(p_body)) {
                    moveRight = player->getPosition().x > body->getPosition().x;
                    body->setVelocity({moveRight ? 150.f : -150.f, 0.f});
                    body->setIsFaceForward(moveRight);

                    if (p_animation->isFaceForward() == moveRight) {
                        p_animation->rotate();
                    }
                }
                lastState = BallState::Walking;
            } else if(currentState == BallState::Walking && lastState == BallState::Walking) {
                if(auto body = dynamic_cast<DynamicBox*>(p_body)) {
                    if(moveRight) {
                        body->moveLeft(true);
                        body->moveRight(false);
                    } else {
                        body->moveRight(true);
                        body->moveLeft(false);
                    }
                    // body->move(moveRight, false);
                    body->setIsFaceForward(moveRight);
                    
                    if (p_animation->isFaceForward() == moveRight) {
                        p_animation->rotate();
                    }
                }
            }

            if(currentState == BallState::DeadSpecial) {
                verticalVelocity += gravity * dt;
                p_body->setPosition(sf::Vector2f(p_body->getPosition().x, p_body->getPosition().y + verticalVelocity * dt));
                setIsCheckCollisionWithBlock(false);
                setIsCheckCollisionWithPlayer(false);
            } 
        }

        void updateWithPlayer(const sf::RenderWindow* window, float dt, const Player* player) {
            if(shouldDelete()) return;

            updateBehavior(dt, player);
            p_animation->update(window, dt);
            p_body->updateSize(p_animation);
            p_body->update(dt);
        }

        void update(const sf::RenderWindow* window, float dt) override {
            // updateWithPlayer(window, dt, nullptr);
        }
    };

    #undef FILE_PATH
}
