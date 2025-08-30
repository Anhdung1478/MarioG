#include "FlagPole.hpp"

namespace mario::entity {

    FlagPole::FlagPole() : Entity(), enemiesFlagAnimation(nullptr), alliesFlagAnimation(nullptr)
    {
        
    }

    FlagPole::~FlagPole() {
        for(auto &pole : flagPoles) delete pole;
        delete enemiesFlagAnimation;
        delete alliesFlagAnimation;
    }

    void FlagPole::addEnemiesFlag(sf::Vector2f pos) {
        std::cout << "Position: " << pos.x << ", " << pos.y << "\n";
        enemiesFlagAnimation = new Animation("asset/maps/Image/tiles-8.png", BLOCK_SCALE, {
            {"enemies-flag[0]", 1, 171, 16, 16},
            {"enemies-flag[1]", 18, 171, 16, 16},
            {"enemies-flag[2]", 35, 171, 16, 16}
        });
        enemiesFlagAnimation->addAnimationStep("enemies-flag[0]");
        enemiesFlagAnimation->addAnimationStep("enemies-flag[1]");
        enemiesFlagAnimation->addAnimationStep("enemies-flag[2]");
        enemiesFlagAnimation->setTimeBetweenStep(1/7.0f);
        enemiesFlagAnimation->setPosition(pos);
        enemiesFlagAnimation->setAnimationState(true);
    }

    void FlagPole::addAlliesFlag(sf::Vector2f pos) {
        alliesFlagAnimation = new Animation("asset/maps/Image/tiles-8.png", BLOCK_SCALE, {
            {"win-flag[0]", 1, 188, 16, 16},
            {"win-flag[1]", 18, 188, 16, 16},
            {"win-flag[2]", 35, 188, 16, 16},
            {"win-flag[3]", 52, 188, 16, 16}
        });
        alliesFlagAnimation->addAnimationStep("win-flag[0]");
        alliesFlagAnimation->addAnimationStep("win-flag[1]");
        alliesFlagAnimation->addAnimationStep("win-flag[2]");
        alliesFlagAnimation->addAnimationStep("win-flag[3]");
        alliesFlagAnimation->setTimeBetweenStep(1/7.0f);
        alliesFlagAnimation->setAnimationState(true);
        startPosAlliesFlag = pos + sf::Vector2f(0.0f, 40.0f);
        alliesFlagAnimation->setPosition(startPosAlliesFlag);
        finalPosAlliesFlag = pos;
    }

    void FlagPole::addFlagPole(sf::Vector2f pos, sf::Vector2f size, std::string name) {
        BackgroundBlock* newFlagPole;
        if(name == "flag-pole[0]") {
            newFlagPole = new BackgroundBlock(pos, size, name, {"flag-pole[0]", 8, 137, 2, 16});
        } else if(name == "flag-pole[1]") {
            newFlagPole = new BackgroundBlock(pos, size, name, {"flag-pole[1]", 25, 137, 2, 16});
        } else if(name == "flag-pole[2]") {
            newFlagPole = new BackgroundBlock(pos, size, name, {"flag-pole[2]", 59, 120, 2, 16});
        }
        if(pos.y < startPosEnemiesFlag.y) startPosEnemiesFlag = pos;
        if(pos.y > finalPosEnemiesFlag.y) finalPosEnemiesFlag = pos;
        flagPoles.push_back(newFlagPole);
    }

    int FlagPole::reactToCollision(Player* player) {
        if(isWin) return -1;
        BackgroundBlock* pole;
        for(int i = 0; i < flagPoles.size(); i++) {
            pole = flagPoles[i];
            if(pole->getHitbox().findIntersection(player->getHitbox())) {
                if(!isClimbing) {
                    isClimbing = true;
                    // player->toggleClimbingBehavior(true);
                    player->setPosition(sf::Vector2f(pole->getPosition().x - pole->getSize().x/2.0f - player->getSize().x/2.0f, player->getPosition().y));
                    player->startClimbingBehavior(pole->getPosition().x);
                    player->addScoreToPlayer(((int)flagPoles.size() - i + 1)  * 100, true);
                }
            }
            else if(player->getPosition().x >= finalPosEnemiesFlag.x && player->getPosition().y >= startPosEnemiesFlag.y - 40.0f){
                if(!isClimbing) {
                    i = flagPoles.size() - 1;
                    pole = flagPoles[i];
                    isClimbing = true;
                    // player->toggleClimbingBehavior(true);
                    player->setPosition(sf::Vector2f(pole->getPosition().x - pole->getSize().x/2.0f - player->getSize().x/2.0f, player->getPosition().y));
                    player->startClimbingBehavior(pole->getPosition().x);
                    player->addScoreToPlayer(((int)flagPoles.size() - 1 + 1)  * 100, true);
                }
            }
        }
        return -1;
    }

    bool FlagPole::getWinState(){
        return isWin;
    }

    void FlagPole::update(const sf::RenderWindow *window, float dt, Player* player) {
        if(isClimbing){
            if(enemiesFlagAnimation->getPosition().y >= finalPosEnemiesFlag.y) {
                isClimbing = false;
                isWin = true;
                enemiesFlagAnimation->setPosition(sf::Vector2f(enemiesFlagAnimation->getPosition().x, finalPosEnemiesFlag.y));
                player->finishClimbingBehavior();
            }
            else {
                enemiesFlagAnimation->setPosition(sf::Vector2f(
                    enemiesFlagAnimation->getPosition().x,
                    enemiesFlagAnimation->getPosition().y + 150.0f * dt
                ));
            }
        }
        enemiesFlagAnimation->update(window, dt);
        if(isWin) {
            if(alliesFlagAnimation->getPosition().y <= finalPosAlliesFlag.y) {
                alliesFlagAnimation->setPosition(sf::Vector2f(alliesFlagAnimation->getPosition().x, finalPosAlliesFlag.y));
            }
            else {
                alliesFlagAnimation->setPosition(sf::Vector2f(
                    alliesFlagAnimation->getPosition().x,
                    alliesFlagAnimation->getPosition().y - 15.0f * dt
                ));
            }
            alliesFlagAnimation->update(window, dt);
        }
    }

    void FlagPole::handleEvent(const sf::RenderWindow *window, const sf::Event &event) {

    }

    void FlagPole::render(sf::RenderWindow *window) {
        for(auto& flagPole : flagPoles) {
            flagPole->render(window);
        }
        enemiesFlagAnimation->render(window);
        if(isWin) alliesFlagAnimation->render(window);
    }

} // namespace mario::entity