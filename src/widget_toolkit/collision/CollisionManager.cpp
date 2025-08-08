#include "CollisionManager.hpp"

#include <algorithm>

namespace mario {
namespace entity {

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
    auto itL = std::lower_bound(blocks.begin(), blocks.end(), EntityA->getPosition().x - EntityA->getSize().x, 
        [](const Block *block, float posX) {
            return block->getPosition().x < posX;
        });

    auto itR = std::upper_bound(blocks.begin(), blocks.end(), EntityA->getPosition().x + EntityA->getSize().x, 
        [](float posX, const Block *block) {
            return posX < block->getPosition().x;
        });

        // Set the collision bounds
        L = std::distance(blocks.begin(), itL);
        R = std::distance(blocks.begin(), itR);
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
    if(player->isInDeadAnimation())
        return;

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

        sf::Vector2f vel = player->getVelocity();
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

    void CollisionManager::checkCollisionEnemyWithBlocks(std::vector<Enemy*> &enemies, std::vector<Block*> &blocks){
        for (auto& enemy : enemies) {
            if(!enemy->getHitbox().findIntersection(cameraBounds)) continue;
            mario::entity::Piranha* piranha = dynamic_cast<mario::entity::Piranha*>(enemy);
            if(!piranha) {
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
                        
                        fixPosition(enemy, block, side);
                    }
                }

                sf::Vector2f vel = enemy->getVelocity();
                if (hasBottomCollision) {
                    vel.y = 0.f;
                    enemy->setOnGround(true); // Đặt trạng thái trên mặt đất
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
        if(player->isInDeadAnimation() || player->isShadow())
            return;

        for (auto& enemy : enemies) {
            if (!enemy->getHitbox().findIntersection(cameraBounds)) continue;
            SideCollision side = findCollisionSide(player, enemy);
            if (side != SideCollision::None) {
                enemy->reactCollision(side ^ 1, Collision(Collision::Type::Player));
                switch (side) {
                    case SideCollision::Top:
                        player->beingHit();
                        break;
                    case SideCollision::Bottom:
                        player->resetJump();
                        player->setOnGround(true);
                        player->jump(false);
                        break;
                    case SideCollision::Left:
                        player->beingHit();
                        break;
                    case SideCollision::Right:
                        player->beingHit();
                        break;
                    default:
                        break;
                }
                fixPosition(player, enemy, side);
            }
        }
    }


    void CollisionManager::checkCollisionPlayerWithItems(Player *&player, std::vector<mario::entity::Item*>& items) {
        if(player->isInDeadAnimation())
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

    void CollisionManager::checkCollisionItemsWithBlocks(std::vector<mario::entity::Item*>& items, std::vector<mario::entity::Block*>& blocks) {
        for (auto& item : items) {
            if (!item->isCollected()) {
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

} // namespace entity
} // namespace mario
