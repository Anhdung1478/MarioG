#pragma once

#include <SFML/Audio.hpp>
#include <iostream>
#include <map>
#include "../event.hpp"

using namespace std;

namespace game::audio{
    class SoundManager {
    private:
        map<game::event::SoundEvent, string> soundFiles;
        map<game::event::SoundEvent, sf::SoundBuffer> soundBuffers;
        map<game::event::SoundEvent, sf::Sound> sounds;

        map<game::event::BackgroundMusicState, string> backgroundMusicFiles;
        map<game::event::BackgroundMusicState, sf::Music*> backgroundMusics;
        
        game::event::BackgroundMusicState currentMusicState;
        sf::Music* currentMusic;
        
        bool isMusicEnabled;
        bool isSoundEffectsEnabled;
        float musicVolume;
        float soundEffectsVolume;
        
        void loadSoundFile(game::event::SoundEvent event, const string& filePath);
        void loadBackgroundMusicFile(game::event::BackgroundMusicState state, const string& filePath);
    public:
        SoundManager();
        ~SoundManager();
        
        void loadSounds();
        void playSound(game::event::SoundEvent event);
        void setBackgroundMusic(game::event::BackgroundMusicState state);
        void toggleBackgroundMusic(bool enable);
        void toggleSoundEffects(bool enable);
        void pauseBackgroundMusic();
        void resumeBackgroundMusic();
        void adjustBackgroundMusicVolume(float volume);
        void adjustSoundEffectsVolume(float volume);
    };
}