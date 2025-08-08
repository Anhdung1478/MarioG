#pragma once

#include<bits/stdc++.h>
#include "../entity.hpp"
#include "player-list.hpp"
#include "../box/dynamic-box.hpp"
#include "mario-state-manager.hpp"
#include "luigi-state-manager.hpp"
#include "../../resource/LevelState.hpp"
#include "../item/item.hpp"
#include "fireball.hpp"

namespace mario::entity {
    static constexpr sf::Vector2f PLAYER_SCALE = sf::Vector2f(2.5f, 2.5f);
    static constexpr float ANIMATION_TIME_BETWEEN_STEP_WHEN_BEING_HIT = 1.f / 10.f;
    static constexpr float ANIMATION_TIME_BETWEEN_STEP_WHEN_TRANSFORM_TO_BIG = 1.f / 10.f;
    static constexpr float ANIMATION_TIME_BETWEEN_STEP_IN_NORMAL_BEHAVIOR = 1.f / 10.f;

    class Player : public Entity {
        private:
            mario::entity::player_state::PlayerStateManager *p_stateManager;
            CharacterListType _characterType;
            PlayerBehavior playerBehavior;

            std::vector<Fireball*> fireballs;

            sf::Time shootingDelayTimer;
            sf::Time behaviorTimer;
            bool _isOnGround;

            bool hasPlayedJumpSound_ = false; // For sound effect
            bool _canMove = true, _isAlive = true, _isDeadAlready = false;
            bool _canCollisionWithEnemy = true, _canCollisionWithItem = true, _canCollisionWithBlock = true;
            bool _isTransforming = false;

            int score = 0;
            int lives = 0;
            int coinCount = 0;


            void managePlayerAnimation(); // manage Animation for Player (idle, run or jump animation)
            void updatePlayerBehavior(float dt); // update for Player Behavior (some behavior will change when ran out of time)

        public:
            Player(sf::Vector2f spawnPoint, CharacterListType characterType, player_state::PlayerStateType stateType);
            ~Player() override;
            
            void rotateDirection();
            void jump(bool isReleased);
            sf::Vector2f getVelocity() const;
            void setVelocity(sf::Vector2f vel);
            void shotFireball(bool isReleased);
            void move(bool isMoveRight, bool isReleased);
        
            void update(const sf::RenderWindow *window, float dt) override;
            void handleEvent(const sf::RenderWindow *window, const sf::Event &event) override;
            void render(sf::RenderWindow *window) override;
            
            void updateToLevelState(mario::resource::LevelState &levelState);
            void setOnGround(bool isOnGround);
            void togglePlayerMove(bool canMove); // toggle turn on/off movement of Player (when turn off, Player won't be able to move)
            void resetJump();

            void beingHit(); // being hit by enemy or entity like level trap
            void changePlayerBehavior(PlayerBehavior newBehavior); // change Player behavior to newBehavior
            void changeState(player_state::PlayerStateType newStateType); // change Player State into newStateType

            bool isDead() const;
            bool isTransforming() const;
            bool isInBehavior(PlayerBehavior behavior) const;
            bool canCollisionWithEnemy() const;
            bool canCollisionWithItem() const;
            bool canCollisionWithBlock() const;

            void breakBrick();
            void hitEmptyBlock();
            void collectCoin();   
            void collectCoinInBlock();         
            void collectRedMushroom();            
            void collectFireFlower();
            void collect1UpMushroom();
            void collectStarman();

            PlayerBehavior getPlayerBehavior() const;
            CharacterListType getCharacterType() const;
            player_state::PlayerStateType getPlayerStateType() const;
            
            void setJumpSoundPlayed(bool played);
            bool hasPlayedJumpSound() const;
    };
}