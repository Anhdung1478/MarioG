#pragma once

#include<bits/stdc++.h>
#include "enemy/enemy.hpp"

namespace mario::entity {
    class EnemyFactory {
        public:
            Enemy* create(const std::string &enemyID, sf::Vector2f spawnPoint) {

            }

            ~EnemyFactory() {}
    }; 
}