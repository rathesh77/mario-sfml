#include <SFML/Graphics.hpp>

#include "Constants.hpp"
#include "Body.hpp"

class Mario: public Body {
   public:

    Mario();
    void loop(SpriteObject *, int);

    void moveX();

    void loadSpriteForward(int);

    bool marioIsFreezed();
    bool hasHitEnnemy();
    void detectCollisions(SpriteObject*);

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