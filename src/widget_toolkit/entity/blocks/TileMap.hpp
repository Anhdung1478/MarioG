#pragma once

#include <algorithm>

#include "../player/player.hpp"
#include "../../resource/json.hpp"
#include "../../interfaces.hpp"
#include "../item/item.hpp"
#include "../item/ItemManager.hpp"
#include "../enemy/enemy.hpp"
#include "../enemy/goomba.hpp"
#include "../enemy/koopa.hpp"
#include "../enemy/piranha.hpp"
#include "../blockfactory.hpp"
#include "../enemy/lakitu.hpp"
  
namespace mario::entity {
    
    // ObjectData struct is now defined in ItemManager.hpp

    class TileMap : public mario::IScreenElement {
    private:
        std::unique_ptr<mario::entity::BlockFactory> blockFactory;

        sf::Texture tilesetTexture;
        std::unordered_map<int, std::string> tileProperties;
        std::vector<int> tileIds; // Store tile IDs for rendering
        std::vector<int> objectIds; // Store object IDs for rendering
        std::vector<ObjectData> objects; // Store object data for item spawning
        std::string mapPath;
        std::string tilesetPath;

        int levelState = 1;
        int themeID = 0;
        int mapWidth;
        int mapHeight;
        float tileWidth;  // Original tile size
        float tileHeight;
        int tilesetColumns;
        int margin;  // Margin around the tileset
        int spacing; // Spacing between tiles
    public:
        TileMap();
        TileMap(const std::string &tilesetPath, const std::string &mapPathm, int levelState, int themeID);
        ~TileMap();

        //const std::vector<ObjectData>& getObjects() const { return objects; }

        bool loadTileset(const std::string &tilesetPath);
        bool loadMap(const std::string &mapPath);
        bool loadObjects(std::vector<mario::entity::Enemy*> &enemies, 
                         std::vector<Item*> &items, std::vector<mario::entity::Block*> &blocks, 
                         std::vector<mario::entity::Block*> &groundBlocks, 
                         std::vector<mario::entity::Block*> &backgroundBlocks, 
                         mario::entity::FlagPole* &flagPole);

        void createBlock(std::vector<Block*> &blocks, std::vector<Block*> &backgroundBlocks);
        void sortBlocks(std::vector<Block*> &blocks);

        void update(const sf::RenderWindow *window, float dt) override;
        void handleEvent(const sf::RenderWindow *window, const sf::Event &event) override;
        void render(sf::RenderWindow *window) override;
        sf::FloatRect getWorldBounds() const;
    };
}