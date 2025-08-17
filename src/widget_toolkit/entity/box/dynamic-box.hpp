#pragma once

#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include "box.hpp"

namespace mario::entity {

    class DynamicBox : public Box {
        private:
            static constexpr float MAX_FALL_SPEED = 800.f;
            //static constexpr sf::Time DELAY_TIME_BETWEEN_JUMP = sf::seconds(0.0001f);
            
            sf::Vector2f acceleration, velocity;
            sf::Time timeUntilNextJump;

            float jumpForce, maxVelocityX;
            int jumpRemaining, maxJumps;

            
            bool _isJumpping, _isFaceForward, _isOnGround, hitCelling;
            bool _isRunningLeft, _isRunningRight;

        public:
            DynamicBox(sf::Vector2f _pos, sf::Vector2f _size, float acc = 1000.f, float maxVX = 300.f, float _jumpForce = -280.f, int _maxJump = 2) 
                : Box(_pos, _size), maxVelocityX(maxVX), maxJumps(_maxJump), jumpForce(_jumpForce) 
            {
                acceleration.x = acc;
                acceleration.y = 980.f;
                jumpRemaining = 0;
                velocity = sf::Vector2f(0, 0);

                _isFaceForward = true;
                _isJumpping = _isRunningLeft = _isRunningRight = false;
            }

            ~DynamicBox() override = default;

            void move(bool isMoveRight, bool isReleased) override {
                if(isReleased) {
                    _isRunningLeft = _isRunningRight = false;
                    return;
                }

                if(isMoveRight) {
                    _isRunningRight = true;
                } else {
                    _isRunningLeft = true;
                }
            }

            void moveLeft(bool isReleased) override {
                if(isReleased) {
                    _isRunningLeft = false;
                    return;
                }

                _isRunningLeft = true;
            }

            void moveRight(bool isReleased) override {
                if(isReleased) {
                    _isRunningRight = false;
                    return;
                }

                _isRunningRight = true;
            }

            void jump(bool _isReleased) override {
                jumpByANumberOfJumps(_isReleased, maxJumps);
            }

            void jumpByANumberOfJumps(bool _isReleased, int numJumps) override {
                if(_isReleased) {
                    _isJumpping = false;
                    return;
                }

                if(_isOnGround) {
                    velocity.y = 0.f;
                    jumpRemaining = numJumps;
                    timeUntilNextJump = sf::seconds(0.f);
                    _isJumpping = true;
                }
            }

            void update(float dt) override {
                if(_isRunningLeft != _isRunningRight) {
                    if(_isRunningRight != (velocity.x > 0.f)) {
                        if(_isRunningRight) {
                            velocity.x = std::min(velocity.x + 2 * acceleration.x * dt, maxVelocityX);
                        } else {
                            velocity.x = std::max(velocity.x - 2 * acceleration.x * dt, -maxVelocityX);
                        }
                    } else {
                        if(_isRunningRight) {
                            velocity.x = std::min(velocity.x + acceleration.x * dt, maxVelocityX);
                        } else {
                            velocity.x = std::max(velocity.x - acceleration.x * dt, -maxVelocityX);
                        }
                    }

                } else
                    if(abs(velocity.x) > 0.f)
                        velocity.x += ((velocity.x > 0.f) ? -1 : +1) * std::min(acceleration.x * 2 * dt, abs(velocity.x));
                
                if(abs(velocity.x) > 0.f)
                    _isFaceForward = (velocity.x > 0.f);
                
                if(_isJumpping) {
                    timeUntilNextJump -= sf::seconds(dt);
                    if(timeUntilNextJump <= sf::seconds(0.f)) {
                        velocity.y += jumpForce;
                        timeUntilNextJump = sf::seconds(0.00001f);

                        --jumpRemaining;
                        if(jumpRemaining == 0)
                            _isJumpping = false;
                    }
                } else 
                    if(!_isOnGround) {
                        velocity.y = std::min(velocity.y + acceleration.y * dt, MAX_FALL_SPEED);
                    } else {
                        velocity.y = 0.f;
                    }

                position.x += velocity.x * dt;
                position.y += velocity.y * dt;
                _isOnGround = false;
            }

            void resetMove() override {
                velocity.x = 0.f;
                _isRunningLeft = _isRunningRight = false;
            }

            void resetJump() override {
                velocity.y = std::min(velocity.y, 0.f);
                _isJumpping = false;
            }

            bool isNotMoving() const override {
                return (abs(velocity.x) <= 1.f);
            }

            void setOnGround(bool isOnGround) override {
                _isOnGround = isOnGround;
            }

            bool isOnGround() const override {
                return _isOnGround;
            }

            void setIsFaceForward(bool faceForward) override {
                _isFaceForward = faceForward;
            }

            bool isFaceForward() const override {
                return _isFaceForward;
            }

            void setVelocity(sf::Vector2f vel) override {
                velocity = vel;
            }

            sf::Vector2f getVelocity() const override {
                return velocity;
            }

            void setAcceleration(sf::Vector2f acc) override {
                acceleration = acc;
            }

            sf::Vector2f getAcceleration() const override {
                return acceleration;
            }

            void setGravityEnabled(bool enabled) {
                acceleration.y = enabled ? 980.f : 0.f;
            }

            void setGravityDirection(float dir) {
                acceleration.y = std::abs(acceleration.y) * dir;
            }

            float getGravityDirection() const {
                return (acceleration.y >= 0) ? 1.0f : -1.0f;
            }
    };
}