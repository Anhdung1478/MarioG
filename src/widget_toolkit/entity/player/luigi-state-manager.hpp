#pragma once

#include<bits/stdc++.h>
#include<SFML/Graphics.hpp>
#include "luigi-state.hpp"

namespace mario::entity::player_state {
    class LuigiStateManager : public PlayerStateManager {
        private:
            const std::string LUIGI_TYPE_STR[3] = {
                "luigi-small",
                "luigi-super",
                "luigi-fire",
            };

        public:
            LuigiStateManager(Animation *p_animation, Box *p_box, PlayerStateType stateType) {
                player_small = new LuigiSmallState(LUIGI_TYPE_STR[0]);
                player_super = new LuigiSuperState(LUIGI_TYPE_STR[1]);
                player_fire = new LuigiFireState(LUIGI_TYPE_STR[2]);
                
                currStateType = stateType;
                if(stateType == PlayerStateType::Small)
                    curr_state = player_small;

                if(stateType == PlayerStateType::Super)
                    curr_state = player_super;

                if(stateType == PlayerStateType::Fire)
                    curr_state = player_fire;
                
                loadCurrentStateAnimation(p_animation, p_box);
            }

            ~LuigiStateManager() override {
            }

            void setAnimation(Animation *p_animation, const std::string &prefixSpriteID, const std::string &ID) override {
                p_animation->setSpriteAnimation(prefixSpriteID + LUIGI_TYPE_STR[(int) currStateType] + "." + ID);
            }
            
            void setDeadAnimation(Animation *p_animation) override {
                p_animation->setSpriteAnimation(LUIGI_TYPE_STR[0] + ".dying[0]");
            }

            std::string getCurrentPlayerStateID() const override {
                return LUIGI_TYPE_STR[(int) currStateType];
            }

            std::string getNextPlayerStateID() const override {
                // currStateType != PlayerStateType::Fire
                return LUIGI_TYPE_STR[int(currStateType) + 1];
            }
    };
}