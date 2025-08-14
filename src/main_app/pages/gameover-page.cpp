#include<bits/stdc++.h>
#include "gameover-page.hpp"

mario::pages::GameOverPage::GameOverPage(MainWindow &context) : Page(context) {
    marioFont = std::make_unique<sf::Font>("../../asset/fonts/SuperMario256.ttf");
    delayTimeBeforeBackToMainMenu = sf::seconds(5);

    gameOverText = std::make_unique<sf::Text>(*marioFont, "GAME OVER", 50);
    gameOverText->setFillColor(sf::Color::White);
    
    sf::Vector2f textSize = gameOverText->getGlobalBounds().size;
    sf::Vector2f windowSize = _context->getWindowSize();
    gameOverText->setPosition(sf::Vector2f((windowSize.x - textSize.x) / 2.f, (windowSize.y - textSize.y) / 2.f));
}

mario::pages::GameOverPage::~GameOverPage() {

}

void mario::pages::GameOverPage::update(const sf::RenderWindow *window, float dt) {
    delayTimeBeforeBackToMainMenu -= sf::seconds(dt);
    if(delayTimeBeforeBackToMainMenu <= sf::seconds(0))
        _context->changePage(std::make_shared<pages::ModeSelectPage>(*_context));
}

void mario::pages::GameOverPage::handleEvent(const sf::RenderWindow *window, const sf::Event &event) {

}

void mario::pages::GameOverPage::render(sf::RenderWindow *window) {
    window->draw(*gameOverText);
}