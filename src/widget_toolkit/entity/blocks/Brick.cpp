#include "Brick.hpp"

namespace mario {
namespace entity {

Brick::Brick(sf::Vector2f pos, sf::Vector2f size, std::string name, int _typeOfItem, int _themeID, int _numberOfCoins) 
    : Block(pos, size, name), typeOfItem(_typeOfItem), themeID(_themeID), numberOfCoins(0) 
{
    InitSpritesSheet();
    p_animation = new mario::entity::Animation("../../asset/maps/Image/tiles-8.png", BLOCK_SCALE, sprites);
    p_animation->setSpriteAnimation(name);
    p_animation->setAnimationState(true);

    coins_animation->addAnimationStep("coin[0]");
    coins_animation->addAnimationStep("coin[1]");
    coins_animation->addAnimationStep("coin[2]");
    coins_animation->addAnimationStep("coin[3]");
    coins_animation->addAnimationStep("coin[4]");
    coins_animation->addAnimationStep("coin[5]");
    coins_animation->addAnimationStep("coin[6]");
    coins_animation->addAnimationStep("coin[7]");
    coins_animation->addAnimationStep("coin[8]");
    coins_animation->setPosition(sf::Vector2f(p_body->getPosition().x, p_body->getPosition().y - p_body->getSize().y));
    coins_animation->setVelocity(sf::Vector2f(0.f, -15.f));
    coins_animation->setTimeBetweenStep(1/17.0f);
    coins_animation->setLoop(false);
    coins_animation->setAnimationState(false);
}

void Brick::InitSpritesSheet(){
    sprites = {
        {"brick-block[0]", 1, 86, 16, 16},
        {"brick-block[1]", 35, 86, 16, 16},
        {"brick-block[2]", 69, 86, 16, 16},

        {"brick-block-move[0]", 18, 86, 16, 16},
        {"brick-block-move[1]", 52, 86, 16, 16},
        {"brick-block-move[2]", 86, 86, 16, 16},

        {"empty-brick-block[0]", 1, 69, 16, 16},
        {"empty-brick-block[1]", 35, 69, 16, 16},
        {"empty-brick-block[2]", 69, 69, 16, 16}
    };
    coins_animation = new mario::entity::Animation("../../asset/maps/Image/items-coins.png", BLOCK_SCALE, {
        {"coin[0]", 3, 73, 20, 16},
        {"coin[1]", 24, 73, 20, 16},
        {"coin[2]", 45, 73, 20, 16},
        {"coin[3]", 66, 73, 20, 16},
        {"coin[4]", 87, 73, 20, 16},
        {"coin[5]", 108, 73, 20, 16},
        {"coin[6]", 129, 73, 20, 16},
        {"coin[7]", 150, 73, 20, 16},
        {"coin[8]", 171, 73, 20, 16}
    });
}

void Brick::reactToCollision(int side, Player* player) {
    if (side != SideCollision::Bottom) return; 
    
    if (typeOfItem == 0) { // Coin
        if (numberOfCoins == 0) return;

        numberOfCoins--;

        coins_animation->setAnimationState(true);

        if(numberOfCoins == 0){
            p_animation->setAnimationState(false);
            p_animation->setSpriteAnimation("empty-brick-block[" + std::to_string(themeID) + "]");
        }
    }
    else if (typeOfItem == 1) { // Red-mushroom
        // Spawn Red mushroom
    }
    else if (typeOfItem == 2) { // One-up mushroom
        // Spawn one-up mushroom
    }
    else if (typeOfItem == 3) { // Starman
        // Spawn Starman
    }
}

void Brick::update(const sf::RenderWindow *window, float dt) {

}

void Brick::handleEvent(const sf::RenderWindow *window, const sf::Event &event) {
}

void Brick::render(sf::RenderWindow *window) {
    Entity::render(window);
}

} // namespace entity
} // namespace mario