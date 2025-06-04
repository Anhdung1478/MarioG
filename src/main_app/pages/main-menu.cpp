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
}

void mario::pages::MainMenuPage::handleEvent(const sf::Event &event) {
    if (event.is<sf::Event::KeyPressed>() || event.is<sf::Event::MouseButtonPressed>()) {
        _isMenuVisible = true;
    }
}

void mario::pages::MainMenuPage::update(float dt) {

}

std::unique_ptr<sf::RenderWindow> mario::pages::MainMenuPage::render(std::unique_ptr<sf::RenderWindow> window) {
    window->draw(*p_title);
    if(!_isMenuVisible) {
        window->draw(*p_showMenu);
    } else {
        
    }

    return window;
}