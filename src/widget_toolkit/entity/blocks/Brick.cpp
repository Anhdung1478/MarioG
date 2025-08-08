#include "Brick.hpp"

namespace mario::entity {

    Brick::Brick(sf::Vector2f pos, sf::Vector2f size, std::string name, int _typeOfItem, int _themeID, int _numberOfCoins) 
        : Block(pos, size, name), typeOfItem(_typeOfItem), themeID(_themeID), numberOfCoins(_numberOfCoins) 
    {
        InitSpritesSheet();
        name = name + "[" + std::to_string(themeID) + "]";
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

    int Brick::reactToCollision(int side, Player* player) {
        if (side != SideCollision::Bottom) return -1;
        if(typeOfItem == -1) {
            if(player->getPlayerStateType() == player_state::PlayerStateType::Small) {
                isBouncing = true;
            }
            else{
                createFragments();
                setExist(false);
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
                p_animation->setSpriteAnimation("empty-brick-block[" + std::to_string(themeID) + "]");
                typeOfItem = -2; // Mark as empty
            }
        }
        else if (typeOfItem == 1) { // Red-mushroom
            // Spawn Red mushroom
            if (player->getPlayerStateType() == player_state::PlayerStateType::Small) {
                // Spawn Red mushroom
                p_animation->setSpriteAnimation("empty-brick-block[" + std::to_string(themeID) + "]");
                typeOfItem = -2; // Mark as empty
                return 1; 
            } else if (player->getPlayerStateType() == player_state::PlayerStateType::Super) {
                // Spawn Fire flower
                p_animation->setSpriteAnimation("empty-brick-block[" + std::to_string(themeID) + "]");
                typeOfItem = -2; // Mark as empty
                return 2;
            }
        }
        else if (typeOfItem == 3) { // One-up mushroom
            // Spawn one-up mushroom
            p_animation->setSpriteAnimation("empty-brick-block[" + std::to_string(themeID) + "]");
            typeOfItem = -2; // Mark as empty
            return 3;
        }
        else if (typeOfItem == 4) { // Starman
            // Spawn Starman
            p_animation->setSpriteAnimation("empty-brick-block[" + std::to_string(themeID) + "]");
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
        const sf::Texture& texture = p_animation->getTexture();

        std::vector<sf::IntRect> fragmentRects = {
            sf::IntRect({{1, 103}, {8, 8}}),
            sf::IntRect({{10, 103}, {8, 8}}),
            sf::IntRect({{1, 111}, {8, 8}}),
            sf::IntRect({{10, 111}, {8, 8}}),

            sf::IntRect({{35, 103}, {8, 8}}),
            sf::IntRect({{44, 103}, {8, 8}}),
            sf::IntRect({{35, 111}, {8, 8}}),
            sf::IntRect({{44, 111}, {8, 8}}),

            sf::IntRect({{69, 103}, {8, 8}}),
            sf::IntRect({{78, 103}, {8, 8}}),
            sf::IntRect({{69, 111}, {8, 8}}),
            sf::IntRect({{78, 111}, {8, 8}}),
            
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
            BrickFragment fragment(position, texture, textureRect);

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