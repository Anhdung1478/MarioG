#pragma once
#include "../item.hpp"
#include "../../player.hpp"

namespace mario::entity {
    class OneupMushroom : public Item {
    public:
        OneupMushroom(const std::string& jsonPath, const std::string& texturePath, 
                      sf::Vector2f scale, const std::string& spriteID, 
                      sf::Vector2f position, sf::Vector2f size, sf::Vector2f velocity)
            : Item(ItemType::OneupMushroom, jsonPath, texturePath, scale, spriteID, position, size, velocity) {}

        void onCollect(Entity* collector) override {
            if (Player* player = dynamic_cast<Player*>(collector)) {
                player->collect1UpMushroom();
                collect();
            }
        }

        void update(const sf::RenderWindow* window, float dt) override {
            if (!isCollected()) {
                if (p_body) p_body->update(dt);
                if (p_animation) p_animation->update(window, dt);
            }
        }

        void handleEvent(const sf::RenderWindow* window, const sf::Event& event) override {
            // 1-up mushrooms typically don't need to handle events
            // This method is implemented to satisfy the IUpdateable interface
        }
    };
}

// Add reactCollision(): check collision to determine items' behviour upon collision
// add more functions for behaviour 