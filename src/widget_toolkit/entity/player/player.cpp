#include<bits/stdc++.h>
#include "player.hpp"

#define FILE_PATH "../../asset/sprites/"

mario::entity::Player::Player(sf::Vector2f spawnPoint, CharacterListType characterType, player_state::PlayerStateType stateType) : _characterType(characterType), _isAlive(true) {
    _isDeadAlready = false;
    playerBehavior = PlayerBehavior::Normal;

    std::string fontPath = "../../asset/fonts/SuperMario256.ttf";
    popUpScoreList = new PopUpTextList(1.5f, 25, sf::Vector2f(0, -25.f), fontPath, sf::Color::Black);

    p_body = new DynamicBox(spawnPoint, sf::Vector2f(40.f, 40.f));
    if(characterType == CharacterListType::Mario) {
        p_animation = new Animation(FILE_PATH"mario.json", FILE_PATH"mario_sheets.png", PLAYER_SCALE, "mario-small.idle[0]");
        p_stateManager = new mario::entity::player_state::MarioStateManager(p_animation, p_body, stateType);
    }

    if(characterType == CharacterListType::Luigi) {
        p_animation = new Animation(FILE_PATH"luigi.json", FILE_PATH"luigi_sheets.png", PLAYER_SCALE, "luigi-small.idle[0]");
        p_stateManager = new mario::entity::player_state::LuigiStateManager(p_animation, p_body, stateType);
    }
}

mario::entity::Player::~Player() {
    delete p_stateManager;
    delete popUpScoreList;
}

/* =================================================================================================================================================================== */

void mario::entity::Player::jump(bool isReleased) {
    if(!_canMove)
        return;

    p_body->jump(isReleased);
}

void mario::entity::Player::move(bool isMoveRight, bool isReleased) {
    if(!_canMove)
        return;

    p_body->move(isMoveRight, isReleased);
}

void mario::entity::Player::shotFireball(bool isReleased) {
    if(!_canMove)
        return;

    
}

/* =================================================================================================================================================================== */

sf::Vector2f mario::entity::Player::getVelocity() const {
    return p_body->getVelocity();
}

void mario::entity::Player::setVelocity(sf::Vector2f vel) {
    p_body->setVelocity(vel);
}

void mario::entity::Player::rotateDirection() {
    p_animation->rotate();
}

/* =================================================================================================================================================================== */

void mario::entity::Player::managePlayerAnimation() {
    if(playerBehavior != PlayerBehavior::Normal)
        return;

    if(!p_body->isOnGround()) { // change texture to jumping
            p_stateManager->setAnimation(p_animation, "jump[0]");
            p_animation->setAnimationState(false);
        } else {
            if(p_body->isNotMoving()) { // change texture to idle
                p_stateManager->setAnimation(p_animation, "idle[0]");
                p_animation->setAnimationState(false);
            } else 
                if(p_animation->getAnimationState() == false) { // change to run animation
                    p_stateManager->setAnimation(p_animation, "idle[0]");
                    p_animation->setAnimationState(true);
                }
                
            hasPlayedJumpSound_ = false;
        }
}

void mario::entity::Player::updatePlayerBehavior(float dt) {
    behaviorTimer -= sf::seconds(dt);
    if(behaviorTimer <= sf::seconds(0)) {
        if(playerBehavior == PlayerBehavior::Invincible || playerBehavior == PlayerBehavior::Shadow || playerBehavior == PlayerBehavior::TransformSTB)
            changePlayerBehavior(PlayerBehavior::Normal);

        if(playerBehavior == PlayerBehavior::TransformBTS)
            changePlayerBehavior(PlayerBehavior::Shadow);

        if(playerBehavior == PlayerBehavior::Dying)
            changePlayerBehavior(PlayerBehavior::AlreadyDead);
    }
}

void mario::entity::Player::togglePlayerMove(bool canMove) {
    if(_canMove == canMove)
        return;

    if(!canMove) { // released all Player's button
        jump(true);
        move(0, true);
        move(1, true);
        shotFireball(true);
    }
    
    _canMove = canMove;
}

/* =================================================================================================================================================================== */

void mario::entity::Player::changePlayerBehavior(PlayerBehavior newBehavior) {
    if(playerBehavior == newBehavior)
        return;

    // remove last Behavior
    if(playerBehavior == PlayerBehavior::Shadow) {
        p_animation->setFlicker(false);
    }

    if(playerBehavior == PlayerBehavior::TransformSTB) {
        // stop powerup sound

        _canCollisionWithEnemy = _canCollisionWithItem = true;
        p_stateManager->changeToSuperState(p_animation, p_body);

        _isTransforming = false;
        togglePlayerMove(true);
    }

    if(playerBehavior == PlayerBehavior::TransformBTS) {
        _canCollisionWithEnemy = _canCollisionWithItem = true;
        p_stateManager->changeToSmallState(p_animation, p_body);

        _isTransforming = false;
        togglePlayerMove(true);
    }

    if(playerBehavior == PlayerBehavior::Invincible) {
        // stop invincible behavior sound (Player after loot star 10s, ran out of time)
    }

    if(playerBehavior == PlayerBehavior::Climbing) {
        _canCollisionWithEnemy = _canCollisionWithItem = true;
        togglePlayerMove(true);
    }

    if(playerBehavior == PlayerBehavior::Dying) {
        _canCollisionWithEnemy = _canCollisionWithItem = _canCollisionWithBlock = true;
        _isAlive = true;

        p_body->jump(true);
        togglePlayerMove(true);
    }

    /* =================================================================================================================================================================== */

    // change to new Behavior

    playerBehavior = newBehavior;
    if(newBehavior == PlayerBehavior::Shadow) {
        p_animation->setFlicker(true);
        behaviorTimer = sf::seconds(1.5f);
    }

    if(newBehavior == PlayerBehavior::TransformSTB) {
        _canCollisionWithEnemy = _canCollisionWithItem = false;
        togglePlayerMove(false);

        // play powerup sound
        _isTransforming = true;
        behaviorTimer = sf::seconds(1.f);
        p_animation->clearAnimationStep();

        std::string currPlayerStateID = p_stateManager->getCurrentPlayerStateID();
        std::string nextPlayerStateID = p_stateManager->getNextPlayerStateID();
        std::string prefixIDAnimation = currPlayerStateID + "." + "become-" + nextPlayerStateID;
        for (int i = 0; i < 10; ++i)
            p_animation->addAnimationStep(prefixIDAnimation + "[" + std::to_string(i) + "]");

        p_animation->setAnimationState(true);
    }

    if(newBehavior == PlayerBehavior::TransformBTS) {
        _canCollisionWithEnemy = _canCollisionWithItem = false;
        togglePlayerMove(false);

        _isTransforming = true;
        behaviorTimer = sf::seconds(1.5f);
        p_animation->clearAnimationStep();

        std::string currPlayerStateID = p_stateManager->getCurrentPlayerStateID();
        for (int i = 0; i < 15; ++i)
            p_animation->addAnimationStep(currPlayerStateID + ".hit[" + std::to_string(i) + "]");

        p_animation->setAnimationState(true);
    }

    if(newBehavior == PlayerBehavior::Invincible) {
        // play invincible behavior sound (Player after loot star)

        behaviorTimer = sf::seconds(10.f); // 10 seconds of invincibility
    }

    if(newBehavior == PlayerBehavior::Climbing) {
        _canCollisionWithEnemy = _canCollisionWithItem = false;
        togglePlayerMove(false);
    }

    if(newBehavior == PlayerBehavior::Dying) { 
        _canCollisionWithEnemy = _canCollisionWithItem = _canCollisionWithBlock = false;
        // play dead sound
        
        _isAlive = false;
        togglePlayerMove(false);
        
        behaviorTimer = sf::seconds(3);
        p_animation->clearAnimationStep();
        
        p_body->setVelocity(sf::Vector2f(0.f, 0.f));
        p_body->jump(false);

        p_stateManager->setDeadAnimation(p_animation);
        p_animation->setAnimationState(false);
    }

    if(newBehavior == PlayerBehavior::AlreadyDead) {
        _canCollisionWithEnemy = _canCollisionWithItem = _canCollisionWithBlock = false;
        _isAlive = false;
        _isDeadAlready = true;
        togglePlayerMove(false);
    }
}

/* =================================================================================================================================================================== */

void mario::entity::Player::changeState(player_state::PlayerStateType newStateType) {
    player_state::PlayerStateType lastState = p_stateManager->getCurrentState();
    if(newStateType == lastState)
        return;
    
    if(lastState == player_state::PlayerStateType::Small) {
        // newState == player_state::PlayerStateType::Super
        changePlayerBehavior(PlayerBehavior::TransformSTB);
    } else 
        if(newStateType == player_state::PlayerStateType::Small) {
            changePlayerBehavior(PlayerBehavior::TransformBTS);
        } else {
            changePlayerBehavior(PlayerBehavior::Normal);
            p_stateManager->changeToFireState(p_animation, p_body);
        }
}

/* =================================================================================================================================================================== */

void mario::entity::Player::update(const sf::RenderWindow *window, float dt) {
    if(p_body->getPosition().y >= 720.f)
        changePlayerBehavior(PlayerBehavior::Dying);

    if(_isAlive) {
        managePlayerAnimation();

        // change state for debugging
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num1)) {
            std::cerr << "CALL TO FUNCTION CHANGE TO SMALL STATE\n";
            changeState(player_state::PlayerStateType::Small);
            std::cerr << "SUCCESFULLY\n";
        }
    
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num2)) {
            std::cerr << "CALL TO FUNCTION CHANGE TO SUPER STATE\n";
            changeState(player_state::PlayerStateType::Super);
            std::cerr << "SUCCESFULLY\n";
        }
    
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num3)) {
            std::cerr << "CALL TO FUNCTION CHANGE TO FIRE STATE\n";
            changeState(player_state::PlayerStateType::Fire);
            std::cerr << "SUCCESFULLY\n";
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num4)) {
            std::cerr << "CALL TO FUNCTION HITTING PLAYER\n";
            beingHit();
            std::cerr << "SUCCESFULLY\n";
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num5)) {
            std::cerr << "CALL TO FUNCTION KILLED PLAYER\n";
            changePlayerBehavior(PlayerBehavior::Dying);
            std::cerr << "SUCCESFULLY\n";
        }
    }

    popUpScoreList->update(window, dt);

    updatePlayerBehavior(dt);
    p_animation->update(window, dt);
    // p_body->updateSize(p_animation);
    if(playerBehavior != PlayerBehavior::TransformBTS && playerBehavior != PlayerBehavior::TransformSTB)
        p_body->update(dt);
    
    if(p_animation->isFaceForward() != p_body->isFaceForward())
        rotateDirection();
}

void mario::entity::Player::updateToLevelState(mario::resource::LevelState &levelState) {
    levelState.coins += coinCount;
    levelState.num_lives += lives + levelState.coins / 100;
    levelState.score += score;
    levelState.coins %= 100;

    lives = coinCount = score = 0;
}

void mario::entity::Player::handleEvent(const sf::RenderWindow *window, const sf::Event &event) {
}

void mario::entity::Player::render(sf::RenderWindow *window) {
    Entity::render(window);
    popUpScoreList->render(window);
}

/* =================================================================================================================================================================== */

void mario::entity::Player::setOnGround(bool isOnGround) {
    p_body->setOnGround(isOnGround);
    if(isOnGround)
        scoreMultiplier = 0;
}

void mario::entity::Player::resetJump() {
    p_body->resetJump();
}

void mario::entity::Player::beingHit() {
    if(playerBehavior != PlayerBehavior::Normal)
        return;

    if(getPlayerStateType() == player_state::PlayerStateType::Small) {
        changePlayerBehavior(PlayerBehavior::Dying);
    } else {
        changeState(player_state::PlayerStateType::Small);
    }
}

/* =================================================================================================================================================================== */

bool mario::entity::Player::isDead() const {
    return _isDeadAlready;
}

bool mario::entity::Player::isShadow() const {
    return (playerBehavior == PlayerBehavior::Shadow);
}

bool mario::entity::Player::isTransforming() const {
    return _isTransforming;
}

bool mario::entity::Player::isInBehavior(PlayerBehavior behavior) const {
    return (playerBehavior == behavior);
}

bool mario::entity::Player::canCollisionWithEnemy() const {
    return _canCollisionWithEnemy;
}

bool mario::entity::Player::canCollisionWithItem() const {
    return _canCollisionWithItem;
}

bool mario::entity::Player::canCollisionWithBlock() const {
    return _canCollisionWithBlock;
}

/* =================================================================================================================================================================== */

void mario::entity::Player::addPopUpScore(int _score) {
    score += _score;
    popUpScoreList->addAPopUpText(p_body->getPosition(), std::to_string(_score));
}

void mario::entity::Player::breakBrick() {
    // Play break brick sound
    addPopUpScore(50);
}

void mario::entity::Player::hitEmptyBlock() {
    // Play hit empty block sound
}

void mario::entity::Player::collectCoin() {
    ++coinCount;
    addPopUpScore(200);
    
    // 1-up at 100 coins
    if (coinCount >= 100) {
        lives += coinCount / 100;
        coinCount %= 100;
        // Play 1-up sound
    }
    
    // Play coin sound
}

void mario::entity::Player::collectCoinInBlock() {
    collectCoin();
}

void mario::entity::Player::collectRedMushroom() {
    addPopUpScore(1000);
    if (getPlayerStateType() == player_state::PlayerStateType::Small) {
        changeState(player_state::PlayerStateType::Super);
        // Play power-up sound
    } else {
        // Already super or fire, give points instead
    }

    std::cerr << "Collected Red Mushroom: " << score << " points\n";
}

void mario::entity::Player::collectFireFlower() {
    addPopUpScore(1000);
    if (getPlayerStateType() == player_state::PlayerStateType::Small) {
        changeState(player_state::PlayerStateType::Super);
        // Play power-up sound
    } else if(getPlayerStateType() == player_state::PlayerStateType::Super) {
        changeState(player_state::PlayerStateType::Fire);
        // Play power-up sound
    }
}

void mario::entity::Player::collect1UpMushroom() {
    lives++;
    addPopUpScore(1000);
    // Play 1-up sound
}

void mario::entity::Player::collectStarman() {
    changePlayerBehavior(PlayerBehavior::Invincible);
}

void mario::entity::Player::jumpOnEnemyHead() {
    int tempMult = scoreMultiplier;

    resetJump();
    setOnGround(true);
    jump(false);

    scoreMultiplier = tempMult;
    addPopUpScore(++scoreMultiplier * 100);
}

/* =================================================================================================================================================================== */

mario::entity::PlayerBehavior mario::entity::Player::getPlayerBehavior() const {
    return playerBehavior;
}

mario::entity::CharacterListType mario::entity::Player::getCharacterType() const {
    return _characterType;
}

mario::entity::player_state::PlayerStateType mario::entity::Player::getPlayerStateType() const {
    return p_stateManager->getCurrentState();
}

void mario::entity::Player::setJumpSoundPlayed(bool played) {
    hasPlayedJumpSound_ = played;
}

bool mario::entity::Player::hasPlayedJumpSound() const { 
    return hasPlayedJumpSound_; 
}

#undef FILE_PATH