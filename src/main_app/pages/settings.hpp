#pragma once

#include "page.hpp"
#include "../../widget_toolkit/controls/slider.hpp"
#include "../../widget_toolkit/command/key-binding-button.hpp"
#include "../../widget_toolkit/resource/json.hpp"
#include "../../widget_toolkit/sfml-extra-feature/rounded-rectangle-shape.hpp"
#include <memory>
#include <SFML/Graphics.hpp>
#include <fstream>

namespace mario::pages {
    enum class SettingsState {
        Main,
        Volume,
        KeyBindings
    };

    class SettingsPage : public Page {
    private:
        SettingsState currentState;
        std::unique_ptr<sf::Font> p_font;
        std::unique_ptr<sf::Text> p_title;

        std::unique_ptr<sf::Texture> backgroundTexture;
        std::unique_ptr<sf::Sprite> backgroundSprite;

        std::unique_ptr<sf::Texture> sliderBarTexture;
        std::unique_ptr<sf::Texture> sliderHandleTexture;
        std::unique_ptr<Slider> musicSlider;
        std::unique_ptr<Slider> sfxSlider;

        std::unique_ptr<sf::Texture> backTexture;
        std::unique_ptr<sf::Texture> backHoverTexture;
        std::unique_ptr<sf::Sprite> backSprite;

        std::function<void()> _deferredStateChange;
        
        std::vector<std::unique_ptr<KeyBindingButton>> keyBindings;
        std::unique_ptr<sf::RectangleShape> confirmButton;
        std::unique_ptr<sf::Text> confirmText;

        bool confirmClicked = false;
        sf::Clock confirmFlashClock;
        float confirmAnimDuration = 0.2f;

        bool backClicked = false;
        sf::Clock backAnimClock;
        float backAnimDuration = 0.3f;

        // Render textures and sprites for state previews
        std::unique_ptr<sf::RenderTexture> volumeRenderTexture;
        std::unique_ptr<sf::Sprite> volumeSprite;
        std::unique_ptr<sf::RenderTexture> keyBindingsRenderTexture;
        std::unique_ptr<sf::Sprite> keyBindingsSprite;

        std::unique_ptr<sf::RoundedRectangleShape> leftPanel;
        std::unique_ptr<sf::RoundedRectangleShape> rightPanel;

        std::unique_ptr<sf::Text> leftPanelTitle;
        std::unique_ptr<sf::Text> rightPanelTitle;
    public:
        SettingsPage(MainWindow& context);

        void handleEvent(const sf::RenderWindow* window, const sf::Event& event) override;
        void update(const sf::RenderWindow* window, float dt) override;
        void render(sf::RenderWindow* window) override;
    };
}