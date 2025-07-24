#pragma once

#include "page.hpp"
#include "../../widget_toolkit/controls/slider.hpp"
#include <memory>
#include <SFML/Graphics.hpp>

namespace mario::pages {
    class SettingsPage : public Page {
    private:
        std::unique_ptr<sf::Font> p_font;
        std::unique_ptr<sf::Text> p_title;

        std::unique_ptr<sf::Texture> sliderBarTexture;
        std::unique_ptr<sf::Texture> sliderHandleTexture;
        std::unique_ptr<Slider> musicSlider;
        std::unique_ptr<Slider> sfxSlider;

        std::unique_ptr<sf::Texture> backTexture;
        std::unique_ptr<sf::Texture> backHoverTexture;
        std::unique_ptr<sf::Sprite> backSprite;

        std::function<void()> _deferredStateChange;
        
    public:
        SettingsPage(MainWindow& context);

        void handleEvent(const sf::RenderWindow* window, const sf::Event& event) override;
        void update(const sf::RenderWindow* window, float dt) override;
        void render(sf::RenderWindow* window) override;
    };
}