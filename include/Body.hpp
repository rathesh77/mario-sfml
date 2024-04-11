
#ifndef BODY_H_
#define BODY_H_

#include <SFML/Graphics.hpp>

#include "Constants.hpp"

class Body;

struct SpriteObject {
    std::string type = "";
    sf::Sprite *sprite = new sf::Sprite;
    Body *body = NULL;
};

class Body {
   public:
    sf::Vector2f getPosition();
    sf::Sprite* getSprite();

    Body(std::string, float, float);
    Body();

    void loop(SpriteObject *, int);

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
    void jump();

    int getWidth();
    int getHeight();

    bool isOverlaping();

    void detectCollisions(SpriteObject*, int);
    void postCollisionsDetection();

    bool collides(sf::Vector2f, sf::Vector2f);

    bool decelerating = false;

    float lerp(float, float, float);
    sf::Vector2f realCoordinates;

    bool overlap = false;

   protected:
    sf::Sprite sprite;
    std::string spritePath;

    sf::Texture texture;

    int width = TILE_DIMENSION;
    int height = TILE_DIMENSION;
    int direction = -1;

    float acc = 0.0f;

    float velocityX = -1.0f;
    float maxVelocityX = 3.0f;

    float accOffset = 0.0f;

    float initialVelocityY = 9.45f;
    float velocityY = initialVelocityY;

    float initialGravity = 0.6f;
    float gravity = initialGravity;

    bool lookingRight = true;
    bool flipped = false;
    bool isJumping = false;

    float ground;
};


#endif /* BODY_H_ */
