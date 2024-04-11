#include <SFML/Graphics.hpp>

#include "Constants.hpp"

class Mario {
   public:
    sf::Vector2f getPosition();
    sf::Sprite getSprite();
    Mario();

    void moveX();
    void moveY();

    void updateHorizontalVelocity();
    void updateVerticalVelocity();
    void resetY();

    void updateAcc();
    void reduceAcc();

    void setDirectionX(int);

    int getDirection();
    float getVelocity();

    float getX();
    float getY();

    void loadSpriteForward(int);
    void loadSpriteBackward(int);
    void jump();

    int getWidth();
    int getHeight();

    bool marioIsFreezed();
    bool isOverlaping();

    void detectCollisions(sf::Sprite*, int);
    void postCollisionsDetection();

    bool collides(sf::Vector2f, sf::Vector2f);

    bool decelerating = false;

    float lerp(float, float, float);
    sf::Vector2f realCoordinates;

    bool overlap = false;

   private:
    sf::Sprite sprite;
    std::string spritePath = "sprites/sprites_sheet.png";

    float ground;
    sf::Texture texture;
    sf::Texture marioFrameOne;
    sf::Texture marioFrameTwo;
    sf::Texture marioFrameThree;
    sf::Texture marioFrameFour;

    int width = TILE_DIMENSION;
    int height = TILE_DIMENSION;
    int direction = 0;

    float acc = 0.0f;

    float velocityX = 0.0f;
    float maxVelocityX = 3.0f;

    float accOffset = 0.1f;

    float initialVelocityY = 9.45f;
    float velocityY = initialVelocityY;

    float initialGravity = 0.6f;
    float gravity = initialGravity;

    bool lookingRight = true;
    bool flipped = false;
    bool isJumping = false;

    bool freezeMario = false;
};