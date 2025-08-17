#include<bits/stdc++.h>
#include "player.hpp"

#define FILE_PATH "../../asset/sprites/"

mario::entity::Player::Player(sf::Vector2f spawnPoint, CharacterListType characterType, player_state::PlayerStateType stateType, mario::audio::SoundManager& soundManager)
             : _characterType(characterType), _isAlive(true), soundManager(soundManager) {
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

    p_fireballList = std::make_unique<mario::entity::FireballList>();
}

mario::entity::Player::~Player() {
    delete p_stateManager;
    delete popUpScoreList;
}

/* =================================================================================================================================================================== */

void mario::entity::Player::loadDataFrom(const mario::resource::LevelState &levelState) {
    coinCount = levelState.coins;
    lives = levelState.num_lives;
    score = levelState.score;
}

/* =================================================================================================================================================================== */

void mario::entity::Player::jump(bool isReleased) {
    if(!_canMove)
        return;

    if (!isReleased && p_body->isOnGround()) {
        soundManager.playSound(mario::event::SoundEvent::PLAYER_JUMP); // Phát âm thanh nhảy
        hasPlayedJumpSound_ = true;
    }

    p_body->jump(isReleased);
}

void mario::entity::Player::jumpByANumberOfJumps(bool isReleased, int numJumps) {
    if(!_canMove)
        return;

    if (!isReleased && p_body->isOnGround()) {
        soundManager.playSound(mario::event::SoundEvent::PLAYER_JUMP); // Phát âm thanh nhảy
        hasPlayedJumpSound_ = true;
    }

    p_body->jumpByANumberOfJumps(isReleased, numJumps);
}

void mario::entity::Player::resetJump() {
    p_body->resetJump();
}

void mario::entity::Player::moveLeft(bool isReleased) {
    if(!_canMove)
        return;

    p_body->moveLeft(isReleased);
}

void mario::entity::Player::moveRight(bool isReleased) {
    if(!_canMove)
        return;

    p_body->moveRight(isReleased);
}

void mario::entity::Player::resetMove() {
    p_body->resetMove();
}

void mario::entity::Player::shotFireball(bool isReleased) {
    if(!_canMove || isReleased || p_stateManager->getCurrentState() != player_state::PlayerStateType::Fire || shootingDelayTimer > sf::seconds(0.f) || p_fireballList->getNumFireballs() >= 5)
        return;

    timeSinceLastShoot = sf::seconds(0.f);
    shootingDelayTimer = sf::seconds(DEFAULT_SHOOTING_DELAY);
    _isShootingFireball = true;
    bool isFaceForward = p_body->isFaceForward();
    sf::Vector2f fireball_spawnPoint = p_body->getPosition();
    sf::Vector2f playerSize = p_body->getSize();
    sf::Vector2f fireballSize = Fireball::getFireballSize();

    int sign = (!isFaceForward) ? -1 : 1;
    fireball_spawnPoint.y -= (playerSize.y - fireballSize.y) / 2.f;
    fireball_spawnPoint.x += sign * (playerSize.x + fireballSize.x) / 2.f;

    p_fireballList->addAFireball(new mario::entity::Fireball(fireball_spawnPoint, isFaceForward));
}

/* =================================================================================================================================================================== */

void mario::entity::Player::explosionFireballAtPos(int idx) {
    p_fireballList->getFireballAtPos(idx)->exploding();
    addScoreToPlayer(500, false);
}

mario::entity::Fireball* mario::entity::Player::getFireballAtPos(int idx) const {
    return p_fireballList->getFireballAtPos(idx);
}

int mario::entity::Player::getNumberFireballs() const {
    return p_fireballList->getNumFireballs();
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
    if(playerBehavior != PlayerBehavior::Normal && playerBehavior != PlayerBehavior::Invincible)
        return;

    if(!p_body->isOnGround()) { // change texture to jumping
            p_stateManager->setAnimation(p_animation, getPrefixBehavior(), "jump[0]");
            p_animation->setAnimationState(false);
        } else
            if(p_body->isNotMoving()) { // change texture to idle
                bool isInShootingAnimation = (timeSinceLastShoot <= sf::seconds(0.2f));
                p_stateManager->setAnimation(p_animation, getPrefixBehavior(), (isInShootingAnimation ? "shoot[0]" : "idle[0]"));
                p_animation->setAnimationState(false);
            } else 
                if(p_animation->getAnimationState() == false) { // change to run animation
                    p_stateManager->setAnimation(p_animation, getPrefixBehavior(), "idle[0]");
                    p_animation->setAnimationState(true);
                }

    hasPlayedJumpSound_ = false;
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
        moveLeft(true);
        moveRight(true);
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
        soundManager.playSound(mario::event::SoundEvent::POWER_DOWN);

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
        p_body->setAcceleration(sf::Vector2f(p_body->getAcceleration().x, 980.f));
        p_animation->resetTimeBetweenStepToDefault();
    }

    if(playerBehavior == PlayerBehavior::FinishLevel) {
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

        soundManager.playSound(mario::event::SoundEvent::POWER_UP);
        _isTransforming = true;
        behaviorTimer = sf::seconds(1.f);
        p_stateManager->setTransformToSuperAnimation(p_animation);
    }

    if(newBehavior == PlayerBehavior::TransformBTS) {
        _canCollisionWithEnemy = _canCollisionWithItem = false;
        togglePlayerMove(false);

        _isTransforming = true;
        behaviorTimer = sf::seconds(1.5f);
        p_stateManager->setBeingHitAnimation(p_animation);
    }

    if(newBehavior == PlayerBehavior::Invincible) {
        soundManager.playSound(mario::event::SoundEvent::POWERUP_APPEARS);

        behaviorTimer = sf::seconds(10.f); // 10 seconds of invincibility
    }

    if(newBehavior == PlayerBehavior::Climbing) {
        _canCollisionWithEnemy = _canCollisionWithItem = false;
        togglePlayerMove(false);

        p_body->resetJump();
        p_body->setVelocity(sf::Vector2f(0.f, 200.f));
        p_body->setAcceleration(sf::Vector2f(p_body->getAcceleration().x, 0.f));
        p_stateManager->setClimbingAnimation(p_animation);
        p_animation->setTimeBetweenStep(0.2f);
    }

    if(newBehavior == PlayerBehavior::FinishLevel) {
        _canCollisionWithEnemy = _canCollisionWithItem = false;
        togglePlayerMove(false);
    }

    if(newBehavior == PlayerBehavior::Dying) { 
        _canCollisionWithEnemy = _canCollisionWithItem = _canCollisionWithBlock = false;
        
        soundManager.playSound(mario::event::SoundEvent::PLAYER_DIE);
        
        _isAlive = false;
        togglePlayerMove(false);
        behaviorTimer = sf::seconds(3);
        
        p_body->resetJump();
        p_body->setVelocity(sf::Vector2f(0.f, 0.f));
        p_stateManager->setDeadAnimation(p_animation);
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

    if(!_isAlive && playerBehavior == PlayerBehavior::Dying) {
        // Continue falling animation after death
        p_body->update(dt);
        p_animation->update(window, dt);
        
        // Let the death animation play out before marking as dead
        if(behaviorTimer > sf::Time::Zero) {
            behaviorTimer -= sf::seconds(dt);
            if(behaviorTimer <= sf::Time::Zero) {
                changePlayerBehavior(PlayerBehavior::AlreadyDead);
            }
        }
        return;
    }

    popUpScoreList->update(window, dt);

    updatePlayerBehavior(dt);
    p_animation->update(window, dt);
    // p_body->updateSize(p_animation);
    if(playerBehavior != PlayerBehavior::TransformBTS && playerBehavior != PlayerBehavior::TransformSTB) {
        p_body->update(dt);
        p_fireballList->update(window, dt);
        shootingDelayTimer = std::max(sf::seconds(0), shootingDelayTimer - sf::seconds(dt));
        timeSinceLastShoot += sf::seconds(dt);
    }
    
    if(p_animation->isFaceForward() != p_body->isFaceForward())
        rotateDirection();

    _isShootingFireball = false;
}

void mario::entity::Player::updateToLevelState(mario::resource::LevelState &levelState) {
    levelState.coins += addedCoinCount;
    levelState.num_lives += addedLives + levelState.coins / 100;
    levelState.score += addedScore;
    levelState.coins %= 100;

    addedCoinCount = addedScore = addedLives = 0;
}

void mario::entity::Player::handleEvent(const sf::RenderWindow *window, const sf::Event &event) {
    p_fireballList->handleEvent(window, event);
}

void mario::entity::Player::render(sf::RenderWindow *window) {
    Entity::render(window);
    popUpScoreList->render(window);
    p_fireballList->render(window);
}

/* =================================================================================================================================================================== */

void mario::entity::Player::startClimbingBehavior(int flagXPos) {
    if(p_body->isFaceForward() != p_animation->isFaceForward())
        rotateDirection();

    if(getPosition().x > flagXPos)
        setPosition(sf::Vector2f(flagXPos, getPosition().y));

    changePlayerBehavior(PlayerBehavior::Climbing);
}

void mario::entity::Player::finishClimbingBehavior() {
    rotateDirection();
    setPosition(sf::Vector2f(getPosition().x + 40, getPosition().y));
    changePlayerBehavior(PlayerBehavior::FinishLevel);
}

/* =================================================================================================================================================================== */

void mario::entity::Player::setOnGround(bool isOnGround) {
    p_body->setOnGround(isOnGround);
    if(isOnGround)
        scoreMultiplier = 0;
}

void mario::entity::Player::beingHit() {
    if(playerBehavior != PlayerBehavior::Normal || playerBehavior == PlayerBehavior::Dying)
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

void mario::entity::Player::addScoreToPlayer(int _score, bool isPoppingUp) {
    score += _score;
    addedScore += _score;
    if(isPoppingUp)
        popUpScoreList->addAPopUpText(p_body->getPosition(), std::to_string(_score));
}

void mario::entity::Player::breakBrick() {
    soundManager.playSound(mario::event::SoundEvent::BLOCK_BREAK);
    addScoreToPlayer(50, false);
}

void mario::entity::Player::hitEmptyBlock() {
    soundManager.playSound(mario::event::SoundEvent::BLOCK_BUMP);
}

void mario::entity::Player::collectCoin() {
    ++coinCount, ++addedCoinCount;
    addScoreToPlayer(200, true);
    
    soundManager.playSound(mario::event::SoundEvent::COIN_COLLECT);

    if (coinCount >= 100) {
        lives += coinCount / 100;
        coinCount %= 100;
        soundManager.playSound(mario::event::SoundEvent::ONE_UP);
    }
}

void mario::entity::Player::collectCoinInBlock() {
    collectCoin();
}

void mario::entity::Player::collectRedMushroom() {
    addScoreToPlayer(1000, true);
    if (getPlayerStateType() == player_state::PlayerStateType::Small) {
        changeState(player_state::PlayerStateType::Super);
        soundManager.playSound(mario::event::SoundEvent::POWER_UP);
    } else {
        // Already super or fire, give points instead
    }
}

void mario::entity::Player::collectFireFlower() {
    addScoreToPlayer(1000, true);
    if (getPlayerStateType() == player_state::PlayerStateType::Small) {
        changeState(player_state::PlayerStateType::Super);
        soundManager.playSound(mario::event::SoundEvent::POWER_UP);
    } else if(getPlayerStateType() == player_state::PlayerStateType::Super) {
        changeState(player_state::PlayerStateType::Fire);
        soundManager.playSound(mario::event::SoundEvent::POWER_UP);
    }
}

void mario::entity::Player::collect1UpMushroom() {
    lives++, ++addedLives;
    addScoreToPlayer(1000, true);
    soundManager.playSound(mario::event::SoundEvent::ONE_UP);
}

void mario::entity::Player::collectStarman() {
    changePlayerBehavior(PlayerBehavior::Invincible);
}

void mario::entity::Player::jumpOnEnemyHead() {
    int tempMult = scoreMultiplier;

    resetJump();
    setOnGround(true);
    hasPlayedJumpSound_ = false;
    jumpByANumberOfJumps(false, 1);

    soundManager.playSound(mario::event::SoundEvent::ENEMY_STOMP);
    
    scoreMultiplier = tempMult;
    addScoreToPlayer(++scoreMultiplier * 100, true);
}

void mario::entity::Player::hitEnemyWithFireball(bool canEnemyDead) {
    if(canEnemyDead)
        addScoreToPlayer(100, true);
}

/* =================================================================================================================================================================== */

std::string mario::entity::Player::getPrefixBehavior() const {
    return (playerBehavior == PlayerBehavior::Invincible) ? "invincible-" : "";
}

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

void mario::entity::Player::handleNetworkCollision(const sf::Vector2f& otherPosition) {
    if (_isRemotePlayer) return;
    
    sf::Vector2f direction = p_body->getPosition() - otherPosition;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    
    if (distance < 50.f) { // Collision threshold
        direction /= distance; // Normalize
        p_body->setVelocity(p_body->getVelocity() + direction * 5.f); // Push effect
    }
}

void mario::entity::Player::syncNetworkState(const sf::Vector2f& position, const sf::Vector2f& velocity) {
    if (!_isRemotePlayer) 
        return;
    
    p_body->setPosition(position);
    p_body->setVelocity(velocity);
}

#undef FILE_PATH