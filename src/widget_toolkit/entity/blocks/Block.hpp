#pragma once

#include "../entity.hpp"
#include "../../interfaces.hpp"

namespace mario {
namespace entity {

class Player;
class ItemManager;

inline const sf::Vector2f BLOCK_SCALE = sf::Vector2f(2.5f, 2.5f);

class Block : public mario::entity::Entity {
protected:
    std::string name;
    bool exist;
    int blockID;

public:
    Block();
    Block(sf::Vector2f pos, sf::Vector2f size, const std::string &name);
    virtual void InitSpritesSheet() = 0;
    virtual ~Block() = default;

    virtual void onHit(Player* player, ItemManager* itemManager);
    int getBlockID() const { return blockID; }
    void setBlockID(int id) { blockID = id; }
};

} // namespace entity
} // namespace mario
