#pragma once
#include "Block.hpp"
#include "../animation/animation.hpp"

class QuestionBlock : public Block {
public:
    QuestionBlock(){};
    QuestionBlock(sf::Vector2f pos);
    QuestionBlock(sf::Vector2f pos, sf::Vector2f size, std::string name);
    ~QuestionBlock() = default;

    void InitSpritesSheet() override;
    void reactToCollision(int side) override;

    void update(const sf::RenderWindow *window, float dt) override;
    void handleEvent(const sf::RenderWindow *window, const sf::Event &event) override;
    void render(sf::RenderWindow *window) override;
private:
    int numberOfCoins;
    mario::entity::Animation *coins_animation;
    std::vector<SpriteData2> sprites;
};