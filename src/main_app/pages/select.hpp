#pragma once

#include "page.hpp"
#include "../../widget_toolkit/resource/LevelState.hpp"
#include "../../widget_toolkit/entity/player/player.hpp"
#include "../../widget_toolkit/entity/player/player-state.hpp"

namespace mario::pages {

    static const int DEFAULT_NUM_LIVES = 2;

    class SelectPage : public Page {
        private:
            int selectedChar;
            std::vector<mario::entity::CharacterListType> availableChar; 
            int selectedLevel;

            sf::Texture curtainTexture;
            sf::Sprite curtainSprite;
            sf::Texture titleTexture;
            sf::Sprite titleSprite;
            std::vector<sf::Texture> charFrameTextures;
            std::vector<sf::Sprite> charFrameSprites;
            std::vector<sf::Texture> charTextures;
            std::vector<sf::Sprite> charSprites;
            int selectedFrame;

            float FRAME_SCALE;
            float FRAMES_Y;
            float framesStartX;
            float frameWidth;
            float FRAME_SPACING;

            sf::Texture backgroundTexture;
            sf::Sprite backgroundSprite;
        public:
            SelectPage(MainWindow &context, int level);
            ~SelectPage();
            void update(const sf::RenderWindow *window, float dt) override;
            void handleEvent(const sf::RenderWindow *window, const sf::Event &event) override;
            void render(sf::RenderWindow *window) override;
            int getSelectedCharacter() const;
    };
}