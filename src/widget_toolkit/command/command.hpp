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
            void execute(mario::entity::Player &player, bool isReleased) override {
                player.jump(isReleased);
            }
    };

    class RunCommand : public Command {
        private:
            bool _isForward;

        public: 
            RunCommand(bool isForward) : _isForward(isForward) {}

            void execute(mario::entity::Player &player, bool isReleased) override {
                player.run(_isForward, isReleased);
            }
    };
    
    class FireCommand : public Command {
        public: 
            void execute(mario::entity::Player &player, bool isReleased) override {
                player.shotFireball(isReleased);
            }
    };
}