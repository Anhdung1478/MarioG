#include<bits/stdc++.h>
#include "levels.hpp"

mario::pages::LevelsPage::LevelsPage(MainWindow &context, LevelState state) : Page(context) {
    p_player = std::make_unique<mario::entity::Player>("../../asset/entity/mario.png", _context->getWorldId());
    p_inputManager = std::make_unique<mario::input::InputManager>();

    p_inputManager->bind(sf::Keyboard::Scancode::Left, std::make_unique<mario::input::RunCommand>(0));
    p_inputManager->bind(sf::Keyboard::Scancode::Right, std::make_unique<mario::input::RunCommand>(1));
    p_inputManager->bind(sf::Keyboard::Scancode::Up, std::make_unique<mario::input::JumpCommand>());
    p_inputManager->bind(sf::Keyboard::Scancode::Z, std::make_unique<mario::input::FireCommand>());
}

void mario::pages::LevelsPage::update(const sf::RenderWindow *window, float dt) {
    p_player->update(window, dt);
}

void mario::pages::LevelsPage::handleEvent(const sf::RenderWindow *window, const sf::Event &event) {
    p_player->handleEvent(window, event);
    p_inputManager->handleEvent(*p_player, event);
}

void mario::pages::LevelsPage::render(sf::RenderWindow *window) {
    p_player->render(window);
}