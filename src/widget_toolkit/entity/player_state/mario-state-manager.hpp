#pragma once

#include<bits/stdc++.h>
#include<SFML/Graphics.hpp>
#include "mario-state.hpp"

namespace mario::entity::player_state {
    class MarioStateManager : public PlayerStateManager {
        private:
            const std::string MARIO_TYPE_STR[3] = {
                "mario-small",
                "mario-super",
                "mario-fire-super",
            };

        public:
            MarioStateManager(Animation *p_animation, Box *p_box, PlayerStateType stateType) {
                player_small = new MarioSmallState(MARIO_TYPE_STR[0]);
                player_super = new MarioSuperState(MARIO_TYPE_STR[1]);
                player_fire = new MarioFireState(MARIO_TYPE_STR[2]);

                currStateType = stateType;
                if(stateType == PlayerStateType::Small)
                    curr_state = player_small;

                if(stateType == PlayerStateType::Super)
                    curr_state = player_super;

                if(stateType == PlayerStateType::Fire)
                    curr_state = player_fire;

                curr_state->update(p_animation, p_box);
            }

            ~MarioStateManager() override {
            }

            void setAnimation(Animation *p_animation, const std::string &ID) override {
                p_animation->setSpriteAnimation(MARIO_TYPE_STR[(int) currStateType] + "." + ID);
            }
    };
}