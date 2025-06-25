#pragma once

#include<bits/stdc++.h>
#include "entity.hpp"
#include <box2d/box2d.h>
#include "../dynamic-box.hpp"

const float PIXEL_RUN_PER_SCREEN = 5.0f;

namespace mario {

    class Player : public Entity {
        private:
            std::unique_ptr<sf::Sprite> p_sprite;
            std::unique_ptr<sf::Texture> p_texture;
            sf::Vector2f position;
            float shootingDelayTime = 0.f;
            bool _isRunning = false, _isRunningForward, _isJumping = false;
            bool _isFaceForward = true;
            DynamicBox *p_dynamicBox = nullptr;
            sf::Time jumpTimer;

        public:
            Player(std::string fileName, b2WorldId worldId) {
                p_texture = std::make_unique<sf::Texture>(fileName);
                p_sprite = std::make_unique<sf::Sprite>(*p_texture);
                p_sprite->setTextureRect(sf::IntRect({76, 97}, {24, 31}));
                p_sprite->setOrigin({11.5, 31});
                p_sprite->setScale({2.5f, 2.5f});
                position = sf::Vector2f(15, 720);
                p_dynamicBox = new DynamicBox(worldId, sf::Vector2f(500.f, 200.f), sf::Vector2f(24.f, 31.f));
            }

            ~Player() override {
                delete p_dynamicBox;
            }

            bool isInSurface() {
                return (p_dynamicBox->getPosition().y + p_dynamicBox->getDimension().y >= 715.f && p_dynamicBox->getVeclocity().y <= 0.001f && p_dynamicBox->getVeclocity().y >= -0.0001f);
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
                if(_isJumping) {
                    p_dynamicBox->applyForce({0.f, -20.f * 40.f});
                    jumpTimer += sf::seconds(dt);
                }

                if(_isJumping && jumpTimer >= sf::seconds(0.2f))
                    _isJumping = false;

                if(_isRunning && isFaceToBlock()) 
                    _isRunning = false;
                
                if(_isRunning) {
                    if(_isRunningForward != _isFaceForward)
                        rotateDirection();

                    position.x += (_isRunningForward ? 1 : -1) * PIXEL_RUN_PER_SCREEN;
                }
            }
            
            void handleEvent(const sf::RenderWindow *window, const sf::Event &event) override {
                
            }
            
            void render(sf::RenderWindow *window) override {
                p_sprite->setPosition(position);
                p_sprite->setPosition(p_dynamicBox->getPosition());

                window->draw(*p_sprite);
            }

    };
}