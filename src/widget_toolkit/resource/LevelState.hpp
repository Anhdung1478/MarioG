#pragma once

#include <bits/stdc++.h>
#include "../entity/player.hpp"

namespace mario::resource {
    struct LevelState {
        int level, score, coins, num_lives;
        mario::entity::CharacterListType characterType;
        mario::entity::player_state::PlayerStateType stateType;

        LevelState() : level(0), num_lives(0), score(0), coins(0), characterType(mario::entity::CharacterListType::Mario), stateType(mario::entity::player_state::PlayerStateType::Small) {};
    
        LevelState(int _level, int _lives, int _score, int _coins, int _charType, int _stateType)
            : level(_level), score(_score), coins(_coins), num_lives(_lives), characterType(mario::entity::CharacterListType(_charType)), stateType(mario::entity::player_state::PlayerStateType(_stateType)) {};

        LevelState(int _level, int _lives, int _score, int _coins, mario::entity::CharacterListType _charType = mario::entity::CharacterListType::Luigi, mario::entity::player_state::PlayerStateType _stateType = mario::entity::player_state::PlayerStateType::Small) 
            : level(_level), score(_score), coins(_coins), num_lives(_lives), characterType(_charType), stateType(_stateType) {};
    };
}