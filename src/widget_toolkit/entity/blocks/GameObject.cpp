#include "GameObject.hpp"

GameObject::GameObject(){};

GameObject::GameObject(const sf::Vector2f& pos, const sf::Vector2f& size)
    : position(pos), size(size) {}

sf::Vector2f GameObject::getPosition() { 
    return position; 
}

sf::Vector2f GameObject::getSize() { 
    return size; 
}

sf::Vector2f GameObject::getCenter() {
    return sf::Vector2f(position.x + size.x / 2, position.y + size.y / 2);
}

void GameObject::setSize(const sf::Vector2f& newSize) {
    size = newSize;
}

void GameObject::setPosition(const float x, const float y) {
    position.x = x;
    position.y = y;
}

void GameObject::setPosition(const sf::Vector2f& newPos) {
    position = newPos;
}