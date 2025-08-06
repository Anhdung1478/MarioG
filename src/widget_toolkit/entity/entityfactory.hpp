#pragma once

#include<bits/stdc++.h>
#include "entity.hpp"

namespace mario::entity {
    class EntityFactory { // Factory pattern
        public:
            Entity* create(std::string entityID, sf::Vector2f spawnPoint) {
                
            }

            ~EntityFactory() {}
    };

    class EnemyFactory {
        public:
            
            ~EnemyFactory() {}
    };

    
}