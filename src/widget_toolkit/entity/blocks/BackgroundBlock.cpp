#include "BackgroundBlock.hpp"

namespace mario::entity {

    BackgroundBlock::BackgroundBlock() : Block()
    {
        // Default constructor
    }

    BackgroundBlock::BackgroundBlock(const sf::Vector2f &pos, const sf::Vector2f &size, const std::string &name) 
        : Block(pos, size, name)
    {
        InitSpritesSheet();
        p_animation = new mario::entity::Animation("../../asset/maps/Image/tiles-8.png", BLOCK_SCALE, sprites);
        if(name == "grass[0][0]"){
            p_animation->addAnimationStep("grass[0][0]");
            p_animation->addAnimationStep("grass[0][1]");
            p_animation->addAnimationStep("grass[0][2]");
            p_animation->addAnimationStep("grass[0][3]");
        }
        else if(name == "grass[1][0]"){
            p_animation->addAnimationStep("grass[1][0]");
            p_animation->addAnimationStep("grass[1][1]");
            p_animation->addAnimationStep("grass[1][2]");
            p_animation->addAnimationStep("grass[1][3]");
        }
        else if(name == "grass[2][0]"){
            p_animation->addAnimationStep("grass[2][0]");
            p_animation->addAnimationStep("grass[2][1]");
            p_animation->addAnimationStep("grass[2][2]");
            p_animation->addAnimationStep("grass[2][3]");
        }
        else if(name == "enemies-flag[0]"){
            p_animation->addAnimationStep("enemies-flag[0]");
            p_animation->addAnimationStep("enemies-flag[1]");
            p_animation->addAnimationStep("enemies-flag[2]");
        }
        else if(name == "win-flag[0]"){
            p_animation->addAnimationStep("win-flag[0]");
            p_animation->addAnimationStep("win-flag[1]");
            p_animation->addAnimationStep("win-flag[2]");
            p_animation->addAnimationStep("win-flag[3]");
        }
        else if (name == "castle-flag[0]") {
            p_animation->addAnimationStep("castle-flag[0]");
            p_animation->addAnimationStep("castle-flag[1]");
            p_animation->addAnimationStep("castle-flag[2]");
            p_animation->addAnimationStep("castle-flag[3]");
        }
        p_animation->setTimeBetweenStep(1/7.0f);
        p_animation->setSpriteAnimation(name);
        p_animation->setAnimationState(true);
    }

    BackgroundBlock::BackgroundBlock(const sf::Vector2f &pos, const sf::Vector2f &size, const std::string &name, const SpriteData2& sprite)
        : Block(pos, size, name)
    {
        p_animation = new mario::entity::Animation("../../asset/maps/Image/tiles-8.png", BLOCK_SCALE, sprite);
        // p_animation->setSpriteAnimation(name);
        p_animation->setSpriteAnimation(name);
        p_animation->setAnimationState(false);
    }

    BackgroundBlock::BackgroundBlock(const std::string &imagePath, const sf::Vector2f &pos, const sf::Vector2f &size, const std::string &name, const SpriteData2 &sprite)
        : Block(pos * BLOCK_SCALE.x, size, name)
    {
        p_animation = new mario::entity::Animation(imagePath, BLOCK_SCALE, sprite);
        p_animation->setSpriteAnimation(name);
        p_animation->setAnimationState(false);
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
            {"grass[2][1]", 86, 35, 16, 16},
            {"grass[2][2]", 137, 35, 16, 16},
            {"grass[2][3]", 188, 35, 16, 16},

            // Flag
            {"enemies-flag[0]", 1, 171, 16, 16},
            {"enemies-flag[1]", 18, 171, 16, 16},
            {"enemies-flag[2]", 35, 171, 16, 16},

            {"win-flag[0]", 1, 188, 16, 16},
            {"win-flag[1]", 18, 188, 16, 16},
            {"win-flag[2]", 35, 188, 16, 16},
            {"win-flag[3]", 52, 188, 16, 16},

            {"castle-flag[0]", 1, 341, 16, 16},
            {"castle-flag[1]", 18, 341, 16, 16},
            {"castle-flag[2]", 1, 358, 16, 16},
            {"castle-flag[3]", 18, 358, 16, 16}

        };
    }

    int BackgroundBlock::reactToCollision(int side, Player* player) {
        return -1;
    }

    void BackgroundBlock::update(const sf::RenderWindow *window, float dt) {
        p_animation->update(window, dt);
    }

    void BackgroundBlock::handleEvent(const sf::RenderWindow *window, const sf::Event &event) {

    }

    void BackgroundBlock::render(sf::RenderWindow *window) {
        Entity::render(window);
    }

} // namespace mario::entity