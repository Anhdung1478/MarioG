#include "QuestionBlock.hpp"

QuestionBlock::QuestionBlock(b2WorldId worldId, sf::Vector2f pos) : Block(worldId, "question-block", pos, sf::Vector2f(16, 16)) {
    p_animation = new mario::entity::Animation("../../asset/maps/Image/tiles-8.png", BLOCK_SCALE, sprites);

    InitSpritesSheet();
    p_animation->setAnimationState(true);
}

void QuestionBlock::InitSpritesSheet(){
    p_animation->addAnimationStep("question-block[0]");
    p_animation->addAnimationStep("question-block[1]");
    p_animation->addAnimationStep("question-block[2]");
}

void QuestionBlock::update(const sf::RenderWindow *window, float dt) {
    p_animation->update(window, dt);
}

void QuestionBlock::handleEvent(const sf::RenderWindow *window, const sf::Event &event) {
}

void QuestionBlock::render(sf::RenderWindow *window) {
    p_animation->renderWithPosition(window, p_body->getPosition());
}