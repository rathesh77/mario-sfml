
#ifndef BODY_H_
#define BODY_H_

#include <SFML/Graphics.hpp>


#include "Constants.hpp"

class Body;

struct SpriteObject {
    std::string type = "";
    Body *body = NULL;
};

class Body {
   public:
    sf::Vector2f getPosition();
    sf::Sprite* getSprite();

    Body(sf::Texture*, float, float, float, float, int, int, std::string);
    Body();

    void loop(SpriteObject *);

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

    std::map<std::string, std::vector<SpriteObject *>>  detectCollisions(SpriteObject*);
    void handleCollision(SpriteObject *s_objects);
    void upwardCollision(SpriteObject *,SpriteObject *);
    void downwardCollision(SpriteObject *,SpriteObject *);
    void sideCollision(SpriteObject *,SpriteObject * );

    void postCollisionsDetection();

    bool collides(sf::Vector2f, sf::Vector2f, int, int, int, int);
    bool compare(Body*, Body*);


    float lerp(float, float, float);
    sf::Vector2f realCoordinates;


    sf::Sprite m_sprite;
   protected:
    std::string m_type;

    sf::Texture *m_texture;

    int m_width;
    int m_height;
    int m_direction = -1;

    float m_acc = 0.0f;

    float m_velocityX = 0.0f;
    float m_maxVelocityX = 1.0f;

    float m_accOffset = 0.25f;

    float m_initialVelocityY = 9.45f;
    float m_velocityY = m_initialVelocityY;

    float m_initialGravity = 0.6f;
    float m_gravity = m_initialGravity;

    bool m_lookingRight = true;
    bool m_flipped = false;
    bool m_isJumping = false;

    float m_ground;
    bool m_overlap = false;
    bool m_decelerating = false;
    bool m_jumpEnabled = true;

};


#endif /* BODY_H_ */
