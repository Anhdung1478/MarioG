#include "Block.hpp"
#include "../box/static-box.hpp"

Block::Block() : Entity(), name(""), exist(true) {
}

Block::Block(sf::Vector2f pos, sf::Vector2f size, const std::string &name) : name(name), exist(true) {
    sf::Vector2f globalSize = sf::Vector2f(size.x * BLOCK_SCALE.x, size.y * BLOCK_SCALE.y);
    p_body = new mario::entity::StaticBox(pos, globalSize);
}