#include "SoundManager.hpp"

using namespace std;

namespace game::audio {
    SoundManager::SoundManager() :
        isMusicEnabled(true), isSoundEffectsEnabled(true),
        musicVolume(100.0f), soundEffectsVolume(100.0f),
        currentMusicState(game::event::BackgroundMusicState::MAIN_MENU),
        currentMusic(nullptr)
        {
            loadSounds();
    }
    
    SoundManager::~SoundManager() {
        for(auto& pair : backgroundMusics) {
            delete pair.second;
        }
        backgroundMusics.clear();
    }

    void SoundManager::loadSounds() {
        // Sound effects
        soundFiles[game::event::SoundEvent::ONE_UP] = "assets/sounds/1_up.wav";
        soundFiles[game::event::SoundEvent::BLOCK_BREAK] = "assets/sounds/break_brick_block.wav";
        soundFiles[game::event::SoundEvent::BLOCK_BUMP] = "assets/sounds/bump.wav";
        soundFiles[game::event::SoundEvent::COIN_COLLECT] = "assets/sounds/coin.wav";
        soundFiles[game::event::SoundEvent::LEVEL_ENTER] = "assets/sounds/enter_level.wav";
        soundFiles[game::event::SoundEvent::FIREBALL_THROW] = "assets/sounds/fireball.wav";
        soundFiles[game::event::SoundEvent::GAME_OVER] = "assets/sounds/game_over.wav";
        soundFiles[game::event::SoundEvent::PLAYER_JUMP] = "assets/sounds/jump.wav";
        soundFiles[game::event::SoundEvent::SHELL_KICK] = "assets/sounds/kick.wav";
        soundFiles[game::event::SoundEvent::LEVEL_CLEAR] = "assets/sounds/level_clear.wav";
        soundFiles[game::event::SoundEvent::PLAYER_DIE] = "assets/sounds/lost_life.wav";
        soundFiles[game::event::SoundEvent::POWER_DOWN] = "assets/sounds/lost_suit.wav";
        soundFiles[game::event::SoundEvent::POWERUP_APPEARS] = "assets/sounds/mushroom_appears.wav";
        soundFiles[game::event::SoundEvent::GAME_PAUSE] = "assets/sounds/pause.wav";
        soundFiles[game::event::SoundEvent::PMETER_FULL] = "assets/sounds/pmeter.wav";
        soundFiles[game::event::SoundEvent::POWER_UP] = "assets/sounds/power_up.wav";
        soundFiles[game::event::SoundEvent::CANNON_SHOT] = "assets/sounds/shot.wav";
        soundFiles[game::event::SoundEvent::PLAYER_SKID] = "assets/sounds/skid.wav";
        soundFiles[game::event::SoundEvent::ENEMY_STOMP] = "assets/sounds/stomp.wav";
        soundFiles[game::event::SoundEvent::TIME_WARNING] = "assets/sounds/time_up.wav";

        // Background Music
        backgroundMusicFiles[game::event::BackgroundMusicState::MAIN_MENU] = "assets/music/main_menu.mp3";
        backgroundMusicFiles[game::event::BackgroundMusicState::LEVEL_1] = "assets/music/level_1.mp3";
        backgroundMusicFiles[game::event::BackgroundMusicState::LEVEL_2] = "assets/music/level_2.mp3";
        backgroundMusicFiles[game::event::BackgroundMusicState::LEVEL_3] = "assets/music/level_3.mp3";
        backgroundMusicFiles[game::event::BackgroundMusicState::GAME_OVER_SCREEN] = "assets/music/boss_battle.mp3";
        backgroundMusicFiles[game::event::BackgroundMusicState::SETTING_SCREEN] = "assets/music/game_over_screen.mp3";
        backgroundMusicFiles[game::event::BackgroundMusicState::LEVEL_SCREEN] = "assets/music/level_clear_screen.mp3";

        // Load các file sound effect
        for (const auto& pair : soundFiles) {
            loadSoundFile(pair.first, pair.second);
        }

        // Load các file nhạc nền
        for (const auto& pair : backgroundMusicFiles) {
            loadBackgroundMusicFile(pair.first, pair.second);
        }

        // Phát nhạc nền mặc định (MAIN_MENU)
        setBackgroundMusic(currentMusicState);
    }

    void SoundManager::loadSoundFile(game::event::SoundEvent event, const string& filePath) {
        sf::SoundBuffer buffer;
        if(!buffer.loadFromFile(filePath)) {
            cerr << "Failed to load sound file: " << filePath << endl;
            return;
        }
        soundBuffers[event] = buffer;
        sounds[event].setBuffer(soundBuffers[event]);
        sounds[event].setVolume(soundEffectsVolume);
    }

    void SoundManager::loadBackgroundMusicFile(game::event::BackgroundMusicState state, const string& filePath) {
        sf::Music* music = new sf::Music();
        if(!music->openFromFile(filePath)){
            cerr << "Failed to load background music: " << filePath << endl;
            delete music;
            return;
        }
        music->setLoop(true);
        music->setVolume(musicVolume);
        backgroundMusics[state] = music;
    }

    void SoundManager::playSound(game::event::SoundEvent event) {
        if(!isSoundEffectsEnabled) {
            return;
        }
        if(sounds.find(event) != sounds.end()) {
            sounds[event].play();
        } else {
            cerr << "Sound for event " << static_cast<int>(event) << " not found!" << endl;
        }
    }

    void SoundManager::setBackgroundMusic(game::event::BackgroundMusicState state) {
        if(!isMusicEnabled) {
            return;
        }
        if(backgroundMusics.find(state) != backgroundMusics.end()) {
            if(currentMusic != nullptr) {
                currentMusic->stop();
            }
            currentMusicState = state;
            currentMusic = backgroundMusics[state];
            currentMusic->setVolume(musicVolume);
            currentMusic->play();
        } else {
            cerr << "Background music for state " << static_cast<int>(state) << " not found!" << endl;
        }
    }

    void SoundManager::toggleBackgroundMusic(bool enable) {
        isMusicEnabled = enable;
        if(isMusicEnabled) {
            setBackgroundMusic(currentMusicState);
        } else {
            if(currentMusic != nullptr) {
                currentMusic->stop();
            }
        }
    }

    void SoundManager::toggleSoundEffects(bool enable) {
        isSoundEffectsEnabled =  enable;
    }

    void SoundManager::pauseBackgroundMusic() {
        if(isMusicEnabled && currentMusic != nullptr) {
            currentMusic->pause();
        }
    }

    void SoundManager::resumeBackgroundMusic() {
        if(isMusicEnabled && currentMusic != nullptr) {
            currentMusic->play();
        }
    }

    void SoundManager::adjustBackgroundMusicVolume(float volume) {
        musicVolume = std::max(0.0f, std::min(100.0f, volume));
        if (currentMusic != nullptr) {
            currentMusic->setVolume(musicVolume);
        }
    }

    void SoundManager::adjustSoundEffectsVolume(float volume) {
        soundEffectsVolume = std::max(0.0f, std::min(100.0f, volume));
        for (auto& pair : sounds) {
            pair.second.setVolume(soundEffectsVolume);
        }
    }

}