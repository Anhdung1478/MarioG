#pragma once
#include "../item.hpp"
#include "../../player/player.hpp"

namespace mario::entity {
    class FireFlower : public Item {
    public:
        FireFlower(const std::string& jsonPath, const std::string& texturePath, 
                   sf::Vector2f scale, const std::string& spriteID, 
                   sf::Vector2f position, sf::Vector2f size, sf::Vector2f velocity)
            : Item(ItemType::FireFlower, jsonPath, texturePath, scale, spriteID, position, size, velocity) {}

        void onCollect(Entity* collector) override {
            if (Player* player = dynamic_cast<Player*>(collector)) {
                player->collectFireFlower();               
                collect();
            }
        }

        void update(const sf::RenderWindow* window, float dt) override {
            if (!isCollected()) {
                if (p_body) p_body->update(dt); // Might be static, but physics can still apply
                if (p_animation) p_animation->update(window, dt);
            }
        }

        void handleEvent(const sf::RenderWindow* window, const sf::Event& event) override {
            // Fire flowers typically don't need to handle events
            // This method is implemented to satisfy the IUpdateable interface
        }
    };
}
