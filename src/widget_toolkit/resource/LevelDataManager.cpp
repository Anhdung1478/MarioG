#include "LevelDataManager.hpp"

mario::resource::LevelDataManager::LevelDataManager() {
    timeUntilNextAutoSave = defaultTimeDiffereceBetweenAutoSave;
}

void mario::resource::LevelDataManager::update(float dt, LevelState levelData) {
    timeUntilNextAutoSave -= sf::seconds(dt);
    if(timeUntilNextAutoSave <= sf::seconds(0)) {
        autoSave(levelData);
        timeUntilNextAutoSave += defaultTimeDiffereceBetweenAutoSave;
    }
}

void mario::resource::LevelDataManager::autoSave(LevelState levelData) {
    saveLevelDataIntoFile(levelData, defaultAutoSaveFilePath);
}

void encryptDataIntoFile(std::ofstream &ofst, mario::resource::LevelState levelData) {
    ofst << "Level = " << levelData.level << '\n';
    ofst << "Lives = " << levelData.num_lives << '\n';
    ofst << "Coins = " << levelData.coins << '\n';
    ofst << "Score = " << levelData.score << '\n';
    ofst << "CharacterType = " << int(levelData.characterType) << '\n';
    ofst << "StateType = " << int(levelData.stateType) << '\n';
}

mario::resource::LevelState decryptDataFromFile(std::ifstream &ifst) {
    mario::resource::LevelState levelData;
    std::string strTemp;
    char charTemp;
    int intTemp;

    ifst >> strTemp;
    ifst >> charTemp;
    ifst >> intTemp;

    assert(strTemp == "Level" && charTemp == '=');
    levelData.level = intTemp;

    ifst >> strTemp;
    ifst >> charTemp;
    ifst >> intTemp;

    assert(strTemp == "Lives" && charTemp == '=');
    levelData.num_lives = intTemp;

    ifst >> strTemp;
    ifst >> charTemp;
    ifst >> intTemp;

    assert(strTemp == "Coins" && charTemp == '=');
    levelData.coins = intTemp;

    ifst >> strTemp;
    ifst >> charTemp;
    ifst >> intTemp;

    assert(strTemp == "Score" && charTemp == '=');
    levelData.score = intTemp;

    ifst >> strTemp;
    ifst >> charTemp;
    ifst >> intTemp;

    assert(strTemp == "CharacterType" && charTemp == '=');
    levelData.characterType = mario::entity::CharacterListType(intTemp);

    ifst >> strTemp;
    ifst >> charTemp;
    ifst >> intTemp;

    assert(strTemp == "StateType" && charTemp == '=');
    levelData.stateType = mario::entity::player_state::PlayerStateType(intTemp);

    return levelData;
}

void mario::resource::LevelDataManager::saveLevelDataIntoFile(LevelState levelData, const std::string &fileName) {
    std::ofstream ofst(fileName);
    encryptDataIntoFile(ofst, levelData);
}

mario::resource::LevelState mario::resource::LevelDataManager::loadLevelDataFromFile(const std::string &fileName) {
    std::ifstream ifst(fileName);
    return decryptDataFromFile(ifst);
}