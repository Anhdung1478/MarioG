#include "Block.hpp"
#include "../box/static-box.hpp"
#include "../item/ItemManager.hpp"

namespace mario {
namespace entity {

Block::Block() : Entity(), name(""), exist(true) {
}

Block::Block(sf::Vector2f pos, sf::Vector2f size, const std::string &name) : name(name), exist(true) {
    sf::Vector2f globalSize = sf::Vector2f(size.x * BLOCK_SCALE.x, size.y * BLOCK_SCALE.y);
    p_body = new StaticBox(pos, globalSize);
}

void Block::onHit(Player* player, ItemManager* itemManager) {
    if (itemManager) {
        itemManager->triggerSpawn(blockID, SpawnTrigger::BlockHit);
    }
}

} // namespace entity
} // namespace mario