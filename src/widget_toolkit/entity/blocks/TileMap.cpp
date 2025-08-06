#include "TileMap.hpp"

using json = nlohmann::json;

namespace mario {
namespace entity {

TileMap::TileMap(){
}

TileMap::TileMap(const std::string &tilesetPath, const std::string &mapPath, int _themeID) : themeID(_themeID) {
    loadTileset(tilesetPath);
    loadMap(mapPath);

    blockFactory = std::make_unique<mario::entity::BlockFactory>();
}

TileMap::~TileMap(){

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
    
    sprites.clear();
    // Load tile layer
    for (const auto& layerJson : mapJson["layers"]) {
        if (layerJson["type"] == "tilelayer") {
            tileIds = layerJson["data"].get<std::vector<int>>();
            mapWidth = layerJson["width"];
            mapHeight = layerJson["height"];
            break;
        }
    }

    for (int y = 0; y < mapHeight; ++y) {
        for (int x = 0; x < mapWidth; ++x) {
            int tileId = tileIds[y * mapWidth + x];
            
            if (tileId == 0) continue; // Empty tile
            
            // Adjust for 1-based indexing in Tiled
            tileId -= 1;

            // std::cout << "Tile ID: " << tileId << " at (" << x << ", " << y << ")" << std::endl;
            SpriteData2 spriteData;
            spriteData.id = std::to_string(tileId);
            spriteData.x = x * 16 + margin + x * spacing;
            spriteData.y = y * 16 + margin + y * spacing;
            spriteData.z = 16;
            spriteData.t = 16;
            tileSprites[tileId] = spriteData;
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

    //             if (obj.contains("properties")) {
    //                 for (const auto& prop : obj["properties"]) {
    //                     if (prop["name"] == "item_type") {
    //                         objData.itemType = prop["value"];
    //                     } else if (prop["name"] == "trigger_type") {
    //                         objData.triggerType = prop["value"];
    //                     } else if (prop["name"] == "trigger_id") {
    //                         objData.triggerID = prop["value"];
    //                     } 
    //                 }
    //             }

    //             objects.push_back(objData);
    //         }
    //         break;
    //     }
    // }
    
    return true;
}

void TileMap::createBlock(std::vector<Block*> &blocks, std::vector<Block*> &backgroundBlocks) {
    blocks.clear();
    backgroundBlocks.clear();
    for (int y = 0; y < mapHeight; ++y) {
        for (int x = 0; x < mapWidth; ++x) {
            int tileId = tileIds[y * mapWidth + x];
            
            if (tileId == 0) continue; // Empty tile
            
            // Adjust for 1-based indexing in Tiled
            tileId -= 1;

            // Create the block based on the tile type
            std::string tileType = tileProperties[tileId];
            Block* new_block = blockFactory->createBlock(tileId, sf::Vector2f(x * tileWidth, y * tileHeight), themeID);
            if(new_block == nullptr) {
                new_block = blockFactory->createBackgroundBlock(tileId, sf::Vector2f(x * tileWidth, y * tileHeight), themeID, tilesetColumns, tileWidth, tileHeight, margin, spacing);
                
                assert(new_block != nullptr);
                backgroundBlocks.push_back(new_block);
            } else {
                blocks.push_back(new_block);
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
    sortBlocks(blocks);
}

void TileMap::sortBlocks(std::vector<Block*> &blocks) {
    std::sort(blocks.begin(), blocks.end(), [](const Block *a, const Block *b){
        if (a->getPosition().x == b->getPosition().x){
            return a->getPosition().y < b->getPosition().y; // Sort by Y if X is the same
        }
        return a->getPosition().x < b->getPosition().x; // Sort by X first
    });
}

// void TileMap::checkCollisionEn(mario::entity::Enemy* enemy) {
//     int L, R;
//     findBlocksCollisions(L, R, enemy);

//     bool hasTopCollision = false;
//     bool hasBottomCollision = false;
//     bool hasLeftCollision = false;
//     bool hasRightCollision = false;

//     for (int i = 0; i < blocks.size(); ++i) {
//         auto& block = blocks[i];
//         if (!block->isExist()) continue;

//         SideCollision side = findCollisionSide(enemy, block);
//         if (side != SideCollision::None) {
//             switch (side) {
//                 case SideCollision::Top:
//                     hasTopCollision = true;
//                     break;
//                 case SideCollision::Bottom:
//                     hasBottomCollision = true;
//                     break;
//                 case SideCollision::Left:
//                     hasLeftCollision = true;
//                     break;
//                 case SideCollision::Right:
//                     hasRightCollision = true;
//                     break;
//                 default:
//                     break;
//             }
            
//             fixPosition(enemy, block, side);
//         }
//     }

//     sf::Vector2f vel = enemy->getVelocity();
//     if (hasBottomCollision) {
//         vel.y = 0.f;
//         enemy->setOnGround(true); // Đặt trạng thái trên mặt đất
//     }

//     if (hasTopCollision) {
//         vel.y = 0.f;
//     }

//     if (hasLeftCollision || hasRightCollision) {
//         vel.x = 0.f;
//     }

//     enemy->setVelocity(vel);
// }

void TileMap::update(const sf::RenderWindow *window, float dt){

}

void TileMap::handleEvent(const sf::RenderWindow *window, const sf::Event &event){

}

void TileMap::render(sf::RenderWindow *window){

}

sf::FloatRect TileMap::getWorldBounds() const {
    return sf::FloatRect(sf::Vector2f(0.f, 0.f), sf::Vector2f(mapWidth * 16 * BLOCK_SCALE.x, mapHeight * 16 * BLOCK_SCALE.y));
}

// SideCollision TileMap::checkItemCollision(Item* item) {
//     int L, R;
//     //findItemBlockCollisions(L, R, item);
    
//     for (int i = L; i < R; ++i) {
//         auto& block = blocks[i];
//         if (!block->isExist()) continue;
        
//         SideCollision side = findCollisionSide(item, block);
//         if (side != SideCollision::None) {
//             fixPosition(item, block, side);
//             return side;
//         }
//     }
//     return SideCollision::None;
// }

// bool TileMap::hasGroundAt(sf::Vector2f position) {
//     // Check if there's a solid block at the given position
//     // Used for edge detection for walking items
//     for (const auto& block : blocks) {
//         if (!block->isExist()) continue;
        
//         sf::FloatRect blockBounds = block->getHitbox();
//         if (blockBounds.contains(position)) {
//             return true;
//         }
//     }
//     return false;
// }

} // namespace entity
} // namespace mario
