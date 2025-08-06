#pragma once

#include "Block.hpp"

namespace mario::entity {

class SolidBlock : public Block {
private:
    std::vector<SpriteData2> sprites;
public:
    SolidBlock();
    SolidBlock(const sf::Vector2f &pos, const sf::Vector2f &size, const std::string &name);
    ~SolidBlock();

    void InitSpritesSheet() override;
    int reactToCollision(int side, Player* player = nullptr) override;

    void update(const sf::RenderWindow *window, float dt) override;
    void handleEvent(const sf::RenderWindow *window, const sf::Event &event) override;
    void render(sf::RenderWindow *window) override;
};

} // namespace mario::entity
