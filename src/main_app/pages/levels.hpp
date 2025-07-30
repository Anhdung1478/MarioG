#include<bits/stdc++.h>
#include "page.hpp"
#include "../../widget_toolkit/entity/player.hpp"
#include "../../widget_toolkit/command/input-manager.hpp"
#include "../../widget_toolkit/entity/blocks/TileMap.hpp"
#include "../../widget_toolkit/resource/LevelState.hpp"
#include "../../widget_toolkit/resource/LevelDataManager.hpp"
#include "../../widget_toolkit/camera/camera.hpp"

#include "../../widget_toolkit/controls/button.hpp"
#include "../../widget_toolkit/controls/button-list.hpp"
#include "../../widget_toolkit/controls/slider.hpp"

namespace mario::pages {
    class LevelsPage : public Page {
        private:
            std::unique_ptr<mario::input::InputManager> p_inputManager;
            std::unique_ptr<mario::resource::LevelDataManager> p_levelDataManager;
            std::unique_ptr<TileMap> tileMap;

            mario::entity::Player *p_player;
            sf::Time timeRemaining;
            Camera camera;

            // for Sound Manager
            mario::resource::LevelState currLevelState, levelState;

            // Pause/Resume game
            bool _isPaused = false;
            std::unique_ptr<sf::Texture> pauseTexture;
            std::unique_ptr<sf::Sprite> pauseSprite;
            std::unique_ptr<sf::Texture> pauseHoverTexture;
            
            std::unique_ptr<sf::Texture> panelTexture;
            std::unique_ptr<sf::Sprite> panelSprite;

            // Home
            std::unique_ptr<sf::Texture> homeTexture;
            std::unique_ptr<sf::Sprite> homeSprite;
            std::unique_ptr<sf::Texture> homeHoverTexture;

            // Settings
            std::unique_ptr<sf::Texture> settingsTexture;
            std::unique_ptr<sf::Sprite> settingsSprite;
            std::unique_ptr<sf::Texture> settingsHoverTexture;

            // Settings panel
            bool isSettingsOpen = false;
            std::unique_ptr<sf::Texture> settingsPanelTexture;
            std::unique_ptr<sf::Sprite> settingsPanelSprite;

            std::unique_ptr<sf::Font> font;
            std::unique_ptr<sf::Texture> sliderBarTexture;
            std::unique_ptr<sf::Texture> sliderHandleTexture;

            std::unique_ptr<Slider> musicSlider;
            std::unique_ptr<Slider> sfxSlider;

        public:
            LevelsPage(MainWindow &context, mario::resource::LevelState state);
            ~LevelsPage();
            void update(const sf::RenderWindow *window, float dt) override;
            void handleEvent(const sf::RenderWindow *window, const sf::Event &event) override;
            void render(sf::RenderWindow *window) override;
            void autoSave();

            // for Sound Manager
            mario::resource::LevelState getLevelState() const;

            // Pause Game
            bool isPaused() const;
    };
}