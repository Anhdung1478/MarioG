#pragma once
#include "../item.hpp"
#include "../../player.hpp"

namespace mario::entity {
    class FireFlower : public Item {
    public:
        FireFlower(sf::Vector2f startPosition)
            : Item(ItemType::FireFlower,
                   "../../../asset/sprites/fireflower.json", "../../../asset/sprites/mario_sheets.png",
                   {2.f, 2.f}, "fireflower",
                   startPosition, {16.f, 16.f}, {0.f, 0.f}) {}

        void onCollect(Entity* collector) override {
            // Add player's logic for turning into fire mode
            collect();
        }

        void update(const sf::RenderWindow* window, float dt) override {
            if (!isCollected()) {
                if (p_body) p_body->update(dt); // Might be static, but physics can still apply
                if (p_animation) p_animation->update(window, dt);
            }
        }
    };
}
