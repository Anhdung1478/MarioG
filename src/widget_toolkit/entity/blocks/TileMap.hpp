#pragma once

#include <algorithm>

#include "Block.hpp"
#include "Brick.hpp"
#include "QuestionBlock.hpp"
#include "SolidBlock.hpp"
#include "../player.hpp"
#include "../../resource/json.hpp"
#include "../../interfaces.hpp"
#include "../enemy/enemy.hpp"

class TileMap : public mario::IScreenElement {
private:
    sf::Texture tilesetTexture;
    std::unordered_map<int, std::string> tileProperties;
    std::vector<int> tileIds; // Store tile IDs for rendering
    std::vector<int> objectIds; // Store object IDs for rendering
    int mapWidth;
    int mapHeight;
    float tileWidth;  // Original tile size
    float tileHeight;
    int tilesetColumns;
    int margin;  // Margin around the tileset
    int spacing; // Spacing between tiles
public:
    TileMap();
    TileMap(const std::string &tilesetPath, const std::string &mapPath);
    ~TileMap();

    bool loadTileset(const std::string &tilesetPath);
    bool loadMap(const std::string &mapPath);

    void checkCollisionEn(mario::entity::Enemy* enemy);
    void createBlock(std::vector<Block*> &blocks);
    void sortBlocks(std::vector<Block*> &blocks);

    void update(const sf::RenderWindow *window, float dt) override;
    void handleEvent(const sf::RenderWindow *window, const sf::Event &event) override;
    void render(sf::RenderWindow *window) override;
    sf::FloatRect getWorldBounds() const;
};