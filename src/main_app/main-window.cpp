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

    b2WorldDef worldDef = b2DefaultWorldDef(); // Create a default world definition
    worldDef.gravity = b2Vec2({0.0f, 20.f}); // Set gravity for the Box2D world
    worldId = b2CreateWorld(&worldDef); // Create the Box2D world

    sf::Vector2f groundPos = sf::Vector2f(640, 725);
    sf::Vector2f groundDim = sf::Vector2f(1280, 10);
    mario::entity::Box *p_ground = new mario::entity::StaticBox(worldId, groundPos, groundDim, 1.f, 0.3f);

    isRunning = true;

    sf::Time accumalator = sf::Time::Zero;
    sf::sleep(timeStep);
    while (isRunning) {
        sf::Time deltaTime = clock.restart(); // Get the time elapsed since the last frame
        if(deltaTime < timeStep) {
            sf::sleep(timeStep - deltaTime);
            deltaTime += clock.restart();
        }

        std::cerr << deltaTime.asSeconds() << ' ' << timeStep.asSeconds() << '\n';
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
            b2World_Step(worldId, stepTime, 8);
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