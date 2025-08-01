#pragma once
#include "../entity.hpp"
#include "../../interfaces.hpp"

const sf::Vector2f BLOCK_SCALE = sf::Vector2f(2.5f, 2.5f);

enum SideCollision{
    Top    = 0,
    Bottom = 1,
    Left   = 2,
    Right  = 3,
    None   = 4
};

class Block : public mario::entity::Entity {
protected:
    std::string name;
    bool exist;

public:
    Block();
    Block(sf::Vector2f pos, sf::Vector2f size, const std::string &name);
    virtual ~Block() = default;

    virtual void InitSpritesSheet() = 0;
    virtual void reactToCollision(int side) = 0;
}; 
