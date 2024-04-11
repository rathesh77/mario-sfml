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
    std::string spritePath = "sprites/sprites_sheet.png";

    sf::Texture texture;
    sf::Texture marioFrameOne;
    sf::Texture marioFrameTwo;
    sf::Texture marioFrameThree;
    sf::Texture marioFrameFour;

    bool freezeMario = false;

    bool hitEnnemy = false;
};