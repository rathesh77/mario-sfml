#include <SFML/Graphics.hpp>

#include "Constants.hpp"
#include "Body.hpp"
#include "Event.hpp"

class Mario: public Body {
   public:

    Mario();
    std::vector<Event> loop(SpriteObject *);
    std::vector<Event> handleCollision(SpriteObject *s_objects);

    void moveX();

    void loadSpriteForward(int);

    bool marioIsFreezed();
    bool hasHitEnnemy();

   private:
    std::string m_spritePath = "sprites/sprites_sheet.png";

    sf::Texture m_texture;
    sf::Texture m_marioFrameOne;
    sf::Texture m_marioFrameTwo;
    sf::Texture m_marioFrameThree;
    sf::Texture m_marioFrameFour;

    bool m_freezeMario = false;

    bool m_hitEnnemy = false;

};