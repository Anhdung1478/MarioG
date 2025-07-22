#include "QuestionBlock.hpp"

QuestionBlock::QuestionBlock(b2WorldId worldId, sf::Vector2f pos) 
    : Block(worldId, pos, sf::Vector2f(16, 16), "question-block") 
{
    InitSpritesSheet();
    p_animation = new mario::entity::Animation("../../asset/maps/Image/tiles-8.png", BLOCK_SCALE, sprites);
    p_animation->addAnimationStep("question-block[0]");
    p_animation->addAnimationStep("question-block[1]");
    p_animation->addAnimationStep("question-block[2]");
    p_animation->setAnimationState(true);
}

QuestionBlock::QuestionBlock(b2WorldId worldId, sf::Vector2f pos, sf::Vector2f size, std::string name) 
    : Block(worldId, pos, size, name) 
{
    InitSpritesSheet();
    p_animation = new mario::entity::Animation("../../asset/maps/Image/tiles-8.png", sf::Vector2f(2.5f, 2.5f), sprites);
    p_animation->addAnimationStep("question-block[0]");
    p_animation->addAnimationStep("question-block[1]");
    p_animation->addAnimationStep("question-block[2]");
    p_animation->setAnimationState(true);
}

void QuestionBlock::InitSpritesSheet(){
    sprites = {
        {"question-block[0]", 1, 52, 16, 16},
        {"question-block[1]", 18, 52, 16, 16},
        {"question-block[2]", 35, 52, 16, 16}
    };
}
void QuestionBlock::update(const sf::RenderWindow *window, float dt) {
    p_animation->update(window, dt);
}

void QuestionBlock::handleEvent(const sf::RenderWindow *window, const sf::Event &event) {
}

void QuestionBlock::render(sf::RenderWindow *window) {
    p_animation->renderWithPosition(window, p_body->getPosition());
}