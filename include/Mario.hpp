#include <SFML/Graphics.hpp>
#include "Constants.hpp"

class Mario
{
public:
    sf::Sprite sprite;

    Mario();
    void move();
    void updateAcc();
    void reduceAcc();

    void setDirection(int);
    void setVel(float);

    int getDirection();

    sf::Vector2f getPosition();

    int getWidth();
    int getHeight();

    float lerp (float, float, float);

private:
    std::string spritePath = "sprites/sprites_sheet.png";
    sf::Texture texture;
    int width = 16;
    int height = 17;
    float acc = 0;
    float vel = 0;

    float maxVel = 3;
    float maxAcc = 0.7;
    float accOffset = 0.4;

    int direction = 0;
};