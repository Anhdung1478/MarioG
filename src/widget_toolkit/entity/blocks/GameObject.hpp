#pragma once
#include <SFML/Graphics.hpp>


class GameObject {
protected:
    sf::Vector2f position;
    sf::Vector2f size;

public:
    GameObject();
    GameObject(const sf::Vector2f& pos, const sf::Vector2f& size);
    virtual ~GameObject(){};

    sf::Vector2f getPosition();
    sf::Vector2f getSize();
    sf::Vector2f getCenter();
    void setSize(const sf::Vector2f& newSize);
    virtual void setPosition(const float x, const float y);
    virtual void setPosition(const sf::Vector2f& newPos);
};