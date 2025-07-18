#pragma once

#include<bits/stdc++.h>
#include "../interfaces.hpp"
#include "animation/animation.hpp"
#include "box/box.hpp"

namespace mario::entity {
    class Entity : public IScreenElement {
        protected:
            Animation *p_animation;
            Box *p_body;

        public:
            Entity() : p_animation(nullptr), p_body(nullptr) {};
            
            Entity(std::string jsonPath, std::string texturePath, sf::Vector2f scale, const std::string& randomSpriteID) {
                p_animation = new Animation(jsonPath, texturePath, scale, randomSpriteID);
                p_body = nullptr;
            }

            Entity(const std::string& imagePath, sf::Vector2f _scale, const std::vector<SpriteData2>& sprites) {
                p_animation = new Animation(imagePath, _scale, sprites);
            }

            virtual ~Entity() {
                delete p_animation;
                delete p_body;
            }

            void update(const sf::RenderWindow *window, float dt) override {
                
            }

            void render(sf::RenderWindow *window) override {
                
            }
    };
}