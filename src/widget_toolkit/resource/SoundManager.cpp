#include "SoundManager.hpp"

using namespace std;

namespace mario::audio {
    SoundManager::SoundManager() :
        isMusicEnabled(true), isSoundEffectsEnabled(true),
        musicVolume(100.0f), soundEffectsVolume(100.0f),
        currentMusicState(mario::event::BackgroundMusicState::MAIN_MENU),
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
        soundFiles[mario::event::SoundEvent::ONE_UP] = "../../asset/sounds/sound/1_up.wav";
        soundFiles[mario::event::SoundEvent::BLOCK_BREAK] = "../../asset/sounds/sound/break_brick_block.wav";
        soundFiles[mario::event::SoundEvent::BLOCK_BUMP] = "../../asset/sounds/sound/bump.wav";
        soundFiles[mario::event::SoundEvent::COIN_COLLECT] = "../../asset/sounds/sound/coin.wav";
        soundFiles[mario::event::SoundEvent::LEVEL_ENTER] = "../../asset/sounds/sound/enter_level.wav";
        soundFiles[mario::event::SoundEvent::FIREBALL_THROW] = "../../asset/sounds/sound/fireball.wav";
        soundFiles[mario::event::SoundEvent::GAME_OVER] = "../../asset/sounds/sound/game_over.wav";
        soundFiles[mario::event::SoundEvent::PLAYER_JUMP] = "../../asset/sounds/sound/jump.wav";
        soundFiles[mario::event::SoundEvent::SHELL_KICK] = "../../asset/sounds/sound/kick.wav";
        soundFiles[mario::event::SoundEvent::LEVEL_CLEAR] = "../../asset/sounds/sound/level_clear.wav";
        soundFiles[mario::event::SoundEvent::PLAYER_DIE] = "../../asset/sounds/sound/lost_life.wav";
        soundFiles[mario::event::SoundEvent::POWER_DOWN] = "../../asset/sounds/sound/lost_suit.wav";
        soundFiles[mario::event::SoundEvent::POWERUP_APPEARS] = "../../asset/sounds/sound/mushroom_appears.wav";
        soundFiles[mario::event::SoundEvent::GAME_PAUSE] = "../../asset/sounds/sound/pause.wav";
        soundFiles[mario::event::SoundEvent::PMETER_FULL] = "../../asset/sounds/sound/pmeter.wav";
        soundFiles[mario::event::SoundEvent::POWER_UP] = "../../asset/sounds/sound/power_up.wav";
        soundFiles[mario::event::SoundEvent::CANNON_SHOT] = "../../asset/sounds/sound/shot.wav";
        soundFiles[mario::event::SoundEvent::PLAYER_SKID] = "../../asset/sounds/sound/skid.wav";
        soundFiles[mario::event::SoundEvent::ENEMY_STOMP] = "../../asset/sounds/sound/stomp.wav";
        soundFiles[mario::event::SoundEvent::TIME_WARNING] = "../../asset/sounds/sound/time_up.wav";

        // Background Music
        backgroundMusicFiles[mario::event::BackgroundMusicState::MAIN_MENU] = "../../asset/sounds/music/MainMenu.mp3";
        backgroundMusicFiles[mario::event::BackgroundMusicState::LEVEL_1] = "../../asset/sounds/music/World1.mp3";
        backgroundMusicFiles[mario::event::BackgroundMusicState::LEVEL_2] = "../../asset/sounds/music/World2.mp3";
        backgroundMusicFiles[mario::event::BackgroundMusicState::LEVEL_3] = "../../asset/sounds/music/World3.mp3";
        backgroundMusicFiles[mario::event::BackgroundMusicState::GAME_OVER_SCREEN] = "../../asset/sounds/music/MainMenu.mp3";
        backgroundMusicFiles[mario::event::BackgroundMusicState::SETTING_SCREEN] = "../../asset/sounds/music/Athletic.mp3";
        backgroundMusicFiles[mario::event::BackgroundMusicState::LEVEL_SCREEN] = "../../asset/sounds/music/Coin Heaven.mp3";

        // Load file sound effect
        for (const auto& pair : soundFiles) {
            loadSoundFile(pair.first, pair.second);
        }

        // Load file music background
        for (const auto& pair : backgroundMusicFiles) {
            loadBackgroundMusicFile(pair.first, pair.second);
        }
    }

    void SoundManager::loadSoundFile(mario::event::SoundEvent event, const string& filePath) {
        sf::SoundBuffer buffer;
        if(!buffer.loadFromFile(filePath)) {
            cerr << "Failed to load sound file: " << filePath << endl;
            return;
        }
        soundBuffers[event] = buffer;

        auto [iter, inserted] = sounds.emplace(event, sf::Sound(soundBuffers[event]));
        if (!inserted) {
            iter->second = sf::Sound(soundBuffers[event]);
        }

        iter->second.setVolume(soundEffectsVolume);
    }

    void SoundManager::loadBackgroundMusicFile(mario::event::BackgroundMusicState state, const string& filePath) {
        sf::Music* music = new sf::Music();
        if(!music->openFromFile(filePath)){
            cerr << "Failed to load background music: " << filePath << endl;
            delete music;
            return;
        }
        music->setLooping(true);
        music->setVolume(musicVolume);
        backgroundMusics[state] = music;
    }

    void SoundManager::playSound(mario::event::SoundEvent event) {
        if(!isSoundEffectsEnabled) {
            return;
        }
        auto it = sounds.find(event);
        if(it != sounds.end()) {
            it->second.play();
        } else {
            cerr << "Sound for event " << static_cast<int>(event) << " not found!" << endl;
        }
    }

    void SoundManager::setBackgroundMusic(mario::event::BackgroundMusicState state) {
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