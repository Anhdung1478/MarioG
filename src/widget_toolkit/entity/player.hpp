#pragma once

#include<bits/stdc++.h>
#include "Entity.hpp"

const float PIXEL_RUN_PER_SCREEN = 5.0f;

namespace mario {

    class Player : public Entity {
        private:
            std::unique_ptr<sf::Sprite> p_sprite;
            std::unique_ptr<sf::Texture> p_texture;
            sf::Vector2f position;
            float shootingDelayTime = 0.f;
            float startJumpYPos = -10000.f;
            bool _isRunning = false, _isRunningForward, _isJumping = false;
            bool _isFaceForward = true;

        public:
            Player(std::string fileName) {
                p_texture = std::make_unique<sf::Texture>(fileName);
                p_sprite = std::make_unique<sf::Sprite>(*p_texture);
                p_sprite->setTextureRect(sf::IntRect({76, 97}, {24, 31}));
                p_sprite->setOrigin({11.5, 31});
                p_sprite->setScale({2.5f, 2.5f});
                position = sf::Vector2f(15, 720);
            }

            bool isInSurface() {
                return true;
            }

            bool isFaceToBlock() {
                return false;
            }

            void jump(bool isReleased) {
                if(isReleased) {
                    _isJumping = false;
                    startJumpYPos = -10000.f;
                    return;
                }

                if(!_isJumping && isInSurface()) {
                    _isJumping = true;
                    startJumpYPos = position.y;
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
                if(!_isJumping && position.y < 720)
                    position.y = std::min(720.f, position.y + 20 * dt * PIXEL_RUN_PER_SCREEN);
                
                if(_isJumping && startJumpYPos - position.y > 500.f) {
                    _isJumping = false;
                    startJumpYPos = -10000.f;
                }

                if(_isRunning && isFaceToBlock()) 
                    _isRunning = false;
                
                if(_isJumping)
                    position.y -= PIXEL_RUN_PER_SCREEN;
                
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

                window->draw(*p_sprite);
            }

    };
}