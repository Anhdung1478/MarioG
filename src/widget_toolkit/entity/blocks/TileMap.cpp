#include "TileMap.hpp"

using json = nlohmann::json;

namespace mario::entity {

    TileMap::TileMap() {}

    TileMap::TileMap(const std::string &_tilesetPath, const std::string &_mapPath, int _levelState, int _themeID) : tilesetPath(_tilesetPath), mapPath(_mapPath), levelState(_levelState), themeID(_themeID) {
        loadTileset(tilesetPath);
        loadMap(mapPath);

        blockFactory = std::make_unique<mario::entity::BlockFactory>();
    }

    TileMap::~TileMap() {}

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

    bool TileMap::loadObjects(std::vector<mario::entity::Enemy*> &enemies, std::vector<Item*> &items, std::vector<mario::entity::Block*> &blocks, std::vector<mario::entity::Block*>& groundBlocks, std::vector<mario::entity::Block*>& backgroundBlocks) {
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
                    
                    // std::cout << "Object: " << objName << " at (" << x << ", " << y << ") with size (" << width << ", " << height << ")\n";

                    // -1 - None, 0 - coin, 1 - Red-mushroom, 2 - Fire-flower, 3 - One-up-mushroom, 4 - Starman
                    if(layerName == "Items"){
                        if(objName == "coin"){
                            if(objType == "brick"){
                                blocks.push_back(new Brick(sf::Vector2f(x, y), sf::Vector2f(16, 16), "brick-block", 0, themeID, 10));
                            }
                        }
                        if (objName == "red_mushroom"){
                            if(objType == "brick"){
                                blocks.push_back(new Brick(sf::Vector2f(x, y), sf::Vector2f(16, 16), "brick-block", 1, themeID));
                            }
                            if(objType == "question"){
                                blocks.push_back(new QuestionBlock(sf::Vector2f(x, y), sf::Vector2f(16, 16), "question-block[0]", 1, themeID));
                            }
                            if(objType == "invisible"){
                                blocks.push_back(new InvisibleBlock(sf::Vector2f(x, y), sf::Vector2f(16, 16), "invisible-block[0]", 1, themeID));
                            }
                        }
                        else if (objName == "oneup_mushroom"){
                            if(objType == "brick"){
                                blocks.push_back(new Brick(sf::Vector2f(x, y), sf::Vector2f(16, 16), "brick-block", 3, themeID));
                            }
                            if(objType == "question"){
                                blocks.push_back(new QuestionBlock(sf::Vector2f(x, y), sf::Vector2f(16, 16), "question-block[0]", 3, themeID));
                            }
                            if(objType == "invisible"){
                                blocks.push_back(new InvisibleBlock(sf::Vector2f(x, y), sf::Vector2f(16, 16), "invisible-block[0]", 3, themeID));
                            }
                        }
                        else if (objName == "starman"){
                            if(objType == "brick"){
                                // blocks.push_back(new Brick(sf::Vector2f(x, y), sf::Vector2f(16, 16), "brick-block", 4, themeID));
                            }
                            if(objType == "question"){
                                // blocks.push_back(new QuestionBlock(sf::Vector2f(x, y), sf::Vector2f(16, 16), "question-block[0]", 4, themeID));
                            }
                            if(objType == "invisible"){
                                // blocks.push_back(new InvisibleBlock(sf::Vector2f(x, y), sf::Vector2f(16, 16), "invisible-block[0]", 4, themeID));
                            }
                        }
                    }
                    else if(layerName == "Background"){
                        backgroundBlocks.push_back(new BackgroundBlock(sf::Vector2f(x, y), sf::Vector2f(16, 16), objName));
                    }
                    
                    if (objType == "goomba"){
                        enemies.push_back(new mario::entity::Goomba(sf::Vector2f(x, y-100), mario::entity::GoombaType::Brown));
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
                        enemies.push_back(new mario::entity::PiranhaGreen(sf::Vector2f(x, y)));
                    }
                    else if (objType == "lakitu"){
                        enemies.push_back(new mario::entity::Lakitu(sf::Vector2f(x, y)));
                    }
                }
                // std::cout << '\n';
            }
        }
        // posX = x + size / 2 - 8
        // posY = y + 16
        if(levelState == 1){
            // ground
            groundBlocks.push_back(new SolidBlock("../../asset/maps/Image/Map_1.png", sf::Vector2f(0 + 544, 256 + 16), sf::Vector2f(1104, 32), "ground", {"ground", 0, 256, 1104, 32}));
            groundBlocks.push_back(new SolidBlock("../../asset/maps/Image/Map_1.png", sf::Vector2f(1136 + 112, 256 + 16), sf::Vector2f(240, 32), "ground", {"ground", 1136, 256, 240, 32}));
            groundBlocks.push_back(new SolidBlock("../../asset/maps/Image/Map_1.png", sf::Vector2f(1424 + 504, 256 + 16), sf::Vector2f(1024, 32), "ground", {"ground", 1424, 256, 1024, 32}));
            groundBlocks.push_back(new SolidBlock("../../asset/maps/Image/Map_1.png", sf::Vector2f(2480 + 464, 256 + 16), sf::Vector2f(944, 32), "ground", {"ground", 2480, 256, 944, 32}));
            // pipe
            blocks.push_back(new SolidBlock("../../asset/maps/Image/Map_1.png", sf::Vector2f(448 + 8, 224 + 16), sf::Vector2f(32, 32), "pipe", {"pipe", 448, 224, 32, 32}));
            blocks.push_back(new SolidBlock("../../asset/maps/Image/Map_1.png", sf::Vector2f(608 + 8, 208 + 32), sf::Vector2f(32, 48), "pipe", {"pipe", 608, 208, 32, 48}));
            blocks.push_back(new SolidBlock("../../asset/maps/Image/Map_1.png", sf::Vector2f(736 + 8, 192 + 48), sf::Vector2f(32, 64), "pipe", {"pipe", 736, 192, 32, 64}));
            blocks.push_back(new SolidBlock("../../asset/maps/Image/Map_1.png", sf::Vector2f(912 + 8, 192 + 48), sf::Vector2f(32, 64), "pipe", {"pipe", 912, 192, 32, 64}));
            blocks.push_back(new SolidBlock("../../asset/maps/Image/Map_1.png", sf::Vector2f(2608 + 8, 224 + 16), sf::Vector2f(32, 32), "pipe", {"pipe", 2608, 224, 32, 32}));
            blocks.push_back(new SolidBlock("../../asset/maps/Image/Map_1.png", sf::Vector2f(2864 + 8, 224 + 16), sf::Vector2f(32, 32), "pipe", {"pipe", 2864, 224, 32, 32}));
        }
        else if (levelState == 2) {
            // ground
            groundBlocks.push_back(new SolidBlock("../../asset/maps/Image/Map_2.png", sf::Vector2f(0 + 631, 256 + 16), sf::Vector2f(1278, 32), "ground", {"ground", 0, 256, 1278, 32}));
            groundBlocks.push_back(new SolidBlock("../../asset/maps/Image/Map_2.png", sf::Vector2f(1329 + 287, 256 + 16), sf::Vector2f(590, 32), "ground", {"ground", 1329, 256, 590, 32}));
            groundBlocks.push_back(new SolidBlock("../../asset/maps/Image/Map_2.png", sf::Vector2f(1953 + 7, 256 + 16), sf::Vector2f(30, 32), "ground", {"ground", 1953, 256, 30, 32}));
            groundBlocks.push_back(new SolidBlock("../../asset/maps/Image/Map_2.png", sf::Vector2f(2017 + 87, 256 + 16), sf::Vector2f(190, 32), "ground", {"ground", 2017, 256, 190, 32}));
            groundBlocks.push_back(new SolidBlock("../../asset/maps/Image/Map_2.png", sf::Vector2f(2321 + 55, 256 + 16), sf::Vector2f(126, 32), "ground", {"ground", 2321, 256, 126, 32}));
            groundBlocks.push_back(new SolidBlock("../../asset/maps/Image/Map_2.png", sf::Vector2f(2560 + 288, 256 + 16), sf::Vector2f(591, 32), "ground", {"ground", 2560, 256, 591, 32}));
            // pipe
            blocks.push_back(new SolidBlock("../../asset/maps/Image/Map_2.png", sf::Vector2f(1648 + 8, 208 + 32), sf::Vector2f(32, 48), "pipe", {"pipe", 1648, 208, 32, 48}));
            blocks.push_back(new SolidBlock("../../asset/maps/Image/Map_2.png", sf::Vector2f(1744 + 8, 192 + 48), sf::Vector2f(32, 64), "pipe", {"pipe", 1744, 192, 32, 64}));
            blocks.push_back(new SolidBlock("../../asset/maps/Image/Map_2.png", sf::Vector2f(1840 + 8, 224 + 16), sf::Vector2f(32, 32), "pipe", {"pipe", 1840, 224, 32, 32}));
        }
        else if (levelState == 3){
            // ground
            groundBlocks.push_back(new SolidBlock("../../asset/maps/Image/Map_3.png", sf::Vector2f(0 + 736 - 8, 256 + 16), sf::Vector2f(1472, 32), "ground", {"ground", 0, 256, 1472, 32}));
            groundBlocks.push_back(new SolidBlock("../../asset/maps/Image/Map_3.png", sf::Vector2f(1536 + 80 - 8, 256 + 16), sf::Vector2f(160, 32), "ground", {"ground", 1536, 256, 160, 32}));
            groundBlocks.push_back(new SolidBlock("../../asset/maps/Image/Map_3.png", sf::Vector2f(1744 + 240 - 8, 256 + 16), sf::Vector2f(480, 32), "ground", {"ground", 1744, 256, 480, 32}));
            groundBlocks.push_back(new SolidBlock("../../asset/maps/Image/Map_3.png", sf::Vector2f(2272 + 80 - 8, 256 + 16), sf::Vector2f(160, 32), "ground", {"ground", 2272, 256, 160, 32}));
            groundBlocks.push_back(new SolidBlock("../../asset/maps/Image/Map_3.png", sf::Vector2f(2464 + 504 - 8, 256 + 16), sf::Vector2f(1008, 32), "ground", {"ground", 2464, 256, 1008, 32}));
            // pipe
            blocks.push_back(new SolidBlock("../../asset/maps/Image/Map_3.png", sf::Vector2f(736 + 8, 192 + 48), sf::Vector2f(32, 64), "pipe", {"pipe", 736, 192, 32, 64}));
            blocks.push_back(new SolidBlock("../../asset/maps/Image/Map_3.png", sf::Vector2f(1184 + 8, 192 + 48), sf::Vector2f(32, 64), "pipe", {"pipe", 1184, 192, 32, 64}));
            blocks.push_back(new SolidBlock("../../asset/maps/Image/Map_3.png", sf::Vector2f(1648 + 8, 192 + 48), sf::Vector2f(32, 64), "pipe", {"pipe", 1648, 192, 32, 64}));
            blocks.push_back(new SolidBlock("../../asset/maps/Image/Map_3.png", sf::Vector2f(1840 + 8, 224 + 16), sf::Vector2f(32, 32), "pipe", {"pipe", 1840, 224, 32, 32}));
            blocks.push_back(new SolidBlock("../../asset/maps/Image/Map_3.png", sf::Vector2f(1952 + 8, 192 + 48), sf::Vector2f(32, 64), "pipe", {"pipe", 1952, 192, 32, 64}));
            blocks.push_back(new SolidBlock("../../asset/maps/Image/Map_3.png", sf::Vector2f(2016 + 8, 208 + 32), sf::Vector2f(32, 48), "pipe", {"pipe", 2016, 208, 32, 48}));
            blocks.push_back(new SolidBlock("../../asset/maps/Image/Map_3.png", sf::Vector2f(2080 + 8, 176 + 64), sf::Vector2f(32, 80), "pipe", {"pipe", 2080, 176, 32, 80}));
            blocks.push_back(new SolidBlock("../../asset/maps/Image/Map_3.png", sf::Vector2f(2816 + 8, 208 + 32), sf::Vector2f(32, 48), "pipe", {"pipe", 2816, 208, 32, 48}));

        }

        for (int y = 0; y < mapHeight; ++y) {
            for (int x = 0; x < mapWidth; ++x) {
                int tileId = tileIds[y * mapWidth + x];
                
                if (tileId == 0) continue; // Empty tile
                
                // Adjust for 1-based indexing in Tiled
                tileId -= 1;

                if(tileId == 120){ // Coin
                    items.push_back(new mario::entity::Coin(
                                    "../../asset/sprites/coin.json",
                                    "../../asset/maps/Image/tiles-8.png",
                                    sf::Vector2f(2.5f, 2.5f),
                                    "coin[0]",
                                    sf::Vector2f(x * tileWidth, y * tileHeight),
                                    sf::Vector2f(16.f, 16.f)
                                ));
                }

                // Create the block based on the tile type
                std::string tileType = tileProperties[tileId];
                Block* new_block = blockFactory->createBlock(tileId, sf::Vector2f(x * tileWidth, y * tileHeight), themeID);
                if(new_block == nullptr) {
                    new_block = blockFactory->createBackgroundBlock(tileId, sf::Vector2f(x * tileWidth, y * tileHeight), themeID, tilesetColumns, tileWidth, tileHeight, margin, spacing);
                    
                    if(new_block) backgroundBlocks.push_back(new_block);
                } else {
                    blocks.push_back(new_block);
                }
            }
        }
        sortBlocks(blocks);
        
        return true;
    }

    void TileMap::createBlock(std::vector<Block*> &blocks, std::vector<Block*> &backgroundBlocks) {
        
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
        std::cout << "Map Width: " << mapWidth << ", Map Height: " << mapHeight << "\n";
        return sf::FloatRect(sf::Vector2f(0.f, 0.f), sf::Vector2f((mapWidth - 1) * 16 * BLOCK_SCALE.x, (mapHeight - 1) * 16 * BLOCK_SCALE.y));
    }

} // namespace mario::entity
