#pragma once

#include<bits/stdc++.h>
#include "blocks/SolidBlock.hpp"
#include "blocks/QuestionBlock.hpp"
#include "blocks/Brick.hpp"
#include "blocks/BackgroundBlock.hpp"
#include "blocks/InvisibleBlock.hpp"

namespace mario::entity {
    class BlockFactory {
        public:
            Block* createBlock(int tileID, sf::Vector2f spawnPoint, int themeID) {
                switch (tileID) {
                    // Grass ground tiles
                    case 0:
                        return nullptr;
                    case 1:
                        return nullptr;
                    case 2:
                        return nullptr;
                    case 39:
                        return nullptr;
                    case 40:
                        return nullptr;
                    case 41:
                        return nullptr;
                        
                    // Stone ground tiles
                    case 4:
                        return nullptr;
                    case 5:
                        return nullptr;
                    case 6:
                        return nullptr;
                    case 43:
                        return nullptr;
                    case 44:
                        return nullptr;
                    case 45:
                        return nullptr;
                    
                    // Snow ground tiles
                    case 8:
                        return nullptr;
                    case 9:
                        return nullptr;
                    case 10:
                        return nullptr;
                    case 47:
                        return nullptr;
                    case 48:
                        return nullptr;
                    case 49:
                        return nullptr;

                    // Green pipe tiles
                    case 123:
                        return nullptr;
                    case 124:
                        return nullptr;
                    case 162:
                        return nullptr;
                    case 163:
                        return nullptr;
                    
                    // Gray pipe tiles
                    case 128:
                        return nullptr;
                    case 129:
                        return nullptr;
                    case 167:
                        return nullptr;
                    case 168:
                        return nullptr;

                    // Stair blocks
                    case 351:
                        return new SolidBlock(spawnPoint, sf::Vector2f(16, 16), "stair-block[0]");
                    case 352:
                        return new SolidBlock(spawnPoint, sf::Vector2f(16, 16), "stair-block[1]");
                    case 353:
                        return new SolidBlock(spawnPoint, sf::Vector2f(16, 16), "stair-block[2]");

                    // Question blocks
                    case 117:
                        return new QuestionBlock(spawnPoint, sf::Vector2f(16, 16), "question-block[0]", 0, themeID);

                    // Brick blocks
                    case 195:
                        return new Brick(spawnPoint, sf::Vector2f(16, 16), "brick-block", -1, themeID);
                    case 197:
                        return new Brick(spawnPoint, sf::Vector2f(16, 16), "brick-block", -1, themeID);
                    case 199:
                        return new Brick(spawnPoint, sf::Vector2f(16, 16), "brick-block", -1, themeID);
                    
                    // Invisible blocks
                    case 161:
                        return new InvisibleBlock(spawnPoint, sf::Vector2f(16, 16), "invisible-block[0]", 0, themeID);
                    
                    default:
                        return nullptr;
                }
            }

            Block* createBackgroundBlock(int tileID, sf::Vector2f spawnPoint, int themeID, int tilesetColumns, int tileWidth, int tileHeight, int margin, int spacing) {
                switch (tileID) {
                    // Grass ground tiles
                    case 0:
                        return nullptr;
                    case 1:
                        return nullptr;
                    case 2:
                        return nullptr;
                    case 39:
                        return nullptr;
                    case 40:
                        return nullptr;
                    case 41:
                        return nullptr;
                        
                    // Stone ground tiles
                    case 4:
                        return nullptr;
                    case 5:
                        return nullptr;
                    case 6:
                        return nullptr;
                    case 43:
                        return nullptr;
                    case 44:
                        return nullptr;
                    case 45:
                        return nullptr;
                    
                    // Snow ground tiles
                    case 8:
                        return nullptr;
                    case 9:
                        return nullptr;
                    case 10:
                        return nullptr;
                    case 47:
                        return nullptr;
                    case 48:
                        return nullptr;
                    case 49:
                        return nullptr;

                    // Green pipe tiles
                    case 123:
                        return nullptr;
                    case 124:
                        return nullptr;
                    case 162:
                        return nullptr;
                    case 163:
                        return nullptr;
                    
                    // Gray pipe tiles
                    case 128:
                        return nullptr;
                    case 129:
                        return nullptr;
                    case 167:
                        return nullptr;
                    case 168:
                        return nullptr;

                    // Background blocks
                    case 78:
                        return new BackgroundBlock(spawnPoint, sf::Vector2f(16, 16), "grass[0][0]");
                    case 79:
                        return new BackgroundBlock(spawnPoint, sf::Vector2f(16, 16), "grass[1][0]");
                    case 80:
                        return new BackgroundBlock(spawnPoint, sf::Vector2f(16, 16), "grass[2][0]");
                    case 120: // Coin
                        break;
                    default:
                        int _x = tileID % tilesetColumns;
                        int _y = tileID / tilesetColumns;
                        return new BackgroundBlock(spawnPoint, sf::Vector2f(16, 16), std::to_string(tileID), {
                            std::to_string(tileID),
                            _x * 16 + margin + _x * spacing, 
                            _y * 16 + margin + _y * spacing, 
                            16, 
                            16
                        });
                }
                return nullptr;
            }

            ~BlockFactory() {}
    };
}