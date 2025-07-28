#pragma once

#include "Block.hpp"

class SolidBlock : public Block {
private:
    sf::Texture* texture;
    sf::Sprite* sprite;
    std::unordered_map<std::string, sf::IntRect> spritesSheet;

    std::vector<SpriteData2> sprites;
public:
    SolidBlock();
    // SolidBlock(const sf::Vector2f &pos, const sf::Vector2f &size, const std::string &name);
    SolidBlock(const sf::Vector2f &pos, const sf::Vector2f &size, const std::string &name);
    ~SolidBlock();

    void InitSpritesSheet() override;

    void update(const sf::RenderWindow *window, float dt) override;
    void handleEvent(const sf::RenderWindow *window, const sf::Event &event) override;
    void render(sf::RenderWindow *window) override;
};
