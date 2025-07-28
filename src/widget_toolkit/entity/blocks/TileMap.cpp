#include "TileMap.hpp"

using json = nlohmann::json;

TileMap::TileMap(){
}

TileMap::TileMap(const std::string &tilesetPath, const std::string &mapPath) {
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
    // tileWidth = tilesetJson["tilewidth"];
    // tileHeight = tilesetJson["tileheight"];
    tileWidth = 40;
    tileHeight = 40;
    
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

void TileMap::createBlock(std::vector<std::unique_ptr<Block>> &Blocks) {
    Blocks.clear();

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
                    Blocks.push_back(std::make_unique<SolidBlock>(sf::Vector2f(x * tileWidth, y * tileHeight), sf::Vector2f(16, 16), "grass-ground[0]"));
                    break;
                case 1:
                    Blocks.push_back(std::make_unique<SolidBlock>(sf::Vector2f(x * tileWidth, y * tileHeight), sf::Vector2f(16, 16), "grass-ground[1]"));
                    break;
                case 2:
                    Blocks.push_back(std::make_unique<SolidBlock>(sf::Vector2f(x * tileWidth, y * tileHeight), sf::Vector2f(16, 16), "grass-ground[2]"));
                    break;
                case 39:
                    Blocks.push_back(std::make_unique<SolidBlock>(sf::Vector2f(x * tileWidth, y * tileHeight), sf::Vector2f(16, 16), "grass-ground[3]"));
                    break;
                case 40:
                    Blocks.push_back(std::make_unique<SolidBlock>(sf::Vector2f(x * tileWidth, y * tileHeight), sf::Vector2f(16, 16), "grass-ground[4]"));
                    break;
                case 41:
                    Blocks.push_back(std::make_unique<SolidBlock>(sf::Vector2f(x * tileWidth, y * tileHeight), sf::Vector2f(16, 16), "grass-ground[5]"));
                    break;
                    
                // Stone ground tiles
                case 4:
                    Blocks.push_back(std::make_unique<SolidBlock>(sf::Vector2f(x * tileWidth, y * tileHeight), sf::Vector2f(16, 16), "stone-ground[0]"));
                    break;
                case 5:
                    Blocks.push_back(std::make_unique<SolidBlock>(sf::Vector2f(x * tileWidth, y * tileHeight), sf::Vector2f(16, 16), "stone-ground[1]"));
                    break;
                case 6:
                    Blocks.push_back(std::make_unique<SolidBlock>(sf::Vector2f(x * tileWidth, y * tileHeight), sf::Vector2f(16, 16), "stone-ground[2]"));
                    break;
                case 43:
                    Blocks.push_back(std::make_unique<SolidBlock>(sf::Vector2f(x * tileWidth, y * tileHeight), sf::Vector2f(16, 16), "stone-ground[3]"));
                    break;
                case 44:
                    Blocks.push_back(std::make_unique<SolidBlock>(sf::Vector2f(x * tileWidth, y * tileHeight), sf::Vector2f(16, 16), "stone-ground[4]"));
                    break;
                case 45:
                    Blocks.push_back(std::make_unique<SolidBlock>(sf::Vector2f(x * tileWidth, y * tileHeight), sf::Vector2f(16, 16), "stone-ground[5]"));
                    break;
                
                // Snow ground tiles
                case 8:
                    Blocks.push_back(std::make_unique<SolidBlock>(sf::Vector2f(x * tileWidth, y * tileHeight), sf::Vector2f(16, 16), "snow-ground[0]"));
                    break;
                case 9:
                    Blocks.push_back(std::make_unique<SolidBlock>(sf::Vector2f(x * tileWidth, y * tileHeight), sf::Vector2f(16, 16), "snow-ground[1]"));
                    break;
                case 10:
                    Blocks.push_back(std::make_unique<SolidBlock>(sf::Vector2f(x * tileWidth, y * tileHeight), sf::Vector2f(16, 16), "snow-ground[2]"));
                    break;
                case 47:
                    Blocks.push_back(std::make_unique<SolidBlock>(sf::Vector2f(x * tileWidth, y * tileHeight), sf::Vector2f(16, 16), "snow-ground[3]"));
                    break;
                case 48:
                    Blocks.push_back(std::make_unique<SolidBlock>(sf::Vector2f(x * tileWidth, y * tileHeight), sf::Vector2f(16, 16), "snow-ground[4]"));
                    break;
                case 49:
                    Blocks.push_back(std::make_unique<SolidBlock>(sf::Vector2f(x * tileWidth, y * tileHeight), sf::Vector2f(16, 16), "snow-ground[5]"));
                    break;

                // Green pipe tiles
                case 123:
                    Blocks.push_back(std::make_unique<SolidBlock>(sf::Vector2f(x * tileWidth, y * tileHeight), sf::Vector2f(16, 16), "green-pipe[0]"));
                    break;
                case 124:
                    Blocks.push_back(std::make_unique<SolidBlock>(sf::Vector2f(x * tileWidth, y * tileHeight), sf::Vector2f(16, 16), "green-pipe[1]"));
                    break;
                case 162:
                    Blocks.push_back(std::make_unique<SolidBlock>(sf::Vector2f(x * tileWidth, y * tileHeight), sf::Vector2f(16, 16), "green-pipe[2]"));
                    break;
                case 163:
                    Blocks.push_back(std::make_unique<SolidBlock>(sf::Vector2f(x * tileWidth, y * tileHeight), sf::Vector2f(16, 16), "green-pipe[3]"));
                    break;
                
                // Gray pipe tiles
                case 128:
                    Blocks.push_back(std::make_unique<SolidBlock>(sf::Vector2f(x * tileWidth, y * tileHeight), sf::Vector2f(16, 16), "gray-pipe[0]"));
                    break;
                case 129:
                    Blocks.push_back(std::make_unique<SolidBlock>(sf::Vector2f(x * tileWidth, y * tileHeight), sf::Vector2f(16, 16), "gray-pipe[1]"));
                    break;
                case 167:
                    Blocks.push_back(std::make_unique<SolidBlock>(sf::Vector2f(x * tileWidth, y * tileHeight), sf::Vector2f(16, 16), "gray-pipe[2]"));
                    break;
                case 168:
                    Blocks.push_back(std::make_unique<SolidBlock>(sf::Vector2f(x * tileWidth, y * tileHeight), sf::Vector2f(16, 16), "gray-pipe[3]"));
                    break;

                // Stair blocks
                case 351:
                    Blocks.push_back(std::make_unique<SolidBlock>(sf::Vector2f(x * tileWidth, y * tileHeight), sf::Vector2f(16, 16), "stair-block[0]"));
                    break;
                case 352:
                    Blocks.push_back(std::make_unique<SolidBlock>(sf::Vector2f(x * tileWidth, y * tileHeight), sf::Vector2f(16, 16), "stair-block[1]"));
                    break;
                case 353:
                    Blocks.push_back(std::make_unique<SolidBlock>(sf::Vector2f(x * tileWidth, y * tileHeight), sf::Vector2f(16, 16), "stair-block[2]"));
                    break;

                default:
                    break;
            }


            //     block = std::make_unique<Brick>(x * tileWidth, y * tileHeight);
            // else if (tileType == "Question") 
            //     block = std::make_unique<QuestionBlock>(x * tileWidth, y * tileHeight);
            // else if (tileType == "Solid")
            //     block = std::make_unique<SolidBlock>(x * tileWidth, y * tileHeight);

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