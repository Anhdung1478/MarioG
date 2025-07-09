#pragma once

#include<bits/stdc++.h>
#include<SFML/Graphics.hpp>
#include "../../interfaces.hpp"
#include "../../resource/TextureManager.hpp"

namespace mario::entity {
    constexpr static float TIME_BETWEEN_STEP = 1.0f / 5.0f;

    class Animation : public IScreenElement {
        private:
            std::unique_ptr<TextureManager> p_textureResource;
            std::unique_ptr<sf::Sprite> p_sprite;
            std::vector<SpriteData> animationSteps;
            sf::Vector2f scale;
            
            sf::Time animationTimer;
            
            int step = 0;
            bool _isFaceForward = true, _isRunning = true;
            
            void setAnimationStep(int x) {
                step = x % int(animationSteps.size());
            }

            void performNextAnimation() {
                setSprite(animationSteps[step]);
                if(++step >= int(animationSteps.size()))
                    step = 0;
            }
    
        public:
            Animation(const std::string& jsonPath, const std::string& texturePath, sf::Vector2f _scale, const std::string& randomSpriteID) : scale(_scale) {
                p_textureResource = std::make_unique<TextureManager>();
                p_textureResource->loadSheet(jsonPath, texturePath);

                SpriteData data = p_textureResource->getSpriteData(randomSpriteID);
                p_sprite = std::make_unique<sf::Sprite>(*(data.texture));
                p_sprite->setTextureRect(sf::IntRect({data.x, data.y}, {data.z, data.t}));
                p_sprite->setOrigin({data.z / 2.f, 1.f * data.t});
                p_sprite->setScale(scale);
            }

            void addAnimationStep(const std::string& spriteID) {
                animationSteps.push_back(p_textureResource->getSpriteData(spriteID));
            }


            void setSprite(const SpriteData &data) {
                p_sprite->setTextureRect(sf::IntRect({data.x, data.y}, {data.z, data.t}));
            }


            void setSpriteAnimation(const std::string &spriteID) {
                setSprite(p_textureResource->getSpriteData(spriteID));
            }
            
            void setAnimationState(bool running) {
                _isRunning = running;
                if(!_isRunning) {
                    setAnimationStep(0);
                    animationTimer = sf::seconds(0.f);
                }
            }

            void rotate() {
                _isFaceForward = 1 - _isFaceForward;
                
                scale.x = -scale.x;
                p_sprite->setScale(scale);
            }

            bool isFaceForward() {
                return _isFaceForward;
            }

            sf::Vector2f getScale() {
                return scale;
            }

            void update(const sf::RenderWindow *window, float dt) override {
                animationTimer += (_isRunning ? sf::seconds(dt) : sf::seconds(0));
                if(animationTimer >= sf::seconds(TIME_BETWEEN_STEP)) {
                    animationTimer -= sf::seconds(TIME_BETWEEN_STEP);
                    performNextAnimation();
                }
            }

            void handleEvent(const sf::RenderWindow *window, const sf::Event &event) override {

            }

            void render(sf::RenderWindow *window) override {
                window->draw(*p_sprite);
            }

            void renderWithPosition(sf::RenderWindow *window, sf::Vector2f pos) {
                p_sprite->setPosition(pos);
                render(window);
            }
    };
}