// mode-select.hpp
#pragma once
#include "page.hpp"
#include "../../widget_toolkit/networking/GameMode.hpp"
#include "../../widget_toolkit/networking/NetworkManager.hpp"
#include "../../widget_toolkit/entity/player/player-list.hpp"
#include "../../widget_toolkit/entity/player/player-state.hpp"
#include <SFML/Graphics/Font.hpp>
#include <memory>

namespace mario::pages {
    class ModeSelectPage : public Page {
    public:
        ModeSelectPage(MainWindow& context);
        void handleEvent(const sf::RenderWindow* window, const sf::Event& event) override;
        void render(sf::RenderWindow* window) override;
        void update(const sf::RenderWindow* window, float deltaTime) override;
    private:
        std::unique_ptr<sf::Font> marioFont;
        std::unique_ptr<sf::Text> _titleText;
        std::unique_ptr<sf::Text> _modeText;
        std::unique_ptr<sf::Text> _statusText;
        int _selectedLevel{1}; // Initialize to 1 or another default value
    };
}