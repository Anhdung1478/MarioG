#include "TileMap.hpp"

using json = nlohmann::json;

namespace mario {
namespace entity {

TileMap::TileMap(){
}

TileMap::TileMap(const std::string &_tilesetPath, const std::string &_mapPath, int _themeID) : tilesetPath(_tilesetPath), mapPath(_mapPath), themeID(_themeID) {
    loadTileset(tilesetPath);
    loadMap(mapPath);
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
    for (const auto& layerJson : mapJson["layers"]) {
        if (layerJson["type"] == "objectgroup") {
            std::string layerName = layerJson["name"];
            for (const auto& obj : layerJson["objects"]) {
                std::string objName = obj["name"];
                float x = obj["x"];
                float y = obj["y"];
                float width = obj["width"];
                float height = obj["height"];

                if(layerName == "Items"){

                }
                else if(layerName == "Enemies"){

                }
                else if(layerName == "Background"){

                }
            }
            break;
        }
    }
    
    return true;
}

bool TileMap::loadObjects(std::vector<mario::entity::Block*>& backgroundBlocks) {
    std::ifstream file(mapPath);
    if (!file.is_open()) {
        std::cerr << "Failed to open map file: " << mapPath << std::endl;
        return false;
    }
    
    json mapJson;
    file >> mapJson;
    file.close();
    
    // Load objects
    for (const auto& layerJson : mapJson["layers"]) {
        if (layerJson["type"] == "objectgroup") {
            std::string layerName = layerJson["name"];
            // std::cout << "Loading objects from layer: " << layerName << "\n";
            for (const auto& obj : layerJson["objects"]) {
                std::string objName = obj["name"];
                float x = obj["x"];
                float y = obj["y"];
                float width = obj["width"];
                float height = obj["height"];
                
                y -= 1 * 16;
                x = x * BLOCK_SCALE.x;
                y = y * BLOCK_SCALE.y;

                if(layerName == "Items"){

                }
                else if(layerName == "Enemies"){

                }
                else if(layerName == "Background"){
                    // std::cout << "Object: " << objName << " at (" << x << ", " << y << ") with size (" << width << ", " << height << ")\n";
                    backgroundBlocks.push_back(new BackgroundBlock(sf::Vector2f(x, y), sf::Vector2f(16, 16), objName));
                }
            }
            // std::cout << '\n';
        }
    }
    
    return true;
}

void TileMap::createBlock(std::vector<Block*> &blocks, std::vector<Block*> &backgroundBlocks) {
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

                // Question blocks
                case 117:
                    blocks.push_back(new QuestionBlock(sf::Vector2f(x * tileWidth, y * tileHeight), sf::Vector2f(16, 16), "question-block[0]", 0, themeID));
                    break;

                // Brick blocks
                case 195:
                    blocks.push_back(new Brick(sf::Vector2f(x * tileWidth, y * tileHeight), sf::Vector2f(16, 16), "brick-block[0]", -1, themeID));
                    break;
                case 197:
                    blocks.push_back(new Brick(sf::Vector2f(x * tileWidth, y * tileHeight), sf::Vector2f(16, 16), "brick-block[1]", -1, themeID));
                    break;
                case 199:
                    blocks.push_back(new Brick(sf::Vector2f(x * tileWidth, y * tileHeight), sf::Vector2f(16, 16), "brick-block[2]", -1, themeID));
                    break;

                // Background blocks
                case 78:
                    backgroundBlocks.push_back(new BackgroundBlock(sf::Vector2f(x * tileWidth, y * tileHeight), sf::Vector2f(16, 16), "grass[0][0]"));
                    break;
                case 79:
                    backgroundBlocks.push_back(new BackgroundBlock(sf::Vector2f(x * tileWidth, y * tileHeight), sf::Vector2f(16, 16), "grass[1][0]"));
                    break;
                case 80:
                    backgroundBlocks.push_back(new BackgroundBlock(sf::Vector2f(x * tileWidth, y * tileHeight), sf::Vector2f(16, 16), "grass[2][0]"));
                    break;

                default:
                    int _x = tileId % tilesetColumns;
                    int _y = tileId / tilesetColumns;
                    backgroundBlocks.push_back(new BackgroundBlock(sf::Vector2f(x * tileWidth, y * tileHeight), sf::Vector2f(16, 16), std::to_string(tileId), {
                        std::to_string(tileId), 
                        _x * 16 + margin + _x * spacing, 
                        _y * 16 + margin + _y * spacing, 
                        16, 
                        16
                    }));
                    // std::cout << "Unknown tile ID: " << tileId << "with x = " << x << ", y = " << y << " at [(" << _x * 16 + margin + _x * spacing << ", " << _y * 16 + margin + _y * spacing << "), 16x16] with type: " << tileType << std::endl;
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
