#pragma once

#include "items/coin.hpp"
#include "items/fireflower.hpp"
#include "items/oneup_mushroom.hpp"
#include "items/red_mushroom.hpp"
#include "item.hpp"

namespace mario::entity {

    class ItemFactory {
    public:
        static Item* createItem(ItemType type,
                                const std::string& jsonPath,
                                const std::string& texturePath,
                                sf::Vector2f scale,
                                const std::string& spriteID,
                                sf::Vector2f position,
                                sf::Vector2f size,
                                sf::Vector2f velocity = {0.f, 0.f})
        {
            switch (type) {
                case ItemType::Coin:
                    return new Coin(jsonPath, texturePath, scale, spriteID, position, size);
                case ItemType::RedMushroom:
                    return new RedMushroom(jsonPath, texturePath, scale, spriteID, position, size, velocity);
                case ItemType::OneupMushroom:
                    return new OneupMushroom(jsonPath, texturePath, scale, spriteID, position, size, velocity);
                case ItemType::FireFlower:
                    return new FireFlower(jsonPath, texturePath, scale, spriteID, position, size, velocity);
                default:
                    return nullptr; 
            }
        }
    };

}
