#include "ItemManager.hpp"
#include "ItemFactory.hpp"

using namespace mario::entity;

/*
void ItemManager::spawnItem(ItemType type,
                            const std::string& jsonPath,
                            const std::string& texturePath,
                            sf::Vector2f scale,
                            const std::string& spriteID,
                            sf::Vector2f position,
                            sf::Vector2f size,
                            sf::Vector2f velocity)
{
    //Item* item = ItemFactory::createItem(type, jsonPath, texturePath, scale, spriteID, position, size, velocity);
    //if (item)
        //items.emplace_back(item); 
}

ItemType ItemManager::stringToItemType(const std::string& name) {
    if (name == "coin") return ItemType::Coin;
    if (name == "red_mushroom") return ItemType::RedMushroom;
    if (name == "oneup_mushroom") return ItemType::OneupMushroom;
    if (name == "fire_flower") return ItemType::FireFlower;
    if (name == "starman") return ItemType::Starman;

    throw std::runtime_error("Unknown item name: " + name);
}

void ItemManager::loadSpawnPoints(const std::vector<ObjectData>& objects) {
    spawnPoints.clear();
    
    for (const auto& obj : objects) {
        if (obj.name.empty()) continue; // Not an item object
        
        ItemSpawnData spawnData;
        
        spawnData.type = stringToItemType(obj.name);

        // Set position (will convert from Tiled coordinates)
        spawnData.position = sf::Vector2f(obj.x, obj.y);
        
        // Set initial velocity based on item type
        if (spawnData.type == ItemType::RedMushroom || 
            spawnData.type == ItemType::OneupMushroom) {
            spawnData.initialVelocity = sf::Vector2f(30.0f, -50.0f); // Move right, pop up
        } else {
            spawnData.initialVelocity = sf::Vector2f(0.0f, 0.0f); // Static
        }
        
        spawnPoints.push_back(spawnData);
    }
}
*/

void ItemManager::update(const sf::RenderWindow* window, float dt) {
    for (auto& item : items) {
        item->update(window, dt);
    }

    // Update physics
    //updateItemPhysics(dt);

    // Check collisions
    //checkPlayerCollisions(); // This is handled in CollisionManager.cpp, should I still keep it?
    
    // Remove collected items
    removeCollectedItems();
    
}

void ItemManager::render(sf::RenderWindow* window) {
    for (auto& item : items) {
        item->render(window);
    }
}

void ItemManager::removeCollectedItems() {
    items.erase(
        std::remove_if(items.begin(), items.end(),
            [](const auto& item) {
                return item->isCollected(); 
            }), 
        items.end()
    );
}

/*
void ItemManager::checkPlayerCollisions() {
    if (!playerRef) return;
    
    // Get player bounds for optimization
    sf::Vector2f playerPos = playerRef->getPosition();
    const float COLLISION_RANGE = 100.0f; // Only check items within range
    
    for (auto& item : items) {
        if (item->isCollected()) continue;
        
        // Quick distance check for optimization
        sf::Vector2f itemPos = item->getPosition();
        sf::Vector2f distance = itemPos - playerPos;
        
        if (std::abs(distance.x) > COLLISION_RANGE || std::abs(distance.y) > COLLISION_RANGE) {
            continue;
        }
        
        // Perform actual collision check using existing logic
       
        if (checkCollision(playerRef, item.get())) {
            // Trigger item collection
            item->onCollect(playerRef);
            
            // Handle collection effects
            handleCollectionEffects(item.get());
        }
    }
}


void ItemManager::handleCollectionEffects(Item* item) {
    // This method can be used for sound effects, visual effects, etc.
    // For now, it's a placeholder for future enhancements
    switch (item->getType()) {
        case ItemType::Coin:
            // Future: Play coin sound, add score
            break;
            
        case ItemType::RedMushroom:
            // Future: Play power-up sound, add score
            break;
            
        case ItemType::FireFlower:
            // Future: Play power-up sound, add score
            break;
            
        case ItemType::OneupMushroom:
            // Future: Play 1-up sound, add life
            break;
    }
}
*/