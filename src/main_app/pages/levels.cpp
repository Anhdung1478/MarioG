#include<bits/stdc++.h>
#include "levels.hpp"
#include "main-menu.hpp"
#include "../../widget_toolkit/resource/SoundManager.hpp"

mario::pages::LevelsPage::LevelsPage(MainWindow &context, mario::resource::LevelState state, 
                                   std::shared_ptr<NetworkManager> networkManager, GameMode mode) 
    : Page(context), 
      camera({1280, 720}), 
      currLevelState(state),
      networkManager(networkManager),
      gameMode(mode),
      remotePlayer(nullptr),
      gameOverReceivedForLocal(false),
      remotePlayerDead(false) {

    // Initialize player with the correct character type and state
    p_player = new mario::entity::Player(
        sf::Vector2f(100, 200),
        state.characterType, 
        state.stateType,
        state.level,
        context.getSoundManager(), 
        context.getNetworkManager()
    );
    
    if(gameMode == GameMode::SinglePlayer) {
        p_player->loadDataFrom(currLevelState);
        ofstream ofs("../../asset/save_data/autosave.svx");
        ofs << " ";
    }

    p_inputManager = std::make_unique<mario::input::InputManager>(context);
    
    // Initialize remote player for multiplayer mode
    if (gameMode != GameMode::SinglePlayer && networkManager) {
        remotePlayer = new mario::entity::Player(
            sf::Vector2f(200, 400),  // Different starting position than local player
            state.characterType == mario::entity::CharacterListType::Mario ? 
                mario::entity::CharacterListType::Luigi : mario::entity::CharacterListType::Mario,
            state.stateType,
            state.level,
            context.getSoundManager(),
            context.getNetworkManager()
        );
        remotePlayer->setRemote(true);
    }

    if (gameMode != GameMode::SinglePlayer) {
        if (gameMode == GameMode::Host) {
            p_player->setNetworkPlayerId(0);
            if (remotePlayer) remotePlayer->setNetworkPlayerId(1);
        } else {
            p_player->setNetworkPlayerId(1);
            if (remotePlayer) remotePlayer->setNetworkPlayerId(0);
        }
    }
    // Load level and other game elements
    flagPole = new mario::entity::FlagPole();
    tileMap = std::make_unique<mario::entity::TileMap>("../../asset/maps/tiles-8.json", "../../asset/maps/Map_" + std::to_string(currLevelState.level) + ".json", currLevelState.level, currLevelState.level-1);
    tileMap->loadObjects(enemies, items, blocks, groundBlocks, backgroundBlocks, flagPole);
    collisionManager.loadGroundBlocks(groundBlocks);
    // enemies.push_back(std::make_shared<InverseGoomba>(Vector2f(500, 400)));

    // Generate unique IDs for items and enemies for network sync
    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i]) {
            items[i]->setNetworkId(static_cast<int>(i));
        }
    }
    for (size_t i = 0; i < enemies.size(); ++i) {
        if (enemies[i]) {
            enemies[i]->setNetworkId(static_cast<int>(i));
            if (gameMode == GameMode::SinglePlayer) {
                // Single player - local authority for all enemies
                enemies[i]->setNetworkAuthority(true);
            } else if (gameMode == GameMode::Host) {
                // Host controls ALL enemies
                enemies[i]->setNetworkAuthority(true);
                std::cout << "[Network] Host has authority over enemy " << i << "\n";
            } else if (gameMode == GameMode::Client) {
                // Client receives ALL enemy states from host
                enemies[i]->setNetworkAuthority(false);
                std::cout << "[Network] Client will receive enemy " << i << " from host\n";
            }
        }
    }

    if (!backgroundTexture.loadFromFile("../../asset/maps/MapBackground/map_" + std::to_string(currLevelState.level) + "_background.png")) {
        std::cout << "Failed to load background texture for level " << currLevelState.level << "\n";
        exit(1);
    }
    sf::Vector2f boundWorldSize = tileMap->getWorldBounds().size;
    // if(currLevelState.level != 1) {
    //     boundWorldSize -= sf::Vector2f(0, 2 * 16 * mario::entity::BLOCK_SCALE.y);
    // }    
    backgroundSprite = new sf::Sprite(backgroundTexture);
    backgroundSprite->setPosition({0, 0});
    float scaleX = boundWorldSize.x / backgroundTexture.getSize().x;
    float scaleY = boundWorldSize.y / backgroundTexture.getSize().y;
    backgroundSprite->setScale({scaleX, scaleY});
    std::cout << "Bound World: " << boundWorldSize.x << ", " << boundWorldSize.y << '\n'; 
    // tileMap->createBlock(blocks, backgroundBlocks);
    // testBlock = new mario::entity::BackgroundBlock(sf::Vector2f(100, 500), sf::Vector2f(16, 16), "enemies-flag[0]");
    // testBlock = new mario::entity::BackgroundBlock(sf::Vector2f(100, 500), sf::Vector2f(16, 16), std::to_string(390), {"390", 1, 171, 16, 16});
    // testFireWorks = new mario::entity::FireWorks(boundWorldSize - sf::Vector2f(500, 500), sf::Vector2f(450, 250));
    testFireWorks = new mario::entity::FireWorks(sf::Vector2f(boundWorldSize.x - 1000, 50), sf::Vector2f(1000, 350));
    testFireWorks->setShowFireworks(true);



    // Mario font initalize
    marioFont = std::make_unique<sf::Font>("../../asset/fonts/SuperMario256.ttf");

    // Pause/Resume game
    pauseTexture = std::make_unique<sf::Texture>("../../asset/textures/pause-button.png");
    pauseSprite = std::make_unique<sf::Sprite>(*pauseTexture);
    pauseSprite->setPosition({20, 97});
    pauseSprite->setScale({0.9f, 0.9f});

    pauseHoverTexture = std::make_unique<sf::Texture>("../../asset/textures/pause-button-hover.png");
    resumeTexture = std::make_unique<sf::Texture>("../../asset/textures/resume-button-hover.png");

    // Home
    homeTexture = std::make_unique<sf::Texture>("../../asset/textures/home.png");
    homeSprite = std::make_unique<sf::Sprite>(*homeTexture);
    homeSprite->setPosition({20, 30});
    homeSprite->setScale({0.9f, 0.9f});

    homeHoverTexture = std::make_unique<sf::Texture>("../../asset/textures/home-hover.png");

    // Settings
    settingsTexture = std::make_unique<sf::Texture>("../../asset/textures/settings.png");
    settingsSprite = std::make_unique<sf::Sprite>(*settingsTexture);
    settingsSprite->setPosition({20, 164});
    settingsSprite->setScale({0.9f, 0.9f});

    settingsHoverTexture = std::make_unique<sf::Texture>("../../asset/textures/settings-hover.png");

    // Panel
    panelTexture = std::make_unique<sf::Texture>("../../asset/textures/pause-text.png");
    panelSprite = std::make_unique<sf::Sprite>(*panelTexture);

    sf::Vector2u windowSize = sf::Vector2u(1280, 720);
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
        sf::Vector2f(0.3f, 0.3f),
        0.0f, 100.0f, // Add minValue and maxValue
        _context->getSoundManager().getMusicVolume()
    );

    sfxSlider = std::make_unique<Slider>(
        std::make_unique<sf::Texture>(*sliderBarTexture), // Copy texture
        std::make_unique<sf::Texture>(*sliderHandleTexture), // Copy texture
        std::make_unique<sf::Font>(*font), // Copy font
        sf::Vector2f(panelCenter.x + 80, panelCenter.y + 50),
        std::string("SFX"), // Explicitly convert to std::string
        sf::Vector2f(0.3f, 0.3f),
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
  
    p_levelDataManager = std::make_unique<mario::resource::LevelDataManager>();
    camera.setMapBounds(tileMap->getWorldBounds());
}

/* ========================================================================================================================================================================== */

void mario::pages::LevelsPage::autoSave() {
    if(gameMode == GameMode::SinglePlayer)
        p_levelDataManager->autoSave(currLevelState);
}

mario::pages::LevelsPage::~LevelsPage() {
    autoSave();
    delete p_player;

    camera.resetToDefaultView();
    for (auto &enemy : enemies) {
        delete enemy;
    }

    for (auto &item : items) {
        delete item;
    }

    for (auto &block : blocks) {
        delete block;
    }

    if (remotePlayer) {
        delete remotePlayer;
        remotePlayer = nullptr;
    }
}
// for Sound Manager
mario::resource::LevelState mario::pages::LevelsPage::getLevelState() const { return currLevelState; }

// Pause Game
bool mario::pages::LevelsPage::isPaused() const { return _isPaused; }

/* ========================================================================================================================================================================== */

// Clean up deleted objects
void mario::pages::LevelsPage::cleanUpDeletedObject() {
    for (auto it = enemies.begin(); it != enemies.end();) {
        if ((*it)->shouldDelete()) {
            delete *it;
            it = enemies.erase(it); 
        } else {
            ++it;
        }
    }

    for (auto it = blocks.begin(); it != blocks.end();) {
        if ((*it)->shouldDelete()) {
            delete *it;
            it = blocks.erase(it); 
        } else {
            ++it;
        }
    }
}

// Send player state every frame after local movement is resolved
void mario::pages::LevelsPage::updateRemotePlayerForMultiplayer(const sf::RenderWindow *window, float dt) {
    if (gameMode != GameMode::SinglePlayer && networkManager) {
        networkManager->sendPlayerState(
            p_player->getPosition(),
            p_player->getVelocity()
        );
    }

    // Update remote player for multiplayer
    if (gameMode != GameMode::SinglePlayer && remotePlayer) {
        if (remotePlayerDead) {
            remotePlayer = nullptr;
        } else {
            sf::Vector2f currentPos = remotePlayer->getPosition();
            remotePlayer->syncNetworkState(
                currentPos + (remoteTargetPos - currentPos) * 0.2f,
                remoteTargetVel
            );
            collisionManager.checkCollisionPlayerWithBlocks(remotePlayer, blocks, items);
            remotePlayer->update(window, dt);
            if (remotePlayer->isDead() && !remotePlayerDead) {
                remotePlayerDead = true;
                // Don't remove immediately, let the death animation play
            }
        }
    }
}

void mario::pages::LevelsPage::updateBlocksEnemiesAndItems(const sf::RenderWindow *window, float dt) {
    // testBlock->update(window, dt);   
    for(auto &backgroundBlock : backgroundBlocks) {
        // if(backgroundBlock->getHitbox().findIntersection(cameraBounds)) {
            backgroundBlock->update(window, dt);
        // }
    }

    if(flagPole->getWinState()) testFireWorks->update(window, dt);
    flagPole->update(window, dt, p_player);

    for(auto &enemy : enemies) {
        if (!enemy->shouldDelete()) {
            mario::entity::Piranha* piranha = dynamic_cast<mario::entity::Piranha*>(enemy);
            mario::entity::Lakitu* lakitu = dynamic_cast<mario::entity::Lakitu*>(enemy);
            mario::entity::Ball* ball = dynamic_cast<mario::entity::Ball*>(enemy);
            if(piranha) {
                piranha->updateWithPlayer(window, dt, p_player);
            } else if(lakitu) {
                lakitu->updateWithPlayer(window, dt, p_player, enemies);
            } else if(ball) {
                ball->updateWithPlayer(window, dt, p_player);
            } else {
                enemy->update(window, dt);
            }
        }
    }

    if (gameMode != GameMode::SinglePlayer && networkManager) {
        enemyStateSendAccumulator += dt;
        if (enemyStateSendAccumulator >= enemyStateSendInterval) {
            enemyStateSendAccumulator = 0.f;
            
            for (auto &enemy : enemies) {
                if (enemy && !enemy->shouldDelete()) {
                    networkManager->sendEnemyState(
                        enemy->getNetworkId(),
                        enemy->getPosition(),
                        enemy->getVelocity(),
                        !enemy->isDead(),
                        enemy->getActive(),
                        enemy->getCurrentSpriteId(), 
                        enemy->isFaceForward()       
                    );
                }
            }
        }
    }

    for (auto &block : blocks) {
        if (!block->shouldDelete()) {
            block->update(window, dt);
        }
    }

    for(auto &item : items) {
        if (item && !item->isCollected()) {
            item->update(window, dt);
        }
    }
}

void mario::pages::LevelsPage::checkForPlayerFinishLevel() {
    if(p_player->isFinishLevel()) {
        if(++currLevelState.level > 3) {
            std::cerr << "Finish all level. Return to main menu.\n";

            _context->changePage(std::make_shared<mario::pages::MainMenuPage>(*_context));
        } else {
            _context->getSoundManager().playSound(mario::event::SoundEvent::LEVEL_ENTER);
            _context->changePage(std::make_shared<mario::pages::LevelsPage>(*_context, currLevelState, networkManager, gameMode));
        }
    }
}

// Camera logic - simplified to avoid weird zooming
void mario::pages::LevelsPage::updateCameraLogic(float dt) {
    if (gameMode != GameMode::SinglePlayer && remotePlayer) {
        // Calculate center point between both players
        sf::Vector2f player1Pos = p_player->getPosition();
        sf::Vector2f player2Pos = remotePlayer->getPosition();
        sf::Vector2f centerPoint = (player1Pos + player2Pos) / 2.0f;
        
        // Calculate distance between players
        float distance = std::sqrt(std::pow(player1Pos.x - player2Pos.x, 2) + 
                                std::pow(player1Pos.y - player2Pos.y, 2));
        
        // If players are reasonably close (within screen bounds), follow center point
        if (distance < 1200.0f) {
            camera.followTwoEntities(*p_player, *remotePlayer, dt, 600.0f, 0.9f, 1.1f);
        } else {
            // If too far apart, just follow local player
            camera.followEntity(*p_player, dt);
        }
    } else {
        // Single player mode - follow local player
        camera.followEntity(*p_player, dt);
    }

    camera.update(dt);
}

// Handle player death
void mario::pages::LevelsPage::handlePlayerDeath() {
    if (p_player->isDead()) {
        if (p_player->isInBehavior(mario::entity::PlayerBehavior::Dying)) {
            // Still in death animation, keep updating
        } else {
            // Death animation finished, handle game over
            if (gameMode != GameMode::SinglePlayer && networkManager) {
                networkManager->sendGameOver();
            }
            
            if (gameMode != GameMode::SinglePlayer) {
                camera.resetToDefaultView();
                _context->getSoundManager().playSound(mario::event::SoundEvent::GAME_OVER);
                _context->changePage(std::make_shared<mario::pages::GameOverPage>(*_context));
            } else {          
                if(currLevelState.num_lives > 0) {
                    currLevelState = mario::resource::LevelState(currLevelState.level, currLevelState.num_lives - 1, currLevelState.score, currLevelState.coins, currLevelState.characterType);
                    _context->getSoundManager().playSound(mario::event::SoundEvent::LEVEL_ENTER);
                    _context->changePage(std::make_shared<mario::pages::LevelsPage>(*_context, currLevelState, networkManager, gameMode));
                } else {
                    camera.resetToDefaultView();
                    _context->getSoundManager().playSound(mario::event::SoundEvent::GAME_OVER);
                    _context->changePage(std::make_shared<mario::pages::GameOverPage>(*_context));
                }
            }
        }
    }
}

// Check for hover state / UI hover logic
void mario::pages::LevelsPage::checkMenuButtonHoverLogic(const sf::RenderWindow *window) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
    sf::Vector2f worldMousePos = camera.screenToWorld(mousePos, *window);

    sf::FloatRect pauseRect = sf::FloatRect(pauseSprite->getPosition(), 
        sf::Vector2f(pauseTexture->getSize().x * pauseSprite->getScale().x, 
                     pauseTexture->getSize().y * pauseSprite->getScale().y));
    
    sf::FloatRect homeRect = sf::FloatRect(homeSprite->getPosition(), 
        sf::Vector2f(homeTexture->getSize().x * homeSprite->getScale().x, 
                     homeTexture->getSize().y * homeSprite->getScale().y));
    
    sf::FloatRect settingsRect = sf::FloatRect(settingsSprite->getPosition(), 
        sf::Vector2f(settingsTexture->getSize().x * settingsSprite->getScale().x, 
                     settingsTexture->getSize().y * settingsSprite->getScale().y));

    if (pauseRect.contains(sf::Vector2f(worldMousePos)) && !_isPaused) {
        pauseSprite->setTexture(*pauseHoverTexture);
    } else if (_isPaused) {
        pauseSprite->setTexture(*resumeTexture);
    } else {
        pauseSprite->setTexture(*pauseTexture);
    }

    if(homeRect.contains(sf::Vector2f(worldMousePos))) {
        homeSprite->setTexture(*homeHoverTexture);
    } else {
        homeSprite->setTexture(*homeTexture);
    }

    if(settingsRect.contains(sf::Vector2f(worldMousePos)) || isSettingsOpen) {
        settingsSprite->setTexture(*settingsHoverTexture);
    } else {
        settingsSprite->setTexture(*settingsTexture);
    }
}

/* ========================================================================================================================================================================== */

void mario::pages::LevelsPage::update(const sf::RenderWindow *window, float dt) {
    // Check for game over first
    if (gameOverReceivedForLocal || isGameOver()) {
        _context->getSoundManager().playSound(mario::event::SoundEvent::GAME_OVER);
        _context->changePage(std::make_unique<GameOverPage>(*_context));
        return;
    }

    sf::FloatRect cameraBounds = camera.getCameraBounds();
    cleanUpDeletedObject();

    // Handle network updates for multiplayer
    if (gameMode != GameMode::SinglePlayer && networkManager) {
        handleNetworkUpdates(dt);
    }
    
    if (backClicked) {
        float t = backAnimClock.getElapsedTime().asSeconds();

        if (t < backAnimDuration) {
            float progress = t / backAnimDuration;
            float scale = 0.9f - 0.1f * std::sin(progress * 3.14159f); 
            homeSprite->setScale(sf::Vector2f(scale, scale));
        } else {
            homeSprite->setScale(sf::Vector2f(0.9f, 0.9f)); // reset scale
            backClicked = false;

            _isPaused = !_isPaused;
            camera.resetToDefaultView();
            _context->changePage(std::make_shared<mario::pages::MainMenuPage>(*_context));
        }
    } else {
        homeSprite->setScale(sf::Vector2f(0.9f, 0.9f));
    }

    if (pauseClicked) {
        float t = pauseAnimClock.getElapsedTime().asSeconds();

        if (t < pauseAnimDuration) {
            float progress = t / pauseAnimDuration;
            float scale = 0.9f - 0.1f * std::sin(progress * 3.14159f); 
            pauseSprite->setScale(sf::Vector2f(scale, scale));
        } else {
            pauseSprite->setScale(sf::Vector2f(0.9f, 0.9f)); // reset scale
            pauseClicked = false;

            _isPaused = !_isPaused;
            if(_isPaused) {
                _context->getSoundManager().playSound(mario::event::SoundEvent::GAME_PAUSE);
                _context->getSoundManager().pauseBackgroundMusic();
                isSettingsPressed = true;
            } else {
                _context->getSoundManager().resumeBackgroundMusic();
                if (p_player) {
                    p_player->resetMove();
                }
                for (auto* enemy : enemies) {
                    mario::entity::Enemy* enemyPtr = dynamic_cast<mario::entity::Enemy*>(enemy);
                    if (enemyPtr && !enemyPtr->getActive()) {
                        enemyPtr->setActive(true); // Resume enemy activity
                    }
                }
                isSettingsPressed = false;
            }
        }
    } else {
        pauseSprite->setScale(sf::Vector2f(0.9f, 0.9f));
    }

    if (settingsClicked) {
        float t = settingsAnimClock.getElapsedTime().asSeconds();

        if (t < settingsAnimDuration) {
            float progress = t / settingsAnimDuration;
            float scale = 0.9f - 0.1f * std::sin(progress * 3.14159f); 
            settingsSprite->setScale(sf::Vector2f(scale, scale));
        } else {
            settingsSprite->setScale(sf::Vector2f(0.9f, 0.9f)); // reset scale
            settingsClicked = false;

            isSettingsOpen = !isSettingsOpen;
            if(!isSettingsPressed) {
                _isPaused = !_isPaused;
            }
            if(isSettingsOpen) {
                _context->getSoundManager().playSound(mario::event::SoundEvent::GAME_PAUSE);
                _isPaused = true;
            }
        }
    } else {
        settingsSprite->setScale(sf::Vector2f(0.9f, 0.9f));
    }

    if(!_isPaused) {
        if(!p_player->isTransforming() && !p_player->isInBehavior(mario::entity::PlayerBehavior::Dying) && !p_player->isInBehavior(mario::entity::PlayerBehavior::Climbing)
            && !p_player->isInBehavior(mario::entity::PlayerBehavior::GoToFortress) && !p_player->isInBehavior(mario::entity::PlayerBehavior::EnterFortress)
            && !p_player->isInBehavior(mario::entity::PlayerBehavior::FinishLevel)) {
                currLevelState.update(dt);
                if(currLevelState.times <= sf::seconds(0.f)) {
                    _context->getSoundManager().playSound(mario::event::SoundEvent::TIME_WARNING);
                    p_player->changePlayerBehavior(mario::entity::PlayerBehavior::Dying);
                    currLevelState.times = sf::seconds(0.f);
                }
            }

        p_player->update(window, dt);
        p_player->updateToLevelState(currLevelState);
        checkForPlayerFinishLevel();

        /*std::cerr << "PLAYER X POS: " << p_player->getPosition().x << '\n';
        if(p_player->getPosition().x >= 7980)
            p_player->enterFortressDoor();*/

        updateRemotePlayerForMultiplayer(window, dt);
        if(!p_player->isTransforming())
            updateBlocksEnemiesAndItems(window, dt);

        collisionManager.checkCollisionPlayerWithBlocks(p_player, blocks, items);
        
        if(!p_player->isTransforming()) {
            collisionManager.updateCameraBounds(cameraBounds);

            // handle collision
            if(currLevelState.level == 1 && p_player->getPosition().x >= 8180 && (p_player->getPlayerBehavior() != mario::entity::PlayerBehavior::EnterFortress)) {
                p_player->setPosition(sf::Vector2f(8180, p_player->getPosition().y));
                _context->getSoundManager().playSound(mario::event::SoundEvent::LEVEL_CLEAR);
                p_player->enterFortressDoor();
            }
            else if(currLevelState.level == 2 && p_player->getPosition().x >= 7540 && (p_player->getPlayerBehavior() != mario::entity::PlayerBehavior::EnterFortress)) {
                p_player->setPosition(sf::Vector2f(7540, p_player->getPosition().y));
                _context->getSoundManager().playSound(mario::event::SoundEvent::LEVEL_CLEAR);
                p_player->enterFortressDoor();
            }
            else if(currLevelState.level == 3 && p_player->getPosition().x >= 8340 && (p_player->getPlayerBehavior() != mario::entity::PlayerBehavior::EnterFortress)) {
                p_player->setPosition(sf::Vector2f(8340, p_player->getPosition().y));
                _context->getSoundManager().playSound(mario::event::SoundEvent::LEVEL_CLEAR);
                p_player->enterFortressDoor();
            }
            collisionManager.checkCollisionEnemyWithBlocks(enemies, blocks);
            collisionManager.checkCollisionPlayerWithEnemies(p_player, enemies);
            collisionManager.checkCollisionPlayerWithItems(p_player, items);
            collisionManager.checkCollisionItemsWithBlocks(items, blocks);
            collisionManager.checkCollisionEnemyWithEnemy(enemies);
            flagPole->reactToCollision(p_player);

            // Check for item collection and notify network
            checkItemCollection();
            
            // Check for enemy defeats and notify network
            checkEnemyDefeats();
        

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

            // if (gameMode != GameMode::SinglePlayer && remotePlayer) {
        //     if (p_player->getHitbox().findIntersection(remotePlayer->getHitbox())) {
        //         sf::Vector2f diff = p_player->getPosition() - remotePlayer->getPosition();
        //         float len = std::max(1.f, std::sqrt(diff.x * diff.x + diff.y * diff.y));
        //         diff /= len;
        //         p_player->setPosition(p_player->getPosition() + diff * 5.f);
        //         remotePlayer->setPosition(remotePlayer->getPosition() - diff * 5.f);
        //     }
        // }

            updateCameraLogic(dt);
            
            currLevelState.stateType = p_player->getPlayerStateType();
            p_levelDataManager->update(dt, currLevelState);
            removeCollectedItems();

            handlePlayerDeath();
        }
    }

    checkMenuButtonHoverLogic(window);

    // Settings
    if (isSettingsOpen) {
        musicSlider->update(*window);
        sfxSlider->update(*window);
    }

    // itemManager->update(window, dt);
    // itemManager->processSpawnTriggers(p_player, dt);
}

/* ========================================================================================================================================================================== */

void mario::pages::LevelsPage::handleEvent(const sf::RenderWindow *window, const sf::Event &event) {
    if (const auto key = event.getIf<sf::Event::KeyPressed>(); (key && key->code == sf::Keyboard::Key::Escape && !isSettingsOpen 
      || !_isPaused && event.is<sf::Event::FocusLost>() && gameMode == GameMode::SinglePlayer)) {
        _isPaused = !_isPaused;
        if(_isPaused) {
            _context->getSoundManager().playSound(mario::event::SoundEvent::GAME_PAUSE);
            _context->getSoundManager().pauseBackgroundMusic();
        } else {
            _context->getSoundManager().resumeBackgroundMusic();
            if(p_player) {
                p_player->resetMove();
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
            sf::Vector2f worldMousePos = camera.screenToWorld(mousePos, *window);   

            sf::FloatRect pauseRectF = sf::FloatRect(pauseSprite->getPosition(), sf::Vector2f(pauseTexture->getSize().x * pauseSprite->getScale().x, pauseTexture->getSize().y * pauseSprite->getScale().y));
            sf::FloatRect homeRectF = sf::FloatRect(homeSprite->getPosition(), sf::Vector2f(homeTexture->getSize().x * homeSprite->getScale().x, homeTexture->getSize().y * homeSprite->getScale().y));
            sf::FloatRect settingsRectF = sf::FloatRect(settingsSprite->getPosition(), sf::Vector2f(settingsTexture->getSize().x * settingsSprite->getScale().x, settingsTexture->getSize().y * settingsSprite->getScale().y));

            if(pauseRectF.contains(sf::Vector2f(worldMousePos)) && !isSettingsOpen) {
                pauseClicked = true;
                pauseAnimClock.restart();
                // _isPaused = !_isPaused;
                // if(_isPaused) {
                //     _context->getSoundManager().playSound(mario::event::SoundEvent::GAME_PAUSE);
                //     _context->getSoundManager().pauseBackgroundMusic();
                //     isSettingsPressed = true;
                // } else {
                //     _context->getSoundManager().resumeBackgroundMusic();
                //     if (p_player) {
                //         p_player->resetMove();
                //     }
                //     for (auto* enemy : enemies) {
                //         mario::entity::Enemy* enemyPtr = dynamic_cast<mario::entity::Enemy*>(enemy);
                //         if (enemyPtr && !enemyPtr->getActive()) {
                //             enemyPtr->setActive(true); // Resume enemy activity
                //         }
                //     }
                //     isSettingsPressed = false;
                // }
            } else      
                if(homeRectF.contains(sf::Vector2f(worldMousePos))) {
                    backClicked = true;
                    backAnimClock.restart();
                    // _context->changePage(std::make_shared<mario::pages::MainMenuPage>(*_context));
                } else 
                    if(settingsRectF.contains(sf::Vector2f(worldMousePos))) {
                        settingsClicked = true;
                        settingsAnimClock.restart();
                        // isSettingsOpen = !isSettingsOpen;
                        // if(!isSettingsPressed) {
                        //     _isPaused = !_isPaused;
                        // }
                        // if(isSettingsOpen) {
                        //     _context->getSoundManager().playSound(mario::event::SoundEvent::GAME_PAUSE);
                        //     _isPaused = true;
                        // }
                    }
        } 
    }

    if(isSettingsOpen) {
        musicSlider->handleEvent(event, *window);
        sfxSlider->handleEvent(event, *window);
    }

    if(!_isPaused) {
        p_player->handleEvent(window, event);
        p_inputManager->handleEvent(p_player, event);
        tileMap->handleEvent(window, event);
        for (auto &enemy : enemies) {
            enemy->handleEvent(window, event);
        }
    }
}

/* ========================================================================================================================================================================== */

void mario::pages::LevelsPage::checkItemCollection() {
    for (auto& item : items) {
        if (item && item->isCollected() && !item->isNetworkNotified()) {
            // Send network message about collected item
            if (gameMode != GameMode::SinglePlayer && networkManager) {
                networkManager->sendItemCollected(item->getNetworkId(), item->getPosition());
            }
            item->setNetworkNotified(true);
        }
    }
}

void mario::pages::LevelsPage::checkEnemyDefeats() {
    for (auto& enemy : enemies) {
        if (enemy && enemy->isDead() && !enemy->isNetworkNotified()) {
            // Send network message about defeated enemy
            if (gameMode != GameMode::SinglePlayer && networkManager) {
                networkManager->sendEnemyDefeated(enemy->getNetworkId(), enemy->getPosition());
            }
            enemy->setNetworkNotified(true);
        }
    }
}

void mario::pages::LevelsPage::handleNetworkUpdates(float dt) {
    static const int localPlayerId = (gameMode == GameMode::Host) ? 0 : 1;

    if (!networkManager) return;

    std::unique_ptr<NetworkMessage> msg;
    while ((msg = networkManager->pollMessage())) {
        switch (msg->type) {
            case NetworkMessage::PlayerState:
                if (remotePlayer) {
                    remoteTargetPos = msg->position;
                    remoteTargetVel = msg->velocity;
                    if (remoteTargetVel.x > 0) {
                        remotePlayer->setFaceForward(true);
                    } else if (remoteTargetVel.x < 0) {
                        remotePlayer->setFaceForward(false);
                    }
                }
                break;
            case NetworkMessage::EnemyState:
                if (gameMode == GameMode::Client) {
                    for (auto &enemy : enemies) {
                        if (enemy && enemy->getNetworkId() == msg->enemyId) {
                            enemy->syncNetworkState(
                                msg->enemyPosition, 
                                msg->enemyVelocity,
                                msg->isAlive,
                                msg->isActive,
                                msg->spriteId,
                                msg->faceForward,
                                msg->timestamp
                            );
                            break;
                        }
                    }
                }
                break;
            case NetworkMessage::PlayerPowerupState:
                if (msg->playerId != localPlayerId) {
                    handleRemotePlayerPowerupState(msg->playerId, msg->powerupState);
                }
                break;
            case NetworkMessage::ItemCollected:
                handleRemoteItemCollection(msg->itemId, msg->position);
                break;

            case NetworkMessage::EnemyDefeated:
                handleRemoteEnemyDefeat(msg->enemyId, msg->position);
                break;

            case NetworkMessage::GameOver:
                if (msg->playerId == localPlayerId) {
                    gameOverReceivedForLocal = true;
                } else {
                    remotePlayerDead = true;
                }
                break;

            case NetworkMessage::PlayerWin:
                // Future win condition
                break;
        }
    }
}

void mario::pages::LevelsPage::handleRemoteItemCollection(int itemId, const sf::Vector2f& position) {
    // Find and mark item as collected by remote player
    for (auto& item : items) {
        if (item && item->getNetworkId() == itemId) {
            if (!item->isCollected()) {
                item->setCollected(true);
                item->setNetworkNotified(true);
            }
            break;
        }
    }
}

void mario::pages::LevelsPage::handleRemoteEnemyDefeat(int enemyId, const sf::Vector2f& position) {
    // Find and mark enemy as defeated by remote player
    for (auto& enemy : enemies) {
        if (enemy && enemy->getNetworkId() == enemyId) {
            if (!enemy->isDead()) {
                enemy->setDead(true);
                enemy->setNetwork(true);
            }
            break;
        }
    }
}

void mario::pages::LevelsPage::handleRemotePlayerPowerupState(int playerId, int powerupState) {
    int localPlayerId = (gameMode == GameMode::Host) ? 0 : 1;

    // Ignore invalid states
    mario::entity::player_state::PlayerStateType newState;
    switch (powerupState) {
        case 0: newState = mario::entity::player_state::PlayerStateType::Small; break;
        case 1: newState = mario::entity::player_state::PlayerStateType::Super; break;
        case 2: newState = mario::entity::player_state::PlayerStateType::Fire; break;
        default:
            std::cerr << "[Network] Invalid powerup state received: " << powerupState << "\n";
            return;
    }

    // Only apply state changes to the remote player, never to local player
    if (playerId != localPlayerId && remotePlayer) {
        auto currentState = remotePlayer->getPlayerStateType();
        if (currentState != newState) {
            std::cout << "[Network] Remote player " << playerId << " changing from " 
                     << static_cast<int>(currentState) << " to " << powerupState << "\n";
            
            // Use direct state change for remote player (bypass network sending)
            remotePlayer->_isRemotePlayer = true; // Ensure it's marked as remote
            remotePlayer->changeState(newState);
        }
    } else {
        std::cout << "[Network] Ignoring powerup message for local player " << localPlayerId << "\n";
    }
}

/* ========================================================================================================================================================================== */

void mario::pages::LevelsPage::renderLevelState(sf::RenderWindow *window, mario::resource::LevelState levelState) {
    sf::Vector2f windowSize = _context->getWindowSize();
    sf::Vector2f rectMove(0.f, 0.f);
    int rectX = windowSize.x / 5.f;
    int rectY1 = 2, rectY2 = 35;

    sf::Text text(*marioFont, "", 30);

    text.setString("SCORE");
    rePositionTextToMiddle(text, rectX, rectY1);
    text.setPosition(getPositionRelativeToCamera(text.getPosition()));
    text.move(rectMove);
    window->draw(text);

    text.setString(to_string(p_player->getScore()));
    rePositionTextToMiddle(text, rectX, rectY2);
    text.setPosition(getPositionRelativeToCamera(text.getPosition()));
    text.move(rectMove);
    window->draw(text);

    rectMove.x += rectX;
    text.setString("COINS");
    rePositionTextToMiddle(text, rectX, rectY1);
    text.setPosition(getPositionRelativeToCamera(text.getPosition()));
    text.move(rectMove);
    window->draw(text);

    text.setString(to_string(p_player->getCoins()));
    rePositionTextToMiddle(text, rectX, rectY2);
    text.setPosition(getPositionRelativeToCamera(text.getPosition()));
    text.move(rectMove);
    window->draw(text);

    rectMove.x += rectX;
    text.setString("WORLD");
    rePositionTextToMiddle(text, rectX, rectY1);
    text.setPosition(getPositionRelativeToCamera(text.getPosition()));
    text.move(rectMove);
    window->draw(text);

    text.setString(to_string(levelState.level));
    rePositionTextToMiddle(text, rectX, rectY2);
    text.setPosition(getPositionRelativeToCamera(text.getPosition()));
    text.move(rectMove);
    window->draw(text);

    rectMove.x += rectX;
    text.setString("TIME");
    rePositionTextToMiddle(text, rectX, rectY1);
    text.setPosition(getPositionRelativeToCamera(text.getPosition()));
    text.move(rectMove);
    window->draw(text);

    text.setString(to_string(int(levelState.times.asSeconds())));
    rePositionTextToMiddle(text, rectX, rectY2);
    text.setPosition(getPositionRelativeToCamera(text.getPosition()));
    text.move(rectMove);
    window->draw(text);

    rectMove.x += rectX;
    text.setString("LIVES");
    rePositionTextToMiddle(text, rectX, rectY1);
    text.setPosition(getPositionRelativeToCamera(text.getPosition()));
    text.move(rectMove);
    window->draw(text);

    text.setString(to_string(p_player->getLives()));
    rePositionTextToMiddle(text, rectX, rectY2);
    text.setPosition(getPositionRelativeToCamera(text.getPosition()));
    text.move(rectMove);
    window->draw(text);
}

sf::Vector2f mario::pages::LevelsPage::getPositionRelativeToCamera(sf::Vector2f pos) {
    sf::FloatRect rect = camera.getCameraBounds();
    sf::Vector2f cameraPos = rect.position;

    return sf::Vector2f(pos.x + cameraPos.x, pos.y + cameraPos.y);
}

void mario::pages::LevelsPage::rePositionTextToMiddle(sf::Text &text, int rectX, int rectY) {
    float textLenX = text.getGlobalBounds().size.x;
    float textLenY = text.getGlobalBounds().size.y;

    text.setFillColor(sf::Color::White);
    text.setPosition(sf::Vector2f(int((rectX - textLenX) / 2.0), rectY));
}

void mario::pages::LevelsPage::render(sf::RenderWindow *window) {
    window->draw(*backgroundSprite);
    if(flagPole->getWinState())testFireWorks->render(window);
    // std::cout << "Position's player: " << p_player->getPosition().x << ", " << p_player->getPosition().y << "\n";
    camera.applyTo(*window);

    // draw background here

    sf::FloatRect cameraBounds = camera.getCameraBounds();
    sf::Vector2f topLeft = cameraBounds.position;
    
    // Set new position of sprites with camera
    pauseSprite->setPosition(topLeft + sf::Vector2f(20.f, 97.f));
    homeSprite->setPosition(topLeft + sf::Vector2f(20.f, 30.f));
    settingsSprite->setPosition(topLeft + sf::Vector2f(20.f, 164.f));

    sf::Vector2u windowSize(1280, 720); // Size of window
    sf::Vector2f cameraCenter = camera.getPosition(); // Center of camera

    if (panelSprite) {
        sf::Vector2u textureSize = panelTexture->getSize();
        panelSprite->setScale({0.6f, 0.6f});
        sf::Vector2f panelSize = sf::Vector2f(textureSize.x * 0.6f, textureSize.y * 0.6f);
        panelSprite->setPosition(cameraCenter - panelSize / 2.0f);
    }

    if (settingsPanelSprite) {
        settingsPanelSprite->setOrigin(sf::Vector2f(settingsPanelTexture->getSize().x / 2.f, settingsPanelTexture->getSize().y / 2.f));
        settingsPanelSprite->setPosition(cameraCenter);
        settingsPanelSprite->setScale(sf::Vector2f(0.7f, 0.7f));

        sf::Vector2f panelCenter = settingsPanelSprite->getPosition();
        musicSlider->setPosition(sf::Vector2f(panelCenter.x + 80, panelCenter.y - 50));
        sfxSlider->setPosition(sf::Vector2f(panelCenter.x + 80, panelCenter.y + 50));
    }

    // testBlock->render(window);

    for(auto &block : groundBlocks){
        if (block->getHitbox().findIntersection(cameraBounds)) {
            block->render(window);
        }
    }

    flagPole->render(window);

    for (auto &backgroundBlock : backgroundBlocks) {
        if (backgroundBlock->getHitbox().findIntersection(cameraBounds)) {
            backgroundBlock->render(window);
        }
    }

    // Render Piranha enemies before blocks
    for (auto* enemy : enemies) {
        if (!enemy->shouldDelete() && enemy->getHitbox().findIntersection(cameraBounds)) {
            mario::entity::Piranha* piranha = dynamic_cast<mario::entity::Piranha*>(enemy);
            if (piranha) {
                piranha->render(window);
            }
        }
    }

    for (auto &block : blocks) {
        if (!block->shouldDelete() && block->getHitbox().findIntersection(cameraBounds)) {
            block->render(window);
        }
    }

    p_player->render(window);
    if (gameMode != GameMode::SinglePlayer && remotePlayer) {
        remotePlayer->render(window);
    }

    // Render enemies
    for (auto* enemy : enemies) {
        if (!enemy->shouldDelete() && enemy->getHitbox().findIntersection(cameraBounds)) {
            mario::entity::Piranha* piranha = dynamic_cast<mario::entity::Piranha*>(enemy);
            if (!piranha) { 
                enemy->render(window);
            }
        }
    }

    p_player->render(window);

    // // Render enemies
    // for (auto* enemy : enemies) {
    //     if (!enemy->shouldDelete()) {
    //         enemy->render(window);
    //     }
    // }

    // for (auto &block : blocks) {
    //     block->render(window);
    // }

    for (auto &item : items) {
        if (item && !item->isCollected() && item->getHitbox().findIntersection(cameraBounds)) {
            item->render(window);
        }
    }


    p_player->render(window);


    //testItem->render(window);

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

    renderLevelState(window, currLevelState);
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