#include<bits/stdc++.h>
#include "levels.hpp"
#include "main-menu.hpp"
#include "../../widget_toolkit/resource/SoundManager.hpp"

mario::pages::LevelsPage::LevelsPage(MainWindow &context, LevelState state) : Page(context), levelState(state) {
    p_player = std::make_unique<mario::entity::Player>(_context->getWorldId(), sf::Vector2f(15, 10), mario::entity::CharacterListType::Luigi);
    p_inputManager = std::make_unique<mario::input::InputManager>();

    p_inputManager->bind(sf::Keyboard::Scancode::Left, std::make_unique<mario::input::RunCommand>(0));
    p_inputManager->bind(sf::Keyboard::Scancode::Right, std::make_unique<mario::input::RunCommand>(1));
    p_inputManager->bind(sf::Keyboard::Scancode::Up, std::make_unique<mario::input::JumpCommand>());
    p_inputManager->bind(sf::Keyboard::Scancode::Z, std::make_unique<mario::input::FireCommand>());

    // Pause/Resume game
    pauseTexture = std::make_unique<sf::Texture>("../../asset/textures/pause-button.png");
    pauseSprite = std::make_unique<sf::Sprite>(*pauseTexture);
    pauseSprite->setPosition({20, 95});
    pauseSprite->setScale({1.f, 1.f});

    pauseHoverTexture = std::make_unique<sf::Texture>("../../asset/textures/pause-button-hover.png");

    // Home
    homeTexture = std::make_unique<sf::Texture>("../../asset/textures/home.png");
    homeSprite = std::make_unique<sf::Sprite>(*homeTexture);
    homeSprite->setPosition({20, 20});
    homeSprite->setScale({1.f, 1.f});

    homeHoverTexture = std::make_unique<sf::Texture>("../../asset/textures/home-hover.png");

    // Settings
    settingsTexture = std::make_unique<sf::Texture>("../../asset/textures/settings.png");
    settingsSprite = std::make_unique<sf::Sprite>(*settingsTexture);
    settingsSprite->setPosition({20, 170});
    settingsSprite->setScale({1.f, 1.f});

    settingsHoverTexture = std::make_unique<sf::Texture>("../../asset/textures/settings-hover.png");

    // Panel
    panelTexture = std::make_unique<sf::Texture>("../../asset/textures/pause-text.png");
    panelSprite = std::make_unique<sf::Sprite>(*panelTexture);

    sf::Vector2u windowSize =  sf::Vector2u(1280, 720);
    sf::Vector2u textureSize = panelTexture->getSize();
    sf::Vector2f panelSize = sf::Vector2f(textureSize.x * 0.15f, textureSize.y * 0.15f);
    panelSprite->setPosition(sf::Vector2f((windowSize.x - panelSize.x) / 2.0f, (windowSize.y - panelSize.y) / 2.0f));
    panelSprite->setScale({0.15f, 0.15f});

    p_questionBlock = std::make_unique<QuestionBlock>(_context->getWorldId(), sf::Vector2f(100, 600));
}

mario::pages::LevelsPage::~LevelsPage() {
    p_player.reset();
}

void mario::pages::LevelsPage::update(const sf::RenderWindow *window, float dt) {
    if(!isPaused) {
        p_player->update(window, dt);
        p_questionBlock->update(window, dt);
    }

    // Check for hover state
    sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
    sf::FloatRect pauseRect = sf::FloatRect(pauseSprite->getPosition(), 
        sf::Vector2f(pauseTexture->getSize().x * pauseSprite->getScale().x, 
                     pauseTexture->getSize().y * pauseSprite->getScale().y));
    
    sf::FloatRect homeRect = sf::FloatRect(homeSprite->getPosition(), 
        sf::Vector2f(homeTexture->getSize().x * homeSprite->getScale().x, 
                     homeTexture->getSize().y * homeSprite->getScale().y));
    
    sf::FloatRect settingsRect = sf::FloatRect(settingsSprite->getPosition(), 
        sf::Vector2f(settingsTexture->getSize().x * settingsSprite->getScale().x, 
                     settingsTexture->getSize().y * settingsSprite->getScale().y));

    if (pauseRect.contains(sf::Vector2f(mousePos)) || isPaused) {
        pauseSprite->setTexture(*pauseHoverTexture);
    } else {
        pauseSprite->setTexture(*pauseTexture);
    }

    if(homeRect.contains(sf::Vector2f(mousePos))) {
        homeSprite->setTexture(*homeHoverTexture);
    } else {
        homeSprite->setTexture(*homeTexture);
    }

    if(settingsRect.contains(sf::Vector2f(mousePos))) {
        settingsSprite->setTexture(*settingsHoverTexture);
    } else {
        settingsSprite->setTexture(*settingsTexture);
    }
}

void mario::pages::LevelsPage::handleEvent(const sf::RenderWindow *window, const sf::Event &event) {
    if (const auto key = event.getIf<sf::Event::KeyPressed>(); key && key->code == sf::Keyboard::Key::Escape) {
        isPaused = !isPaused;
        if(isPaused) {
            _context->getSoundManager().pauseBackgroundMusic();
        } else {
            _context->getSoundManager().resumeBackgroundMusic();
            if(p_player) {
                p_player->run(false, true);
            }
        }
    }
    
    if (auto* mouseButtonPressed = event.getIf<sf::Event::MouseButtonPressed>()) {
        if(mouseButtonPressed->button == sf::Mouse::Button::Left) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
            sf::FloatRect iconRect = sf::FloatRect(pauseSprite->getPosition(), sf::Vector2f(pauseTexture->getSize().x * pauseSprite->getScale().x, pauseTexture->getSize().y * pauseSprite->getScale().y));
            if (iconRect.contains(sf::Vector2f(mousePos))) {
                isPaused = !isPaused;
                if (isPaused) {
                    _context->getSoundManager().pauseBackgroundMusic();
                } else {
                    _context->getSoundManager().resumeBackgroundMusic();
                    if (p_player) {
                        p_player->run(false, true);
                    }
                }
            }
        }  
    }

    if(!isPaused) {
        p_player->handleEvent(window, event);
        p_inputManager->handleEvent(*p_player, event);
        p_questionBlock->handleEvent(window, event);
    }
}

void mario::pages::LevelsPage::render(sf::RenderWindow *window) {
    p_player->render(window);
    p_questionBlock->render(window);
    window->draw(*pauseSprite);
    window->draw(*homeSprite);
    window->draw(*settingsSprite);
    if(isPaused && panelSprite) {
        window->draw(*panelSprite);
    }
}