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

void ItemManager::loadSpawnPoints(const std::vector<ObjectData>& objects) {
    spawnPoints.clear();
    
    for (const auto& obj : objects) {
        if (obj.itemType.empty()) continue; // Not an item object
        
        ItemSpawnData spawnData;
        
        // Convert string to ItemType enum
        if (obj.itemType == "coin") {
            spawnData.type = ItemType::Coin;
        } else if (obj.itemType == "red_mushroom") {
            spawnData.type = ItemType::RedMushroom;
        } else if (obj.itemType == "fire_flower") {
            spawnData.type = ItemType::FireFlower;
        } else if (obj.itemType == "1up_mushroom") {
            spawnData.type = ItemType::OneupMushroom;
        } else {
            continue; // Unknown item type
        }
        
        // Set position (will convert from Tiled coordinates)
        spawnData.position = sf::Vector2f(obj.x, obj.y);
        
        // Convert trigger type
        if (obj.triggerType == "immediate") {
            spawnData.trigger = SpawnTrigger::Immediate;
        } else if (obj.triggerType == "block_hit") {
            spawnData.trigger = SpawnTrigger::BlockHit;
        } else if (obj.triggerType == "proximity") {
            spawnData.trigger = SpawnTrigger::Proximity;
        } else {
            spawnData.trigger = SpawnTrigger::Immediate; // Default
        }
        
        spawnData.triggerID = obj.triggerID;
        spawnData.triggerRadius = obj.triggerRadius;
        spawnData.triggerDelay = obj.triggerDelay;
        spawnData.hasSpawned = false;
        
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

void ItemManager::processSpawnTriggers(const Player* player, float deltaTime) {
    for (auto& spawnData : spawnPoints) {
        if (spawnData.hasSpawned && !spawnData.isRepeatable) {
            continue; // Already spawned and not repeatable
        }
        
        bool shouldSpawn = false;
        
        switch (spawnData.trigger) {
            case SpawnTrigger::Immediate:
                if (!spawnData.hasSpawned) {
                    shouldSpawn = true;
                }
                break;
                
            case SpawnTrigger::Proximity:
                if (player) {
                    sf::Vector2f playerPos = player->getPosition();
                    sf::Vector2f distance = spawnData.position - playerPos;
                    float distanceMagnitude = std::sqrt(distance.x * distance.x + distance.y * distance.y);
                    
                    if (distanceMagnitude <= spawnData.triggerRadius) {
                        shouldSpawn = true;
                    }
                }
                break;
                
            case SpawnTrigger::Timed:
                spawnData.triggerDelay -= deltaTime;
                if (spawnData.triggerDelay <= 0.0f) {
                    shouldSpawn = true;
                }
                break;
                
            case SpawnTrigger::BlockHit:
            case SpawnTrigger::EnemyDeath:
            case SpawnTrigger::Hidden:
                // These are triggered externally via triggerSpawn()
                break;
        }
        
        if (shouldSpawn) {
            spawnFromData(spawnData);
            spawnData.hasSpawned = true;
        }
    }
}

void ItemManager::spawnFromData(const ItemSpawnData& spawnData) {
    // Get appropriate asset paths based on item type
    std::string jsonPath, texturePath, spriteID;
    sf::Vector2f size;
    
    switch (spawnData.type) {
        case ItemType::Coin:
            jsonPath = "../../asset/sprites/10-coin.json";
            texturePath = "../../asset/sprites/coins_sheet.png";
            spriteID = "10-coin.front[0]";
            size = sf::Vector2f(16.0f, 16.0f);
            break;
            
        case ItemType::RedMushroom:
            jsonPath = "../../asset/sprites/red-mushroom.json";
            texturePath = "../../asset/sprites/red_mushroom.png"; // use same png as goomba
            spriteID = "red-mushroom[0]";
            size = sf::Vector2f(16.0f, 16.0f);
            break;
            
        case ItemType::FireFlower:
            jsonPath = "../../asset/sprites/fireflower.json";
            texturePath = "../../asset/sprites/mario_sheets.png";
            spriteID = "fireflower[0]";
            size = sf::Vector2f(16.0f, 16.0f);
            break;
            
        case ItemType::OneupMushroom:
            // Similar to red mushroom but different sprite
            jsonPath = "../../asset/sprites/1up-mushroom.json";
            texturePath = "../../asset/sprites/1up_mushroom.png"; // use same png as goomba
            spriteID = "1up-mushroom[0]";
            size = sf::Vector2f(16.0f, 16.0f);
            break;
    }
    
    spawnItem(spawnData.type, jsonPath, texturePath, 
              sf::Vector2f(2.0f, 2.0f), spriteID, 
              spawnData.position, size, spawnData.initialVelocity);
}

void ItemManager::triggerSpawn(int triggerID, SpawnTrigger triggerType) {
    for (auto& spawnData : spawnPoints) {
        if (spawnData.triggerID == triggerID && 
            spawnData.trigger == triggerType && 
            (!spawnData.hasSpawned || spawnData.isRepeatable)) {
            
            spawnFromData(spawnData);
            spawnData.hasSpawned = true;
        }
    }
}

void ItemManager::update(const sf::RenderWindow* window, float dt) {
    for (auto& item : items) {
        item->update(window, dt);
    }

    // Update physics
    //updateItemPhysics(dt);

    // Check collisions
    checkPlayerCollisions();
    
    // Remove collected items
    removeCollectedItems();
    
    // Process spawn triggers
    processSpawnTriggers(playerRef, dt);
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

bool ItemManager::checkCollision(const Entity* entityA, const Entity* entityB) const {
    // Placeholder only
    return false;
}

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
