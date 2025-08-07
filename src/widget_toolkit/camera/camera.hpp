#pragma once
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cmath>
#include "../entity/entity.hpp"

class Camera {
private:
    sf::View view;
    sf::Vector2u windowSize;
    sf::Vector2f targetPosition;
    float smoothFactor;
    sf::FloatRect bounds;
    sf::FloatRect mapBounds;
    bool hasBounds;
    bool hasMapBounds;
    float cornerThreshold;
    bool isInCornerMode;

public:
    Camera(sf::Vector2u windowSize)
        : windowSize(windowSize), smoothFactor(5.0f), hasBounds(false), hasMapBounds(false),
          cornerThreshold(100.0f), isInCornerMode(false) {
        view.setSize(sf::Vector2f(static_cast<float>(windowSize.x), static_cast<float>(windowSize.y)));
        view.setCenter(sf::Vector2f(static_cast<float>(windowSize.x) / 2.0f, static_cast<float>(windowSize.y) / 2.0f));
        targetPosition = view.getCenter();
    }

    void setTarget(const sf::Vector2f& target) {
        targetPosition = target;
    }

    void setPosition(const sf::Vector2f& position) {
        view.setCenter(position);
        targetPosition = position;
    }

    void move(const sf::Vector2f& offset) {
        view.move(offset);
        targetPosition = view.getCenter();
    }

    void update(float deltaTime) {
        sf::Vector2f currentPos = view.getCenter();
        sf::Vector2f difference = targetPosition - currentPos;
        sf::Vector2f newPosition = currentPos + difference * smoothFactor * deltaTime;

        if (hasBounds) {
            sf::Vector2f viewSize = view.getSize();
            float halfWidth = viewSize.x / 2.0f;
            float halfHeight = viewSize.y / 2.0f;

            newPosition.x = std::max(bounds.position.x + halfWidth,
                                     std::min(bounds.position.x + bounds.size.x - halfWidth, newPosition.x));
            newPosition.y = std::max(bounds.position.y + halfHeight,
                                     std::min(bounds.position.y + bounds.size.y - halfHeight, newPosition.y));
        }

        view.setCenter(newPosition);
    }

    void setSize(const sf::Vector2f& size) {
        view.setSize(size);
    }

    void setZoom(float zoom) {
        sf::Vector2f baseSize(static_cast<float>(windowSize.x), static_cast<float>(windowSize.y));
        view.setSize(sf::Vector2f(baseSize.x * zoom, baseSize.y * zoom));
    }

    void setSmoothFactor(float factor) {
        smoothFactor = factor;
    }

    void setBounds(const sf::FloatRect& cameraBounds) {
        bounds = cameraBounds;
        hasBounds = true;
    }

    void removeBounds() {
        hasBounds = false;
    }

    void setMapBounds(const sf::FloatRect& mb) {
        mapBounds = mb;
        hasMapBounds = true;
    }

    void removeMapBounds() {
        hasMapBounds = false;
    }

    void setCornerThreshold(float threshold) {
        cornerThreshold = threshold;
    }

    void followEntity(const mario::entity::Entity& entity, float deltaTime) {
        sf::Vector2f entityPosition = entity.getPosition();

        if (!hasMapBounds) {
            sf::Vector2f offset(0.0f, -50.0f);
            setTarget(entityPosition + offset);
            update(deltaTime);
            return;
        }

        sf::Vector2f viewSize = view.getSize();
        float halfViewWidth = viewSize.x / 2.0f;
        float halfViewHeight = viewSize.y / 2.0f;

        bool nearLeftEdge = entityPosition.x < (mapBounds.position.x + cornerThreshold);
        bool nearRightEdge = entityPosition.x > (mapBounds.position.x + mapBounds.size.x - cornerThreshold);
        bool nearTopEdge = entityPosition.y < (mapBounds.position.y + cornerThreshold);
        bool nearBottomEdge = entityPosition.y > (mapBounds.position.y + mapBounds.size.y - cornerThreshold);

        sf::Vector2f desiredCameraCenter;

        if (nearLeftEdge && nearTopEdge) {
            desiredCameraCenter = mapBounds.position + sf::Vector2f(halfViewWidth, halfViewHeight);
            isInCornerMode = true;
        }
        else if (nearRightEdge && nearTopEdge) {
            desiredCameraCenter = mapBounds.position + sf::Vector2f(mapBounds.size.x - halfViewWidth, halfViewHeight);
            isInCornerMode = true;
        }
        else if (nearLeftEdge && nearBottomEdge) {
            desiredCameraCenter = mapBounds.position + sf::Vector2f(halfViewWidth, mapBounds.size.y - halfViewHeight);
            isInCornerMode = true;
        }
        else if (nearRightEdge && nearBottomEdge) {
            desiredCameraCenter = mapBounds.position + sf::Vector2f(mapBounds.size.x - halfViewWidth, mapBounds.size.y - halfViewHeight);
            isInCornerMode = true;
        }
        else if (nearLeftEdge) {
            desiredCameraCenter = sf::Vector2f(mapBounds.position.x + halfViewWidth, entityPosition.y - 50.0f);
            isInCornerMode = true;
        }
        else if (nearRightEdge) {
            desiredCameraCenter = sf::Vector2f(mapBounds.position.x + mapBounds.size.x - halfViewWidth, entityPosition.y - 50.0f);
            isInCornerMode = true;
        }
        else if (nearTopEdge) {
            desiredCameraCenter = sf::Vector2f(entityPosition.x, mapBounds.position.y + halfViewHeight);
            isInCornerMode = true;
        }
        else if (nearBottomEdge) {
            desiredCameraCenter = sf::Vector2f(entityPosition.x, mapBounds.position.y + mapBounds.size.y - halfViewHeight);
            isInCornerMode = true;
        }
        else {
            sf::Vector2f offset(0.0f, -50.0f);
            desiredCameraCenter = entityPosition + offset;
            isInCornerMode = false;
        }

        desiredCameraCenter.x = std::max(mapBounds.position.x + halfViewWidth,
                                         std::min(mapBounds.position.x + mapBounds.size.x - halfViewWidth, desiredCameraCenter.x));
        desiredCameraCenter.y = std::max(mapBounds.position.y + halfViewHeight,
                                         std::min(mapBounds.position.y + mapBounds.size.y - halfViewHeight, desiredCameraCenter.y));

        setTarget(desiredCameraCenter);
    }

    template<typename PlayerType>
    void followPlayer(const PlayerType& player, float deltaTime) {
        followEntity(player, deltaTime);
    }

    bool isInCorner() const {
        return isInCornerMode;
    }

    sf::FloatRect getCameraBounds() const {
        sf::Vector2f center = view.getCenter();
        sf::Vector2f size = view.getSize();
        return sf::FloatRect(center - size / 2.0f, size);
    }

    sf::View& getView() {
        return view;
    }

    sf::Vector2f getPosition() const {
        return view.getCenter();
    }

    sf::Vector2f getSize() const {
        return view.getSize();
    }

    void applyTo(sf::RenderWindow& window) {
        window.setView(view);
    }

    sf::Vector2f screenToWorld(const sf::Vector2i& screenPos, const sf::RenderWindow& window) const {
        return window.mapPixelToCoords(screenPos, view);
    }

    sf::Vector2i worldToScreen(const sf::Vector2f& worldPos, const sf::RenderWindow& window) const {
        return window.mapCoordsToPixel(worldPos, view);
    }

    void resetToDefaultView() {
        view.setSize(static_cast<sf::Vector2f>(windowSize));
        view.setCenter(static_cast<sf::Vector2f>(windowSize) / 2.f);
        targetPosition = view.getCenter();
        removeBounds();
        removeMapBounds();
        isInCornerMode = false;
    }
};