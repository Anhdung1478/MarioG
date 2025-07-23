#include<bits/stdc++.h>
#include "page.hpp"
#include "../../widget_toolkit/entity/player.hpp"
#include "../../widget_toolkit/command/input-manager.hpp"
#include "../../widget_toolkit/entity/blocks/TileMap.hpp"
#include "../../widget_toolkit/resource/LevelState.hpp"
#include "../../widget_toolkit/resource/LevelDataManager.hpp"

namespace mario::pages {
    class LevelsPage : public Page {
        private:
            std::unique_ptr<mario::entity::Player> p_player;
            std::unique_ptr<mario::input::InputManager> p_inputManager;
            std::vector<std::unique_ptr<Block>> blocks;
            std::unique_ptr<TileMap> tileMap;

            std::unique_ptr<QuestionBlock> p_questionBlock;

            std::unique_ptr<mario::resource::LevelDataManager> p_levelDataManager;

            mario::resource::LevelState currLevelState;
            sf::Time timeRemaining;

        public:
            LevelsPage(MainWindow &context, mario::resource::LevelState state);
            ~LevelsPage();
            void update(const sf::RenderWindow *window, float dt) override;
            void handleEvent(const sf::RenderWindow *window, const sf::Event &event) override;
            void render(sf::RenderWindow *window) override;
            void autoSave();
    };
}