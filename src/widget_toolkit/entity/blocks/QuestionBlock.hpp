#pragma once
#include "Block.hpp"
#include "../animation/animation.hpp"

namespace mario::entity {

class Player;
class ItemManager;

class QuestionBlock : public Block {
public:
    QuestionBlock(){};
    QuestionBlock(sf::Vector2f pos, sf::Vector2f size, std::string name, int _typeOfItem, int _themeID);
    ~QuestionBlock();

    void InitSpritesSheet() override;
    int reactToCollision(int side, Player* player = nullptr) override;

    void update(const sf::RenderWindow *window, float dt) override;
    void handleEvent(const sf::RenderWindow *window, const sf::Event &event) override;
    void render(sf::RenderWindow *window) override;
    void onHit(Player* player, ItemManager* itemManager) override;
private:
    int typeOfItem = 0; // 0: coin, 1: red-mushroom, 2: fire-flower, 3: one-up-mushroom, 4: starman
    int themeID;
    int numberOfCoins;
    mario::entity::Animation *coins_animation;
    std::vector<SpriteData2> sprites;
    bool hasBeenHit = false;

    sf::Vector2f originalPosition;
    bool isBouncing = false;
    float bouncingTimer = 0.0f;
    float bouncingDistance = 0.0f;
    void bouncingAnimation(float dt);
};

} // namespace mario::entity