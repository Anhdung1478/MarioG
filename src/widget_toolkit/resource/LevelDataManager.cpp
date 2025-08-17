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

bool mario::resource::LevelDataManager::checkExistAutoSave() {
    return (fopen(defaultAutoSaveFilePath.c_str(), "r"));
}

void mario::resource::LevelDataManager::autoSave(LevelState levelData) {
    saveLevelDataIntoFile(levelData, defaultAutoSaveFilePath);
}

void encryptDataIntoFile(std::ofstream &ofst, mario::resource::LevelState levelData) {
    ofst << "Level= " << levelData.level << '\n';
    ofst << "Lives= " << levelData.num_lives << '\n';
    ofst << "Coins= " << levelData.coins << '\n';
    ofst << "Score= " << levelData.score << '\n';
    ofst << "CharacterType= " << int(levelData.characterType) << '\n';
    ofst << "StateType= " << int(levelData.stateType) << '\n';
}

mario::resource::LevelState decryptDataFromFile(std::ifstream &ifst) {
    mario::resource::LevelState levelData;
    std::string strTemp;
    int intTemp;

    ifst >> strTemp;
    ifst >> intTemp;

    assert(strTemp == "Level=");
    levelData.level = intTemp;

    ifst >> strTemp;
    ifst >> intTemp;

    assert(strTemp == "Lives=");
    levelData.num_lives = intTemp;

    ifst >> strTemp;
    ifst >> intTemp;

    assert(strTemp == "Coins=");
    levelData.coins = intTemp;

    ifst >> strTemp;
    ifst >> intTemp;

    assert(strTemp == "Score=");
    levelData.score = intTemp;

    ifst >> strTemp;
    ifst >> intTemp;

    assert(strTemp == "CharacterType=");
    levelData.characterType = mario::entity::CharacterListType(intTemp);

    ifst >> strTemp;
    ifst >> intTemp;

    assert(strTemp == "StateType=");
    levelData.stateType = mario::entity::player_state::PlayerStateType(intTemp);

    levelData.times = sf::seconds(mario::resource::LevelState::DEFAULT_LEVEL_TIME[levelData.level]);
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

mario::resource::LevelState mario::resource::LevelDataManager::loadAutoSaveLevelData() {
    return loadLevelDataFromFile(defaultAutoSaveFilePath);
}