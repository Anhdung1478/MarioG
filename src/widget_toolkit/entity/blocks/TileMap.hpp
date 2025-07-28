#pragma once

#include "Block.hpp"
#include "Brick.hpp"
#include "QuestionBlock.hpp"
#include "SolidBlock.hpp"
#include "../../resource/json.hpp"

class TileMap{
private:
    sf::Texture tilesetTexture;
    std::unordered_map<int, std::string> tileProperties;
    std::vector<int> tileIds; // Store tile IDs for rendering
    std::vector<int> objectIds; // Store object IDs for rendering
    int mapWidth;
    int mapHeight;
    int tileWidth;  // Original tile size
    int tileHeight;
    int tilesetColumns;
    int margin;  // Margin around the tileset
    int spacing; // Spacing between tiles
public:
    TileMap();
    TileMap(const std::string &tilesetPath, const std::string &mapPath);
    ~TileMap();

    bool loadTileset(const std::string &tilesetPath);
    bool loadMap(const std::string &mapPath);
    void createBlock(std::vector<std::unique_ptr<Block>> &Blocks);
};