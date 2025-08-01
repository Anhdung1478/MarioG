#include "CollisionManager.hpp"

#include <algorithm>

void CollisionManager::sortEntitiesByX(std::vector<Block*> &Entities) {
    std::sort(Entities.begin(), Entities.end(), [](const Block *a, const Block *b){
        if (a->getPosition().x == b->getPosition().x){
            return a->getPosition().y < b->getPosition().y; // Sort by Y if X is the same
        }
        return a->getPosition().x < b->getPosition().x; // Sort by X first
    });
}

void CollisionManager::findBlocksCollisions(int &L, int &R, const mario::entity::Entity *EntityA, std::vector<Block*> &blocks){
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

    sf::Vector2f centerA = hitBoxA.position + hitBoxA.size / 2.f;
    sf::Vector2f centerB = hitBoxB.position + hitBoxB.size / 2.f;

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

void CollisionManager::fixPosition(mario::entity::Entity *entity, const Block *block, SideCollision side) {
    if(side == SideCollision::None) return;
    switch (side) {
        case SideCollision::Top:
            entity->setPosition(sf::Vector2f(entity->getPosition().x, block->getPosition().y + entity->getSize().y));
            break;
        case SideCollision::Bottom:
            entity->setPosition(sf::Vector2f(entity->getPosition().x, block->getHitbox().position.y));
            break;
        case SideCollision::Left:
            entity->setPosition(sf::Vector2f(block->getHitbox().position.x + block->getSize().x + entity->getSize().x / 2.0f, entity->getPosition().y));
            break;
        case SideCollision::Right:
            entity->setPosition(sf::Vector2f(block->getHitbox().position.x - entity->getSize().x / 2.0f, entity->getPosition().y));
            break;
    }
}

void CollisionManager::checkCollisionPlayerWithBlocks(mario::entity::Player *player, std::vector<Block*> &blocks) {
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
            block->reactToCollision(side ^ 1);
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
        }
    }

    sf::Vector2f vel = player->getVelocity();
    if(hasBottomCollision) {
        vel.y = -10.f; // Adjust this value as needed
        player->resetJump();
        player->setOnGround(true);
    }

    if(hasTopCollision) {
        vel.y = 0.f; // Adjust this value as needed
    }

    if(hasLeftCollision) {
        vel.x = 0.f; // Adjust this value as needed
    }

    if(hasRightCollision) {
        vel.x = 0.f; // Adjust this value as needed
    }

    player->setVelocity(vel);
}