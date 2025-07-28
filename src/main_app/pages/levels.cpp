#include<bits/stdc++.h>
#include "levels.hpp"

mario::pages::LevelsPage::LevelsPage(MainWindow &context, mario::resource::LevelState state) : Page(context) {
    p_player = std::make_unique<mario::entity::Player>(sf::Vector2f(15, 10), state.characterType, state.stateType);
    p_inputManager = std::make_unique<mario::input::InputManager>();

    p_inputManager->bind(sf::Keyboard::Scancode::Left, std::make_unique<mario::input::MoveCommand>(0));
    p_inputManager->bind(sf::Keyboard::Scancode::Right, std::make_unique<mario::input::MoveCommand>(1));
    p_inputManager->bind(sf::Keyboard::Scancode::Up, std::make_unique<mario::input::JumpCommand>());
    p_inputManager->bind(sf::Keyboard::Scancode::Z, std::make_unique<mario::input::FireCommand>());

    tileMap = std::make_unique<TileMap>("../../asset/maps/tiles-8.json", "../../asset/maps/Map_1.json");
    tileMap->createBlock(blocks);

    p_levelDataManager = std::make_unique<mario::resource::LevelDataManager>();
    currLevelState = state;
}

void mario::pages::LevelsPage::autoSave() {
    p_levelDataManager->autoSave(currLevelState);
}

mario::pages::LevelsPage::~LevelsPage() {
    autoSave();
    p_player.reset();
}

void mario::pages::LevelsPage::update(const sf::RenderWindow *window, float dt) {
    timeRemaining -= sf::seconds(dt);

    if(timeRemaining <= sf::seconds(0.f)) {
        // failed !!
    }

    p_player->update(window, dt);
    currLevelState.stateType = p_player->getPlayerStateType();
    p_levelDataManager->update(dt, currLevelState);
    
    std::cerr << "!";
    for(const auto &block : blocks) {
        block->update(window, dt);
    }
}

void mario::pages::LevelsPage::handleEvent(const sf::RenderWindow *window, const sf::Event &event) {
    p_player->handleEvent(window, event);
    p_inputManager->handleEvent(*p_player, event);

    for (const auto &block : blocks) {
        block->handleEvent(window, event);
    }
}

void mario::pages::LevelsPage::render(sf::RenderWindow *window) {
    for (const auto &block : blocks) {
        block->render(window);
    }

    p_player->render(window);
}