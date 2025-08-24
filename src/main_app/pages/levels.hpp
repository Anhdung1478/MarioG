#include<bits/stdc++.h>

#include "page.hpp"
#include "gameover-page.hpp"

#include "../../widget_toolkit/entity/player/player.hpp"
#include "../../widget_toolkit/entity/player/player-state.hpp"
#include "../../widget_toolkit/command/input-manager.hpp"
#include "../../widget_toolkit/entity/blocks/TileMap.hpp"
#include "../../widget_toolkit/resource/LevelState.hpp"
#include "../../widget_toolkit/resource/LevelDataManager.hpp"
#include "../../widget_toolkit/camera/camera.hpp"
#include "../../widget_toolkit/collision/CollisionManager.hpp"

#include "../../widget_toolkit/controls/button.hpp"
#include "../../widget_toolkit/controls/button-list.hpp"
#include "../../widget_toolkit/controls/slider.hpp"
#include "../../widget_toolkit/entity/item/ItemManager.hpp"
#include "../../widget_toolkit/entity/item/item.hpp"
#include "../../widget_toolkit/entity/item/ItemFactory.hpp"

#include "../../widget_toolkit/entity/enemy/goomba.hpp"
#include "../../widget_toolkit/entity/enemy/koopa.hpp"
#include "../../widget_toolkit/entity/enemy/piranha.hpp"

#include "../../widget_toolkit/networking/NetworkManager.hpp"
#include "../../widget_toolkit/networking/GameMode.hpp"

namespace mario::pages {
    class LevelsPage : public Page {
        private:
            std::unique_ptr<mario::input::InputManager> p_inputManager;
            std::unique_ptr<mario::resource::LevelDataManager> p_levelDataManager;
      
            std::unique_ptr<mario::entity::TileMap> tileMap;
            std::vector<mario::entity::Block*> blocks;
            std::vector<mario::entity::Block*> groundBlocks;
            std::vector<mario::entity::Block*> backgroundBlocks;
            std::vector<mario::entity::Enemy*> enemies;
            std::vector<mario::entity::Item*> items;  // Direct item management
            mario::entity::FlagPole* flagPole;
            mario::entity::Item* testItem;
            sf::IntRect fortressDoor;
            sf::Vector2f fortressDoorPosition;

            // Background
            sf::Texture backgroundTexture;
            sf::Sprite *backgroundSprite;

            mario::entity::Block* testBlock;
            mario::entity::FireWorks* testFireWorks;
      
            Camera camera;
            mario::entity::Player *p_player;
            mario::entity::CollisionManager collisionManager;

            std::unique_ptr<sf::Font> marioFont;

            // for Sound Manager
            mario::resource::LevelState currLevelState;

            // Pause/Resume game
            bool _isPaused = false;
            std::unique_ptr<sf::Texture> pauseTexture;
            std::unique_ptr<sf::Sprite> pauseSprite;
            std::unique_ptr<sf::Texture> pauseHoverTexture;
            std::unique_ptr<sf::Texture> resumeTexture;
            
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

            std::unique_ptr<mario::entity::ItemManager> itemManager;

            bool isSettingsPressed = false;
            void removeCollectedItems();
            //bool _gameOver = false;
            std::shared_ptr<NetworkManager> networkManager;
            GameMode gameMode;
            mario::entity::Player *remotePlayer;
            sf::Clock networkUpdateTimer;
            // Add these new members:
            bool gameOverReceivedForLocal = false;  // replaces old gameOverReceived for local only
            bool remotePlayerDead = false;          // track remote death without ending game
            static constexpr float NETWORK_UPDATE_INTERVAL = 1.0f/30.0f; // 30 FPS network updates
            float enemyStateSendInterval = 0.1f; // seconds (100 ms)
            float enemyStateSendAccumulator = 0.0f;

            // Add these new method declarations:
            void handleNetworkUpdates(float dt);
            void handleRemoteItemCollection(int itemId, const sf::Vector2f& position);
            void handleRemoteEnemyDefeat(int enemyId, const sf::Vector2f& position);
            void handleRemotePlayerPowerupState(int playerId, int powerupState);
            void checkItemCollection();
            void checkEnemyDefeats();
            void sendEnemyState();

            sf::Vector2f remoteTargetPos = sf::Vector2f(150.f, 400.f);;
            sf::Vector2f remoteTargetVel = sf::Vector2f(0.f, 0.f);;
            
            bool backClicked = false;
            sf::Clock backAnimClock;
            float backAnimDuration = 0.3f;

            bool pauseClicked = false;
            sf::Clock pauseAnimClock;
            float pauseAnimDuration = 0.3f;

            bool settingsClicked = false;
            sf::Clock settingsAnimClock;
            float settingsAnimDuration = 0.3f;
        public:
            LevelsPage(MainWindow &context, mario::resource::LevelState state, 
                        std::shared_ptr<NetworkManager> networkManager = nullptr, GameMode mode = GameMode::SinglePlayer);
            
            ~LevelsPage();

            void handlePlayerDeath();
            void cleanUpDeletedObject();
            void checkForPlayerFinishLevel();
            void updateCameraLogic(float dt);
            void updateBlocksEnemiesAndItems(const sf::RenderWindow *window, float dt);
            void updateRemotePlayerForMultiplayer(const sf::RenderWindow *window, float dt);
            void checkMenuButtonHoverLogic(const sf::RenderWindow *window);

            void update(const sf::RenderWindow *window, float dt) override;
            void handleEvent(const sf::RenderWindow *window, const sf::Event &event) override;
            void render(sf::RenderWindow *window) override;
            sf::Vector2f getPositionRelativeToCamera(sf::Vector2f pos);

            void autoSave();
            void rePositionTextToMiddle(sf::Text &text, int rectX, int rectY);
            void renderLevelState(sf::RenderWindow *window, mario::resource::LevelState levelState);

            // for Sound Manager
            mario::resource::LevelState getLevelState() const;

            // Pause Game
            bool isPaused() const;
            
            // Game over state
            bool isGameOver() const { return gameOverReceivedForLocal; }
            void setGameOver(bool state) { gameOverReceivedForLocal = state; }

            // void addEnemy(mario::entity::Enemy* enemy) { 
            //     enemies.push_back(enemy);
            // }
    };
}