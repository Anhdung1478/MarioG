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
                "mario-fire",
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

                loadCurrentStateAnimation(p_animation, p_box);
            }

            ~MarioStateManager() override {
            }

            void setAnimation(Animation *p_animation, const std::string &prefixSpriteID, const std::string &ID) override {
                p_animation->setSpriteAnimation(prefixSpriteID + MARIO_TYPE_STR[(int) currStateType] + "." + ID);
            }
            
            void setDeadAnimation(Animation *p_animation) override {
                p_animation->setSpriteAnimation(MARIO_TYPE_STR[0] + ".dying[0]");
                p_animation->setAnimationState(false);
            }

            void setClimbingAnimation(Animation *p_animation) override {
                p_animation->clearAnimationStep();
                p_animation->addAnimationStep(MARIO_TYPE_STR[int(currStateType)] + ".climb[0]");
                p_animation->addAnimationStep(MARIO_TYPE_STR[int(currStateType)] + ".climb[1]");
                p_animation->setAnimationState(true);
            }

            void setBeingHitAnimation(Animation *p_animation) override {
                p_animation->clearAnimationStep();
                for (int i = 0; i < 15; ++i)
                    p_animation->addAnimationStep(MARIO_TYPE_STR[int(currStateType)] + ".hit[" + std::to_string(i) + "]");

                p_animation->setAnimationState(true);
            }

            std::string getCurrentPlayerStateID() const override {
                return MARIO_TYPE_STR[int(currStateType)];
            }

            std::string getNextPlayerStateID() const override {
                // currStateType != PlayerStateType::Fire
                return MARIO_TYPE_STR[int(currStateType) + 1];
            }
    };
}