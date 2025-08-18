#pragma once

#include<bits/stdc++.h>
#include "../interfaces.hpp"
#include "animation/animation.hpp"
#include "box/box.hpp"

namespace mario::entity {
    class Player;

    class Entity : public IScreenElement {
        protected:
            Animation *p_animation;
            Box *p_body; // input position as middle at the bottom, it means if box has size (x, y) its origin will be (x / 2, y)
            bool _exist;

        public:
            Entity() : p_animation(nullptr), p_body(nullptr), _exist(true) {};
            
            Entity(std::string jsonPath, std::string texturePath, sf::Vector2f scale, const std::string& randomSpriteID, Box* body = nullptr) : _exist(true) {
                p_animation = new Animation(jsonPath, texturePath, scale, randomSpriteID);
                p_body = body;
            }

            Entity(const std::string& imagePath, sf::Vector2f _scale, const std::vector<SpriteData2>& sprites, Box* body = nullptr) : _exist(true) {
                p_animation = new Animation(imagePath, _scale, sprites);
                p_body = body;
            }

            virtual ~Entity() {
                delete p_animation;
                if(p_body != nullptr)
                    delete p_body;
            }
            
            void update(const sf::RenderWindow* window, float dt) override {
                if (p_animation && p_body) {
                    p_animation->update(window, dt);
                    p_body->update(dt);
                }
            }

            void handleEvent(const sf::RenderWindow* window, const sf::Event& event) override {}

            void render(sf::RenderWindow *window) override {
                if(p_animation && p_body)
                    p_animation->renderWithPosition(window, p_body->getPosition());
                
                bool isRenderWithHitbox = false;
                if(p_body && isRenderWithHitbox)
                    p_body->renderHitboxRect(window);
            }

            void setPosition(sf::Vector2f pos) {
                p_body->setPosition(pos);
            }

            sf::Vector2f getPosition() const {
                return p_body->getPosition();
            }

            sf::Vector2f getSize() const {
                return p_body->getSize();
            }

            sf::FloatRect getHitbox() const {
                return p_body->getHitbox();
            }

            bool isExist() const {
                return _exist;
            }

            void setExist(bool exist) {
                _exist = exist;
            }

            Box* getBody() const {
                return p_body;
            }

            Animation* getAnimation() const {
                return p_animation;
            }

            virtual void updateBehavior(float dt, Player* player = nullptr) {}
    };
}