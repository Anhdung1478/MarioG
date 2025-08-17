#include "CollisionManager.hpp"

#include <algorithm>

namespace mario::entity {

    void CollisionManager::loadGroundBlocks(const std::vector<Block*> &blocks) {
        groundBlocks = blocks;
    }

    void CollisionManager::updateCameraBounds(const sf::FloatRect &bounds) {
        cameraBounds = bounds;
    }

    void CollisionManager::sortEntitiesByX(std::vector<Block*> &Entities) {
        std::sort(Entities.begin(), Entities.end(), [](const Block *a, const Block *b){
            if (a->getPosition().x == b->getPosition().x){
                return a->getPosition().y < b->getPosition().y; // Sort by Y if X is the same
            }
            return a->getPosition().x < b->getPosition().x; // Sort by X first
        });
    }

    void CollisionManager::findBlocksCollisions(int &L, int &R, const mario::entity::Entity *EntityA, std::vector<Block*> &blocks) {
        //using lower_bound and upper_bound to find the range of blocks that might collide with the entity
        float boundX = 2 * EntityA->getSize().x; 
        auto itL = std::lower_bound(blocks.begin(), blocks.end(), EntityA->getPosition().x - boundX, 
            [](const Block *block, float posX) {
                return block->getPosition().x < posX;
            });

        auto itR = std::upper_bound(blocks.begin(), blocks.end(), EntityA->getPosition().x + boundX, 
            [](float posX, const Block *block) {

                return posX < block->getPosition().x;
            });

        // Set the collision bounds
        L = std::distance(blocks.begin(), itL);
        R = std::distance(blocks.begin(), itR);
        R = std::min(R, static_cast<int>(blocks.size()) - 1); // Ensure R does not exceed the size of the vector
    }

    SideCollision CollisionManager::findCollisionSide(const mario::entity::Entity *entityA, const mario::entity::Entity *entityB) {
        sf::FloatRect hitBoxA = entityA->getHitbox();
        sf::FloatRect hitBoxB = entityB->getHitbox();
    
        sf::Vector2f centerA = hitBoxA.getCenter();
        sf::Vector2f centerB = hitBoxB.getCenter();

        float deltaX = centerB.x - centerA.x;
        float deltaY = centerB.y - centerA.y;

        float overlapX = (hitBoxA.size.x + hitBoxB.size.x) / 2.f - std::abs(deltaX);
        float overlapY = (hitBoxA.size.y + hitBoxB.size.y) / 2.f - std::abs(deltaY);

        if (overlapX >= 0 && overlapY >= 0) {
            if (overlapX >= overlapY) {
                return (deltaY > 0) ? SideCollision::Bottom : SideCollision::Top;
            } 
            else {
                return (deltaX < 0) ? SideCollision::Left : SideCollision::Right;
            }
        }
        return SideCollision::None; // No collision
    }

    void CollisionManager::fixPosition(Entity *entityA, Entity *entityB, SideCollision side) {
        if(side == SideCollision::None) return;
        switch (side) {
            case SideCollision::Top:
                entityA->setPosition(sf::Vector2f(entityA->getPosition().x, entityB->getPosition().y + entityA->getSize().y));
                break;
            case SideCollision::Bottom:
                entityA->setPosition(sf::Vector2f(entityA->getPosition().x, entityB->getHitbox().position.y));
                break;
            case SideCollision::Left:
                entityA->setPosition(sf::Vector2f(entityB->getHitbox().position.x + entityB->getSize().x + entityA->getSize().x / 2.0f, entityA->getPosition().y));
                break;
            case SideCollision::Right:
                entityA->setPosition(sf::Vector2f(entityB->getHitbox().position.x - entityA->getSize().x / 2.0f, entityA->getPosition().y));
                break;
        }
    }

    void CollisionManager::checkCollisionPlayerWithBlocks(mario::entity::Player *&player, std::vector<Block*> &blocks, std::vector<Item*> &items) {
        if(player->canCollisionWithBlock()) {
            sf::Vector2f vel = player->getVelocity();

            for(const auto& block : groundBlocks) {
                if(!block->isExist()) continue;
                SideCollision side = findCollisionSide(player, block);
                if(side != SideCollision::None) {
                    switch (side) {
                        case SideCollision::Top:
                            vel.y = 0.f;
                            break;
                        case SideCollision::Bottom:
                            vel.y = -10.f;
                            player->resetJump();
                            player->setOnGround(true);
                            break;
                        case SideCollision::Left:
                            vel.x = 0.f;
                            break;
                        case SideCollision::Right:
                            vel.x = 0.f;
                            break;
                        default:
                            break;
                    }

                    fixPosition(player, block, side);
                }
            }
        
            int L, R;
            findBlocksCollisions(L, R, player, blocks);

            bool hasTopCollision = false;
            bool hasBottomCollision = false;
            bool hasLeftCollision = false;
            bool hasRightCollision = false;

            for(int i = L; i <= R; ++i){
                auto& block = blocks[i];
                if (!block->isExist()) continue;

                SideCollision side = findCollisionSide(player, block);
                if(side != SideCollision::None) {
                    int typeOfItem = block->reactToCollision(side ^ 1, player);
                    switch (side) {
                        case SideCollision::Top:
                            hasTopCollision = true;
                            break;
                        case SideCollision::Bottom:
                            hasBottomCollision = true;
                            break;
                        case SideCollision::Left:
                            hasLeftCollision = true;
                            break;
                        case SideCollision::Right:
                            hasRightCollision = true;
                            break;
                        default:
                            break;
                    }
                    
                    fixPosition(player, block, side);
                    
                    if(typeOfItem == 1) { // Red-mushroom
                        items.push_back(new RedMushroom(
                            "../../asset/sprites/red-mushroom.json",
                            "../../asset/maps/Image/tiles-8.png",
                            sf::Vector2f(2.5f, 2.5f),
                            "red-mushroom[0]",
                            sf::Vector2f(block->getPosition().x, block->getPosition().y - block->getSize().y/2.0f),
                            sf::Vector2f(16.f, 16.f),
                            sf::Vector2f(80.f, 0.f)
                        ));
                    } 
                    else if(typeOfItem == 2) { // Fire flower
                        items.push_back(new FireFlower(
                            "../../asset/sprites/fireflower.json",
                            "../../asset/maps/Image/tiles-8.png",
                            sf::Vector2f(2.5f, 2.5f),
                            "fireflower[0]",
                            sf::Vector2f(block->getPosition().x, block->getPosition().y - block->getSize().y/2.0f),
                            sf::Vector2f(16.f, 16.f),
                            sf::Vector2f(0.f, 0.f)
                        ));
                    } 
                    else if(typeOfItem == 3) { // One-up mushroom
                        items.push_back(new OneupMushroom(
                            "../../asset/sprites/1up-mushroom.json",
                            "../../asset/maps/Image/tiles-8.png",
                            sf::Vector2f(2.5f, 2.5f),
                            "1up-mushroom[0]",
                            sf::Vector2f(block->getPosition().x, block->getPosition().y - block->getSize().y/2.0f),
                            sf::Vector2f(16.f, 16.f),
                            sf::Vector2f(0.f, 0.f)
                        ));
                    } 
                    else if(typeOfItem == 4) { // Starman
                        // items.push_back(new Starman(
                        //     "../../asset/sprites/starman.json",
                        //     "../../asset/maps/Image/tiles-8.png",
                        //     sf::Vector2f(2.5f, 2.5f),
                        //     "starman[0]",
                        //     sf::Vector2f(500.f, 500.f),
                        //     sf::Vector2f(16.f, 16.f),
                        //     sf::Vector2f(0.f, 0.f)
                        // ));
                    } 
                }
            }
    
            if(hasBottomCollision) {
                vel.y = -10.f;
                player->resetJump();
                player->setOnGround(true);
                // std::cerr << "\nBottom Collision Detected\n";
            }
    
            if(hasTopCollision) {
                vel.y = 0.f;
                // std::cerr << "\nTop Collision Detected\n";
            }
    
            if(hasLeftCollision) {
                vel.x = 0.f;
                //player->setMoveLeft(false);
                // std::cerr << "\nLeft Collision Detected\n";
            }
    
            if(hasRightCollision) {
                vel.x = 0.f;
                //player->setMoveRight(false);
                // std::cerr << "\nRight Collision Detected\n";
            }
    
            player->setVelocity(vel);
        }

        int numFireballs = player->getNumberFireballs();
        for (int j = 0; j < numFireballs; ++j) {
            mario::entity::Fireball *fireball = player->getFireballAtPos(j);
            if(fireball->isExploding())
                continue;

            bool hasTopCollision = false;
            bool hasBottomCollision = false;
            bool hasLeftCollision = false;
            bool hasRightCollision = false;

            sf::Vector2f vel = fireball->getVelocity();
            for(const auto& block : groundBlocks) {
                if(!block->isExist()) continue;
                SideCollision side = findCollisionSide(fireball, block);
                if(side != SideCollision::None) {
                    switch (side) {
                        case SideCollision::Top:
                            hasTopCollision = true;
                            break;
                        case SideCollision::Bottom:
                            hasBottomCollision = true;
                            break;
                        case SideCollision::Left:
                           hasLeftCollision = true;
                            break;
                        case SideCollision::Right:
                            hasRightCollision = true;
                            break;
                        default:
                            break;
                    }

                    fixPosition(fireball, block, side);
                }
            }

            int L, R;
            findBlocksCollisions(L, R, fireball, blocks);
    
            for(int i = L; i <= R; ++i){
                auto& block = blocks[i];
                if (!block->isExist()) 
                    continue;
    
                SideCollision side = findCollisionSide(fireball, block);
                if(side == SideCollision::None)
                    continue;

                switch (side) {
                    case SideCollision::Top:
                        hasTopCollision = true;
                        break;
                    case SideCollision::Bottom:
                        hasBottomCollision = true;
                        break;
                    case SideCollision::Left:
                        hasLeftCollision = true;
                        break;
                    case SideCollision::Right:
                        hasRightCollision = true;
                        break;
                    default:
                        break;
                }
                
                fixPosition(fireball, block, side);
            }

            if(hasTopCollision || hasBottomCollision)
                vel.y = 0.f;
    
            if(hasBottomCollision)
                fireball->setOnGround(true);
    
            fireball->setVelocity(vel);
            if(hasLeftCollision || hasRightCollision)
                player->explosionFireballAtPos(j);
        }
    }

    void CollisionManager::checkCollisionEnemyWithBlocks(std::vector<Enemy*> &enemies, std::vector<Block*> &blocks){
        for (auto& enemy : enemies) {
            mario::entity::Piranha* piranha = dynamic_cast<mario::entity::Piranha*>(enemy);
            if(!piranha) {
                bool hasTopCollisionF = false;
                bool hasBottomCollisionF = false;
                bool hasLeftCollisionF = false;
                bool hasRightCollisionF = false;

                for(const auto& block : groundBlocks) {
                    if(!block->isExist()) continue;

                    SideCollision side = findCollisionSide(enemy, block);
                    if(side != SideCollision::None) {
                        enemy->reactCollision(side, Collision(Collision::Type::Wall));
                        switch (side) {
                            case SideCollision::Top:
                                hasTopCollisionF = true;
                                break;
                            case SideCollision::Bottom:
                                if(enemy->getIsCheckCollisionWithBlock()) {
                                    hasBottomCollisionF = true;
                                } else {
                                    hasBottomCollisionF = false;
                                }
                                break;
                            case SideCollision::Left:
                                hasLeftCollisionF = true;
                                break;
                            case SideCollision::Right:
                                hasRightCollisionF = true;
                                break;
                            default:
                                break;
                        }

                        if(enemy->getIsCheckCollisionWithBlock()) {
                            fixPosition(enemy, block, side);
                        }
                    }
                }
                sf::Vector2f vel = enemy->getVelocity();
                if (hasBottomCollisionF) {
                    vel.y = -10.f;
                    enemy->setOnGround(true);
                } else {
                    enemy->setOnGround(false);
                }

                if (hasTopCollisionF) {
                    vel.y = 0.f;
                }
            
                if (hasLeftCollisionF || hasRightCollisionF) {
                    vel.x = 0.f;
                }

                int L, R;
                findBlocksCollisions(L, R, enemy, blocks);                

                bool hasTopCollision = false;
                bool hasBottomCollision = false;
                bool hasLeftCollision = false;
                bool hasRightCollision = false;

                for (int i = L; i < R; ++i) {
                    auto& block = blocks[i];
                    if (!block->isExist()) continue;

                    SideCollision side = findCollisionSide(enemy, block);
                    if (side != SideCollision::None) {
                        enemy->reactCollision(side, Collision(Collision::Type::Wall));
                        switch (side) {
                            case SideCollision::Top:
                                hasTopCollision = true;
                                break;
                            case SideCollision::Bottom:
                                if(enemy->getIsCheckCollisionWithBlock()) {
                                    hasBottomCollision = true;
                                } else {
                                    hasBottomCollision = false;
                                }
                                break;
                            case SideCollision::Left:
                                hasLeftCollision = true;
                                break;
                            case SideCollision::Right:
                                hasRightCollision = true;
                                break;
                            default:
                                break;
                        }
                        if(enemy->getIsCheckCollisionWithBlock()) {
                            fixPosition(enemy, block, side);
                        }
                    }
                }

                if (hasBottomCollision) {
                    vel.y = -10.f;
                    enemy->setOnGround(true);
                } else {
                    enemy->setOnGround(false);
                }

                if (hasTopCollision) {
                    vel.y = 0.f;
                }
            
                if (hasLeftCollision || hasRightCollision) {
                    vel.x = 0.f;
                }

                enemy->setVelocity(vel);
            }
        }
    }

    void CollisionManager::checkCollisionPlayerWithEnemies(Player *&player, std::vector<Enemy*> &enemies) {
        for (auto& enemy : enemies) {
            if(!enemy->getIsCheckCollisionWithPlayer()) continue;

            if(player->canCollisionWithEnemy()) {
                SideCollision side = findCollisionSide(player, enemy);
                if (side != SideCollision::None) {
                    enemy->reactCollision(side ^ 1, Collision(Collision::Type::Player));
                    if(player->isShadow() && side != SideCollision::Bottom)
                        side = SideCollision::None;
    
                    switch (side) {
                        case SideCollision::Top:
                            player->beingHit();
                            break;
                        case SideCollision::Bottom:
                            if(enemy->getIsPlayerDeadWhenCollisionT()) {
                                player->beingHit();
                            } else {
                                player->jumpOnEnemyHead();
                            }
                            break;
                        case SideCollision::Left:
                            if(enemy->getIsPlayerDeadWhenCollisionLF()) {
                                player->beingHit();
                            }
                            break;
                        case SideCollision::Right:
                            if(enemy->getIsPlayerDeadWhenCollisionLF()) {
                                player->beingHit();
                            }
                            break;
                        default:
                            break;
                    }

                    fixPosition(player, enemy, side);
                }
            }

            // fireballs and enemies
            int numFireballs = player->getNumberFireballs();
            for (int j = 0; j < numFireballs; ++j) {
                mario::entity::Fireball *fireball = player->getFireballAtPos(j);
                if(fireball->isExploding())
                    continue;

                SideCollision side = findCollisionSide(fireball, enemy);
                if (side == SideCollision::None)
                    continue;

                enemy->reactCollision(side ^ 1, Collision(Collision::Type::Fireball));
                player->hitEnemyWithFireball(!enemy->getIsPlayerDeadWhenCollisionT());
                player->explosionFireballAtPos(j);
                break;
            }
        }
    }


    void CollisionManager::checkCollisionPlayerWithItems(Player *&player, std::vector<mario::entity::Item*>& items) {
        if(!player->canCollisionWithItem())
            return;

        for (auto& item : items) {
            if (!item->isCollected()) {
                SideCollision side = findCollisionSide(player, item);
                if (side != SideCollision::None) {
                    item->onCollect(player);
                }
            }
        }
    }

    void CollisionManager::checkCollisionEnemyWithEnemy(std::vector<Enemy*> &enemies) {
        for (size_t i = 0; i < enemies.size(); ++i) {
        auto& enemyA = enemies[i];
        if (!enemyA->isExist() || !enemyA->getIsCheckCollisionWithEnemy()) continue;

        mario::entity::Piranha* piranhaA = dynamic_cast<mario::entity::Piranha*>(enemyA);
        if (piranhaA) continue;

        // Check if koopaA is in ShellState
        mario::entity::Koopa* koopaA = dynamic_cast<mario::entity::Koopa*>(enemyA);
        bool isKoopaAShell = koopaA && koopaA->checkShellState();

        for (size_t j = i + 1; j < enemies.size(); ++j) {
            auto& enemyB = enemies[j];
            if (!enemyB->isExist() || !enemyB->getIsCheckCollisionWithEnemy()) continue;

            // skip Piranha
            mario::entity::Piranha* piranhaB = dynamic_cast<mario::entity::Piranha*>(enemyB);
            if (piranhaB) continue;

            // Check if koopaB is in ShellState
            mario::entity::Koopa* koopaB = dynamic_cast<mario::entity::Koopa*>(enemyB);
            bool isKoopaBShell = koopaB && koopaB->checkShellState();

            // Skip if all of them is Koopa and in ShellState
            if (isKoopaAShell && isKoopaBShell) continue;

            // Skip if no one is Koopa and in ShellState
            if (!isKoopaAShell && !isKoopaBShell) continue;

            SideCollision side = findCollisionSide(enemyA, enemyB);
            if (side != SideCollision::None) {
                // Which one is Koopa and in ShellState
                if (isKoopaAShell) {
                    // if enemyA is Koopa and in ShellState, we call reactCollision for enemyB
                    SideCollision oppositeSide = side;
                    switch (side) {
                        case SideCollision::Top:
                            oppositeSide = SideCollision::Bottom;
                            break;
                        case SideCollision::Bottom:
                            oppositeSide = SideCollision::Top;
                            break;
                        case SideCollision::Left:
                            oppositeSide = SideCollision::Right;
                            break;
                        case SideCollision::Right:
                            oppositeSide = SideCollision::Left;
                            break;
                        default:
                            break;
                    }
                    enemyB->reactCollision(oppositeSide, Collision(Collision::Type::Enemy));
                    fixPosition(enemyB, enemyA, oppositeSide);

                    // Update velocity for enemyB
                    sf::Vector2f velB = enemyB->getVelocity();
                    if (oppositeSide == SideCollision::Bottom) {
                        velB.y = 0.f;
                        enemyB->setOnGround(true);
                    } else if (oppositeSide == SideCollision::Top) {
                        velB.y = 0.f;
                    } else if (oppositeSide == SideCollision::Left || oppositeSide == SideCollision::Right) {
                        velB.x = 0.f;
                    }
                    enemyB->setVelocity(velB);
                } else if (isKoopaBShell) {
                    // if enemyB is Koopa and in Shell State, call reactCollision for enemyA
                    enemyA->reactCollision(side, Collision(Collision::Type::Enemy));
                    fixPosition(enemyA, enemyB, side);

                    // update velocity for enemyA
                    sf::Vector2f velA = enemyA->getVelocity();
                    if (side == SideCollision::Bottom) {
                        velA.y = 0.f;
                        enemyA->setOnGround(true);
                    } else if (side == SideCollision::Top) {
                        velA.y = 0.f;
                    } else if (side == SideCollision::Left || side == SideCollision::Right) {
                        velA.x = 0.f;
                    }
                    enemyA->setVelocity(velA);
                }
            }
        }
    }
    }

    void CollisionManager::checkCollisionItemsWithBlocks(std::vector<mario::entity::Item*>& items, std::vector<mario::entity::Block*>& blocks) {
        for (auto& item : items) {
            if (!item->isCollected()) {

                for(const auto& block : groundBlocks) {
                    if(!block->isExist()) continue;
                    SideCollision side = findCollisionSide(item, block);
                    if(side != SideCollision::None) {
                        fixPosition(item, block, side);
                    }
                }

                int L, R;
                findBlocksCollisions(L, R, item, blocks);
                for (int i = L; i <= R; ++i) {
                    auto& block = blocks[i];
                    if (!block->isExist()) continue;

                    SideCollision side = findCollisionSide(item, block);
                    if (side != SideCollision::None) {
                        fixPosition(item, block, side);
                    }
                }
            }
        }
    }
    
} // namespace mario::entity
