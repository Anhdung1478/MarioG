#pragma once

#include <algorithm>

#include "Block.hpp"
#include "Brick.hpp"
#include "QuestionBlock.hpp"
#include "SolidBlock.hpp"
#include "../player.hpp"
#include "../../resource/json.hpp"
#include "../../interfaces.hpp"


enum class SideCollision : uint8_t{
    Top    = 0,
    Bottom = 1,
    Left   = 2,
    Right  = 3,
    None   = 4
};

class TileMap : public mario::IScreenElement {
private:
    sf::Texture tilesetTexture;
    std::unordered_map<int, std::string> tileProperties;
    std::vector<int> tileIds; // Store tile IDs for rendering
    std::vector<int> objectIds; // Store object IDs for rendering
    std::vector<std::unique_ptr<Block>> blocks;
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
    void createBlock();

    // Sort the blocks based on their position X first and Y second
    void sortBlocks(); // Sort blocks based on their X and Y positions - X first, Y second
    void findBlocksCollisions(int &L, int &R, const std::unique_ptr<mario::entity::Player> &player);

    // SideCollision findCollisionSide(const std::unique_ptr<mario::entity::Entity> &EntityA, const std::unique_ptr<mario::entity::Entity> &EntityB);
    SideCollision findCollisionSide(const std::unique_ptr<mario::entity::Player> &EntityA, const std::unique_ptr<Block> &EntityB);
    void fixPosition(const std::unique_ptr<mario::entity::Player> &player, const std::unique_ptr<Block> &block, SideCollision side);
    void checkCollision(const std::unique_ptr<mario::entity::Player> &player);

    void update(const sf::RenderWindow *window, float dt) override;
    void handleEvent(const sf::RenderWindow *window, const sf::Event &event) override;
    void render(sf::RenderWindow *window) override;
    sf::FloatRect getWorldBounds() const;
};