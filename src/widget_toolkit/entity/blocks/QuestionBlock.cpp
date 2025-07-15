#include "QuestionBlock.hpp"

QuestionBlock::QuestionBlock(sf::Vector2f pos) : Block(pos, sf::Vector2f(16, 16), "question-block") {
    InitSpritesSheet();
    p_animation->setAnimationState(true);
}

void QuestionBlock::InitSpritesSheet(){
    p_animation = new mario::entity::Animation("../../asset/maps/Image/tiles-8.png", sf::Vector2f(2.5f, 2.5f), sprites);
    p_animation->addAnimationStep("question-block[0]");
    p_animation->addAnimationStep("question-block[1]");
    p_animation->addAnimationStep("question-block[2]");
}

void QuestionBlock::update(const sf::RenderWindow *window, float dt) {
    if(p_animation) {
        p_animation->update(window, dt);
    }
}

void QuestionBlock::handleEvent(const sf::RenderWindow *window, const sf::Event &event) {
}

void QuestionBlock::render(sf::RenderWindow *window) {
    if(p_animation) {
        p_animation->renderWithPosition(window, getPosition());
    }
}