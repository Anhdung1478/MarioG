#include "Brick.hpp"

namespace mario::entity {

    Brick::Brick(sf::Vector2f pos, sf::Vector2f size, std::string name, int _typeOfItem, int _themeID, int _numberOfCoins) 
        : Block(pos, size, name), typeOfItem(_typeOfItem), themeID(_themeID), numberOfCoins(_numberOfCoins) 
    {
        InitSpritesSheet();
        name = "brick_" + std::to_string(themeID + 1) + ".png";
        // p_animation = new mario::entity::Animation("../../asset/maps/Image/tiles-8.png", BLOCK_SCALE, sprites);
        p_animation = new mario::entity::Animation(
            "../../asset/maps/blocks/brick_" + std::to_string(themeID + 1) + ".png", 
            BLOCK_SCALE, 
            {"brick_" + std::to_string(themeID + 1) + ".png", 0, 0, 16, 16}
        );
        p_animation->setSpriteAnimation(name);
        p_animation->setAnimationState(false);

        if(!textureFragments.loadFromFile("../../asset/maps/blocks/brick_fragment.png")) {
            std::cout << "can load brick fragments from tiles-8.png" << std::endl;
        }

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
        bouncingDistance = p_body->getSize().y / 10.0f;
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
        coins_animation = new mario::entity::Animation("../../asset/maps/items/coin_effect.png", BLOCK_SCALE, {
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
        // coins_animation = new mario::entity::Animation("../../asset/maps/Image/items-coins.png", BLOCK_SCALE, {
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
    }

    void Brick::changeToEmptyBlock() {
        delete p_animation;
        name = "empty_block_" + std::to_string(themeID + 1) + ".png";
        p_animation = new mario::entity::Animation(
            "../../asset/maps/blocks/empty_block_" + std::to_string(themeID + 1) + ".png", 
            BLOCK_SCALE, 
            {"empty_block_" + std::to_string(themeID + 1) + ".png", 0, 0, 16, 16}
        );
        // p_animation->setSpriteAnimation(name);
        p_animation->setAnimationState(false);
    }

    int Brick::reactToCollision(int side, Player* player) {
        if (side != SideCollision::Bottom) return -1;
        
        if(typeOfItem == -2) {
            player->hitEmptyBlock();
            return -1;
        }
        else if(typeOfItem == -1) {
            if(player->getPlayerStateType() == player_state::PlayerStateType::Small) {
                isBouncing = true;
            }
            else{
                createFragments();
                setExist(false);
                player->breakBrick();
                // break the brick block
            }
        }    
        else if (typeOfItem == 0) { // Coin
            if (numberOfCoins == 0) return -1;
            
            numberOfCoins--;

            isBouncing = true;
            
            coins_animation->setPosition(sf::Vector2f(originalPosition.x, originalPosition.y - 16 * BLOCK_SCALE.y));
            coins_animation->setVelocity(sf::Vector2f(0.f, -15.f));
            coins_animation->setAnimationState(true);

            if(numberOfCoins == 0){
                changeToEmptyBlock();
                typeOfItem = -2; // Mark as empty
            }
            player->collectCoinInBlock();
        }
        else if (typeOfItem == 1) { // Red-mushroom
            // Spawn Red mushroom
            if (player->getPlayerStateType() == player_state::PlayerStateType::Small) {
                // Spawn Red mushroom
                changeToEmptyBlock();
                typeOfItem = -2; // Mark as empty
                return 1; 
            } else {
                // Spawn Fire flower
                changeToEmptyBlock();
                typeOfItem = -2; // Mark as empty
                return 2;
            }
        }
        else if (typeOfItem == 3) { // One-up mushroom
            // Spawn one-up mushroom
            changeToEmptyBlock();
            typeOfItem = -2; // Mark as empty
            return 3;
        }
        else if (typeOfItem == 4) { // Starman
            // Spawn Starman
            changeToEmptyBlock();
            typeOfItem = -2; // Mark as empty
            return 4;
        }
        return -1; // No item to spawn
    }

    void Brick::bouncingAnimation(float dt) {
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

    void Brick::createFragments() {
        fragments.clear();

        std::vector<sf::IntRect> fragmentRects = {
            sf::IntRect({{0, 0}, {8, 8}}),
            sf::IntRect({{9, 0}, {8, 8}}),
            sf::IntRect({{0, 8}, {8, 8}}),
            sf::IntRect({{9, 8}, {8, 8}}),

            sf::IntRect({{17, 0}, {8, 8}}),
            sf::IntRect({{26, 0}, {8, 8}}),
            sf::IntRect({{17, 8}, {8, 8}}),
            sf::IntRect({{26, 8}, {8, 8}}),

            sf::IntRect({{34, 0}, {8, 8}}),
            sf::IntRect({{43, 0}, {8, 8}}),
            sf::IntRect({{34, 8}, {8, 8}}),
            sf::IntRect({{43, 8}, {8, 8}}),

        };

        int numberOfFragments = rand() % 5 + 4; // Random number of fragments between 4 and 8
        for(int i = 0; i < numberOfFragments; ++i) {
            // Randomly select a fragment rectangle
            // If themeID = 0, randomly select from 0 to 3 rectangles
            // If themeID = 1, randomly select from 4 to 7 rectangles
            // If themeID = 2, randomly select from 8 to 11 rectangles

            sf::IntRect textureRect;
            if (themeID == 0) {
                textureRect = fragmentRects[rand() % 4];
            } 
            else if (themeID == 1) {
                textureRect = fragmentRects[rand() % 4 + 4];
            } 
            else {
                textureRect = fragmentRects[rand() % 4 + 8];
            }

            // Randomly position the fragment around the brick's position
            sf::Vector2f position(
                p_body->getPosition().x + (rand() % 16 - 8), // Random offset within 8 pixels
                p_body->getPosition().y + (rand() % 16 - 8)  // Random offset within 8 pixels
            );
            BrickFragment fragment(position, textureFragments, textureRect);

            // Random scale between 2.0 and 2.5
            float scale = static_cast<float>(rand() % 6 + 20) / 10.0f; // Scale between 2.0 and 2.5
            fragment.sprite.setScale({scale, scale});
            fragment.sprite.setColor(sf::Color(255, 255, 255, 200 + rand() % 56)); // Semi-transparent
            fragments.push_back(fragment);
        }
    }

    void Brick::update(const sf::RenderWindow *window, float dt) {
        if (isBouncing) bouncingAnimation(dt);
        coins_animation->update(window, dt);
        if (!fragments.empty()) {
            for (auto &fragment : fragments) {
                fragment.update(dt);
                if(fragment.sprite.getPosition().y > window->getSize().y) {
                    // Remove fragment if it goes out of bounds
                    fragment = fragments.back();
                    fragments.pop_back();
                }
            }
        }
        shouldBeDeleted = fragments.empty() && !isExist();
    }

    void Brick::handleEvent(const sf::RenderWindow *window, const sf::Event &event) {
    }

    void Brick::render(sf::RenderWindow *window) {
        if(isExist())
            Entity::render(window);

        if(coins_animation->getAnimationState()) {
            if(coins_animation->getStep() == 4) coins_animation->setVelocity(sf::Vector2f(0.f, 0.f));
            coins_animation->render(window);
        }
        if (!fragments.empty()) {
            for (const auto &fragment : fragments) {
                window->draw(fragment.sprite);
            }
        }
    }

} // namespace mario::entity