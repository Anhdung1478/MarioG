#pragma once

#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include "page.hpp"

namespace mario::pages {
    class MainMenuPage : public Page {
    private:
        std::unique_ptr<sf::Font> p_font;
        std::unique_ptr<sf::Text> p_title;
        std::unique_ptr<sf::Text> p_showMenu;

        std::function<void()> _deferredStateChange;
        bool _isMenuVisible = false;

    public:
        MainMenuPage(MainWindow &context);

        void update(float dt) override;

        void handleEvent(const sf::Event &event) override;

        std::unique_ptr<sf::RenderWindow> render(std::unique_ptr<sf::RenderWindow> window) override;
    };
}