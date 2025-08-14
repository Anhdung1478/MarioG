#pragma once

#include<bits/stdc++.h>
#include "../entity.hpp"
#include "player-list.hpp"
#include "../box/dynamic-box.hpp"
#include "mario-state-manager.hpp"
#include "luigi-state-manager.hpp"
#include "../../resource/LevelState.hpp"
#include "../item/item.hpp"
#include "fireball-list.hpp"
#include "../../controls/popup-text-list.hpp"
#include "../../resource/SoundManager.hpp"

namespace mario::entity {
    static constexpr sf::Vector2f PLAYER_SCALE = sf::Vector2f(2.5f, 2.5f);
    static constexpr float ANIMATION_TIME_BETWEEN_STEP_WHEN_BEING_HIT = 1.f / 10.f;
    static constexpr float ANIMATION_TIME_BETWEEN_STEP_WHEN_TRANSFORM_TO_BIG = 1.f / 10.f;
    static constexpr float ANIMATION_TIME_BETWEEN_STEP_IN_NORMAL_BEHAVIOR = 1.f / 10.f;
    static constexpr float DEFAULT_SHOOTING_DELAY = 0.3f;

    class Player : public Entity {
        private:
            mario::audio::SoundManager& soundManager; // for sound effects

            player_state::PlayerStateManager *p_stateManager;
            CharacterListType _characterType;
            PlayerBehavior playerBehavior;

            std::unique_ptr<FireballList> p_fireballList;
            mario::PopUpTextList *popUpScoreList;

            sf::Time timeSinceLastShoot = sf::seconds(10);
            sf::Time shootingDelayTimer;
            sf::Time behaviorTimer;
            bool _isOnGround;

            bool hasPlayedJumpSound_ = false; // For sound effect
            bool _canMove = true, _isAlive = true, _isDeadAlready = false;
            bool _canCollisionWithEnemy = true, _canCollisionWithItem = true, _canCollisionWithBlock = true;
            bool _isTransforming = false, _isShootingFireball = false;

            int score = 0;
            int lives = 0;
            int coinCount = 0;
            int scoreMultiplier = 0;

            void managePlayerAnimation(); // manage Animation for Player (idle, run or jump animation)
            void updatePlayerBehavior(float dt); // update for Player Behavior (some behavior will change when ran out of time)

        public:
            Player(sf::Vector2f spawnPoint, CharacterListType characterType, player_state::PlayerStateType stateType, mario::audio::SoundManager& soundManager);
            ~Player() override;
            
            void addScoreToPlayer(int _score, bool isPoppingUp);  // pop up score when getting some new score

            void rotateDirection();
            sf::Vector2f getVelocity() const;
            void setVelocity(sf::Vector2f vel);
            
            void jump(bool isReleased);
            void move(bool isMoveRight, bool isReleased);
            void shotFireball(bool isReleased);

            void explosionFireballAtPos(int idx);
            int getNumberFireballs() const;
            Fireball* getFireballAtPos(int idx) const;
        
            void update(const sf::RenderWindow *window, float dt) override;
            void handleEvent(const sf::RenderWindow *window, const sf::Event &event) override;
            void render(sf::RenderWindow *window) override;
            
            void updateToLevelState(mario::resource::LevelState &levelState);
            void setOnGround(bool isOnGround);
            void togglePlayerMove(bool canMove); // toggle turn on/off movement of Player (when turn off, Player won't be able to move)
            void resetJump();

            void toggleClimbingBehavior(bool isFinished); // toggle turn on/off climbing behavior of Player (when turn on, Player will climbing from top of flag to ground)

            void beingHit(); // being hit by enemy or entity like level trap
            void changePlayerBehavior(PlayerBehavior newBehavior); // change Player behavior to newBehavior
            void changeState(player_state::PlayerStateType newStateType); // change Player State into newStateType

            bool isDead() const;
            bool isShadow() const;
            bool isTransforming() const;
            bool isInBehavior(PlayerBehavior behavior) const;
            bool canCollisionWithEnemy() const;
            bool canCollisionWithItem() const;
            bool canCollisionWithBlock() const;

            void breakBrick();                                    // appear when Player break the brick block
            void hitEmptyBlock();                                 // appear when Player hit the empty block 
            void collectCoin();                                   // appear when Player collect a coin in map
            void collectCoinInBlock();                            // appear when Player collect a coin in block when break or hit it    
            void collectRedMushroom();                            // appear when Player collect a Red Mushroom
            void collectFireFlower();                             // appear when Player collect a Fire Flower
            void collect1UpMushroom();                            // appear when Player collect a 1-Up Mushroom
            void collectStarman();                                // appear when Player collect a Starman
            void jumpOnEnemyHead();                               // appear when Player jump on an enemy head an kill it
            void hitEnemyWithFireball(bool canEnemyDead);         // appear when Player shot fireball and hit enemy

            std::string getPrefixBehavior() const;
            PlayerBehavior getPlayerBehavior() const;
            CharacterListType getCharacterType() const;
            player_state::PlayerStateType getPlayerStateType() const;
            
            void setJumpSoundPlayed(bool played);
            bool hasPlayedJumpSound() const;

            void handleNetworkCollision(const sf::Vector2f& otherPosition);
            void syncNetworkState(const sf::Vector2f& position, const sf::Vector2f& velocity);
    
            bool _isRemotePlayer = false;

            int getScore() { return score; }
            int getCoins() { return coinCount; }
            int getLives() { return lives; }
            void setRemote(bool isRemote) { _isRemotePlayer = isRemote; } 
    };
}