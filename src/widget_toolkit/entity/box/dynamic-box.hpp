#pragma once

#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include "box.hpp"

namespace mario::entity {

    class DynamicBox : public Box {
        private:
            static constexpr float MAX_FALL_SPEED = 1400.f;
            
            sf::Vector2f acceleration, velocity;

            float jumpForce, maxVelocityX;
            int jumpRemaining, maxJumps;
            
            bool isJumpping, isMoveRight, onGround, hitCelling;
            bool isRunning;

        public:
            DynamicBox(sf::Vector2f _pos, sf::Vector2f _size, float acc = 0.2f, float maxVX = 200.f, float _maxJump = 2, float _jumpForce = -900.f) 
                : Box(_pos, _size), maxVelocityX(maxVX), maxJumps(_maxJump), jumpForce(_jumpForce) 
            {
                acceleration.x = acc;
                acceleration.y = 98.f;
                jumpRemaining = 0;
                velocity = sf::Vector2f(0, 0);
                isJumpping = isRunning = false;
            }

            void move(bool _isMoveRight, bool _isReleased) override {
                if(_isReleased) {
                    isRunning = false;
                    return;
                }

                isRunning = true;
                isMoveRight = _isMoveRight;
            }

            void jump(bool _isReleased) override {
                if(_isReleased) {
                    isJumpping = false;
                    return;
                }

                jumpRemaining = maxJumps;
                isJumpping = true;
            }

            void update(float dt) override {
                if(isRunning) {
                    if(isMoveRight) {
                        velocity.x = std::min(velocity.x + acceleration.x * dt, maxVelocityX);
                    } else {
                        velocity.x = std::max(velocity.x - acceleration.x * dt, -maxVelocityX);
                    }
                } else
                    if(abs(velocity.x) > 0)
                        velocity.x += ((velocity.x > 0) ? -1 : +1) * std::min(acceleration.x * dt, abs(velocity.x));
                
                if(isJumpping) {
                    velocity.y += jumpForce;
                    
                    --jumpRemaining;
                    if(jumpRemaining == 0)
                        isJumpping = false;
                } else {
                    velocity.y = std::min(velocity.y + acceleration.y * dt, MAX_FALL_SPEED);
                }

                position.x += velocity.x * dt;
                position.y += velocity.y * dt;
            }

            sf::Vector2f getVelocity() const override {
                return velocity;
            }
    };
}