#pragma once
#include "../entity/blocks/Block.hpp"
#include "../entity/enemy/enemy.hpp"
#include "../entity/player.hpp"
// #include "../entity/item/item.hpp"


class CollisionManager {
public:
    void sortEntitiesByX(std::vector<Block*> &blocks); // Sort blocks based on their X and Y positions - X first, Y second
    void findBlocksCollisions(int &L, int &R, const mario::entity::Entity *EntityA, std::vector<Block*> &blocks);
    SideCollision findCollisionSide(const mario::entity::Entity *EntityA, const mario::entity::Entity *EntityB);
    void fixPosition(mario::entity::Entity *entity, const Block *block, SideCollision side);
    void checkCollisionPlayerWithBlocks(mario::entity::Player *&player, std::vector<Block*> &blocks);
    void checkCollisionEnemyWithBlocks(std::vector<mario::entity::Enemy*> &enemies, std::vector<Block*> &blocks);
};