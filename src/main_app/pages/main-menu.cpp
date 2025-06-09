#include <bits/stdc++.h>
#include "main-menu.hpp"

mario::pages::MainMenuPage::MainMenuPage(mario::MainWindow &context) : Page(context) {
    p_font = std::make_unique<sf::Font>("../../asset/fonts/Cascadia.ttf");
    
    p_title = std::make_unique<sf::Text>(*p_font, "Super Mario Bros", 50);
    p_title->setPosition({400, 50});
    p_title->setFillColor(sf::Color::White);

    p_showMenu = std::make_unique<sf::Text>(*p_font, "Press any key to continue", 17);
    p_showMenu->setPosition({500, 500});
    p_showMenu->setFillColor(sf::Color::White);

    p_newGameButton = std::make_unique<mario::Button>();
    p_newGameButton->buttonText = "New Game";
    p_newGameButton->buttonRect = sf::FloatRect(sf::Vector2f(540, 490), sf::Vector2f(200, 30));
    p_newGameButton->enabled = false;
    p_newGameButton->Click.append([this]() {
        for (int i = 1; i <= NUM_LEVELS; ++i) {
            p_levelButton[i]->enabled = true;
        }

        p_newGameButton->enabled = p_continueButton->enabled = p_settingsButton->enabled = p_exitButton->enabled = false;
    });

    p_continueButton = std::make_unique<mario::Button>();
    p_continueButton->buttonText = "Continue";
    p_continueButton->buttonRect = sf::FloatRect(sf::Vector2f(540, 540), sf::Vector2f(200, 30));
    p_continueButton->enabled = false;
    p_continueButton->Click.append([this]() {
        
    });

    p_settingsButton = std::make_unique<mario::Button>();
    p_settingsButton->buttonText = "Settings";
    p_settingsButton->buttonRect = sf::FloatRect(sf::Vector2f(540, 590), sf::Vector2f(200, 30));
    p_settingsButton->enabled = false;
    p_settingsButton->Click.append([this]() {

    });

    p_exitButton = std::make_unique<mario::Button>();
    p_exitButton->buttonText = "Exit Game";
    p_exitButton->buttonRect = sf::FloatRect(sf::Vector2f(540, 640), sf::Vector2f(200, 30));
    p_exitButton->enabled = false;
    p_exitButton->Click.append([this]() {
        closeWindow();
    });

    for (int i = 1; i <= NUM_LEVELS; ++i) {
        p_levelButton[i] = std::make_unique<mario::Button>();
        p_levelButton[i]->buttonText = "Level " + std::to_string(i);
        p_levelButton[i]->buttonRect = sf::FloatRect(sf::Vector2f(540, 440 + 50 * i), sf::Vector2f(200, 30));
        p_levelButton[i]->enabled = false;
    }


}

void mario::pages::MainMenuPage::handleEvent(const sf::RenderWindow *window, const sf::Event &event) {
    if (_isMenuVisible == false && (event.is<sf::Event::KeyPressed>() || event.is<sf::Event::MouseButtonPressed>())) {
        _isMenuVisible = true;
        p_newGameButton->enabled = p_continueButton->enabled = p_settingsButton->enabled = p_exitButton->enabled = true;
    }

    p_newGameButton->handleEvent(window, event);
    p_continueButton->handleEvent(window, event);
    p_settingsButton->handleEvent(window, event);
    p_exitButton->handleEvent(window, event);

    for (int i = 1; i <= NUM_LEVELS; ++i)
        p_levelButton[i]->handleEvent(window, event);
}

void mario::pages::MainMenuPage::update(const sf::RenderWindow *window, float dt) {
    p_newGameButton->update(window, dt);
    p_continueButton->update(window, dt);
    p_settingsButton->update(window, dt);
    p_exitButton->update(window, dt);

    for (int i = 1; i <= NUM_LEVELS; ++i)
        p_levelButton[i]->update(window, dt);
}

void mario::pages::MainMenuPage::render(sf::RenderWindow *window) {
    window->draw(*p_title);
    if(!_isMenuVisible) {
        window->draw(*p_showMenu);
    } 

    p_newGameButton->render(window);
    p_continueButton->render(window);
    p_settingsButton->render(window);
    p_exitButton->render(window);

    for (int i = 1; i <= NUM_LEVELS; ++i)
        p_levelButton[i]->render(window);
}