#pragma once
#include "Block.hpp"

namespace mario::entity {

    struct BrickFragment;

    class Brick : public Block {
    public:
        Brick(){};
        Brick(sf::Vector2f pos, sf::Vector2f size, std::string name, int _typeOfItem = -1, int _themeID = 0, int _numberOfCoins = 0);
        ~Brick() = default;
        void InitSpritesSheet() override;
        int reactToCollision(int side, Player* player = nullptr) override;

        void update(const sf::RenderWindow *window, float dt) override;
        void handleEvent(const sf::RenderWindow *window, const sf::Event &event) override;
        void render(sf::RenderWindow *window) override;
    private:
        

        // mario::entity::Animation *fragment_animation;
        int typeOfItem = -1; // -2: empty block , -1: None, 0: coin, 1: Red-mushroom, 2: Fire-flower, 3: One-up-mushroom, 4: Starman
        int themeID = 0;
        int numberOfCoins = 0;
        mario::entity::Animation *coins_animation;
        std::vector<SpriteData2> sprites;

        std::vector<mario::entity::BrickFragment> fragments;
        void createFragments();

        sf::Vector2f originalPosition;
        bool isBouncing = false;
        float bouncingTimer = 0.0f;
        float bouncingDistance = 0.0f;
        void bouncingAnimation(float dt);
    };

} // namespace mario::entity


struct mario::entity::BrickFragment {
    sf::Sprite sprite;
    sf::Vector2f velocity;
    float rotation;
    float rotationSpeed;

    BrickFragment() = default;
    BrickFragment(const sf::Vector2f& position, const sf::Texture& texture, sf::IntRect textureRect)
        : sprite(texture) {
        sprite.setTextureRect(textureRect);

        // Position the fragment with wider spread
        sprite.setPosition(position);

        // Increased spread velocity
        // Horizontal spread: much wider range
        // Random horizontal velocity between -400 and 400
        velocity.x = static_cast<float>(rand() % 800 - 400);  // Wider horizontal spread

        // Vertical spread: more dramatic initial upward movement
        velocity.y = -(200.0f + (rand() % 300));  // Upward initial movement

        // Initial rotation with more variation
        rotation = 0.0f;
        rotationSpeed = (rand() % 720 - 360);  // Faster, more dramatic rotation
    }

    void update(float dt) {
        sprite.move(velocity * dt);

        // Rotate the sprite
        rotation += rotationSpeed * dt;
        sprite.setRotation(sf::degrees(rotation));

        // More aggressive gravity simulation
        velocity.y += 980.0f * dt;  // Increased gravity effect

        // Optional: Slow down horizontal movement
        velocity.x *= 0.95f;
    }
};