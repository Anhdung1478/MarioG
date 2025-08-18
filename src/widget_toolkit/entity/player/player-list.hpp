#pragma once

#include<bits/stdc++.h>

namespace mario::entity {
    enum class CharacterListType {
        Mario = 0,
        Luigi = 1
    };

    enum class PlayerBehavior {
        Normal = 0,        // normal
        Shadow = 1,        // appear when Player being hit by Enemy or level trap and become Small State from Super/Fire
        TransformSTB = 2,  // appear when Player transform from Small to Super
        TransformBTS = 3,  // appear when Player transform from Super/Fire to Small
        Invincible = 4,    // appear when Player loot the Star Item, Player become Invincible, kill all Enemy touch him and be unable to die except the case Player jump out of map
        Climbing = 5,      // appear when Player finish the level and are climbing on the flag
        GoToFortress = 6,  // appear when Player finish climbing on the flag and go to the fortress
        EnterFortress = 7, // appear when Player go to the fortress door and enter
        FinishLevel = 8,   // appear when Player enter the fortress door and disappeared to finish level and start new level
        Dying = 9,         // appear when Player in Small State being hit and dead, this Behavior should run the Dying Animation of Player
        AlreadyDead = 10,  // appear when Player finish Dying Animation, and dead already
    };
}