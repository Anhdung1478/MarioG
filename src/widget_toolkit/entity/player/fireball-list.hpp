#pragma once

#include "fireball.hpp"
#include "../../resource/SoundManager.hpp"

namespace mario::entity {
    class FireballList : public IScreenElement {
        private:
            std::vector<Fireball*> fireballs;
            mario::audio::SoundManager& soundManager;
        public:
            FireballList(mario::audio::SoundManager& soundManager) : soundManager(soundManager) {
                fireballs.clear();
            }

            ~FireballList() override {
                for (int it = 0; it < int(fireballs.size()); ++it)
                    delete fireballs[it];
            }

            void addAFireball(Fireball* fireball) {
                soundManager.playSound(mario::event::SoundEvent::FIREBALL_THROW);
                fireballs.push_back(fireball);
            }

            void update(const sf::RenderWindow *window, float dt) override {
                for (int it = 0; it < int(fireballs.size()); ++it) {
                    fireballs[it]->update(window, dt);
                    if(fireballs[it]->isDisappeared()) {
                        std::swap(fireballs[it], fireballs.back());
                        fireballs.pop_back();
                        --it;
                    }
                }
            }

            void handleEvent(const sf::RenderWindow *window, const sf::Event &event) override {
                for (int it = 0; it < int(fireballs.size()); ++it)
                    fireballs[it]->handleEvent(window, event);
            }

            void render(sf::RenderWindow *window) {
                for (int it = 0; it < int(fireballs.size()); ++it)
                    fireballs[it]->render(window);
            }

            Fireball* getFireballAtPos(int pos) const {
                return fireballs[pos];
            }

            int getNumFireballs() const {
                return int(fireballs.size());
            }
    };
}