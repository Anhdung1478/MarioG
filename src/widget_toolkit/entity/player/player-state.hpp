#pragma once

#include<bits/stdc++.h>
#include<SFML/Graphics.hpp>
#include "../animation/animation.hpp"
#include "../box/box.hpp"

namespace mario::entity::player_state {
    enum class PlayerStateType {
        Small = 0,  // next state of Small is Super
        Super = 1,  // next state of Super is Fire
        Fire = 2    // next state of Fire doesn't exist
    };



    class PlayerState {
        public:
            virtual ~PlayerState() = default;
            virtual void update(Animation *p_eAnimation, Box *p_eBox) = 0;
    };



    class PlayerStateManager {
        protected:
            PlayerState *player_small;
            PlayerState *player_super;
            PlayerState *player_fire;
            PlayerState *curr_state;

            PlayerStateType currStateType;

        public:
            PlayerStateManager() {
                currStateType = PlayerStateType::Small;
                player_small = player_super = player_fire = curr_state = nullptr;
            }

            virtual ~PlayerStateManager() {
                delete player_small;
                delete player_super;
                delete player_fire;
            }

            void changeState(PlayerStateType newState, Animation *p_animation, Box *p_box) {
                if(currStateType == newState)
                    return;

                if(newState == PlayerStateType::Small)
                    changeToSmallState(p_animation, p_box);

                if(newState == PlayerStateType::Super)
                    changeToSuperState(p_animation, p_box);

                if(newState == PlayerStateType::Fire)
                    changeToFireState(p_animation, p_box);
            }

            void changeToSmallState(Animation *p_animation, Box *p_box) {
                curr_state = player_small;
                currStateType = PlayerStateType::Small;
                curr_state->update(p_animation, p_box);
            }

            void changeToSuperState(Animation *p_animation, Box *p_box) {
                curr_state = player_super;
                currStateType = PlayerStateType::Super;
                curr_state->update(p_animation, p_box);
            }

            void changeToFireState(Animation *p_animation, Box *p_box) {
                curr_state = player_fire;
                currStateType = PlayerStateType::Fire;
                curr_state->update(p_animation, p_box);
            }

            void loadCurrentStateAnimation(Animation *p_animation, Box *p_box) {
                curr_state->update(p_animation, p_box);
            }

            PlayerStateType getCurrentState() const {
                return currStateType;
            }

            virtual void setAnimation(Animation *p_animation, const std::string &ID) = 0;
            virtual void setDeadAnimation(Animation *p_animation) = 0;
            virtual std::string getCurrentPlayerStateID() const = 0;
            virtual std::string getNextPlayerStateID() const = 0;
    };
}