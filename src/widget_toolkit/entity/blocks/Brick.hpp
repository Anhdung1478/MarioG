#pragma once
#include "Block.hpp"

namespace mario {
namespace entity {

class Brick : public Block {
public:
    Brick(){};
    Brick(sf::Vector2f pos, sf::Vector2f size, std::string name, int _typeOfItem = -1, int _themeID = 0, int _numberOfCoins = 0);
    ~Brick() = default;
    void InitSpritesSheet() override;
    void reactToCollision(int side, Player* player = nullptr) override;

    void update(const sf::RenderWindow *window, float dt) override;
    void handleEvent(const sf::RenderWindow *window, const sf::Event &event) override;
    void render(sf::RenderWindow *window) override;
private:
    // mario::entity::Animation *fragment_animation;
    int typeOfItem = -1; // -1 - None, 0 - coin, 1 - Red-mushroom, 2 - One-up-mushroom, 3 - Starman
    int themeID = 0;
    int numberOfCoins = 0;
    mario::entity::Animation *coins_animation;
    std::vector<SpriteData2> sprites;
};

} // namespace entity
} // namespace mario
