#pragma once

#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include "entity.hpp" 

namespace mario::entity {
    class Item : public Entity { 
        private:
            sf::Vector2f position;
            sf::Vector2f velocity;
            bool isMoving;
        public:
            Item(const std::string& jsonPath, const std::string& texturePath, 
                 sf::Vector2f scale, const std::string& spriteID, 
                 sf::Vector2f startPosition = {0, 0}, 
                 sf::Vector2f startVelocity = {0, 0}) 
                : Entity(jsonPath, texturePath, scale, spriteID), 
                  position(startPosition), velocity(startVelocity), isMoving(true) {
                    p_animation->setAnimationState(false);
                    p_animation->setSpriteAnimation(spriteID);
            }

            void setPosition(sf::Vector2f newPos) {
                position = newPos;
            }

            sf::Vector2f getPos() const {
                return position;
            }

            void setVelocity(sf::Vector2f newVelo) {
                velocity = newVelo;
            }

            sf::Vector2f getVelo() const {
                return velocity;
            }

            void setMoving(bool moving) {
                isMoving = moving;
            }

            bool getMoving() const {
                return isMoving;
            }

            void update(const sf::RenderWindow *window, float dt) override {
                if (isMoving) {
                    position += velocity * dt;
                }
                
                p_animation->update(window, dt);
            }

            void handleEvent(const sf::RenderWindow *window, const sf::Event &event) override {

            }

            void render(sf::RenderWindow *window) override {
                p_animation->renderWithPosition(window, position);
            }

            // For power-up transformations
            void changeSprite(const std::string& newSpriteID) {
                p_animation->setSpriteAnimation(newSpriteID);
            }
    };
}