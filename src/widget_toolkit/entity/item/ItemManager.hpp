#pragma once
#include <vector>
#include <memory>
#include "item.hpp"

namespace mario::entity {

class ItemManager {
private:
    std::vector<std::unique_ptr<Item>> items;

public:
    void spawnItem(ItemType type,
                   const std::string& jsonPath,
                   const std::string& texturePath,
                   sf::Vector2f scale,
                   const std::string& spriteID,
                   sf::Vector2f position,
                   sf::Vector2f size,
                   sf::Vector2f velocity = {0.f, 0.f});

    void update(const sf::RenderWindow* window, float dt);
    void render(sf::RenderWindow* window);

    void removeCollectedItems(); 

    const std::vector<std::unique_ptr<Item>>& getItems() const { return items; }
};

}
