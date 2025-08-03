#include "Brick.hpp"

Brick::Brick(sf::Vector2f pos, sf::Vector2f size, std::string name) 
    : Block(pos, size, name) 
{
    InitSpritesSheet();
    p_animation = new mario::entity::Animation("../../asset/maps/Image/tiles-8.png", BLOCK_SCALE, sprites);
    p_animation->setSpriteAnimation(name);
    p_animation->setAnimationState(true);
}

void Brick::InitSpritesSheet(){
    sprites = {
        {"brick-block[0]", 1, 86, 16, 16},
        {"brick-block[1]", 35, 86, 16, 16},
        {"brick-block[2]", 69, 86, 16, 16}
    };
}

void Brick::reactToCollision(int side) {
    
}

void Brick::update(const sf::RenderWindow *window, float dt) {

}

void Brick::handleEvent(const sf::RenderWindow *window, const sf::Event &event) {
}

void Brick::render(sf::RenderWindow *window) {
    Entity::render(window);
}