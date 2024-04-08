#include <SFML/Graphics.hpp>
#include "Constants.hpp"

class Mario
{
public:
    sf::Vector2f getPosition();
    sf::Sprite getSprite();
    Mario();

    void moveX();
    void moveY();

    void updateVelocity();

    void updateAcc();
    void reduceAcc();

    void setDirection(int);

    int getDirection();
    float getVelocity();

    float getX();
    float getY();

    void  loadSpriteForward(int);
    void  loadSpriteBackward(int);
    void  jump();

    int getWidth();
    int getHeight();

    bool marioIsFreezed();

    void detectCollisions(sf::Sprite*, int);
    bool collides(sf::Vector2f, sf::Vector2f);

    bool decelerating = false;

    float lerp (float, float, float);
    sf::Vector2f realCoordinates;

private:
    sf::Sprite sprite;
    std::string spritePath = "sprites/sprites_sheet.png";
    
    sf::Texture texture;
    sf::Texture marioFrameOne;
    sf::Texture marioFrameTwo;
    sf::Texture marioFrameThree;
    sf::Texture marioFrameFour;

    int width = TILE_DIMENSION;
    int height = TILE_DIMENSION;
    int direction = 0;
    
    float acc = 0;
    float velocity = 0;

    float maxVelocity = 3;
    float accOffset = 0.1f;
    float vertVelocity = 9.45f; 
    float gravity = 0.7f;
    
    bool lookingRight = true;
    bool flipped = false;
    bool isJumping = false;

    bool freezeMario = false;
};