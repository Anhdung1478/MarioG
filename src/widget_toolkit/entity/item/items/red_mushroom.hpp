#pragma once
#include "../item.hpp"
#include "../../player.hpp"

namespace mario::entity {
    class RedMushroom : public Item {
    public:
        RedMushroom(sf::Vector2f startPosition)
            : Item(ItemType::RedMushroom,
                   "../../../asset/sprites/red-mushroom.json", "../../../asset/sprites/red_mushroom.png",
                   {2.f, 2.f}, "red-mushroom",
                   startPosition, {16.f, 16.f}, {30.f, 0.f}) {}

        void onCollect(Entity* collector) override {
            // add player's logic to make mario grow
            collect();
        }

        void update(const sf::RenderWindow* window, float dt) override {
            if (!isCollected()) {
                if (p_body) p_body->update(dt);
                if (p_animation) p_animation->update(window, dt);
            }
        }
    };
}
