#pragma once

#include "../entity.hpp"
#include "item.hpp"
#include "ItemFactory.hpp"
#include <SFML/Graphics.hpp>

namespace mario::entity {
    class TileMap;
    class Player;
    class Entity;
    
    struct ObjectData {
        int gid;
        int x;
        int y;
        int width;
        int height;
        std::string name;
    };

    class ItemManager {
    private:
        std::vector<std::unique_ptr<Item>> items;
        std::vector<ItemSpawnData> spawnPoints;
        TileMap* tileMapRef = nullptr;
        Player* playerRef = nullptr;

        //void handleWalkingItemCollision(Item* item, float dt);
        //void handleBouncingItemCollision(Item* item, float dt);

    public:
        /*
        void spawnItem(ItemType type,
                    const std::string& jsonPath,
                    const std::string& texturePath,
                    sf::Vector2f scale,
                    const std::string& spriteID,
                    sf::Vector2f position,
                    sf::Vector2f size,
                    sf::Vector2f velocity = {0.f, 0.f});

        ItemType stringToItemType(const std::string& name);
        // Load spawn points from map data
        void loadSpawnPoints(const std::vector<ObjectData>& objects);
        */

        void update(const sf::RenderWindow* window, float dt);
        void render(sf::RenderWindow* window);

        void removeCollectedItems(); 

        const std::vector<std::unique_ptr<Item>>& getItems() const { return items; }

        //void setTileMapRef(TileMap* tileMap) { tileMapRef = tileMap; }
        //void setPlayerReference(Player* player) { playerRef = player; }
        
        //void handleCollectionEffects(Item* item);

        //void updateItemPhysics(float dt);
    };
} // namespace entity
