#include <SFML/Graphics.hpp>

class Mario
{
public:
    sf::Sprite sprite;
    Mario(int);
    Mario();

private:
    std::string spritePath = "sprites/mario.png";
    sf::Texture texture;

};