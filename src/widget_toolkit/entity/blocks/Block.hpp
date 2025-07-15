#pragma once
#include "GameObject.hpp"
#include "../animation/animation.hpp"
#include "../../interfaces.hpp"

class Block : public GameObject, public mario::IScreenElement {
protected:
    std::string name;
    bool exist;
    mario::entity::Animation *p_animation;

public:
    Block();
    Block(const sf::Vector2f& pos, const sf::Vector2f& size, const std::string& name);
    virtual ~Block() = default;
}; 
