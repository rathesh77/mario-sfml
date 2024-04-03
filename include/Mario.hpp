#include <SFML/Graphics.hpp>
#include "Constants.hpp"

class Mario
{
public:
    sf::Sprite sprite;

    Mario();
    sf::Vector2f moveForward();
    sf::Vector2f moveBackward();
    sf::Vector2f getPosition();
    void reduceAcceleration();

    int getWidth();
    int getHeight();

private:
    std::string spritePath = "sprites/sprites_sheet.png";
    sf::Texture texture;
    int width;
    int height;
    float speed;
    float acceleration;
};