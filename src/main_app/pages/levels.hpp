#include<bits/stdc++.h>
#include "page.hpp"
#include "../../widget_toolkit/entity/player.hpp"
#include "../../widget_toolkit/command/input-manager.hpp"
#include "../../widget_toolkit/entity/blocks/QuestionBlock.hpp"

#include "../../widget_toolkit/controls/button.hpp"
#include "../../widget_toolkit/controls/button-list.hpp"
#include "../../widget_toolkit/controls/slider.hpp"

namespace mario::pages {
    struct LevelState {
        int level, score, coins, num_lives, player_type;

        LevelState(int _level = 0, int _lives = 0, int _type = 0, int _score = 0, int _coins = 0) : level(_level), score(_score), coins(_coins), num_lives(_lives), player_type(_type) {};
        LevelState(const LevelState &state) : level(state.level), score(state.score), num_lives(state.num_lives), player_type(state.player_type) {};
    };

    class LevelsPage : public Page {
        private:
            std::unique_ptr<mario::entity::Player> p_player;
            std::unique_ptr<mario::input::InputManager> p_inputManager;
            std::unique_ptr<QuestionBlock> p_questionBlock;

            // for Sound Manager
            LevelState levelState;

            // Pause/Resume game
            bool isPaused = false;
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
            friend class MainMenuPage;
        public:
            LevelsPage(MainWindow &context, LevelState state);
            ~LevelsPage();
            void update(const sf::RenderWindow *window, float dt) override;
            void handleEvent(const sf::RenderWindow *window, const sf::Event &event) override;
            void render(sf::RenderWindow *window) override;

            // for Sound Manager
            LevelState getLevelState() const { return levelState; }

            // Pause Game
            bool getPaused() const override { return isPaused; }

            // Save state and continue game
            void initializeWorld();
            void saveState(const std::string& filename);
            void saveStateToMemory();
    };
}