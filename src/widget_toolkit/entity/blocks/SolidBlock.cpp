#include "SolidBlock.hpp"

SolidBlock::SolidBlock() 
    : Block(), texture(nullptr), sprite(nullptr) 
{
    // Default constructor
}

SolidBlock::SolidBlock(const sf::Vector2f &pos, const sf::Vector2f &size, const std::string &name) 
    : Block(pos, size, name), texture(nullptr), sprite(nullptr)
{
    InitSpritesSheet();
    // texture = new sf::Texture();
    // if (!texture->loadFromFile("../../asset/maps/Image/tiles-8.png")) {
    //     std::cerr << "Failed to load texture for SolidBlock" << std::endl;
    // }
    // sprite = new sf::Sprite(*texture);
    // sprite->setTextureRect(spritesSheet[name]);
    // sprite->setScale({size.x / sprite->getLocalBounds().size.x, size.y / sprite->getLocalBounds().size.y}); // Assuming the original sprite size is 16x16
    // sprite->setPosition(pos);

    p_animation = new mario::entity::Animation("../../asset/maps/Image/tiles-8.png", sf::Vector2f(2.5f, 2.5f), sprites);
    p_animation->setSpriteAnimation(name);
    p_animation->setAnimationState(true);
}

SolidBlock::~SolidBlock() {
    delete sprite;
    delete texture;
    delete p_animation;
}
void SolidBlock::InitSpritesSheet() {
    spritesSheet["grass-ground[0]"] = sf::IntRect({1, 1}, {16, 16});
    spritesSheet["grass-ground[1]"] = sf::IntRect({18, 1}, {16, 16});
    spritesSheet["grass-ground[2]"] = sf::IntRect({35, 1}, {16, 16});
    spritesSheet["grass-ground[3]"] = sf::IntRect({1, 18}, {16, 16});
    spritesSheet["grass-ground[4]"] = sf::IntRect({18, 18}, {16, 16});
    spritesSheet["grass-ground[5]"] = sf::IntRect({35, 18}, {16, 16});

    spritesSheet["stone-ground[0]"] = sf::IntRect({69, 1}, {16, 16});
    spritesSheet["stone-ground[1]"] = sf::IntRect({86, 1}, {16, 16});
    spritesSheet["stone-ground[2]"] = sf::IntRect({103, 1}, {16, 16});
    spritesSheet["stone-ground[3]"] = sf::IntRect({69, 18}, {16, 16});
    spritesSheet["stone-ground[4]"] = sf::IntRect({86, 18}, {16, 16});
    spritesSheet["stone-ground[5]"] = sf::IntRect({103, 18}, {16, 16});

    spritesSheet["snow-ground[0]"] = sf::IntRect({137, 1}, {16, 16});
    spritesSheet["snow-ground[1]"] = sf::IntRect({154, 1}, {16, 16});
    spritesSheet["snow-ground[2]"] = sf::IntRect({171, 1}, {16, 16});
    spritesSheet["snow-ground[3]"] = sf::IntRect({137, 18}, {16, 16});
    spritesSheet["snow-ground[4]"] = sf::IntRect({154, 18}, {16, 16});
    spritesSheet["snow-ground[5]"] = sf::IntRect({171, 18}, {16, 16});

    spritesSheet["green-pipe[0]"] = sf::IntRect({103, 52}, {16, 16});
    spritesSheet["green-pipe[1]"] = sf::IntRect({120, 52}, {16, 16});
    spritesSheet["green-pipe[2]"] = sf::IntRect({103, 69}, {16, 16});
    spritesSheet["green-pipe[3]"] = sf::IntRect({120, 69}, {16, 16});

    spritesSheet["gray-pipe[0]"] = sf::IntRect({188, 52}, {16, 16});
    spritesSheet["gray-pipe[1]"] = sf::IntRect({205, 52}, {16, 16});
    spritesSheet["gray-pipe[2]"] = sf::IntRect({188, 69}, {16, 16});
    spritesSheet["gray-pipe[3]"] = sf::IntRect({205, 69}, {16, 16});

    spritesSheet["stair-block[0]"] = sf::IntRect({1, 154}, {16, 16});
    spritesSheet["stair-block[1]"] = sf::IntRect({18, 154}, {16, 16});
    spritesSheet["stair-block[2]"] = sf::IntRect({35, 154}, {16, 16});

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

void SolidBlock::update(const sf::RenderWindow *window, float dt) {
    // if(p_animation) {
    //     p_animation->update(window, dt);
    // }
}

void SolidBlock::handleEvent(const sf::RenderWindow *window, const sf::Event &event) {

}

void SolidBlock::render(sf::RenderWindow *window) {
    Entity::render(window);
    //p_animation->renderWithPosition(window, p_body->getPosition());
    // if (sprite) {
    //     sprite->setPosition(p_body->getPosition());
    //     window->draw(*sprite);
    // }
}