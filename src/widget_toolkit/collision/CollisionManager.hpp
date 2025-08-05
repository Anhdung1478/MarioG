#pragma once
#include "../entity/blocks/Block.hpp"
#include "../entity/enemy/enemy.hpp"
#include "../entity/player.hpp"
#include "../entity/item/item.hpp"
#include "../entity/item/ItemManager.hpp"

namespace mario {
namespace entity {

class CollisionManager {
public:
    void sortEntitiesByX(std::vector<Block*> &blocks); // Sort blocks based on their X and Y positions - X first, Y second
    void findBlocksCollisions(int &L, int &R, const Entity *EntityA, std::vector<Block*> &blocks);
    SideCollision findCollisionSide(const Entity *EntityA, const Entity *EntityB);
    void fixPosition(Entity *entityA, Entity *entityB, SideCollision side);
    void checkCollisionPlayerWithBlocks(Player *&player, std::vector<Block*> &blocks);
    void checkCollisionEnemyWithBlocks(std::vector<Enemy*> &enemies, std::vector<Block*> &blocks);
    void checkCollisionPlayerWithEnemies(Player *&player, std::vector<Enemy*> &enemies);
    void checkCollisionPlayerWithItems(Player *&player, std::vector<mario::entity::Item*> &items);
private:
    ItemManager* itemManager;
};

} // namespace entity
} // namespace mario
