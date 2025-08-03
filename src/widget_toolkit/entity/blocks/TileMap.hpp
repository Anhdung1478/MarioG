#pragma once

#include <algorithm>

#include "Block.hpp"
#include "Brick.hpp"
#include "QuestionBlock.hpp"
#include "SolidBlock.hpp"
#include "../player.hpp"
#include "../../resource/json.hpp"
#include "../../interfaces.hpp"
#include "../item/item.hpp"
#include "../item/ItemManager.hpp"
#include "../enemy/enemy.hpp"
  
namespace mario {
namespace entity {
  
// ObjectData struct is now defined in ItemManager.hpp

class TileMap : public mario::IScreenElement {
private:
    sf::Texture tilesetTexture;
    std::unordered_map<int, std::string> tileProperties;
    std::vector<int> tileIds; // Store tile IDs for rendering
    std::vector<int> objectIds; // Store object IDs for rendering
    std::vector<ObjectData> objects; // Store object data for item spawning

    int mapWidth;
    int mapHeight;
    float tileWidth;  // Original tile size
    float tileHeight;
    int tilesetColumns;
    int margin;  // Margin around the tileset
    int spacing; // Spacing between tiles
    void findItemBlockCollisions(int& L, int& R, const Item* item);
public:
    TileMap();
    TileMap(const std::string &tilesetPath, const std::string &mapPath);
    ~TileMap();

    const std::vector<ObjectData>& getObjects() const { return objects; }

    bool loadTileset(const std::string &tilesetPath);
    bool loadMap(const std::string &mapPath);

    void checkCollisionEn(mario::entity::Enemy* enemy);
    void createBlock(std::vector<Block*> &blocks);
    void sortBlocks(std::vector<Block*> &blocks);

    void update(const sf::RenderWindow *window, float dt) override;
    void handleEvent(const sf::RenderWindow *window, const sf::Event &event) override;
    void render(sf::RenderWindow *window) override;
    sf::FloatRect getWorldBounds() const;

    SideCollision checkItemCollision(Item* item);
    bool hasGroundAt(sf::Vector2f position);
    void handleItemCollision(Item* item);
};
}
}