#pragma once
#include "../item.hpp"
#include "../../player.hpp"

namespace mario::entity {
    class OneupMushroom : public Item {
    public:
        OneupMushroom(sf::Vector2f startPosition)
            : Item(ItemType::OneupMushroom,
                   "../../../asset/sprites/1up-mushroom.json", "../../../asset/sprites/1up_mushroom.png",
                   {2.f, 2.f}, "1up-mushroom",
                   startPosition, {16.f, 16.f}, {30.f, 0.f}) {}

        void onCollect(Entity* collector) override {
            // add player's logic for adding 1 more life
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

// Add reactCollision(): check collision to determine items' behviour upon collision
// add more functions for behaviour 