#pragma once

namespace mario::event{
    enum class SoundEvent {
        ONE_UP,                    // When gaining an extra life (1_up.wav)
        BLOCK_BREAK,               // When breaking a brick block (break_brick_block.wav)
        BLOCK_BUMP,                // When bumping into a block or wall (bump.wav)
        COIN_COLLECT,              // When collecting a coin (coin.wav)
        LEVEL_ENTER,               // When entering a level or pipe (enter_level.wav)
        FIREBALL_THROW,            // When throwing a fireball (fireball.wav)
        GAME_OVER,                 // When the game is over (game_over.wav)
        PLAYER_JUMP,               // When the player jumps (jump.wav)
        SHELL_KICK,                // When kicking a shell (kick.wav)
        LEVEL_CLEAR,               // When clearing a level (level_clear.wav)
        PLAYER_DIE,                // When losing a life (lost_life.wav)
        POWER_DOWN,                // When losing a suit (powering down) (lost_suit.wav)
        POWERUP_APPEARS,           // When a power-up item appears (mushroom_appears.wav)
        GAME_PAUSE,                // When the game is paused (pause.wav)
        PMETER_FULL,               // When the P-Meter is full (pmeter.wav)
        POWER_UP,                  // When collecting a power-up item (power_up.wav)
        CANNON_SHOT,               // For a cannon shot (shot.wav)
        PLAYER_SKID,               // When the player skids to a stop (skid.wav)
        ENEMY_STOMP,               // When stomping on an enemy (stomp.wav)
        TIME_WARNING               // When the timer is running low (time_up.wav)
    };

    enum class BackgroundMusicState {
        MAIN_MENU,
        LEVEL_1,
        LEVEL_2,
        LEVEL_3,
        GAME_OVER_SCREEN,
        SETTING_SCREEN,
        LEVEL_SCREEN
    };
}