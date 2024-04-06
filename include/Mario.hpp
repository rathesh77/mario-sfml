#include <SFML/Graphics.hpp>
#include "Constants.hpp"

class Mario
{
public:
    sf::Sprite sprite;

    Mario();
    void move();
    void updateVelocity();

    void updateAcc();
    void reduceAcc();

    void setDirection(int);
    void setVelocity(float);

    int getDirection();
    float getVelocity();

    sf::Vector2f getPosition();
    void  loadSpriteForward(int);
    void  loadSpriteBackward(int);
    void  jump();

    int getWidth();
    int getHeight();

    float lerp (float, float, float);
    bool decelerating = false;

private:
    std::string spritePath = "sprites/sprites_sheet.png";
    
    sf::Texture texture;
    sf::Texture marioFrameOne;
    sf::Texture marioFrameTwo;
    sf::Texture marioFrameThree;
    sf::Texture marioFrameFour;

    int width = 16;
    int height = 17;
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

};