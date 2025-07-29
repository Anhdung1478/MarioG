#pragma once

#include <bits/stdc++.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "../widget_toolkit/interfaces.hpp"
#include "../widget_toolkit/entity/box/static-box.hpp"

#define DEFAULT_FPS 60
#define DEFAULT_WIN_WIDTH 1280
#define DEFAULT_WIN_HEIGHT 720

namespace mario {
    class Page;

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
        
        public:
            ~MainWindow() override;
            void changePage(std::shared_ptr<Page> to);
            void render(sf::RenderWindow *window) override;
            void closeWindow();
            void run();
    };
}