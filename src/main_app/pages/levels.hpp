#include<bits/stdc++.h>
#include "page.hpp"
#include "../../widget_toolkit/entity/player.hpp"
#include "../../widget_toolkit/command/input-manager.hpp"
#include "../../widget_toolkit/entity/blocks/TileMap.hpp"

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
            std::vector<std::unique_ptr<Block>> blocks;
            std::unique_ptr<TileMap> tileMap;

            std::unique_ptr<QuestionBlock> p_questionBlock;

        public:
            LevelsPage(MainWindow &context, LevelState state);
            ~LevelsPage();
            void update(const sf::RenderWindow *window, float dt) override;
            void handleEvent(const sf::RenderWindow *window, const sf::Event &event) override;
            void render(sf::RenderWindow *window) override;
    };
}