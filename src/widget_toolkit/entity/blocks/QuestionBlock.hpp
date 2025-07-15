#pragma once
#include "Block.hpp"
#include "../../resource/TextureManager.hpp"

class QuestionBlock : public Block {
public:
    QuestionBlock(){};
    QuestionBlock(sf::Vector2f pos);
    void loadSpritesSheet(const std::vector<SpriteData2>& sprites);
    void InitSpritesSheet();
    void update(const sf::RenderWindow *window, float dt) override;
    void handleEvent(const sf::RenderWindow *window, const sf::Event &event) override;
    void render(sf::RenderWindow *window) override;
    ~QuestionBlock() = default;

private:
    std::vector<SpriteData2> sprites = {
        {"question-block[0]", 1, 52, 16, 16},
        {"question-block[1]", 18, 52, 16, 16},
        {"question-block[2]", 35, 52, 16, 16}
    };
};