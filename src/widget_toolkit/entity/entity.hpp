#pragma once

#include<bits/stdc++.h>
#include "../interfaces.hpp"
#include "animation/animation.hpp"
#include "box/box.hpp"

namespace mario::entity {
    class Entity : public IScreenElement {
        protected:
            Animation *p_animation;
            Box *p_body; // input position as middle at the bottom, it means if box has size (x, y) its origin will be (x / 2, y)

        public:
            Entity() : p_animation(nullptr), p_body(nullptr) {};
            
            Entity(std::string jsonPath, std::string texturePath, sf::Vector2f scale, const std::string& randomSpriteID) {
                p_animation = new Animation(jsonPath, texturePath, scale, randomSpriteID);
                p_body = nullptr;
            }

            Entity(const std::string& imagePath, sf::Vector2f _scale, const std::vector<SpriteData2>& sprites) {
                p_animation = new Animation(imagePath, _scale, sprites);
                p_body = nullptr;
            }

            virtual ~Entity() {
                delete p_animation;
                delete p_body;
            }

            void update(const sf::RenderWindow *window, float dt) override {
                
            }

            void render(sf::RenderWindow *window) override {
                p_animation->renderWithPosition(window, p_body->getPosition());
                p_body->renderHitboxRect(window);
            }

            void setPosition(sf::Vector2f pos) {
                if (p_body) {
                    p_body->setPosition(pos);
                }
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
    };
}