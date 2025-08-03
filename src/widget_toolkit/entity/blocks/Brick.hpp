#pragma once
#include "Block.hpp"

namespace mario {
namespace entity {

class Brick : public Block {
public:
    Brick(){};
    Brick(sf::Vector2f pos, sf::Vector2f size, std::string name);
    ~Brick() = default;
    void InitSpritesSheet() override;
    void reactToCollision(int side) override;

    void update(const sf::RenderWindow *window, float dt) override;
    void handleEvent(const sf::RenderWindow *window, const sf::Event &event) override;
    void render(sf::RenderWindow *window) override;
private:
    // mario::entity::Animation *fragment_animation;
    std::vector<SpriteData2> sprites;
};

} // namespace entity
} // namespace mario
