#include<bits/stdc++.h>
#include "levels.hpp"
#include "main-menu.hpp"
#include "../../widget_toolkit/resource/SoundManager.hpp"

mario::pages::LevelsPage::LevelsPage(MainWindow &context, mario::resource::LevelState state) : Page(context), camera({1280, 720}), currLevelState(state) {
    p_player = new mario::entity::Player(sf::Vector2f(30, 30), state.characterType, state.stateType);
    p_inputManager = std::make_unique<mario::input::InputManager>(context);

    // Load enemies
    // enemies.push_back(new mario::entity::Goomba(sf::Vector2f(300.f, 80.f)));
    enemies.push_back(new mario::entity::KoopaPatrol(sf::Vector2f(1400.f, 80.f), mario::entity::KoopaType::Red, false));
    // enemies.push_back(new mario::entity::KoopaPatrol(sf::Vector2f(350.f, 80.f), mario::entity::KoopaType::Green, true));
   
    // Load items
    items.push_back(new mario::entity::FireFlower(
        "../../asset/sprites/fireflower.json",
        "../../asset/sprites/mario_sheets.png",
        sf::Vector2f(1.f, 1.f),
        "fireflower[0]",
        sf::Vector2f(500.f, 500.f),
        sf::Vector2f(16.f, 16.f),
        sf::Vector2f(0.f, 0.f)
    ));

    items.push_back(new mario::entity::RedMushroom(
        "../../asset/sprites/red-mushroom.json",
        "../../asset/sprites/enemies-2.png",
        sf::Vector2f(1.f, 1.f),
        "red-mushroom[0]",
        sf::Vector2f(700.f, 200.f),
        sf::Vector2f(16.f, 16.f),
        sf::Vector2f(0.f, 0.f)
    ));

    items.push_back(new mario::entity::OneupMushroom(
        "../../asset/sprites/1up-mushroom.json",
        "../../asset/sprites/enemies-2.png",
        sf::Vector2f(1.f, 1.f),
        "1up-mushroom[0]",
        sf::Vector2f(900.f, 200.f),
        sf::Vector2f(16.f, 16.f),
        sf::Vector2f(0.f, 0.f)
    ));

    /*
    function:
        Item* A = new mario::entity::FireFlower(
        "../../asset/sprites/fireflower.json",
        "../../asset/sprites/mario_sheets.png",
        sf::Vector2f(1.f, 1.f),
        "fireflower[0]",
        sf::Vector2f(500.f, 500.f),
        sf::Vector2f(16.f, 16.f),
        sf::Vector2f(0.f, 0.f)
    ));
    items.push_back(A);
    */

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

    tileMap = std::make_unique<mario::entity::TileMap>("../../asset/maps/tiles-8.json", "../../asset/maps/Map_1.json");
    tileMap->createBlock(blocks);
  
    p_levelDataManager = std::make_unique<mario::resource::LevelDataManager>();
    camera.setMapBounds(tileMap->getWorldBounds());

    //itemManager = std::make_unique<mario::entity::ItemManager>();
    //itemManager->setPlayerReference(p_player);
    //itemManager->setTileMapRef(tileMap.get());
    //itemManager->loadSpawnPoints(tileMap->getObjects());
}

void mario::pages::LevelsPage::autoSave() {
    p_levelDataManager->autoSave(currLevelState);
}

mario::pages::LevelsPage::~LevelsPage() {
    autoSave();
    delete p_player;
    for (auto &enemy : enemies) {
        delete enemy;
    }
    // Clean up items vector
    for (auto &item : items) {
        delete item;
    }
}

// for Sound Manager
mario::resource::LevelState mario::pages::LevelsPage::getLevelState() const { return currLevelState; }

// Pause Game
bool mario::pages::LevelsPage::isPaused() const { return _isPaused; }

void mario::pages::LevelsPage::update(const sf::RenderWindow *window, float dt) {
    timeRemaining -= sf::seconds(dt);
    if(timeRemaining <= sf::seconds(0.f)) {
        // failed !!
    }

    if(!_isPaused) {
        p_player->update(window, dt);
        
        for(auto &enemy : enemies) {
            enemy->update(window, dt);
        }

        for(auto &block : blocks) {
            block->update(window, dt);
        }

        // Update items directly from vector
        // for(auto &item : items) {
        //     if (item && !item->isCollected()) {
        //         item->update(window, dt);
        //     }
        // }

        collisionManager.checkCollisionPlayerWithBlocks(p_player, blocks);
        collisionManager.checkCollisionEnemyWithBlocks(enemies, blocks);
        // TODO: Add collision detection for items with player
        collisionManager.checkCollisionPlayerWithItems(p_player, items);
        // collisionManager.checkCollisionPlayerWithEnemies(p_player, enemies);
        // collisionManager.checkCollisionPlayerWithItems(p_player, enemies);


        // for (auto* enemy : enemies) {
        //     mario::entity::Enemy* enemyPtr = dynamic_cast<mario::entity::Enemy*>(enemy);
        //     if (enemyPtr) {
        //         enemyPtr->updateBehavior(dt, p_player);
        //         enemyPtr->update(window, dt);
        //         tileMap->checkCollisionEn(enemyPtr); // Sử dụng enemyPtr (Enemy*)
        //     } else {
        //         enemy->update(window, dt); // Xử lý các Entity không phải Enemy (nếu có)
        //     }
        // }



        camera.followEntity(*p_player, dt);
        camera.update(dt);
        
        currLevelState.stateType = p_player->getPlayerStateType();
        p_levelDataManager->update(dt, currLevelState);
        removeCollectedItems();
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

    if (pauseRect.contains(sf::Vector2f(mousePos)) || _isPaused) {
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
        _isPaused = !_isPaused;
        if(_isPaused) {
            _context->getSoundManager().playSound(mario::event::SoundEvent::GAME_PAUSE);
            _context->getSoundManager().pauseBackgroundMusic();
        } else {
            _context->getSoundManager().resumeBackgroundMusic();
            if(p_player) {
                p_player->move(false, true);
            }
            for (auto* enemy : enemies) {
                mario::entity::Enemy* enemyPtr = dynamic_cast<mario::entity::Enemy*>(enemy);
                if (enemyPtr && !enemyPtr->getActive()) {
                    enemyPtr->setActive(true); // Resume enemy activity
                }
            }
        }
    }
    
    if (auto* mouseButtonPressed = event.getIf<sf::Event::MouseButtonPressed>()) {
        if(mouseButtonPressed->button == sf::Mouse::Button::Left) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
            sf::FloatRect pauseRectF = sf::FloatRect(pauseSprite->getPosition(), sf::Vector2f(pauseTexture->getSize().x * pauseSprite->getScale().x, pauseTexture->getSize().y * pauseSprite->getScale().y));
            sf::FloatRect homeRectF = sf::FloatRect(homeSprite->getPosition(), sf::Vector2f(homeTexture->getSize().x * homeSprite->getScale().x, homeTexture->getSize().y * homeSprite->getScale().y));
            sf::FloatRect settingsRectF = sf::FloatRect(settingsSprite->getPosition(), sf::Vector2f(settingsTexture->getSize().x * settingsSprite->getScale().x, settingsTexture->getSize().y * settingsSprite->getScale().y));

            if(pauseRectF.contains(sf::Vector2f(mousePos))) {
                _isPaused = !_isPaused;
                if(_isPaused) {
                    _context->getSoundManager().playSound(mario::event::SoundEvent::GAME_PAUSE);
                    _context->getSoundManager().pauseBackgroundMusic();
                } else {
                    _context->getSoundManager().resumeBackgroundMusic();
                    if (p_player) {
                        p_player->move(false, true);
                    }
                    for (auto* enemy : enemies) {
                        mario::entity::Enemy* enemyPtr = dynamic_cast<mario::entity::Enemy*>(enemy);
                        if (enemyPtr && !enemyPtr->getActive()) {
                            enemyPtr->setActive(true); // Resume enemy activity
                        }
                    }
                }
            } else 
                if(homeRectF.contains(sf::Vector2f(mousePos))) {
                    _context->changePage(std::make_shared<mario::pages::MainMenuPage>(*_context));
                } else 
                    if(settingsRectF.contains(sf::Vector2f(mousePos))) {
                        isSettingsOpen = !isSettingsOpen;
                        if(isSettingsOpen) {
                            _context->getSoundManager().playSound(mario::event::SoundEvent::GAME_PAUSE);
                        }
                    }
        } 
    }

    if(isSettingsOpen) {
        musicSlider->handleEvent(event, *window);
        sfxSlider->handleEvent(event, *window);
    }

    if(!_isPaused) {
        p_player->handleEvent(window, event);
        p_inputManager->handleEvent(*p_player, event);   
        tileMap->handleEvent(window, event);
        for (auto &enemy : enemies) {
            enemy->handleEvent(window, event);
        }
        for(auto &block : blocks) {
            block->handleEvent(window, event);
        }
        // tileMap->handleEvent(window, event);
    }
}

void mario::pages::LevelsPage::render(sf::RenderWindow *window) {
    camera.applyTo(*window);
    for(auto &block : blocks) {
        block->render(window);
    }
    p_player->render(window);

    // Render enemies
    for (auto &enemy : enemies) {
        enemy->render(window);
    }

    // Render items directly from vector
    for (auto &item : items) {
        // if (item && !item->isCollected()) {
            item->render(window);
        // }
    }

    window->draw(*pauseSprite);
    window->draw(*homeSprite);
    window->draw(*settingsSprite);

    if(_isPaused && panelSprite) {
        window->draw(*panelSprite);
    }

    if (isSettingsOpen) {
        window->draw(*settingsPanelSprite);
        musicSlider->render(*window);
        sfxSlider->render(*window);
    }

    //itemManager->render(window);
}

void mario::pages::LevelsPage::removeCollectedItems() {
    items.erase(
        std::remove_if(
            items.begin(), 
            items.end(),
            [](const auto& item) {
                return item->isCollected();
            }
        ),
        items.end()
    );
}