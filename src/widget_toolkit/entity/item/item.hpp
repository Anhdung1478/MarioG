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
        OneupMushroom,
        FireFlower
    };
    class Item : public Entity { 
        protected:
            bool isMoving = true;
            ItemType type;
            bool isCollectedFlag = false;

            void collect() { isCollectedFlag = true; }
        public:
            Item(ItemType itemType,
                const std::string& jsonPath, const std::string& texturePath, 
                sf::Vector2f scale, const std::string& spriteID, 
                sf::Vector2f startPosition,
                sf::Vector2f dimension,
                sf::Vector2f startVelocity) : Entity(jsonPath, texturePath, scale, spriteID),
                type(itemType) {
                    p_body = new DynamicBox(startPosition, dimension);
                    p_body->setVelocity(startVelocity);
            }
            
            virtual ~Item() = default;

            virtual void onCollect(Entity* collector) = 0;

            // Update logic (animation, physics)
            virtual void update(const sf::RenderWindow* window, float dt) override {
                if (p_body) p_body->update(dt);
                if (p_animation) p_animation->update(window, dt);
            }

            // Render logic
            virtual void render(sf::RenderWindow* window) override {
                if (p_animation && p_body) {
                    p_animation->renderWithPosition(window, p_body->getPosition());
                }
                if (p_body) {
                    p_body->renderHitboxRect(window);
                }
            }

            // Getter for item type
            ItemType getType() const { return type; }

            // Check if collected
            bool isCollected() const { return isCollectedFlag; }
    };
}
