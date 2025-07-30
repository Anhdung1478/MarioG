#include<bits/stdc++.h>
#include "page.hpp"
#include "../../widget_toolkit/entity/player.hpp"
#include "../../widget_toolkit/command/input-manager.hpp"
#include "../../widget_toolkit/entity/blocks/TileMap.hpp"
#include "../../widget_toolkit/resource/LevelState.hpp"
#include "../../widget_toolkit/resource/LevelDataManager.hpp"
#include "../../widget_toolkit/camera/camera.hpp"

namespace mario::pages {
    class LevelsPage : public Page {
        private:
            std::unique_ptr<mario::input::InputManager> p_inputManager;
            std::unique_ptr<mario::resource::LevelDataManager> p_levelDataManager;

            std::unique_ptr<TileMap> tileMap;
            mario::entity::Player *p_player;

            mario::resource::LevelState currLevelState;
            sf::Time timeRemaining;
            Camera camera;

        public:
            LevelsPage(MainWindow &context, mario::resource::LevelState state);
            ~LevelsPage();
            void update(const sf::RenderWindow *window, float dt) override;
            void handleEvent(const sf::RenderWindow *window, const sf::Event &event) override;
            void render(sf::RenderWindow *window) override;
            void autoSave();
    };
}