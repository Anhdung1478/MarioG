#include<bits/stdc++.h>
#include "levels.hpp"
#include "main-menu.hpp"
#include "../../widget_toolkit/resource/SoundManager.hpp"

mario::pages::LevelsPage::LevelsPage(MainWindow &context, LevelState state) : Page(context), levelState(state) {
    p_player = std::make_unique<mario::entity::Player>(_context->getWorldId(), sf::Vector2f(15, 10), mario::entity::CharacterListType::Luigi);
    p_inputManager = std::make_unique<mario::input::InputManager>();

    p_inputManager->bind(sf::Keyboard::Scancode::Left, std::make_unique<mario::input::RunCommand>(0));
    p_inputManager->bind(sf::Keyboard::Scancode::Right, std::make_unique<mario::input::RunCommand>(1));
    p_inputManager->bind(sf::Keyboard::Scancode::Up, std::make_unique<mario::input::JumpCommand>(*_context));
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
    panelSprite->setScale({0.6f, 0.6f});
    sf::Vector2f panelSize = sf::Vector2f(textureSize.x *0.6f, textureSize.y * 0.6f);
    panelSprite->setPosition(sf::Vector2f((windowSize.x - panelSize.x) / 2.0f, (windowSize.y - panelSize.y) / 2.0f));

    // Settings panel
    font = std::make_unique<sf::Font>("../../asset/fonts/Cascadia.ttf");
    
    settingsPanelTexture = std::make_unique<sf::Texture>("../../asset/textures/settings-panel.png");
    sliderBarTexture = std::make_unique<sf::Texture>("../../asset/textures/slider-bar.png");
    sliderHandleTexture = std::make_unique<sf::Texture>("../../asset/textures/slider-handle.png");

    settingsPanelSprite = std::make_unique<sf::Sprite>(*settingsPanelTexture);
    settingsPanelSprite->setOrigin(sf::Vector2f(settingsPanelTexture->getSize().x / 2.f, settingsPanelTexture->getSize().y / 2.f));
    settingsPanelSprite->setPosition(sf::Vector2f(windowSize.x / 2.f, windowSize.y / 2.f));
    settingsPanelSprite->setScale(sf::Vector2f(0.7f, 0.7f));
    sf::Vector2f panelCenter = settingsPanelSprite->getPosition();
    musicSlider = std::make_unique<Slider>(
        std::make_unique<sf::Texture>(*sliderBarTexture), // Copy texture
        std::make_unique<sf::Texture>(*sliderHandleTexture), // Copy texture
        std::make_unique<sf::Font>(*font), // Copy font
        sf::Vector2f(panelCenter.x + 80, panelCenter.y - 50),
        std::string("Music"), // Explicitly convert to std::string
        sf::Vector2f(0.5f, 0.5f),
        0.0f, 100.0f, // Add minValue and maxValue
        _context->getSoundManager().getMusicVolume()
    );

    sfxSlider = std::make_unique<Slider>(
        std::make_unique<sf::Texture>(*sliderBarTexture), // Copy texture
        std::make_unique<sf::Texture>(*sliderHandleTexture), // Copy texture
        std::make_unique<sf::Font>(*font), // Copy font
        sf::Vector2f(panelCenter.x + 80, panelCenter.y + 50),
        std::string("SFX"), // Explicitly convert to std::string
        sf::Vector2f(0.5f, 0.5f),
        0.0f, 100.0f, // Add minValue and maxValue
        _context->getSoundManager().getSoundVolume()
    );
    
    // Setup the first value for slider
    musicSlider->setValue(_context->getSoundManager().getMusicVolume());
    sfxSlider->setValue(_context->getSoundManager().getSoundVolume());

    // Callback when slider is changed
    musicSlider->setOnValueChanged([this](float value) {
        _context->getSoundManager().adjustBackgroundMusicVolume(value);
    });

    sfxSlider->setOnValueChanged([this](float value) {
        _context->getSoundManager().adjustSoundEffectsVolume(value);
    });

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

    if(settingsRect.contains(sf::Vector2f(mousePos)) || isSettingsOpen) {
        settingsSprite->setTexture(*settingsHoverTexture);
    } else {
        settingsSprite->setTexture(*settingsTexture);
    }

    // Settings
    if (isSettingsOpen) {
        musicSlider->update(*window);
        sfxSlider->update(*window);
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
            sf::FloatRect pauseRectF = sf::FloatRect(pauseSprite->getPosition(), sf::Vector2f(pauseTexture->getSize().x * pauseSprite->getScale().x, pauseTexture->getSize().y * pauseSprite->getScale().y));
            sf::FloatRect homeRectF = sf::FloatRect(homeSprite->getPosition(), sf::Vector2f(homeTexture->getSize().x * homeSprite->getScale().x, homeTexture->getSize().y * homeSprite->getScale().y));
            sf::FloatRect settingsRectF = sf::FloatRect(settingsSprite->getPosition(), sf::Vector2f(settingsTexture->getSize().x * settingsSprite->getScale().x, settingsTexture->getSize().y * settingsSprite->getScale().y));

            if (pauseRectF.contains(sf::Vector2f(mousePos))) {
                isPaused = !isPaused;
                if (isPaused) {
                    _context->getSoundManager().pauseBackgroundMusic();
                } else {
                    _context->getSoundManager().resumeBackgroundMusic();
                    if (p_player) {
                        p_player->run(false, true);
                    }
                }
            } else if (homeRectF.contains(sf::Vector2f(mousePos))) {
                _context->changePage(std::make_shared<mario::pages::MainMenuPage>(*_context));
            } else if (settingsRectF.contains(sf::Vector2f(mousePos))) {
                isSettingsOpen = !isSettingsOpen;
            }
        } 
    }

    if (isSettingsOpen) {
        musicSlider->handleEvent(event, *window);
        sfxSlider->handleEvent(event, *window);
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

    if (isSettingsOpen) {
        window->draw(*settingsPanelSprite);
        musicSlider->render(*window);
        sfxSlider->render(*window);
    }
}