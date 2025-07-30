#pragma once

#include<bits/stdc++.h>
#include<SFML/Graphics.hpp>
#include "../entity/player.hpp"

namespace mario::input { // Command pattern
    class Command {
        public:
            virtual ~Command() = default;
            virtual void execute(mario::entity::Player &player, bool isReleased) = 0; // isReleased = 1 -> key is released, isReleased = 0 -> key is pressed
    };

    class JumpCommand : public Command {
        public: 
            JumpCommand(MainWindow& context) : context_(context) {}
            void execute(mario::entity::Player &player, bool isReleased) override {
                player.jump(isReleased);

                if(!player.hasPlayedJumpSound()) {
                    context_.getSoundManager().playSound(mario::event::SoundEvent::PLAYER_JUMP);
                    player.setJumpSoundPlayed(true);
                }
            }
        private:
            MainWindow& context_;
    };

    class MoveCommand : public Command {
        private:
            bool _isForward;

        public: 
            MoveCommand(bool isForward) : _isForward(isForward) {}

            void execute(mario::entity::Player &player, bool isReleased) override {
                player.move(_isForward, isReleased);
            }
    };
    
    class FireCommand : public Command {
        public: 
            void execute(mario::entity::Player &player, bool isReleased) override {
                player.shotFireball(isReleased);
            }
    };
}