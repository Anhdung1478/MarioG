#include <bits/stdc++.h>
#include "pages/main-menu.hpp"
#include "main-window.hpp"
#include "pages/settings.hpp"
#include "pages/levels.hpp"
#include "pages/mode-select.hpp"

mario::MainWindow::MainWindow() {
    sf::ContextSettings settings;
    settings.antiAliasingLevel = 4.0f;

    window = new sf::RenderWindow(sf::VideoMode(sf::Vector2u(initScreenWidth, initScreenHeight)), title, sf::Style::Default, sf::State::Windowed, settings);
    //window->setFramerateLimit(fixedFPS);
    isRunning = true;

    sf::Image marioCursorImage;
    if(!marioCursorImage.loadFromFile("../../asset/sprites/mario-cursor.png"))
        throw std::runtime_error("Failed to load texture from file: " + std::string("asset/sprites/mario-cursor.png"));

    sf::Vector2u size = marioCursorImage.getSize();
    sf::Vector2u hotspot(0, 0);

    p_marioCursor = std::make_unique<sf::Cursor>(marioCursorImage.getPixelsPtr(), size, hotspot);
    window->setMouseCursor(*p_marioCursor);
}

mario::MainWindow::~MainWindow() {
}

void mario::MainWindow::changePage(std::shared_ptr<Page> to) {
    content_to = to;
    _deferredStateChange = [this]() {
        content = content_to;
        content_to->setContext(this);
        setPageMusic(content);
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

sf::Vector2f mario::MainWindow::getWindowSize() const {
    sf::Vector2u windowSize = window->getSize();
    sf::Vector2f wSize(windowSize.x, windowSize.y);
    return wSize;
}

mario::audio::SoundManager& mario::MainWindow::getSoundManager() { 
    return soundManager;
}

NetworkManager& mario::MainWindow::getNetworkManager() {
    return networkManager;
}

void mario::MainWindow::run() {
    changePage(std::make_shared<pages::ModeSelectPage>(*this)); // Initialize with ModeSelectPage

    sf::Time accumulate = sf::seconds(0);
    while (isRunning) {
        sf::Time deltaTime = clock.restart(); // Get the time elapsed since the last frame
        //sf::Time deltaTime = sf::seconds(1.0f / (float)(fixedFPS));
        accumulate += deltaTime;
        while(accumulate < timeStep) {
            sf::sleep(timeStep - deltaTime);
            accumulate += clock.restart();
        }

        //std::cerr << deltaTime.asSeconds() << ' ' << timeStep.asSeconds() << '\n';
        while (const std::optional event = window->pollEvent()) {
            if(event->is<sf::Event::Closed>())
                isRunning = false;

            if(event->is<sf::Event::Resized>())
                window->setSize(sf::Vector2u(initScreenWidth, initScreenHeight));

            if(event->is<sf::Event::FocusGained>())
                isFocusOn = true;

            if(event->is<sf::Event::FocusLost>())
                isFocusOn = false;

            if(content && isFocusOn)
                content->handleEvent(window, *event); // Pass the event to the current content page
        }

        accumulate -= timeStep;
        if (content) {
            content->update(window, timeStep.asSeconds());
            content->updateCamera(timeStep.asSeconds());
            content->applyCameraTo(*window);
        }

        render(window);
        if(_deferredStateChange) {
            _deferredStateChange();
            _deferredStateChange = nullptr;
        }
    }

    window->close();
    delete window;
}

void mario::MainWindow::setPageMusic(std::shared_ptr<Page> page) {
    if (dynamic_cast<pages::MainMenuPage*>(page.get())) {
        soundManager.setBackgroundMusic(mario::event::BackgroundMusicState::MAIN_MENU);
    } else if (dynamic_cast<pages::SettingsPage*>(page.get())) {
        soundManager.setBackgroundMusic(mario::event::BackgroundMusicState::SETTING_SCREEN);
    } else if (auto* levelsPage = dynamic_cast<pages::LevelsPage*>(page.get())) {
        int level = levelsPage->getLevelState().level;
        switch (level) {
            case 1:
                soundManager.setBackgroundMusic(mario::event::BackgroundMusicState::LEVEL_1);
                break;
            case 2:
                soundManager.setBackgroundMusic(mario::event::BackgroundMusicState::LEVEL_2);
                break;
            case 3:
                soundManager.setBackgroundMusic(mario::event::BackgroundMusicState::LEVEL_3);
                break;
            default:
                soundManager.setBackgroundMusic(mario::event::BackgroundMusicState::LEVEL_SCREEN);
                break;
        }
    }
}