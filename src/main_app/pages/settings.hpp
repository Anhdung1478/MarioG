#pragma once

#include "page.hpp"
#include "../../widget_toolkit/controls/button.hpp"
#include "../../widget_toolkit/controls/button-list.hpp"
#include <memory>
#include <SFML/Graphics.hpp>

namespace mario::pages {
    class SettingsPage : public Page {
    private:
        std::unique_ptr<sf::Font> p_font;
        std::unique_ptr<sf::Text> p_title;

        mario::ButtonListNode *p_settingsButtonListNode;

        std::unique_ptr<mario::ButtonList> p_currButtonList;
        std::function<void()> _deferredStateChange;
        
    public:
        SettingsPage(MainWindow& context);

        void handleEvent(const sf::RenderWindow* window, const sf::Event& event) override;
        void update(const sf::RenderWindow* window, float dt) override;
        void render(sf::RenderWindow* window) override;
    };
}
