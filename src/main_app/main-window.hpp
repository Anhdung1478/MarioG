#pragma once

#include <bits/stdc++.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include "../widget_toolkit/interfaces.hpp"
#include "../widget_toolkit/entity/box/static-box.hpp"

#include "../widget_toolkit/resource/SoundManager.hpp"

#define DEFAULT_FPS 120
#define DEFAULT_WIN_WIDTH 1280
#define DEFAULT_WIN_HEIGHT 720

namespace mario {
    class Page;

    struct GameState {
        int levelId;
        float playerX, playerY;
        int coins, score;
    };

    class MainWindow : public IRenderable {
        private:
            sf::RenderWindow *window;
            std::shared_ptr<Page> content, content_to;
            std::function<void()> _deferredStateChange = nullptr;
            const int fixedFPS = DEFAULT_FPS;
            const int initScreenWidth = DEFAULT_WIN_WIDTH, initScreenHeight = DEFAULT_WIN_HEIGHT;
            const std::string title = "Mario Visualization";
            const sf::Time timeStep = sf::seconds(1.0f / fixedFPS);
            sf::Clock clock;
            bool isRunning = true;
            b2WorldId worldId;

            // SoundManager
            mario::audio::SoundManager soundManager;

            // For save state and continue in main-menu
            std::unique_ptr<GameState> currentState;
            bool hasStartedNewGame = false;

            // Store current page
            std::shared_ptr<Page> currentPage;
        public:
            ~MainWindow() override;
            b2WorldId getWorldId();
            void changePage(std::shared_ptr<Page> to);
            void render(sf::RenderWindow *window) override;
            void closeWindow();
            void run();

            // Use for pause game
            void stepWorld(float dt);

            // Use for sound manager
            audio::SoundManager& getSoundManager();
            void setPageMusic(std::shared_ptr<Page> page);

            // Use for save state and continue in main-menu
            void saveCurrentState(const GameState& state);
            std::unique_ptr<GameState> getCurrentState();
            bool getHasStartedNewGame() const;
            void setHasStartedNewGame(bool value);
            std::shared_ptr<Page> getCurrentPage() const;
    };
}