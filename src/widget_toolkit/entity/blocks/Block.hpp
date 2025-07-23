#pragma once
#include "../entity.hpp"
#include "../../interfaces.hpp"

const sf::Vector2f BLOCK_SCALE = sf::Vector2f(2.5f, 2.5f);

class Block : public mario::entity::Entity {
protected:
    std::string name;
    bool exist;

public:
    Block();
    Block(b2WorldId worldId, sf::Vector2f pos, sf::Vector2f size, const std::string &name);
    virtual void InitSpritesSheet() = 0;
    virtual ~Block() = default;
}; 
