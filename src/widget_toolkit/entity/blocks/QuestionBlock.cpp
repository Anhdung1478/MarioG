#include "QuestionBlock.hpp"
#include "../item/ItemManager.hpp"

namespace mario {
namespace entity {

QuestionBlock::QuestionBlock(sf::Vector2f pos, sf::Vector2f size, std::string name, int _typeOfItem, int _themeID) 
    : numberOfCoins(1), Block(pos, size, name), typeOfItem(_typeOfItem), themeID(_themeID) 
{
    InitSpritesSheet();
    p_animation = new mario::entity::Animation("../../asset/maps/Image/tiles-8.png", BLOCK_SCALE, sprites);
    p_animation->addAnimationStep("question-block[0]");
    p_animation->addAnimationStep("question-block[1]");
    p_animation->addAnimationStep("question-block[2]");
    p_animation->setTimeBetweenStep(1/5.0f);
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

QuestionBlock::~QuestionBlock() {
    delete coins_animation;
}

void QuestionBlock::InitSpritesSheet(){
    sprites = {
        {"question-block[0]", 1, 52, 16, 16},
        {"question-block[1]", 18, 52, 16, 16},
        {"question-block[2]", 35, 52, 16, 16},
        {"empty-question-block[0]", 1, 69, 16, 16},
        {"empty-question-block[1]", 35, 69, 16, 16},
        {"empty-question-block[2]", 69, 69, 16, 16}
    };
    // coins_animation = new mario::entity::Animation("../../asset/maps/Image/items-coins.png", BLOCK_SCALE, {
    //     {"coin[0]", 12, 74, 2, 14},
    //     {"coin[1]", 31, 74, 6, 14},
    //     {"coin[2]", 50, 74, 10, 14},
    //     {"coin[3]", 73, 74, 6, 14},
    //     {"coin[4]", 93, 77, 7, 7},
    //     {"coin[5]", 110, 73, 15, 16},
    //     {"coin[6]", 130, 74, 18, 15},
    //     {"coin[7]", 150, 74, 20, 15},
    //     {"coin[8]", 172, 76, 18, 11}
    // });
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

void QuestionBlock::reactToCollision(int side, Player* player) {
    if (side != SideCollision::Bottom) return; 
    // Coin
    if (typeOfItem == 0) { 
        if (numberOfCoins == 0) return;

        numberOfCoins--;

        coins_animation->setAnimationState(true);

        if(numberOfCoins == 0){
            p_animation->setAnimationState(false);
            p_animation->setSpriteAnimation("empty-question-block[" + std::to_string(themeID) + "]");
        }
    }
    else if (typeOfItem == 1) { // Red-mushroom or Fire-flower
        if (player->getPlayerStateType() == player_state::PlayerStateType::Small) {
            // Spawn Red mushroom
        } else if (player->getPlayerStateType() == player_state::PlayerStateType::Super) {
            // Spawn Fire flower
        }
    }
    else if (typeOfItem == 2) { // One-up mushroom
        // Spawn one-up mushroom
    }
}
  
void QuestionBlock::update(const sf::RenderWindow *window, float dt) {
    p_animation->update(window, dt);
    coins_animation->update(window, dt);
}

void QuestionBlock::handleEvent(const sf::RenderWindow *window, const sf::Event &event) {
}

void QuestionBlock::render(sf::RenderWindow *window) {
    Entity::render(window);
    // coins_animation->setSpriteAnimation("coin[6]");
    // coins_animation->render(window);
    if(coins_animation->getAnimationState()) {
        if(coins_animation->getStep() == 4) coins_animation->setVelocity(sf::Vector2f(0.f, 0.f));
        coins_animation->render(window);
    }
}

void QuestionBlock::onHit(Player* player, ItemManager* itemManager) {
    if (!hasBeenHit) {
        hasBeenHit = true;

        // We can change to empty box sprite (lemme find :))
        // Play sound effect? (if we have)
        
        if (itemManager) {
            itemManager->triggerSpawn(blockID, SpawnTrigger::BlockHit);
        }
    }
}

} // namespace entity
} // namespace mario
