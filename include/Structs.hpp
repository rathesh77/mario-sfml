#include <SFML/Graphics.hpp>

#include "Constants.hpp"

struct SpriteObject {
    std::string type = "";
    sf::Sprite *sprite = new sf::Sprite;
};