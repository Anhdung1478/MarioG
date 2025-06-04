#pragma once

#include <bits/stdc++.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "pages/page.hpp"

#define DEFAULT_FPS 60
#define DEFAULT_WIN_WIDTH 1280
#define DEFAULT_WIN_HEIGHT 720

namespace mario {
    class Page;

    class MainWindow : public IRenderable {
        private:
            int fixedFPS = DEFAULT_FPS;
            int initScreenWidth = DEFAULT_WIN_WIDTH, initScreenHeight = DEFAULT_WIN_HEIGHT;
            std::string title = "Mario Visualization";
            sf::Clock clock;
        
        public:
            std::unique_ptr<sf::RenderWindow> window;
            std::shared_ptr<Page> content, content_to;
            std::function<void()> _deferredStateChange = nullptr;

            void changePage(std::shared_ptr<Page> to);
            std::unique_ptr<sf::RenderWindow> render(std::unique_ptr<sf::RenderWindow> window) override;
            void run();
    };
}