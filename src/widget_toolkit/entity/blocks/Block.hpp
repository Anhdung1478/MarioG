#pragma once

#include "../entity.hpp"
#include "../../interfaces.hpp"
#include "../player/player.hpp"

enum SideCollision {
    Top    = 0,
    Bottom = 1,
    Left   = 2,
    Right  = 3,
    None   = 4
};

struct Collision {
    enum class Type { Player, Wall, Bullet, Brick };
    Type type;
    Collision(Type t) : type(t) {}
    bool isWithPlayer() const { return type == Type::Player; }
    bool isWithWall() const { return type == Type::Wall; }
    bool isWithBullet() const { return type == Type::Bullet; }
    bool isWithBrick() const { return type == Type::Brick; }
};

namespace mario {
    namespace entity {

    class Player;
    class ItemManager;

    inline const sf::Vector2f BLOCK_SCALE = sf::Vector2f(2.5, 2.5);

    class Block : public mario::entity::Entity {
    protected:
        std::string name;
        bool exist;
        bool shouldBeDeleted = false;
        int blockID;

    public:
        Block();
        Block(sf::Vector2f pos, sf::Vector2f size, const std::string &name);
        virtual ~Block() = default;
        
        virtual void InitSpritesSheet() = 0;
        virtual int reactToCollision(int side, Player* player = nullptr) = 0;
    
        virtual void onHit(Player* player, ItemManager* itemManager);
        int getBlockID() const { return blockID; }
        void setBlockID(int id) { blockID = id; }
        bool shouldDelete() const { return shouldBeDeleted; }
    };

    } // namespace entity
} // namespace mario