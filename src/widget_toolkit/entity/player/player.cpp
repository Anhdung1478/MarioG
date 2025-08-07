#include<bits/stdc++.h>
#include "player.hpp"

#define FILE_PATH "../../asset/sprites/"

mario::entity::Player::Player(sf::Vector2f spawnPoint, CharacterListType characterType, player_state::PlayerStateType stateType) : _characterType(characterType), _isAlive(true) {
    _isFinishedDeadAnimation = false;
    p_body = new DynamicBox(spawnPoint, sf::Vector2f(40.f, 40.f));
    if(characterType == CharacterListType::Mario) {
        p_animation = new Animation(FILE_PATH"mario.json", FILE_PATH"mario_sheets.png", PLAYER_SCALE, "mario-small.idle[0]");
        p_animation->loadSheet(FILE_PATH"mario-fire.json", FILE_PATH"mario-fire.png");

        p_stateManager = new mario::entity::player_state::MarioStateManager(p_animation, p_body, stateType);
    }

    if(characterType == CharacterListType::Luigi) {
        p_animation = new Animation(FILE_PATH"luigi.json", FILE_PATH"luigi_sheets.png", PLAYER_SCALE, "luigi-small.idle[0]");
        p_animation->loadSheet(FILE_PATH"luigi-fire.json", FILE_PATH"luigi-fire.png");

        p_stateManager = new mario::entity::player_state::LuigiStateManager(p_animation, p_body, stateType);
    }
}

mario::entity::Player::~Player() {
    delete p_stateManager;
}

void mario::entity::Player::jump(bool isReleased) {
    if(!_isAlive)
        return;

    p_body->jump(isReleased);
}

void mario::entity::Player::move(bool isMoveRight, bool isReleased) {
    if(!_isAlive)
        return;

    p_body->move(isMoveRight, isReleased);
}

void mario::entity::Player::shotFireball(bool isReleased) {
    if(!_isAlive)
        return;
}

sf::Vector2f mario::entity::Player::getVelocity() const {
    return p_body->getVelocity();
}

void mario::entity::Player::setVelocity(sf::Vector2f vel) {
    p_body->setVelocity(vel);
}

void mario::entity::Player::rotateDirection() {
    p_animation->rotate();
}

void mario::entity::Player::setStartedDead() {
    jump(true);
    move(0, true);
    move(1, true);
    shotFireball(true);
    _isAlive = false;
    
    p_animation->clearAnimationStep();
    
    // running dead sound
    p_body->setVelocity(sf::Vector2f(0.f, 0.f));
    p_body->jump(false);

    p_stateManager->setDeadAnimation(p_animation);
    p_animation->setAnimationState(false);
}

void mario::entity::Player::managePlayerAnimation() {
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

void mario::entity::Player::managePlayerDeadState(float dt) {
    if(!_isAlive && !_isFinishedDeadAnimation) {
        deadAnimationTime -= sf::seconds(dt);
        _isFinishedDeadAnimation = (deadAnimationTime <= sf::seconds(0));
        if(_isFinishedDeadAnimation)
            p_body->jump(true);
    }
}

void mario::entity::Player::update(const sf::RenderWindow *window, float dt) {
    if(p_body->getPosition().y >= 720.f)
        setStartedDead();

    if(_isAlive) {
        managePlayerAnimation();

        // change state for debugging
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num1)) {
            std::cerr << "CALL TO FUNCTION CHANGE TO SMALL STATE\n";
            p_stateManager->changeToSmallState(p_animation, p_body);
            std::cerr << "SUCCESFULLY\n";
        }
    
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num2)) {
            std::cerr << "CALL TO FUNCTION CHANGE TO SUPER STATE\n";
            p_stateManager->changeToSuperState(p_animation, p_body);
            std::cerr << "SUCCESFULLY\n";
        }
    
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num3)) {
            std::cerr << "CALL TO FUNCTION CHANGE TO FIRE STATE\n";
            p_stateManager->changeToFireState(p_animation, p_body);
            std::cerr << "SUCCESFULLY\n";
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num4)) {
            std::cerr << "CALL TO FUNCTION KILLED PLAYER\n";
            setStartedDead();
            std::cerr << "SUCCESFULLY\n";
        }
    }

    managePlayerDeadState(dt); 

    p_animation->update(window, dt);
    p_body->updateSize(p_animation);
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
}

void mario::entity::Player::setOnGround(bool isOnGround) {
    p_body->setOnGround(isOnGround);
}

void mario::entity::Player::resetJump() {
    p_body->resetJump();
}

void mario::entity::Player::beingHit() {
    if(getPlayerStateType() == player_state::PlayerStateType::Small) {
        _isAlive = false;
    } else {
        p_stateManager->changeToSmallState(p_animation, p_body);
    }
}

bool mario::entity::Player::isInDeadAnimation() const {
    return (!_isAlive);
}

bool mario::entity::Player::isDead() const {
    return _isFinishedDeadAnimation;
}

void mario::entity::Player::collectCoin() {
    ++coinCount;
    score += 200;
    
    // 1-up at 100 coins
    if (coinCount >= 100) {
        coinCount = 0;
        lives++;
        // Play 1-up sound
    }
    
    // Play coin sound
}

void mario::entity::Player::collectCoinInBlock() {
    collectCoin();
}

void mario::entity::Player::collectRedMushroom() {
    score += 1000;
    if (getPlayerStateType() == player_state::PlayerStateType::Small) {
        p_stateManager->changeToSuperState(p_animation, p_body);
        // Play power-up sound
    } else {
        // Already super or fire, give points instead
    }
}

void mario::entity::Player::collectFireFlower() {
    score += 1000;
    if (getPlayerStateType() == player_state::PlayerStateType::Small) {
        p_stateManager->changeToSuperState(p_animation, p_body);
            // Play power-up sound
    } else if(getPlayerStateType() == player_state::PlayerStateType::Super) {
        p_stateManager->changeToFireState(p_animation, p_body);
        // Play power-up sound
    }
}

void mario::entity::Player::collect1UpMushroom() {
    lives++;
    score += 1000;
    // Play 1-up sound
}

void mario::entity::Player::collectStarman() {
    isInvincible = true;
    invincibleTimer = 10.f; // 10 seconds of invincibility
    // p_stateManager->changeToStarmanState(p_animation, p_body);
}

mario::entity::CharacterListType mario::entity::Player::getCharacterType() {
    return _characterType;
}

mario::entity::player_state::PlayerStateType mario::entity::Player::getPlayerStateType() {
    return p_stateManager->getCurrentState();
}

void mario::entity::Player::setJumpSoundPlayed(bool played) {
    hasPlayedJumpSound_ = played;
}

bool mario::entity::Player::hasPlayedJumpSound() const { 
    return hasPlayedJumpSound_; 
}

#undef FILE_PATH