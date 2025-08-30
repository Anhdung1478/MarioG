#pragma once

#include<bits/stdc++.h>
#include "LevelState.hpp"

namespace mario::resource {
    class LevelDataManager {
        private:
            sf::Time timeUntilNextAutoSave;

            const std::string defaultAutoSaveFilePath = "asset/save_data/autosave.svx";
            const sf::Time defaultTimeDiffereceBetweenAutoSave = sf::seconds(60);

            friend void encryptDataIntoFile(std::ofstream &ofst, LevelState levelData);
            friend LevelState decryptDataFromFile(std::ifstream &ifst);
            
        public:
            LevelDataManager();
            bool checkExistAutoSave();
            void deleteAutoSaveFile();
            void autoSave(LevelState levelData);
            void update(float dt, LevelState levelData);
            void saveLevelDataIntoFile(LevelState levelData, const std::string &fileName);
            LevelState loadLevelDataFromFile(const std::string &fileName);
            LevelState loadAutoSaveLevelData();
    };
}