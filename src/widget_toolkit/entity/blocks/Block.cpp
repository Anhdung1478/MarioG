#include "Block.hpp"

Block::Block() : GameObject(), name(""), exist(true), p_animation(nullptr) {
}

Block::Block(const sf::Vector2f& pos, const sf::Vector2f& size, const std::string& name) 
    : GameObject(pos, size), name(name), exist(true), p_animation(nullptr) {
}