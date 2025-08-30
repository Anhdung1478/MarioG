#include "QuestionBlock.hpp"
#include "../item/ItemManager.hpp"

namespace mario::entity {

    QuestionBlock::QuestionBlock(sf::Vector2f pos, sf::Vector2f size, std::string name, int _typeOfItem, int _themeID) 
        : numberOfCoins(1), Block(pos, size, name), typeOfItem(_typeOfItem), themeID(_themeID) 
    {
        InitSpritesSheet();
        // p_animation = new mario::entity::Animation("asset/maps/Image/tiles-8.png", BLOCK_SCALE, sprites);
        p_animation = new mario::entity::Animation("asset/maps/blocks/question.png", BLOCK_SCALE, sprites);
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

        originalPosition = p_body->getPosition();
        bouncingDistance = p_body->getSize().y / 10.0f; // Set bouncing distance to 1/10 of the block height
    }

    QuestionBlock::~QuestionBlock() {
        delete coins_animation;
    }

    void QuestionBlock::InitSpritesSheet(){
        sprites = {
            {"question-block[0]", 0, 0, 16, 16},
            {"question-block[1]", 17, 0, 16, 16},
            {"question-block[2]", 34, 0, 16, 16}
        };
        // coins_animation = new mario::entity::Animation("asset/maps/Image/items-coins.png", BLOCK_SCALE, {
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
        // coins_animation = new mario::entity::Animation("asset/maps/Image/items-coins.png", BLOCK_SCALE, {
        //     {"coin[0]", 3, 73, 20, 16},
        //     {"coin[1]", 24, 73, 20, 16},
        //     {"coin[2]", 45, 73, 20, 16},
        //     {"coin[3]", 66, 73, 20, 16},
        //     {"coin[4]", 87, 73, 20, 16},
        //     {"coin[5]", 108, 73, 20, 16},
        //     {"coin[6]", 129, 73, 20, 16},
        //     {"coin[7]", 150, 73, 20, 16},
        //     {"coin[8]", 171, 73, 20, 16}
        // });
        coins_animation = new mario::entity::Animation("asset/maps/items/coin_effect.png", BLOCK_SCALE, {
            {"coin[0]", 0, 0, 20, 16},
            {"coin[1]", 21, 0, 20, 16},
            {"coin[2]", 42, 0, 20, 16},
            {"coin[3]", 63, 0, 20, 16},
            {"coin[4]", 84, 0, 20, 16},
            {"coin[5]", 105, 0, 20, 16},
            {"coin[6]", 126, 0, 20, 16},
            {"coin[7]", 147, 0, 20, 16},
            {"coin[8]", 168, 0, 20, 16}
        });
    }

    void QuestionBlock::changeToEmptyBlock() {
        delete p_animation;
        name = "empty_block_" + std::to_string(themeID + 1) + ".png";
        p_animation = new mario::entity::Animation(
            "asset/maps/blocks/empty_block_" + std::to_string(themeID + 1) + ".png", 
            BLOCK_SCALE, 
            {"empty_block_" + std::to_string(themeID + 1) + ".png", 0, 0, 16, 16}
        );
        // p_animation->setSpriteAnimation(name);
        p_animation->setAnimationState(false);
    }

    int QuestionBlock::reactToCollision(int side, Player* player) {
        if (side != SideCollision::Bottom) return -1; 
        // Coin
        if (typeOfItem == -1) {
            player->hitEmptyBlock();
            return -1;
        }
        else if (typeOfItem == 0) { 
            if (numberOfCoins == 0) return -1;

            numberOfCoins--;

            coins_animation->setAnimationState(true);

            isBouncing = true;

            if(numberOfCoins == 0){
                p_animation->setAnimationState(false);
                changeToEmptyBlock();
            }
            player->collectCoinInBlock();
        }
        else if (typeOfItem == 1) { // Red-mushroom or Fire-flower
            if (player->getPlayerStateType() == player_state::PlayerStateType::Small) {// Spawn Red mushroom
                p_animation->setAnimationState(false);
                changeToEmptyBlock();
                typeOfItem = -1; // Mark as empty
                return 1; 
            } else {// Spawn Fire flower
                p_animation->setAnimationState(false);
                changeToEmptyBlock();
                typeOfItem = -1; // Mark as empty
                return 2;
            }
            
        }
        else if (typeOfItem == 3) {// Spawn one-up mushroom
            p_animation->setAnimationState(false);
            changeToEmptyBlock();
            typeOfItem = -1; // Mark as empty
            return 3;
        }
        return -1;
    }
    
    void QuestionBlock::bouncingAnimation(float dt) {
        // std::cout << "Bouncing animation: " << bouncingTimer << "\n";
        if (bouncingTimer < 0.1f) {
            p_animation->move(sf::Vector2f(0.f, -bouncingDistance));
            p_body->move(sf::Vector2f(0.f, -bouncingDistance));
        } 
        else if (bouncingTimer < 0.2f) {
            p_animation->move(sf::Vector2f(0.f, +bouncingDistance));
            p_body->move(sf::Vector2f(0.f, +bouncingDistance));
        }
        else {
            bouncingTimer = 0.0f;
            isBouncing = false;
            p_animation->setPosition(originalPosition);
            p_body->setPosition(originalPosition);
        }
        bouncingTimer += dt;
    }

    void QuestionBlock::update(const sf::RenderWindow *window, float dt) {
        if (isBouncing) bouncingAnimation(dt);
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
        if (!hasBeenHit && itemManager) {
            hasBeenHit = true;
        }
    }

} // namespace mario::entity
