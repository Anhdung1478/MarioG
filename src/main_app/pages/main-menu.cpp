#include <bits/stdc++.h>
#include "main-menu.hpp"
#include "levels.hpp"
#include "select.hpp"
#include "settings.hpp"

mario::pages::MainMenuPage::MainMenuPage(mario::MainWindow &context) : Page(context) {
    p_font = std::make_unique<sf::Font>("../../asset/fonts/Cascadia.ttf");

    p_levelDataManager = std::make_unique<mario::resource::LevelDataManager>();
    
    p_title = std::make_unique<sf::Text>(*p_font, "Super Mario Bros", 50);
    p_title->setPosition({400, 50});
    p_title->setFillColor(sf::Color::White);

    p_showMenu = std::make_unique<sf::Text>(*p_font, "Press any key to continue", 17);
    p_showMenu->setPosition({500, 500});
    p_showMenu->setFillColor(sf::Color::White);

    p_menuButtonListNode = new ButtonListNode();
    p_newGameButtonListNode = new ButtonListNode();

    mario::Button *p_button;
    p_button = new mario::Button("New Game");
    p_button->buttonRect = sf::FloatRect(sf::Vector2f(540, 490), sf::Vector2f(200, 30));
    p_button->p_nodeOnButton = p_newGameButtonListNode;
    p_button->Click.append([this]() {});

    p_menuButtonListNode->buttonList.push_back(p_button);

    p_button = new mario::Button("Continue");
    p_button->buttonRect = sf::FloatRect(sf::Vector2f(540, 540), sf::Vector2f(200, 30));
    p_button->p_nodeOnButton = nullptr;
    p_button->Click.append([this]() {
        _context->changePage(std::make_shared<mario::pages::LevelsPage>(*_context, p_levelDataManager->loadAutoSaveLevelData()));
    });

    p_menuButtonListNode->buttonList.push_back(p_button);

    p_button = new mario::Button("Settings");
    p_button->buttonRect = sf::FloatRect(sf::Vector2f(540, 590), sf::Vector2f(200, 30));
    p_button->p_nodeOnButton = nullptr;
    p_button->Click.append([this]() {
        _context->changePage(std::make_shared<mario::pages::SettingsPage>(*_context));
    });

    p_menuButtonListNode->buttonList.push_back(p_button);

    p_button = new mario::Button("Exit Game");
    p_button->buttonRect = sf::FloatRect(sf::Vector2f(540, 640), sf::Vector2f(200, 30));
    p_button->p_nodeOnButton = nullptr;
    p_button->Click.append([this]() {
        closeWindow();
    });

    p_menuButtonListNode->buttonList.push_back(p_button);

    Button *p_levelButton[NUM_LEVELS + 1];
    for (int i = 1; i <= NUM_LEVELS; ++i) {
        p_levelButton[i] = new mario::Button("Level " + std::to_string(i));
        p_levelButton[i]->buttonRect = sf::FloatRect(sf::Vector2f(540, 440 + 50 * i), sf::Vector2f(200, 30));
        p_levelButton[i]->p_nodeOnButton = nullptr;
    }

    for (int i = 1; i <= NUM_LEVELS; i++) {
        p_levelButton[i]->Click.append([this, i]() {
            _context->changePage(std::make_shared<mario::pages::SelectPage>(*_context, i));
        });
    }

    for (int i = 1; i <= NUM_LEVELS; ++i) {
        p_newGameButtonListNode->buttonList.push_back(p_levelButton[i]);
    }

    p_currButtonList = std::make_unique<mario::ButtonList>(p_menuButtonListNode, nullptr);
}

void mario::pages::MainMenuPage::handleEvent(const sf::RenderWindow *window, const sf::Event &event) {
    if (_isMenuVisible == false && (event.is<sf::Event::KeyPressed>() || event.is<sf::Event::MouseButtonPressed>())) {
        _isMenuVisible = true;
        p_currButtonList->delay_time = 0.3f;
    }

    if(_isMenuVisible && p_currButtonList != nullptr) {
        p_currButtonList->handleEvent(window, event);
    }
}

void mario::pages::MainMenuPage::update(const sf::RenderWindow *window, float dt) {
    if(_isMenuVisible && p_currButtonList != nullptr) {
        p_currButtonList->update(window, dt);
    }
}

void mario::pages::MainMenuPage::render(sf::RenderWindow *window) {
    window->draw(*p_title);
    if(!_isMenuVisible) {
        window->draw(*p_showMenu);
    } else 
        if(p_currButtonList != nullptr) {
            p_currButtonList->render(window);
        }
}