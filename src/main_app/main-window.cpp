#include <bits/stdc++.h>
#include "pages/main-menu.hpp"
#include "main-window.hpp"
#include <box2d/math_functions.h>

mario::MainWindow::~MainWindow() {
    b2DestroyWorld(worldId); // Clean up Box2D world    
}

b2WorldId mario::MainWindow::getWorldId() {
    return worldId;
}

void mario::MainWindow::changePage(std::shared_ptr<Page> to) {
    content_to = to;
    _deferredStateChange = [this]() {
        content = content_to;
        content_to->setContext(this);
    };
}

void mario::MainWindow::render(sf::RenderWindow *window) {
    window->clear(sf::Color::Black);

    if (content) {
        content->render(window);
    }

    window->display();
}

void mario::MainWindow::closeWindow() {
    isRunning = false;
}

void mario::MainWindow::run() {
    window = new sf::RenderWindow(sf::VideoMode(sf::Vector2u(initScreenWidth, initScreenHeight)), title, sf::Style::Default);
    window->setFramerateLimit(fixedFPS);

    changePage(std::make_shared<pages::MainMenuPage>(*this)); // Initialize with main-menu page
    isRunning = true;

    sf::Time accumalator = sf::Time::Zero;
    while (isRunning) {
        sf::Time deltaTime = clock.restart(); // Get the time elapsed since the last frame
        while(deltaTime < timeStep) {
            sf::sleep(timeStep - deltaTime);
            deltaTime += clock.restart();
        }

        //std::cerr << deltaTime.asSeconds() << ' ' << timeStep.asSeconds() << '\n';
        while (const std::optional event = window->pollEvent()) {
            if(event->is<sf::Event::Closed>()) {
                isRunning = false;
            }

            if(event->is<sf::Event::Resized>()) {
                window->setSize(sf::Vector2u(initScreenWidth, initScreenHeight));
            }

            if(content) {
                content->handleEvent(window, *event); // Pass the event to the current content page
            }
        }

        accumalator += deltaTime;
        if (content) {
            content->update(window, deltaTime.asSeconds()); // Update with a delta time
        }

        while(accumalator >= timeStep) {
            float stepTime = timeStep.asSeconds();
            b2World_Step(worldId, stepTime, 16);
            accumalator -= sf::seconds(stepTime);
        }

        render(window);
        if (_deferredStateChange) {
            _deferredStateChange();
            _deferredStateChange = nullptr;
        }
    }

    window->close();
    delete window;
}