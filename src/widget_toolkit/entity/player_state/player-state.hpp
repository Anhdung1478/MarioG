#pragma once

#include<bits/stdc++.h>
#include<SFML/Graphics.hpp>
#include "../animation/animation.hpp"
#include "../box/box.hpp"

namespace mario::entity::player_state {
    enum class PlayerStateType {
        Small = 0,
        Super = 1,
        Fire = 2
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

            void changeState(PlayerStateType stateType, Animation *p_animation, Box *p_box) {
                if(stateType == PlayerStateType::Small)
                    changeToSmallState(p_animation, p_box);

                if(stateType == PlayerStateType::Super)
                    changeToSuperState(p_animation, p_box);

                if(stateType == PlayerStateType::Fire)
                    changeToFireState(p_animation, p_box);
            }

            void changeToSmallState(Animation *p_animation, Box *p_box) {
                if(currStateType == PlayerStateType::Small)
                    return;

                curr_state = player_small;
                currStateType = PlayerStateType::Small;
                curr_state->update(p_animation, p_box);
            }

            void changeToSuperState(Animation *p_animation, Box *p_box) {
                if(currStateType == PlayerStateType::Super)
                    return;

                curr_state = player_super;
                currStateType = PlayerStateType::Super;
                curr_state->update(p_animation, p_box);
            }

            void changeToFireState(Animation *p_animation, Box *p_box) {
                if(currStateType == PlayerStateType::Fire)
                    return;

                curr_state = player_fire;
                currStateType = PlayerStateType::Fire;
                curr_state->update(p_animation, p_box);
            }

            PlayerStateType getCurrentState() const {
                return currStateType;
            }

            virtual void setAnimation(Animation *p_animation, const std::string &ID) = 0;
    };
}