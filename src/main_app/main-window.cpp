#include <bits/stdc++.h>
#include "pages/main-menu.hpp"
#include "main-window.hpp"
#include "pages/settings.hpp"
#include "pages/levels.hpp"
#include "pages/mode-select.hpp"

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

void mario::MainWindow::run() {
    sf::ContextSettings settings;
    settings.antiAliasingLevel = 0.0f;

    window = new sf::RenderWindow(sf::VideoMode(sf::Vector2u(initScreenWidth, initScreenHeight)), title, sf::Style::Default, sf::State::Windowed, settings);
    window->setFramerateLimit(fixedFPS);

    changePage(std::make_shared<pages::ModeSelectPage>(*this)); // Initialize with ModeSelectPage
    isRunning = true;

    while (isRunning) {
        // sf::Time deltaTime = clock.restart(); // Get the time elapsed since the last frame
        sf::Time deltaTime = sf::seconds(1.0f / (float)(fixedFPS));
        /*while(deltaTime < timeStep) {
            sf::sleep(timeStep - deltaTime);
            deltaTime += clock.restart();
        }*/

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

        if (content) {
            //content->updateWithStyle(window, deltaTime.asSeconds(), content->getPaused()); // Update with a delta time
            content->update(window, deltaTime.asSeconds());
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