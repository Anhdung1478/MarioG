#include "BackgroundBlock.hpp"

namespace mario {
namespace entity {

BackgroundBlock::BackgroundBlock() : Block()
{
    // Default constructor
}

BackgroundBlock::BackgroundBlock(const sf::Vector2f &pos, const sf::Vector2f &size, const std::string &name) 
    : Block(pos, size, name)
{
    InitSpritesSheet();
    p_animation = new mario::entity::Animation("../../asset/maps/Image/tiles-8.png", BLOCK_SCALE, sprites);
    p_animation->setSpriteAnimation(name);
    p_animation->setAnimationState(true);
}

BackgroundBlock::BackgroundBlock(const sf::Vector2f &pos, const sf::Vector2f &size, const std::string &name, const SpriteData2& sprite)
    : Block(pos, size, name)
{
    p_animation = new mario::entity::Animation("../../asset/maps/Image/tiles-8.png", BLOCK_SCALE, sprite);
    p_animation->setSpriteAnimation(name);
    p_animation->setAnimationState(true);
}


BackgroundBlock::~BackgroundBlock() {
}

void BackgroundBlock::InitSpritesSheet() {
    sprites = {
        // Grass
        {"grass[0][0]", 1, 35, 16, 16},
        {"grass[0][1]", 52, 35, 16, 16},
        {"grass[0][2]", 103, 35, 16, 16},
        {"grass[0][3]", 154, 35, 16, 16},

        {"grass[1][0]", 18, 35, 16, 16},
        {"grass[1][1]", 69, 35, 16, 16},
        {"grass[1][2]", 120, 35, 16, 16},
        {"grass[1][3]", 171, 35, 16, 16},

        {"grass[2][0]", 35, 35, 16, 16},
        {"grass[2][0]", 86, 35, 16, 16},
        {"grass[2][2]", 137, 35, 16, 16},
        {"grass[2][3]", 188, 35, 16, 16},

        // Flag
        {"flag-enemies[0]", 1, 171, 16, 16},
        {"flag-enemies[1]", 18, 171, 16, 16},
        {"flag-enemies[2]", 35, 171, 16, 16},
        
        {"win-flag[0]", 1, 171, 16, 16},
        {"win-flag[1]", 18, 171, 16, 16},
        {"win-flag[2]", 35, 171, 16, 16},
        {"win-flag[3]", 52, 171, 16, 16},

        // Flag Pole
        {"flag-pole[0]", 1, 137, 16, 16},
        {"flag-pole[1]", 18, 137, 16, 16},
        {"flag-pole[2]", 35, 137, 16, 16},
        {"flag-pole[3]", 1, 120, 16, 16},
        {"flag-pole[4]", 18, 120, 16, 16},

    };
}

void BackgroundBlock::reactToCollision(int side, Player* player) {

}

void BackgroundBlock::update(const sf::RenderWindow *window, float dt) {

}

void BackgroundBlock::handleEvent(const sf::RenderWindow *window, const sf::Event &event) {

}

void BackgroundBlock::render(sf::RenderWindow *window) {
    Entity::render(window);
}

} // namespace entity
} // namespace mario