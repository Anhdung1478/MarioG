#pragma once

#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include "../../widget_toolkit/controls/button.hpp"
#include "page.hpp"

const int NUM_LEVELS = 3;

namespace mario::pages {
    class MainMenuPage : public Page {
        private:
            std::unique_ptr<sf::Font> p_font;
            std::unique_ptr<sf::Text> p_title;
            std::unique_ptr<sf::Text> p_showMenu;

            std::unique_ptr<mario::Button> p_newGameButton;
            std::unique_ptr<mario::Button> p_continueButton;
            std::unique_ptr<mario::Button> p_settingsButton;
            std::unique_ptr<mario::Button> p_exitButton;

            std::unique_ptr<mario::Button> p_levelButton[NUM_LEVELS + 1];

            std::function<void()> _deferredStateChange;
            bool _isMenuVisible = false;

        public:
            MainMenuPage(MainWindow &context);

            void update(const sf::RenderWindow *window, float dt) override;
            void handleEvent(const sf::RenderWindow *window, const sf::Event &event) override;
            void render(sf::RenderWindow *window) override;
    };
}