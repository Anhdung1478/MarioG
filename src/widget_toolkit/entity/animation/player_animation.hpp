#pragma once

#include<bits/stdc++.h>
#include "animation.hpp"

namespace mario::entity {
    
    class PlayerAnimation : public Animation {
        private:
            float FLICKER_DELAY = 0.1f;
            float INVINCIBLE_SWAPPING_COLOR_DELAY = 1.f / 10.f;

            std::vector<SpriteData> invincibleAnimationSteps;
            sf::Time flickerTimer;
            sf::Time invincibleTimer;

            int id_invincible = 0, invincible_step = 0;
            bool _isFlicker = false, _isInvincible = false;

            void swapColorInvincibleAnimation() {
                if(++id_invincible % 4 == 0)
                    id_invincible = 0;
            }

            void performNextInvincibleAnimation() {
                setSprite(invincibleAnimationSteps[4 * invincible_step + id_invincible]);
                if(4 * ++invincible_step >= invincibleAnimationSteps.size())
                    invincible_step -= invincibleAnimationSteps.size() / 4;
            }

        public:
            PlayerAnimation(const std::string& jsonPath, const std::string& texturePath, sf::Vector2f _scale, const std::string& randomSpriteID) 
                : Animation(jsonPath, texturePath, _scale, randomSpriteID) {}

            ~PlayerAnimation() override {
            }

            void setFlicker(bool isOn) override {
                _isFlicker = isOn;
                flickerTimer = sf::seconds(FLICKER_DELAY);
                setVisible(true);
            }

            bool isFlicker() const override {
                return _isFlicker;
            }

            void setInvincible(bool isOn) override {
                id_invincible = invincible_step = 0;
                _isInvincible = isOn;

                invincibleTimer = sf::seconds(INVINCIBLE_SWAPPING_COLOR_DELAY);
                invincibleAnimationSteps.clear();
            }

            bool isInvincible() const override {
                return _isInvincible;
            }

            int getIDInvincible() const override {
                return id_invincible;
            }

            void addInvincibleAnimationStep(const std::string &spriteID) override {
                invincibleAnimationSteps.push_back(p_textureResource.getSpriteData(spriteID));
            }

            void update(const sf::RenderWindow *window, float dt) override {
                if(!_isInvincible) {
                    Animation::update(window, dt);
                } else
                    if(_isRunning) {
                        animationTimer -= sf::seconds(dt);
                        if(animationTimer <= sf::Time::Zero) {
                            animationTimer = sf::seconds(TIME_BETWEEN_STEP);
                            performNextInvincibleAnimation();
                        }
                    }

                if(_isFlicker) {
                    flickerTimer -= sf::seconds(dt);
                    if(flickerTimer <= sf::Time::Zero) {
                        flickerTimer = sf::seconds(FLICKER_DELAY);
                        _isInvisible = 1 - _isInvisible;
                        setVisible(!_isInvisible);
                    }
                }

                if(_isInvincible) {
                    invincibleTimer -= sf::seconds(dt);
                    if(invincibleTimer <= sf::Time::Zero) {
                        invincibleTimer = sf::seconds(INVINCIBLE_SWAPPING_COLOR_DELAY);
                        swapColorInvincibleAnimation();
                    }
                }
            }

            void handleEvent(const sf::RenderWindow *window, const sf::Event &event) override {
                Animation::handleEvent(window, event);
            }

            void render(sf::RenderWindow *window) override {
                Animation::render(window);
            }
    };
}