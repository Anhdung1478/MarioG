#include "TileMap.hpp"

using json = nlohmann::json;

namespace mario {
namespace entity {

TileMap::TileMap(){
}

TileMap::TileMap(const std::string &_tilesetPath, const std::string &_mapPath, int _themeID) : tilesetPath(_tilesetPath), mapPath(_mapPath), themeID(_themeID) {
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
    
    /*
    for (const auto& layerJson : mapJson["layers"]) {
        if (layerJson["type"] == "objectgroup" && layerJson["name"] == "Items") {
            for (const auto& obj : layerJson["objects"]) {
                ObjectData objData;
                objData.gid = obj["gid"];
                objData.x = obj["x"];
                objData.y = obj["y"];
                objData.width = obj["width"];
                objData.height = obj["height"];
                objData.name = obj["name"];

                objects.push_back(objData);
            }
            break;
        }
    }
    */
   
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

bool TileMap::loadObjects(std::vector<mario::entity::Enemy*> &enemies, std::vector<mario::entity::Item*> &items, std::vector<mario::entity::Block*>& backgroundBlocks) {
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
                std::string objType = obj["type"];
                float x = obj["x"];
                float y = obj["y"];
                float width = obj["width"];
                float height = obj["height"];
                
                y -= 1 * 16;
                x = x * BLOCK_SCALE.x;
                y = y * BLOCK_SCALE.y;

                if(layerName == "Items"){

                }
                else if(layerName == "Background"){
                    // std::cout << "Object: " << objName << " at (" << x << ", " << y << ") with size (" << width << ", " << height << ")\n";
                    backgroundBlocks.push_back(new BackgroundBlock(sf::Vector2f(x, y), sf::Vector2f(16, 16), objName));
                }
                
                if (objType == "goomba"){
                    enemies.push_back(new mario::entity::Goomba(sf::Vector2f(x, y-100)));
                }
                else if (objType == "red-koopa"){
                    enemies.push_back(new mario::entity::KoopaPatrol(sf::Vector2f(x, y-100), mario::entity::KoopaType::Red, false));
                }
                else if (objType == "red-koopa-fly"){
                    enemies.push_back(new mario::entity::KoopaPatrol(sf::Vector2f(x, y-100), mario::entity::KoopaType::Red, true));
                }
                else if (objType == "green-koopa"){
                    enemies.push_back(new mario::entity::KoopaPatrol(sf::Vector2f(x, y-100), mario::entity::KoopaType::Green, false));
                }
                else if (objType == "green-koopa-fly"){
                    enemies.push_back(new mario::entity::KoopaPatrol(sf::Vector2f(x, y-100), mario::entity::KoopaType::Green, true));
                }
                else if (objType == "piranha-plant"){
                    enemies.push_back(new mario::entity::PiranhaGreen(sf::Vector2f(x, y-100)));
                }
                else if (objType == "lakitu"){
                    // enemies.push_back(new mario::entity::Lakitu(sf::Vector2f(x, y)));
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
            Block* new_block = blockFactory->createBlock(tileId, sf::Vector2f(x * tileWidth, y * tileHeight), themeID);
            if(new_block == nullptr) {
                new_block = blockFactory->createBackgroundBlock(tileId, sf::Vector2f(x * tileWidth, y * tileHeight), themeID, tilesetColumns, tileWidth, tileHeight, margin, spacing);
                
                assert(new_block != nullptr);
                backgroundBlocks.push_back(new_block);
            } else {
                blocks.push_back(new_block);
            }
        }
    }
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

void TileMap::update(const sf::RenderWindow *window, float dt){

}

void TileMap::handleEvent(const sf::RenderWindow *window, const sf::Event &event){

}

void TileMap::render(sf::RenderWindow *window){

}

sf::FloatRect TileMap::getWorldBounds() const {
    return sf::FloatRect(sf::Vector2f(0.f, 0.f), sf::Vector2f(mapWidth * 16 * BLOCK_SCALE.x, mapHeight * 16 * BLOCK_SCALE.y));
}



} // namespace entity
} // namespace mario
