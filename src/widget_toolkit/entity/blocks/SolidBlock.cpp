#include "SolidBlock.hpp"

namespace mario::entity {

    SolidBlock::SolidBlock() : Block()
    {
        // Default constructor
    }

    SolidBlock::SolidBlock(const sf::Vector2f &pos, const sf::Vector2f &size, const std::string &name) 
        : Block(pos, size, name)
    {
        InitSpritesSheet();

        p_animation = new mario::entity::Animation("../../asset/maps/Image/tiles-8.png", BLOCK_SCALE, sprites);
        p_animation->setSpriteAnimation(name);
        p_animation->setAnimationState(true);
    }

    SolidBlock::~SolidBlock() {
    }

    void SolidBlock::InitSpritesSheet() {
        sprites = {
            {"grass-ground[0]", 1, 1, 16, 16},
            {"grass-ground[1]", 18, 1, 16, 16},
            {"grass-ground[2]", 35, 1, 16, 16},
            {"grass-ground[3]", 1, 18, 16, 16},
            {"grass-ground[4]", 18, 18, 16, 16},
            {"grass-ground[5]", 35, 18, 16, 16},

            {"stone-ground[0]", 69, 1, 16, 16},
            {"stone-ground[1]", 86, 1, 16, 16},
            {"stone-ground[2]", 103, 1, 16, 16},
            {"stone-ground[3]", 69, 18, 16, 16},
            {"stone-ground[4]", 86, 18, 16, 16},
            {"stone-ground[5]", 103, 18, 16, 16},

            {"snow-ground[0]", 137, 1, 16, 16},
            {"snow-ground[1]", 154, 1, 16, 16},
            {"snow-ground[2]", 171, 1, 16, 16},
            {"snow-ground[3]", 137, 18, 16, 16},
            {"snow-ground[4]", 154, 18, 16, 16},
            {"snow-ground[5]", 171, 18, 16, 16},

            {"green-pipe[0]", 103, 52, 16, 16},
            {"green-pipe[1]", 120, 52, 16, 16},
            {"green-pipe[2]", 103, 69, 16, 16},
            {"green-pipe[3]", 120, 69, 16, 16},

            {"gray-pipe[0]", 188, 52, 16, 16},
            {"gray-pipe[1]", 205, 52, 16, 16},
            {"gray-pipe[2]", 188, 69, 16, 16},
            {"gray-pipe[3]", 205, 69, 16, 16},

            {"stair-block[0]", 1, 154, 16, 16},
            {"stair-block[1]", 18, 154, 16, 16},
            {"stair-block[2]", 35, 154, 16, 16}
        };
    }

    int SolidBlock::reactToCollision(int side, Player* player) {
        return -1;
    }

    void SolidBlock::update(const sf::RenderWindow *window, float dt) {

    }

    void SolidBlock::handleEvent(const sf::RenderWindow *window, const sf::Event &event) {

    }

    void SolidBlock::render(sf::RenderWindow *window) {
        Entity::render(window);
    }

} // namespace mario::entity