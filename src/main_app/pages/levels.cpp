#include<bits/stdc++.h>
#include "levels.hpp"
#include "main-menu.hpp"
#include "../../widget_toolkit/resource/SoundManager.hpp"

mario::pages::LevelsPage::LevelsPage(MainWindow &context, LevelState state) : Page(context), levelState(state) {
    p_player = std::make_unique<mario::entity::Player>(_context->getWorldId(), sf::Vector2f(15, 10), mario::entity::CharacterListType::Luigi);
    p_inputManager = std::make_unique<mario::input::InputManager>();

    p_inputManager->bind(sf::Keyboard::Scancode::Left, std::make_unique<mario::input::RunCommand>(0));
    p_inputManager->bind(sf::Keyboard::Scancode::Right, std::make_unique<mario::input::RunCommand>(1));
    p_inputManager->bind(sf::Keyboard::Scancode::Up, std::make_unique<mario::input::JumpCommand>());
    p_inputManager->bind(sf::Keyboard::Scancode::Z, std::make_unique<mario::input::FireCommand>());

    // Pause/Resume game
    p_menuButtonListNode = new ButtonListNode();

    p_pauseMenu = std::make_unique<mario::ButtonList>(new ButtonListNode(), nullptr);
    mario::Button *p_button;
    p_button = new mario::Button("Resume");
    p_button->buttonRect = sf::FloatRect(sf::Vector2f(540, 340), sf::Vector2f(200, 30));
    p_button->Click.append([this]() { isPaused = false; });

    p_menuButtonListNode->buttonList.push_back(p_button);

    p_button = new mario::Button("Exit");
    p_button->buttonRect = sf::FloatRect(sf::Vector2f(540, 390), sf::Vector2f(200, 30));
    p_button->Click.append([this]() { _context->changePage(std::make_shared<mario::pages::MainMenuPage>(*_context)); });
    
    p_menuButtonListNode->buttonList.push_back(p_button);

    p_pauseMenu = std::make_unique<mario::ButtonList>(p_menuButtonListNode, nullptr); 

    p_questionBlock = std::make_unique<QuestionBlock>(_context->getWorldId(), sf::Vector2f(100, 600));
}

mario::pages::LevelsPage::~LevelsPage() {
    p_player.reset();
}

void mario::pages::LevelsPage::update(const sf::RenderWindow *window, float dt) {
    if(!isPaused) {
        p_player->update(window, dt);
        p_questionBlock->update(window, dt);
    }
}

void mario::pages::LevelsPage::handleEvent(const sf::RenderWindow *window, const sf::Event &event) {
    if (const auto key = event.getIf<sf::Event::KeyPressed>(); key && key->code == sf::Keyboard::Key::P) {
        isPaused = !isPaused;
        if(!isPaused && p_player) {
            p_player->run(false, true);
        }
    }
    
    if(!isPaused) {
        p_player->handleEvent(window, event);
        p_inputManager->handleEvent(*p_player, event);
        p_questionBlock->handleEvent(window, event);
    } else {
        p_pauseMenu->handleEvent(window, event);
    }
}

void mario::pages::LevelsPage::render(sf::RenderWindow *window) {
    p_player->render(window);
    p_questionBlock->render(window);
    if(isPaused) {
        p_pauseMenu->render(window);
    }
}