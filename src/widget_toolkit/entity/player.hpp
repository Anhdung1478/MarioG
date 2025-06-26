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
            std::unique_ptr<sf::Sprite> p_sprite;
            std::unique_ptr<sf::Texture> p_texture;
            sf::Vector2f position;
            float shootingDelayTime = 0.f;
            bool _isRunning = false, _isRunningForward, _isJumping = false;
            bool _isFaceForward = true;
            DynamicBox *p_playerBody = nullptr;
            sf::Time jumpTimer;

        public:
            Player(std::string fileName, b2WorldId worldId) {
                p_texture = std::make_unique<sf::Texture>(fileName);
                p_sprite = std::make_unique<sf::Sprite>(*p_texture);
                p_sprite->setTextureRect(sf::IntRect({76, 97}, {24, 31}));
                p_sprite->setOrigin({11.5, 31});
                p_sprite->setScale({2.5f, 2.5f});
                position = sf::Vector2f(15, 720);
                p_playerBody = new DynamicBox(worldId, sf::Vector2f(500.f, 200.f), sf::Vector2f(24.f, 31.f), 1.0f, 0.8f);
            }

            ~Player() override {
                delete p_playerBody;
            }

            bool isInSurface() {
                return (p_playerBody->getPosition().y + p_playerBody->getDimension().y >= 714.f && p_playerBody->getVelocity().y <= 0.1f && p_playerBody->getVelocity().y >= -0.1f);
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
                _isFaceForward = 1 - _isFaceForward;
                if(_isFaceForward) {
                    p_sprite->setScale(sf::Vector2f(2.5f, 2.5f));
                } else {
                    p_sprite->setScale(sf::Vector2f(-2.5f, 2.5f));
                }
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
                sf::Vector2f vel = p_playerBody->getVelocity();
                if(!isInSurface()) {
                    // change texture to jumping
                } else {
                    if(abs(vel.x) <= 0.001f) {
                        // change texture to idle
                    } else {
                        // change texture to next running texture 
                    }
                }

                float bodyMass = p_playerBody->getMass();

                if(_isJumping) {
                    p_playerBody->applyForce(sf::Vector2f(0.f, -JUMP_ACCELERATION * bodyMass));
                    jumpTimer += sf::seconds(dt);
                }

                if(_isJumping && jumpTimer >= sf::seconds(MAX_JUMP_TIME))
                    _isJumping = false;

                if(_isRunning && isFaceToBlock()) 
                    _isRunning = false;
                
                if(_isRunning) {
                    if(_isFaceForward != _isRunningForward && (vel.x == 0.f || (_isFaceForward != (vel.x > 0.f))))
                        rotateDirection();

                    if(_isRunningForward && vel.x < MAX_SPEED || !_isRunningForward && vel.x > -MAX_SPEED) {
                        sf::Vector2f force = sf::Vector2f((_isRunningForward ? 1 : -1) * MOVE_ACCELERATION * bodyMass, 0.f);
                        p_playerBody->applyForce(force);
                    }
                }

                p_playerBody->setDamping(isInSurface() ? GROUND_DAMPING : AIR_DAMPING);
                std::cerr << "PLAYER VELOCITY: " << vel.x << ' ' << vel.y << '\n';
            }
            
            void handleEvent(const sf::RenderWindow *window, const sf::Event &event) override {
                
            }
            
            void render(sf::RenderWindow *window) override {
                p_sprite->setPosition(position);
                p_sprite->setPosition(p_playerBody->getPosition());

                window->draw(*p_sprite);
            }

    };
}