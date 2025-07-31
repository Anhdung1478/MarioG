#include "ItemManager.hpp"
#include "ItemFactory.hpp"

using namespace mario::entity;

void ItemManager::spawnItem(ItemType type,
                            const std::string& jsonPath,
                            const std::string& texturePath,
                            sf::Vector2f scale,
                            const std::string& spriteID,
                            sf::Vector2f position,
                            sf::Vector2f size,
                            sf::Vector2f velocity)
{
    Item* item = ItemFactory::createItem(type, jsonPath, texturePath, scale, spriteID, position, size, velocity);
    if (item)
        items.emplace_back(item); 
}

void ItemManager::update(const sf::RenderWindow* window, float dt) {
    for (auto& item : items) {
        item->update(window, dt);
    }
}

void ItemManager::render(sf::RenderWindow* window) {
    for (auto& item : items) {
        item->render(window);
    }
}

void ItemManager::removeCollectedItems() {
    items.erase(
        std::remove_if(items.begin(), items.end(),
            [](const std::unique_ptr<Item>& item) {
                return item->isCollected(); 
            }), 
        items.end()
    );
}
