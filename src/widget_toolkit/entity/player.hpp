#pragma once

#include<bits/stdc++.h>
#include "entity.hpp"
#include <box2d/box2d.h>
#include "box/dynamic-box.hpp"

namespace mario::entity {
    
    constexpr static float MOVE_ACCELERATION = 15.f;
    constexpr static float MAX_SPEED = 200.f; // pixel per second
    constexpr static float GROUND_DAMPING = 3.f;
    constexpr static float AIR_DAMPING = 0.5f;

    constexpr static float JUMP_ACCELERATION = 50.f;
    constexpr static float MAX_JUMP_TIME = 0.2f;
    
    class Player : public Entity {
        private:
            sf::Time jumpTimer;
            
            float shootingDelayTime = 0.f;
            bool _isRunning = false;
            bool _isRunningForward;
            bool _isJumping = false;

        public:
            Player(std::string jsonPath, std::string texturePath, std::string randomSpriteID, b2WorldId worldId, sf::Vector2f spawnPoint) 
            : Entity(jsonPath, texturePath, sf::Vector2f(2.5f, 2.5f), randomSpriteID) {
                p_body = new DynamicBox(worldId, spawnPoint, p_animation->getScale(), 1.0f, 0.8f);
                p_animation->addAnimationStep("mario-super.walk[1]");
                p_animation->addAnimationStep("mario-super.walk[0]");
                p_animation->addAnimationStep("mario-super.idle[0]");
            }

            ~Player() override {
                delete p_body;
            }

            bool isInSurface() {
                return (p_body->getPosition().y + p_body->getDimension().y >= 714.f && p_body->getVelocity().y <= 0.1f && p_body->getVelocity().y >= -0.1f);
            }

            bool isFaceToBlock() {
                return false;
            }

            void jump(bool isReleased) {
                if(isReleased) {
                    _isJumping = false;
                    return;
                }

                if(!_isJumping && isInSurface()) {
                    _isJumping = true;
                    jumpTimer = sf::Time::Zero;
                }
            }

            void rotateDirection() {
                p_animation->rotate();
            }
            
            void run(bool isForward, bool isReleased) {
                if(isReleased) {
                    _isRunning = false;
                    return;
                }

                if(!_isRunning && !isFaceToBlock()) {
                    _isRunning = true;
                    _isRunningForward = isForward;
                }
            }
            
            void shotFireball(bool isReleased) {
                if(isReleased)
                    return;
            }
                
            void update(const sf::RenderWindow *window, float dt) override {
                sf::Vector2f vel = p_body->getVelocity();
                if(!isInSurface()) {
                    // change texture to jumping
                    p_animation->setSpriteAnimation("mario-super.jump[0]");
                    p_animation->setAnimationState(false);
                } else {
                    if(abs(vel.x) <= 0.001f) {
                        // change texture to idle
                        p_animation->setSpriteAnimation("mario-super.idle[0]");
                        p_animation->setAnimationState(false);
                    } else {
                        // change to next run animation step
                        p_animation->setAnimationState(true);
                    }
                }
                
                p_animation->update(window, dt);
                float bodyMass = p_body->getMass();

                if(_isJumping) {
                    p_body->applyForce(sf::Vector2f(0.f, -JUMP_ACCELERATION * bodyMass));
                    jumpTimer += sf::seconds(dt);
                }

                if(_isJumping && jumpTimer >= sf::seconds(MAX_JUMP_TIME))
                    _isJumping = false;

                if(_isRunning && isFaceToBlock()) 
                    _isRunning = false;
                
                if(_isRunning) {
                    if(p_animation->isFaceForward() != _isRunningForward && (vel.x == 0.f || (p_animation->isFaceForward() != (vel.x > 0.f))))
                        rotateDirection();

                    if(_isRunningForward && vel.x < MAX_SPEED || !_isRunningForward && vel.x > -MAX_SPEED) {
                        sf::Vector2f force = sf::Vector2f((_isRunningForward ? 1 : -1) * MOVE_ACCELERATION * bodyMass, 0.f);
                        p_body->applyForce(force);
                    }
                }

                p_body->setDamping(isInSurface() ? GROUND_DAMPING : AIR_DAMPING);
                //std::cerr << "PLAYER VELOCITY: " << vel.x << ' ' << vel.y << '\n';
            }
            
            void handleEvent(const sf::RenderWindow *window, const sf::Event &event) override {
                
            }
            
            void render(sf::RenderWindow *window) override {
                p_animation->renderWithPosition(window, p_body->getPosition());
            }

    };
}