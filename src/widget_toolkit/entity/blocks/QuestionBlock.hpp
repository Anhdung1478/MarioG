#pragma once
#include "Block.hpp"
#include "../../resource/TextureManager.hpp"

namespace mario {
namespace entity {

class Player;
class ItemManager;

class QuestionBlock : public Block {
public:
    QuestionBlock(){};
    QuestionBlock(sf::Vector2f pos);
    QuestionBlock(sf::Vector2f pos, sf::Vector2f size, std::string name);
    ~QuestionBlock() = default;

    void InitSpritesSheet() override;
    
    void update(const sf::RenderWindow *window, float dt) override;
    void handleEvent(const sf::RenderWindow *window, const sf::Event &event) override;
    void render(sf::RenderWindow *window) override;
    void onHit(Player* player, ItemManager* itemManager) override;
private:
    std::vector<SpriteData2> sprites;
    bool hasBeenHit = false;
};

} // namespace entity
} // namespace mario