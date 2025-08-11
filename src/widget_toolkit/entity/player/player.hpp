#pragma once

#include<bits/stdc++.h>
#include "../entity.hpp"
#include "player-list.hpp"
#include "../box/dynamic-box.hpp"
#include "mario-state-manager.hpp"
#include "luigi-state-manager.hpp"
#include "../../resource/LevelState.hpp"
#include "../item/item.hpp"

namespace mario::entity {
    static constexpr sf::Vector2f PLAYER_SCALE = sf::Vector2f(2.5f, 2.5f);

    class Player : public Entity {
        private:
            mario::entity::player_state::PlayerStateManager *p_stateManager;
            CharacterListType _characterType;

            sf::Time deadAnimationTimer = sf::seconds(3);
            sf::Time shootingDelayTimer;
            sf::Time invincibleTimer, shadowTimer;
            bool _isOnGround;

            bool hasPlayedJumpSound_ = false; // For sound effect
            bool _isAlive = true, _isFinishedDeadAnimation = false;
            bool _isShadow = false, _isInvincible = false;

            int score = 0;
            int lives = 0;
            int coinCount = 0;

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
            void updateToLevelState(mario::resource::LevelState &levelState);
            void handleEvent(const sf::RenderWindow *window, const sf::Event &event) override;
            void render(sf::RenderWindow *window) override;

            void setOnGround(bool isOnGround);
            void resetJump();

            void setStartedDead(); // set Player status when started to dead
            void managePlayerAnimation(); // manage Player's animation when they did smth
            void managePlayerShadowState(float dt); // manage Player shadow state (state after being hit and last 1.5s. In this state, Player can go through Enemy but not Block)
            void managePlayerDeadState(float dt); // manage Player when being dead
            
            void beingHit(); // being hit by enemy or entity like level trap
            void toggleShadowState(bool isTurnOn); // toggle on/off shadow state, turn on to be a shadow in x second after being hit by enemy or level trap. In this state, Player can go through Enemy but not Block
            
            bool isDead() const;
            bool isShadow() const;
            bool isInDeadAnimation() const;

            void collectCoin();   
            void collectCoinInBlock();         
            void collectRedMushroom();            
            void collectFireFlower();
            void collect1UpMushroom();
            void collectStarman();

            CharacterListType getCharacterType();
            player_state::PlayerStateType getPlayerStateType();
            
            void setJumpSoundPlayed(bool played);
            bool hasPlayedJumpSound() const;

            void handleNetworkCollision(const sf::Vector2f& otherPosition);
            void syncNetworkState(const sf::Vector2f& position, const sf::Vector2f& velocity);
    
            bool _isRemotePlayer = false;

            int getScore() { return score;}
            int getCoins() { return coinCount; }
            int getLives() { return lives; }
            void setRemote(bool isRemote) { _isRemotePlayer = isRemote; } 
    };
}