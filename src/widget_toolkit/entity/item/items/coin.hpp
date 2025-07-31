#pragma once
#include "../item.hpp"
#include "../../player.hpp"

namespace mario::entity {
    class Coin : public Item {
    public:
        Coin(sf::Vector2f startPosition)
            : Item(ItemType::Coin,
                   "../../../asset/sprites/10-coin.json", "../../../asset/sprites/coins_sheet.png",
                   {2.f, 2.f}, "10-coin",
                   startPosition, {16.f, 16.f}, {0.f, 0.f}) {}

        void onCollect(Entity* collector) override {
            // Add player's logic for adding coins here
            collect();
        }

        void update(const sf::RenderWindow* window, float dt) override {
            if (!isCollected()) {
                if (p_body) p_body->update(dt);  // Optional, since coins usually don't move
                if (p_animation) p_animation->update(window, dt);
            }
        }
    };
}
