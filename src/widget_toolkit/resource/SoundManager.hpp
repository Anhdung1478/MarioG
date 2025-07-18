#pragma once

#include <SFML/Audio.hpp>
#include <iostream>
#include <map>
#include "../event.hpp"

using namespace std;

namespace mario::audio{
    class SoundManager {
    private:
        map<mario::event::SoundEvent, string> soundFiles;
        map<mario::event::SoundEvent, sf::SoundBuffer> soundBuffers;
        map<mario::event::SoundEvent, sf::Sound> sounds;

        map<mario::event::BackgroundMusicState, string> backgroundMusicFiles;
        map<mario::event::BackgroundMusicState, sf::Music*> backgroundMusics;
        
        mario::event::BackgroundMusicState currentMusicState;
        sf::Music* currentMusic;
        
        bool isMusicEnabled;
        bool isSoundEffectsEnabled;
        float musicVolume;
        float soundEffectsVolume;
        
        void loadSoundFile(mario::event::SoundEvent event, const string& filePath);
        void loadBackgroundMusicFile(mario::event::BackgroundMusicState state, const string& filePath);
    public:
        SoundManager();
        ~SoundManager();
        
        void loadSounds();
        void playSound(mario::event::SoundEvent event);
        void setBackgroundMusic(mario::event::BackgroundMusicState state);
        void toggleBackgroundMusic(bool enable);
        void toggleSoundEffects(bool enable);
        void pauseBackgroundMusic();
        void resumeBackgroundMusic();
        void adjustBackgroundMusicVolume(float volume);
        void adjustSoundEffectsVolume(float volume);
    };
}