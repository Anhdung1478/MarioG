#pragma once

#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include "../entity.hpp"
#include "../box/box.hpp"
#include "../box/dynamic-box.hpp"

namespace mario::entity {
    enum class ItemType {
        Coin,
        RedMushroom,
        OneupMushroom
    };
    class Item : public Entity { 
        private:
            std::unique_ptr<DynamicBox> p_body;
            bool isMoving;
            ItemType type;
        public:
            Item(ItemType itemType,
                 const std::string& jsonPath, const std::string& texturePath, 
                 sf::Vector2f scale, const std::string& spriteID, 
                 b2WorldId worldId, sf::Vector2f startPosition = {0, 0},
                 sf::Vector2f dimension = {32, 32},
                 float density = 1.0f, float friction = 0.0f, bool isDynamicBody = true,
                 sf::Vector2f startVelocity = {0, 0})
                : Entity(jsonPath, texturePath, scale, spriteID), 
                  p_body(std::make_unique<DynamicBox>(worldId, startPosition, dimension, density, friction, isDynamicBody)),
                  isMoving(true), type(itemType) {
                    p_body->setVelocity(startVelocity);
                    p_animation->setAnimationState(false);
                    p_animation->setSpriteAnimation(spriteID);
            }

            ItemType getType() const {
                return type;
            }

            void enable() {
                p_body->enable();
            }

            void disable() {
                p_body->disable();
            }

            sf::Vector2f getPosition() const {
                return p_body->getPosition();
            }

            sf::Vector2f getDimension() const {
                return p_body->getDimension();
            }

            float getMass() const {
                return p_body->getMass();
            }

            void setVelocity(sf::Vector2f newVelo) {
                p_body->setVelocity(newVelo);
            }

            sf::Vector2f getVelocity() const {
                return p_body->getVelocity();
            }

            void setMoving(bool moving) {
                isMoving = moving;
            }

            bool getMoving() const {
                return isMoving;
            }

            void update(const sf::RenderWindow *window, float dt) override {                
                p_animation->update(window, dt);
            }

            void handleEvent(const sf::RenderWindow *window, const sf::Event &event) override {

            }

            void render(sf::RenderWindow *window) override {
                p_animation->renderWithPosition(window, getPosition());
            }

            // For power-up transformations
            void changeSprite(const std::string& newSpriteID) {
                p_animation->setSpriteAnimation(newSpriteID);
            }

            // For items' behaviour
            virtual void onCollect(Entity* collector) {
                // Each item will have a different onCollect method
            }
    };
}