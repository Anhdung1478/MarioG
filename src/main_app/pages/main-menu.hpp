#pragma once

#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include "../../widget_toolkit/controls/button.hpp"
#include "../../widget_toolkit/controls/button-list.hpp"
#include "../../widget_toolkit/resource/LevelDataManager.hpp"
#include "page.hpp"

const int NUM_LEVELS = 3;

namespace mario::pages {
    class MainMenuPage : public Page {
        private:
            std::unique_ptr<sf::Font> p_font;
            std::unique_ptr<sf::Text> p_title;
            std::unique_ptr<sf::Text> p_showMenu;

            mario::ButtonListNode *p_newGameButtonListNode;
            mario::ButtonListNode *p_menuButtonListNode;

            std::unique_ptr<mario::ButtonList> p_currButtonList;

            std::unique_ptr<mario::resource::LevelDataManager> p_levelDataManager;

            std::function<void()> _deferredStateChange;
            bool _isMenuVisible = false;

        public:
            MainMenuPage(MainWindow &context);

            void update(const sf::RenderWindow *window, float dt) override;
            void handleEvent(const sf::RenderWindow *window, const sf::Event &event) override;
            void render(sf::RenderWindow *window) override;
    };
}