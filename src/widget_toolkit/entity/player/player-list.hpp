#pragma once

#include<bits/stdc++.h>

namespace mario::entity {
    enum class CharacterListType {
        Mario = 0,
        Luigi = 1
    };

    enum class PlayerBehavior {
        Normal = 0,      // normal
        Shadow = 1,      // appear when Player being hit by Enemy or level trap and become Small State from Super/Fire
        Transform = 2,   // appear when Player transform from Small to Super/Fire or vice versa
        Invincible = 3,  // appear when Player loot the Star Item, Player become Invincible, kill all Enemy touch him and be unable to die except the case Player jump out of map
        Climbing = 4,    // appear when Player finish the level and are climbing on the flag
        Dead = 5,        // appear when Player in Small State being hit and dead
    };
}