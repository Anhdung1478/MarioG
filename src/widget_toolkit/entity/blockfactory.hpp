#pragma once

#include<bits/stdc++.h>
#include "blocks/SolidBlock.hpp"
#include "blocks/QuestionBlock.hpp"
#include "blocks/Brick.hpp"
#include "blocks/BackgroundBlock.hpp"

namespace mario::entity {
    class BlockFactory {
        public:
            Block* createBlock(int tileID, sf::Vector2f spawnPoint, int themeID) {
                switch (tileID) {
                    // Grass ground tiles
                    case 0:
                        return new SolidBlock(spawnPoint, sf::Vector2f(16, 16), "grass-ground[0]");
                    case 1:
                        return new SolidBlock(spawnPoint, sf::Vector2f(16, 16), "grass-ground[1]");
                    case 2:
                        return new SolidBlock(spawnPoint, sf::Vector2f(16, 16), "grass-ground[2]");
                    case 39:
                        return new SolidBlock(spawnPoint, sf::Vector2f(16, 16), "grass-ground[3]");
                    case 40:
                        return new SolidBlock(spawnPoint, sf::Vector2f(16, 16), "grass-ground[4]");
                    case 41:
                        return new SolidBlock(spawnPoint, sf::Vector2f(16, 16), "grass-ground[5]");
                        
                    // Stone ground tiles
                    case 4:
                        return new SolidBlock(spawnPoint, sf::Vector2f(16, 16), "stone-ground[0]");
                    case 5:
                        return new SolidBlock(spawnPoint, sf::Vector2f(16, 16), "stone-ground[1]");
                    case 6:
                        return new SolidBlock(spawnPoint, sf::Vector2f(16, 16), "stone-ground[2]");
                    case 43:
                        return new SolidBlock(spawnPoint, sf::Vector2f(16, 16), "stone-ground[3]");
                    case 44:
                        return new SolidBlock(spawnPoint, sf::Vector2f(16, 16), "stone-ground[4]");
                    case 45:
                        return new SolidBlock(spawnPoint, sf::Vector2f(16, 16), "stone-ground[5]");
                    
                    // Snow ground tiles
                    case 8:
                        return new SolidBlock(spawnPoint, sf::Vector2f(16, 16), "snow-ground[0]");
                    case 9:
                        return new SolidBlock(spawnPoint, sf::Vector2f(16, 16), "snow-ground[1]");
                    case 10:
                        return new SolidBlock(spawnPoint, sf::Vector2f(16, 16), "snow-ground[2]");
                    case 47:
                        return new SolidBlock(spawnPoint, sf::Vector2f(16, 16), "snow-ground[3]");
                    case 48:
                        return new SolidBlock(spawnPoint, sf::Vector2f(16, 16), "snow-ground[4]");
                    case 49:
                        return new SolidBlock(spawnPoint, sf::Vector2f(16, 16), "snow-ground[5]");

                    // Green pipe tiles
                    case 123:
                        return new SolidBlock(spawnPoint, sf::Vector2f(16, 16), "green-pipe[0]");
                    case 124:
                        return new SolidBlock(spawnPoint, sf::Vector2f(16, 16), "green-pipe[1]");
                    case 162:
                        return new SolidBlock(spawnPoint, sf::Vector2f(16, 16), "green-pipe[2]");
                    case 163:
                        return new SolidBlock(spawnPoint, sf::Vector2f(16, 16), "green-pipe[3]");
                    
                    // Gray pipe tiles
                    case 128:
                        return new SolidBlock(spawnPoint, sf::Vector2f(16, 16), "gray-pipe[0]");
                    case 129:
                        return new SolidBlock(spawnPoint, sf::Vector2f(16, 16), "gray-pipe[1]");
                    case 167:
                        return new SolidBlock(spawnPoint, sf::Vector2f(16, 16), "gray-pipe[2]");
                    case 168:
                        return new SolidBlock(spawnPoint, sf::Vector2f(16, 16), "gray-pipe[3]");

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
                        return new Brick(spawnPoint, sf::Vector2f(16, 16), "brick-block[0]", -1, themeID);
                    case 197:
                        return new Brick(spawnPoint, sf::Vector2f(16, 16), "brick-block[1]", -1, themeID);
                    case 199:
                        return new Brick(spawnPoint, sf::Vector2f(16, 16), "brick-block[2]", -1, themeID);
                    default:
                        return nullptr;
                }
            }

            Block* createBackgroundBlock(int tileID, sf::Vector2f spawnPoint, int themeID, int tilesetColumns, int tileWidth, int tileHeight, int margin, int spacing) {
                switch (tileID) {
                    // Background blocks
                    case 78:
                        return new BackgroundBlock(spawnPoint, sf::Vector2f(16, 16), "grass[0][0]");
                    case 79:
                        return new BackgroundBlock(spawnPoint, sf::Vector2f(16, 16), "grass[1][0]");
                    case 80:
                        return new BackgroundBlock(spawnPoint, sf::Vector2f(16, 16), "grass[2][0]");
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