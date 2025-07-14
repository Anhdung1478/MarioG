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
    worldDef.gravity = b2Vec2({0.0f, 9.81f}); // Set gravity for the Box2D world
    worldId = b2CreateWorld(&worldDef); // Create the Box2D world

    b2BodyDef groundDef = b2DefaultBodyDef(); // Create a default ground definition
    groundDef.type = b2_staticBody; // Set the ground body as static
    groundDef.position = b2Vec2({640.f / 40.f, 725.f / 40.f}); // Set the position of the ground body
    b2BodyId groundBodyId = b2CreateBody(worldId, &groundDef); // Create the ground body
    b2Polygon groundBox = b2MakeBox(1280.0f / 40.f / 2.0f, 10.0f / 40.f / 2.0f); // Create a box shape for the ground
    b2ShapeDef groundShapeDef = b2DefaultShapeDef(); // Create a default shape definition
    groundShapeDef.density = 1.0f; // Set density for the ground shape
    groundShapeDef.material.friction = 0.3f; // Set friction for the ground shape
    b2ShapeId groundShapeId = b2CreatePolygonShape(groundBodyId, &groundShapeDef, &groundBox); // Create a polygon shape for the ground body

    clock.reset();
    isRunning = true;

    sf::Time accumalator = sf::Time::Zero;
    sf::sleep(timeStep);
    while (isRunning) {
        sf::Time deltaTime = clock.restart(); // Get the time elapsed since the last frame
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

        while(accumalator > sf::seconds(0.f)) {
            float stepTime = std::min(accumalator.asSeconds(), timeStep.asSeconds());
            b2World_Step(worldId, stepTime, 8);
            accumalator -= sf::seconds(stepTime);
        }

        render(window);
        if (_deferredStateChange) {
            _deferredStateChange();
            _deferredStateChange = nullptr;
        }
    }

    b2DestroyBody(groundBodyId);
    window->close();
    delete window;
}