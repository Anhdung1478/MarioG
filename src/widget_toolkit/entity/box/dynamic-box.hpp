#pragma once

#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include "box.hpp"

namespace mario::entity {

    class DynamicBox : public Box {
        private:
            static constexpr float MAX_FALL_SPEED = 800.f;
            static constexpr sf::Time DELAY_TIME_BETWEEN_JUMP = sf::seconds(0.0001f);
            
            sf::Vector2f acceleration, velocity;
            sf::Time timeUntilNextJump;

            float jumpForce, maxVelocityX;
            int jumpRemaining, maxJumps;

            
            bool isJumpping, _isFaceForward, isMoveRight, onGround, hitCelling;
            bool isRunning;

        public:
            DynamicBox(sf::Vector2f _pos, sf::Vector2f _size, float acc = 150.f, float maxVX = 200.f, float _jumpForce = -200.f, int _maxJump = 2) 
                : Box(_pos, _size), maxVelocityX(maxVX), maxJumps(_maxJump), jumpForce(_jumpForce) 
            {
                acceleration.x = acc;
                acceleration.y = 980.f;
                jumpRemaining = 0;
                velocity = sf::Vector2f(0, 0);

                _isFaceForward = true;
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

                if(isOnSurface()) {
                    timeUntilNextJump = sf::seconds(0.f);
                    velocity.y = 0.f;
                    jumpRemaining = maxJumps;
                    isJumpping = true;
                }
            }

            bool isNotMoving() const override {
                return (abs(velocity.x) <= 1.f);
            }

            bool isOnSurface() const override {
                return (position.y >= 600.f);
            }

            bool isFaceForward() const override {
                return _isFaceForward;
            }

            void update(float dt) override {
                if(isRunning) {
                    if(isMoveRight != (velocity.x > 0.f)) {
                        if(isMoveRight) {
                            velocity.x = std::min(velocity.x + 2 * acceleration.x * dt, maxVelocityX);
                        } else {
                            velocity.x = std::max(velocity.x - 2 * acceleration.x * dt, -maxVelocityX);
                        }
                    } else {
                        if(isMoveRight) {
                            velocity.x = std::min(velocity.x + acceleration.x * dt, maxVelocityX);
                        } else {
                            velocity.x = std::max(velocity.x - acceleration.x * dt, -maxVelocityX);
                        }
                    }


                    _isFaceForward = (velocity.x > 0.f);
                } else
                    if(abs(velocity.x) > 0.f)
                        velocity.x += ((velocity.x > 0.f) ? -1 : +1) * std::min(acceleration.x * 2 * dt, abs(velocity.x));
                
                if(isJumpping) {
                    timeUntilNextJump -= sf::seconds(dt);
                    if(timeUntilNextJump <= sf::seconds(0.f)) {
                        velocity.y += jumpForce;
                        timeUntilNextJump = DELAY_TIME_BETWEEN_JUMP;

                        --jumpRemaining;
                        if(jumpRemaining == 0)
                            isJumpping = false;
                    }
                } else 
                    if(!isOnSurface()) {
                        velocity.y = std::min(velocity.y + acceleration.y * dt, MAX_FALL_SPEED);
                    } else {
                        velocity.y = 0.f;
                    }


                position.x += velocity.x * dt;
                position.y += velocity.y * dt;
            }

            sf::Vector2f getVelocity() const override {
                return velocity;
            }

            void setVelocity(sf::Vector2f vel) {
                velocity = vel;
            }
    };
}