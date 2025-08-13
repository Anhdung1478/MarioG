#pragma once

#include "Block.hpp"

namespace mario::entity {

    class FireWorks : public Entity {
    private:
        struct FireworksData {
            sf::Vector2f position;
            sf::Vector2f tempPos;
            sf::Vector2f size;
            sf::Sprite sprite;
            std::vector<sf::IntRect> frames;
            int ID;
            int frameID;
            float animationTimer = 0.0f;
            float animationDuration = 0.1f;
            float timeStart = 0.0f;

            FireworksData(int _ID, const sf::Vector2f &pos, const sf::Vector2f &size, const sf::Texture& texture)
                : ID(_ID), position(pos), size(size), sprite(texture) 
            {
                loadFrames(ID);
                frameID = 0;
                tempPos = position + sf::Vector2f(rand() % (int)size.x, rand() % (int)size.y);

                sprite.setTextureRect(frames[frameID]);
                sprite.setPosition(tempPos);
                sprite.setScale(sf::Vector2f(2.5f, 2.5f));
                // Random float between 0 and 2
                timeStart = rand() % 20000 / 10000.f;
                animationTimer = 0.0f;
            }

            void loadFrames(int ID){
                if(ID == 0){
                    frames.push_back({{2, 76}, {18, 18}});
                    frames.push_back({{26, 71}, {28, 28}});
                    frames.push_back({{64, 69}, {32, 32}});
                    frames.push_back({{104, 69}, {32, 32}});
                }
                else if(ID == 1){
                    frames.push_back({{2, 110}, {18, 18}});
                    frames.push_back({{26, 105}, {28, 28}});
                    frames.push_back({{64, 103}, {32, 32}});
                    frames.push_back({{104, 103}, {32, 32}});
                }
                else if(ID == 2){
                    frames.push_back({{2, 143}, {18, 18}});
                    frames.push_back({{26, 138}, {28, 28}});
                    frames.push_back({{64, 136}, {32, 32}});
                    frames.push_back({{104, 136}, {32, 32}});
                }
            }

            void update(float dt){
                if(frameID >= 4){
                    frameID = -1;
                    timeStart = rand() % 20000 / 10000.f;
                    tempPos = position + sf::Vector2f(rand() % (int)size.x, rand() % (int)size.y);
                    sprite.setPosition(tempPos);
                }                
                animationTimer += dt;
                if(animationTimer < timeStart) return;
                timeStart = -1.0f;
                if(animationTimer >= animationDuration){
                    animationTimer = 0.0f;
                    frameID++;
                    if(frameID < frames.size()){
                        sprite.setTextureRect(frames[frameID]);
                    }
                }
            }
        };

        
        sf::Vector2f position;
        sf::Vector2f size;
        sf::Texture texture;
        bool showFireworks = false;
        float animationTimer = 0.f;
        float animationDuration = 15.f;
        std::vector<FireworksData> fireWorks;
    public:
        FireWorks();
        FireWorks(const sf::Vector2f &pos, const sf::Vector2f &size);
        ~FireWorks();

        void createFireworksAnimation();
        void setShowFireworks(bool show);

        void update(const sf::RenderWindow *window, float dt) override;
        void handleEvent(const sf::RenderWindow *window, const sf::Event &event) override;
        void render(sf::RenderWindow *window) override;
    };

    

} // namespace mario::entity
