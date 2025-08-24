#pragma once

#include<bits/stdc++.h>
#include "page.hpp"
#include "main-menu.hpp"
#include "mode-select.hpp"

namespace mario::pages {
    class GameOverPage : public Page {
        private:
            std::unique_ptr<sf::Font> marioFont;
            std::unique_ptr<sf::Text> gameOverText;
            std::unique_ptr<mario::resource::LevelDataManager> p_levelDataManager;

            sf::Time delayTimeBeforeBackToMainMenu;

        public:
            GameOverPage(MainWindow &context);
            ~GameOverPage() override;

            void update(const sf::RenderWindow *window, float dt) override;
            void handleEvent(const sf::RenderWindow *window, const sf::Event &event) override;
            void render(sf::RenderWindow *window) override;
    };
}