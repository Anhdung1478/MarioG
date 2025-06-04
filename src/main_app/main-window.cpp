#include <bits/stdc++.h>
#include "pages/main-menu.hpp"
#include "main-window.hpp"

void mario::MainWindow::changePage(std::shared_ptr<Page> to) {
    content_to = to;
    _deferredStateChange = [this]() {
        content = content_to;
        content_to->setContext(this);
    };
}

std::unique_ptr<sf::RenderWindow> mario::MainWindow::render(std::unique_ptr<sf::RenderWindow> window) {
    window->clear(sf::Color::Black);

    if (content) {
        window = content->render(std::move(window));
    }

    window->display();
    return window;
}

void mario::MainWindow::run() {
    window = std::make_unique<sf::RenderWindow>(sf::VideoMode({initScreenWidth, initScreenHeight}), title, sf::Style::Default);
    window->setFramerateLimit(fixedFPS);

    changePage(std::make_shared<pages::MainMenuPage>(*this)); // Initialize with a default page

    clock.reset();
    bool running = true;
    while (running) {
        while (const std::optional event = window->pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                running = false;
            }

            if(content) {
                content->handleEvent(*event); // Pass the event to the current content page
            }
        }

        sf::Time deltaTime = clock.restart();
        if (content) {
            content->update(deltaTime.asSeconds()); // Update with a dummy delta time
        }

        window = render(std::move(window));

        if (_deferredStateChange) {
            _deferredStateChange();
            _deferredStateChange = nullptr;
        }
    }
}