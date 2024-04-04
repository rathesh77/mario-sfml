#include <SFML/Graphics.hpp>
#include "Constants.hpp"

class Mario
{
public:
    sf::Sprite sprite;

    Mario();
    void move();
    void increaseAcceleration();
    void decreaseAcceleration();
    void setDirection(int);

    sf::Vector2f getPosition();

    int getWidth();
    int getHeight();

private:
    std::string spritePath = "sprites/sprites_sheet.png";
    sf::Texture texture;
    int width;
    int height;
    float speed;
    float acceleration;

    float accelerationOffset = 0.01;

    int direction = 0;
};