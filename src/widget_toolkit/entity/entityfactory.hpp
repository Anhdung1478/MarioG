#pragma once

#include<bits/stdc++.h>
#include "entity.hpp"

namespace mario::entity {
    class EntityFactory { // Factory pattern
        public:
            Entity* create(std::string entityType, float x, float y) {

            }

            ~EntityFactory() {}
    };
}