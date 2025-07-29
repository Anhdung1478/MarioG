#include "TileMap.hpp"

using json = nlohmann::json;

TileMap::TileMap(){
}

TileMap::TileMap(const std::string &tilesetPath, const std::string &mapPath) {
    loadTileset(tilesetPath);
    loadMap(mapPath);
    createBlock();
}

TileMap::~TileMap(){
    for (Block *block : blocks) {
        delete block;
    }
}

bool TileMap::loadTileset(const std::string& tilesetPath) {
    // Load tileset properties from tiles-8.json
    std::ifstream file(tilesetPath);
    if (!file.is_open()) {
        std::cerr << "Failed to open tileset file: " << tilesetPath << std::endl;
        return false;
    }
    
    json tilesetJson;
    file >> tilesetJson;
    file.close();
    
    // Load texture
    std::string imagePath = tilesetJson["image"];
    // Fix the path - remove "Image\/" prefix and add relative path
    if (imagePath.find("Image/") == 0) {
        imagePath = imagePath.substr(6); // Remove "Image/" prefix
    }
    imagePath = "../../asset/maps/Image/" + imagePath; // Add relative path
    
    if (!tilesetTexture.loadFromFile(imagePath)) {
        std::cerr << "Failed to load tileset texture: " << imagePath << std::endl;
        return false;
    }
    
    // Parse tile properties
    if (tilesetJson.contains("tiles")) {
        for (const auto& tile : tilesetJson["tiles"]) {
            int id = tile["id"];
            std::string type = "";
            
            if (tile.contains("properties")) {
                for (const auto& prop : tile["properties"]) {
                    if (prop["name"] == "Type") {
                        type = prop["value"];
                        break;
                    }
                }
            }
            
            tileProperties[id] = type;
        }
    }
    
    tilesetColumns = tilesetJson["columns"];
    tileWidth = tilesetJson["tilewidth"];
    tileHeight = tilesetJson["tileheight"];
    tileWidth *= BLOCK_SCALE.x;
    tileHeight *= BLOCK_SCALE.y;

    // Load margin and spacing if present
    if (tilesetJson.contains("margin")) {
        margin = tilesetJson["margin"];
    }
    if (tilesetJson.contains("spacing")) {
        spacing = tilesetJson["spacing"];
    }
    
    return true;
}

bool TileMap::loadMap(const std::string& mapPath) {
    std::ifstream file(mapPath);
    if (!file.is_open()) {
        std::cerr << "Failed to open map file: " << mapPath << std::endl;
        return false;
    }
    
    json mapJson;
    file >> mapJson;
    file.close();
    
    // Load tile layer
    for (const auto& layerJson : mapJson["layers"]) {
        if (layerJson["type"] == "tilelayer") {
            tileIds = layerJson["data"].get<std::vector<int>>();
            mapWidth = layerJson["width"];
            mapHeight = layerJson["height"];
            break;
        }
    }
    
    // Load objects
    // for (const auto& layerJson : mapJson["layers"]) {
    //     if (layerJson["type"] == "objectgroup") {
    //         for (const auto& obj : layerJson["objects"]) {
    //             ObjectData objData;
    //             objData.gid = obj["gid"];
    //             objData.x = obj["x"];
    //             objData.y = obj["y"];
    //             objData.width = obj["width"];
    //             objData.height = obj["height"];
    //             objects.push_back(objData);
    //         }
    //         break;
    //     }
    // }
    
    return true;
}

void TileMap::createBlock() {
    blocks.clear();
    for (int y = 0; y < mapHeight; ++y) {
        for (int x = 0; x < mapWidth; ++x) {
            int tileId = tileIds[y * mapWidth + x];
            
            if (tileId == 0) continue; // Empty tile
            
            // Adjust for 1-based indexing in Tiled
            tileId -= 1;

            // Create the block based on the tile type
            std::string tileType = tileProperties[tileId];            
            switch (tileId) {
                // Grass ground tiles
                case 0:
                    blocks.push_back(new SolidBlock(sf::Vector2f(x * tileWidth, y * tileHeight), sf::Vector2f(16, 16), "grass-ground[0]"));
                    break;
                case 1:
                    blocks.push_back(new SolidBlock(sf::Vector2f(x * tileWidth, y * tileHeight), sf::Vector2f(16, 16), "grass-ground[1]"));
                    break;
                case 2:
                    blocks.push_back(new SolidBlock(sf::Vector2f(x * tileWidth, y * tileHeight), sf::Vector2f(16, 16), "grass-ground[2]"));
                    break;
                case 39:
                    blocks.push_back(new SolidBlock(sf::Vector2f(x * tileWidth, y * tileHeight), sf::Vector2f(16, 16), "grass-ground[3]"));
                    break;
                case 40:
                    blocks.push_back(new SolidBlock(sf::Vector2f(x * tileWidth, y * tileHeight), sf::Vector2f(16, 16), "grass-ground[4]"));
                    break;
                case 41:
                    blocks.push_back(new SolidBlock(sf::Vector2f(x * tileWidth, y * tileHeight), sf::Vector2f(16, 16), "grass-ground[5]"));
                    break;
                    
                // Stone ground tiles
                case 4:
                    blocks.push_back(new SolidBlock(sf::Vector2f(x * tileWidth, y * tileHeight), sf::Vector2f(16, 16), "stone-ground[0]"));
                    break;
                case 5:
                    blocks.push_back(new SolidBlock(sf::Vector2f(x * tileWidth, y * tileHeight), sf::Vector2f(16, 16), "stone-ground[1]"));
                    break;
                case 6:
                    blocks.push_back(new SolidBlock(sf::Vector2f(x * tileWidth, y * tileHeight), sf::Vector2f(16, 16), "stone-ground[2]"));
                    break;
                case 43:
                    blocks.push_back(new SolidBlock(sf::Vector2f(x * tileWidth, y * tileHeight), sf::Vector2f(16, 16), "stone-ground[3]"));
                    break;
                case 44:
                    blocks.push_back(new SolidBlock(sf::Vector2f(x * tileWidth, y * tileHeight), sf::Vector2f(16, 16), "stone-ground[4]"));
                    break;
                case 45:
                    blocks.push_back(new SolidBlock(sf::Vector2f(x * tileWidth, y * tileHeight), sf::Vector2f(16, 16), "stone-ground[5]"));
                    break;
                
                // Snow ground tiles
                case 8:
                    blocks.push_back(new SolidBlock(sf::Vector2f(x * tileWidth, y * tileHeight), sf::Vector2f(16, 16), "snow-ground[0]"));
                    break;
                case 9:
                    blocks.push_back(new SolidBlock(sf::Vector2f(x * tileWidth, y * tileHeight), sf::Vector2f(16, 16), "snow-ground[1]"));
                    break;
                case 10:
                    blocks.push_back(new SolidBlock(sf::Vector2f(x * tileWidth, y * tileHeight), sf::Vector2f(16, 16), "snow-ground[2]"));
                    break;
                case 47:
                    blocks.push_back(new SolidBlock(sf::Vector2f(x * tileWidth, y * tileHeight), sf::Vector2f(16, 16), "snow-ground[3]"));
                    break;
                case 48:
                    blocks.push_back(new SolidBlock(sf::Vector2f(x * tileWidth, y * tileHeight), sf::Vector2f(16, 16), "snow-ground[4]"));
                    break;
                case 49:
                    blocks.push_back(new SolidBlock(sf::Vector2f(x * tileWidth, y * tileHeight), sf::Vector2f(16, 16), "snow-ground[5]"));
                    break;

                // Green pipe tiles
                case 123:
                    blocks.push_back(new SolidBlock(sf::Vector2f(x * tileWidth, y * tileHeight), sf::Vector2f(16, 16), "green-pipe[0]"));
                    break;
                case 124:
                    blocks.push_back(new SolidBlock(sf::Vector2f(x * tileWidth, y * tileHeight), sf::Vector2f(16, 16), "green-pipe[1]"));
                    break;
                case 162:
                    blocks.push_back(new SolidBlock(sf::Vector2f(x * tileWidth, y * tileHeight), sf::Vector2f(16, 16), "green-pipe[2]"));
                    break;
                case 163:
                    blocks.push_back(new SolidBlock(sf::Vector2f(x * tileWidth, y * tileHeight), sf::Vector2f(16, 16), "green-pipe[3]"));
                    break;
                
                // Gray pipe tiles
                case 128:
                    blocks.push_back(new SolidBlock(sf::Vector2f(x * tileWidth, y * tileHeight), sf::Vector2f(16, 16), "gray-pipe[0]"));
                    break;
                case 129:
                    blocks.push_back(new SolidBlock(sf::Vector2f(x * tileWidth, y * tileHeight), sf::Vector2f(16, 16), "gray-pipe[1]"));
                    break;
                case 167:
                    blocks.push_back(new SolidBlock(sf::Vector2f(x * tileWidth, y * tileHeight), sf::Vector2f(16, 16), "gray-pipe[2]"));
                    break;
                case 168:
                    blocks.push_back(new SolidBlock(sf::Vector2f(x * tileWidth, y * tileHeight), sf::Vector2f(16, 16), "gray-pipe[3]"));
                    break;

                // Stair blocks
                case 351:
                    blocks.push_back(new SolidBlock(sf::Vector2f(x * tileWidth, y * tileHeight), sf::Vector2f(16, 16), "stair-block[0]"));
                    break;
                case 352:
                    blocks.push_back(new SolidBlock(sf::Vector2f(x * tileWidth, y * tileHeight), sf::Vector2f(16, 16), "stair-block[1]"));
                    break;
                case 353:
                    blocks.push_back(new SolidBlock(sf::Vector2f(x * tileWidth, y * tileHeight), sf::Vector2f(16, 16), "stair-block[2]"));
                    break;

                default:
                    break;
            }


            //     block = std::make_unique<Brick>(x * tileWidth, y * tileHeight);
            // else if (tileType == "Question") 
            //     block = std::make_unique<QuestionBlock>(x * tileWidth, y * tileHeight);
            // else if (tileType == "Solid")
            //     block = new SolidBlock(x * tileWidth, y * tileHeight);

            // if (block) {
            //     blocks.push_back(std::move(block));
            // }
            
            // // Calculate source rectangle in tileset with margin and spacing
            // int sourceX = margin + (tileId % tilesetColumns) * (tileWidth + spacing);
            // int sourceY = margin + (tileId / tilesetColumns) * (tileHeight + spacing);
            
            // sf::IntRect textureRect(sf::Vector2i(sourceX, sourceY), sf::Vector2i(tileWidth, tileHeight));
            // sf::Sprite sprite(tilesetTexture, textureRect);
            
            // // Scale the sprite from 16x16 to 32x32
            // float scaleX = static_cast<float>(scaledTileWidth) / tileWidth;
            // float scaleY = static_cast<float>(scaledTileHeight) / tileHeight;
            // sprite.setScale(sf::Vector2f(scaleX, scaleY));
            
            // // Position the sprite
            // sprite.setPosition(sf::Vector2f(x * scaledTileWidth, y * scaledTileHeight));
            
            // tiles.push_back(sprite);
        }
    }
    
    // Create sprites for objects
    // for (const auto& obj : objects) {
    //     int tileId = obj.gid - 1; // Adjust for 1-based indexing
        
    //     int sourceX = margin + (tileId % tilesetColumns) * (tileWidth + spacing);
    //     int sourceY = margin + (tileId / tilesetColumns) * (tileHeight + spacing);
        
    //     sf::IntRect textureRect(sf::Vector2i(sourceX, sourceY), sf::Vector2i(tileWidth, tileHeight));
    //     sf::Sprite sprite(tilesetTexture, textureRect);
        
    //     // Scale the sprite
    //     float scaleX = static_cast<float>(scaledTileWidth) / tileWidth;
    //     float scaleY = static_cast<float>(scaledTileHeight) / tileHeight;
    //     sprite.setScale(sf::Vector2f(scaleX, scaleY));
        
    //     // Position the object (note: Tiled Y coordinates need adjustment)
    //     sprite.setPosition(sf::Vector2f(obj.x * scaleX, (obj.y - obj.height) * scaleY));
        
    //     tiles.push_back(sprite);
    // }
}

void TileMap::sortBlocks() {
    std::sort(blocks.begin(), blocks.end(), [](const Block *a, const Block *b){
        if (a->getPosition().x == b->getPosition().x){
            return a->getPosition().y < b->getPosition().y; // Sort by Y if X is the same
        }
        return a->getPosition().x < b->getPosition().x; // Sort by X first
    });
}

void TileMap::findBlocksCollisions(int &L, int &R, const mario::entity::Entity *entity){
    //using lower_bound and upper_bound to find the range of blocks that might collide with the entity
    auto itL = std::lower_bound(blocks.begin(), blocks.end(), entity->getPosition().x - entity->getSize().x / 2, 
        [](const Block *block, float posX) {
            return block->getPosition().x < posX;
        });
    auto itR = std::upper_bound(blocks.begin(), blocks.end(), entity->getPosition().x + entity->getSize().x / 2, 
        [](float posX, const Block *block) {
            return posX < block->getPosition().x;
        });

    // Set the collision bounds
    L = std::distance(blocks.begin(), itL);
    R = std::distance(blocks.begin(), itR);
}

SideCollision TileMap::findCollisionSide(const mario::entity::Entity *entityA, const mario::entity::Entity *entityB) {
    sf::FloatRect hitBoxA = entityA->getHitbox();
    sf::FloatRect hitBoxB = entityB->getHitbox();

    sf::Vector2f centerA = hitBoxA.position + hitBoxA.size / 2.f;
    sf::Vector2f centerB = hitBoxB.position + hitBoxB.size / 2.f;

    float deltaX = centerB.x - centerA.x;
    float deltaY = centerB.y - centerA.y;

    float overlapX = (hitBoxA.size.x + hitBoxB.size.x) / 2.f - std::abs(deltaX);
    float overlapY = (hitBoxA.size.y + hitBoxB.size.y) / 2.f - std::abs(deltaY);

    if (overlapX >= 0 && overlapY >= 0) {
        if (overlapX >= overlapY) {
            return (deltaY > 0) ? SideCollision::Bottom : SideCollision::Top;
        } 
        else {
            return (deltaX < 0) ? SideCollision::Left : SideCollision::Right;
        }
    }
    return SideCollision::None; // No collision
}

void TileMap::fixPosition(mario::entity::Entity *entity, const Block *block, SideCollision side) {
    if(side == SideCollision::None) return;
    switch (side) {
        case SideCollision::Top:
            entity->setPosition(sf::Vector2f(entity->getPosition().x, block->getPosition().y + entity->getSize().y));
            break;
        case SideCollision::Bottom:
            entity->setPosition(sf::Vector2f(entity->getPosition().x, block->getHitbox().position.y));
            break;
        case SideCollision::Left:
            entity->setPosition(sf::Vector2f(block->getHitbox().position.x + block->getSize().x + entity->getSize().x / 2.0f, entity->getPosition().y));
            break;
        case SideCollision::Right:
            entity->setPosition(sf::Vector2f(block->getHitbox().position.x - entity->getSize().x / 2.0f, entity->getPosition().y));
            break;
    }
}

void TileMap::checkCollision(mario::entity::Player *player) {
    int L, R;
    findBlocksCollisions(L, R, player);

    //player->setMoveLeft(true); 
    //player->setMoveRight(true);

    bool hasTopCollision = false;
    bool hasBottomCollision = false;
    bool hasLeftCollision = false;
    bool hasRightCollision = false;

    for(int i = 0; i < blocks.size(); ++i){
        auto& block = blocks[i];
        if (!block->getExist()) continue;

        SideCollision side = findCollisionSide(player, block);
        if(side != SideCollision::None) {
            switch (side) {
                case SideCollision::Top:
                    hasTopCollision = true;
                    break;
                case SideCollision::Bottom:
                    hasBottomCollision = true;
                    break;
                case SideCollision::Left:
                    hasLeftCollision = true;
                    break;
                case SideCollision::Right:
                    hasRightCollision = true;
                    break;
                default:
                    break;
            }
            
            fixPosition(player, block, side);
        }
    }

    sf::Vector2f vel = player->getVelocity();
    if(hasBottomCollision) {
        vel.y = 0.f;
        player->resetJump();
        player->setOnGround(true);
        std::cerr << "\nBottom Collision Detected\n";
    }

    if(hasTopCollision) {
        vel.y = 0.f;
        std::cerr << "\nTop Collision Detected\n";
    }

    if(hasLeftCollision) {
        vel.x = 0.f;
        //player->setMoveLeft(false);
        std::cerr << "\nLeft Collision Detected\n";
    }

    if(hasRightCollision) {
        vel.x = 0.f;
        //player->setMoveRight(false);
        std::cerr << "\nRight Collision Detected\n";
    }

    player->setVelocity(vel);
}


void TileMap::update(const sf::RenderWindow *window, float dt){
    for (const auto& block : blocks){
        block->update(window, dt);
    }
}

void TileMap::handleEvent(const sf::RenderWindow *window, const sf::Event &event){
    for (const auto& block : blocks){
        block->handleEvent(window, event);
    }
}

void TileMap::render(sf::RenderWindow *window){
    for (const auto& block : blocks){
        block->render(window);
    }
}

sf::FloatRect TileMap::getWorldBounds() const {
    return sf::FloatRect(sf::Vector2f(0.f, 0.f), sf::Vector2f(1280.f, 720.f));
}