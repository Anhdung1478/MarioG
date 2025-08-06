#pragma once

#include<bits/stdc++.h>
#include<SFML/Graphics.hpp>
#include "../../interfaces.hpp"
#include "../../resource/TextureManager.hpp"

namespace mario::entity {
    
    class Animation : public IScreenElement {
        private:
            float TIME_BETWEEN_STEP = 1.0f / 10.0f;
            TextureManager& p_textureResource = TextureManager::getInstance();
            std::vector<SpriteData> animationSteps;
            sf::Sprite *p_sprite;
            sf::Vector2f scale;
            sf::Vector2f velocity;
            bool loop = true;

            sf::Time animationTimer;
            
            int step = 0;
            bool _isFaceForward = true;
            bool _isRunning = true;
            
            void setAnimationStep(int x) {
                step = x % int(animationSteps.size());
            }

            void setSprite(const SpriteData &data) {
                p_sprite->setTexture(*data.texture);
                p_sprite->setTextureRect(sf::IntRect({data.x, data.y}, {data.z, data.t}));
                p_sprite->setOrigin({data.z / 2.f, 1.f * data.t});
            }

            void performNextAnimation() {
                setSprite(animationSteps[step]);
                p_sprite->move(sf::Vector2f(velocity.x * step, velocity.y * step));
                if(++step >= int(animationSteps.size())){
                    step = 0;
                    if(!loop) {
                        _isRunning = false;
                        animationTimer = sf::seconds(0.f);
                    }
                }
            }
    
        public:
            Animation(const std::string& jsonPath, const std::string& texturePath, sf::Vector2f _scale, const std::string& randomSpriteID) : scale(_scale), velocity(sf::Vector2f(0.f, 0.f)) {
                p_textureResource.loadSheet(jsonPath, texturePath);

                SpriteData data = p_textureResource.getSpriteData(randomSpriteID);
                p_sprite = new sf::Sprite(*(data.texture));
                p_sprite->setScale(scale);
                setSprite(data);
            }

            Animation(const std::string& imagePath, sf::Vector2f _scale, const std::vector<SpriteData2>& sprites) : scale(_scale), velocity(sf::Vector2f(0.f, 0.f)) {
                p_textureResource.loadSheet(imagePath, sprites);

                SpriteData data = p_textureResource.getSpriteData(sprites[0].id);
                p_sprite = new sf::Sprite(*(data.texture));
                p_sprite->setScale(scale);
                setSprite(data);
            }

            Animation(const std::string& imagePath, sf::Vector2f _scale, const SpriteData2& sprite) : scale(_scale), velocity(sf::Vector2f(0.f, 0.f)) {
                p_textureResource.loadSheet(imagePath, sprite);

                SpriteData data = p_textureResource.getSpriteData(sprite.id);
                p_sprite = new sf::Sprite(*(data.texture));
                p_sprite->setScale(scale);
                setSprite(data);
            }

            ~Animation() override {
                delete p_sprite;
            }

            void loadSheet(const std::string& jsonPath, const std::string& texturePath) {
                p_textureResource.loadSheet(jsonPath, texturePath);
            }

            void addAnimationStep(const std::string& spriteID) {
                animationSteps.push_back(p_textureResource.getSpriteData(spriteID));
            }

            void clearAnimationStep() {
                animationSteps.clear();
                step = 0;
            }

            void setSpriteAnimation(const std::string &spriteID) {
                setSprite(p_textureResource.getSpriteData(spriteID));
            }
            
            void setAnimationState(bool running) {
                _isRunning = running;
                if(!_isRunning) {
                    setAnimationStep(0);
                    animationTimer = sf::seconds(0.f);
                }
            }

            bool getAnimationState() {
                return _isRunning;
            }

            int getStep() const {
                return step;
            }

            const sf::Texture& getTexture() const {
                return p_sprite->getTexture();
            }

            void move(sf::Vector2f offset) {
                p_sprite->move(offset);
            }
            
            void setTimeBetweenStep(float time) {
                TIME_BETWEEN_STEP = time;
            }

            void setPosition(sf::Vector2f pos) {
                p_sprite->setPosition(pos);
            }

            void setVelocity(sf::Vector2f v) {
                velocity = v;
            }

            void setLoop(bool value) {
                loop = value;
            }

            void rotate() {
                _isFaceForward = 1 - _isFaceForward;
                
                scale.x = -scale.x;
                p_sprite->setScale(scale);
            }

            bool isFaceForward() const {
                return _isFaceForward;
            }

            sf::Vector2f getScale() const {
                return scale;
            }

            sf::Vector2f getSize() const {
                return p_sprite->getGlobalBounds().size;
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