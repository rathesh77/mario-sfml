#include "Mario.hpp"
#include <iostream>

Mario::Mario(int height)
{
    if (!this->texture.loadFromFile(this->spritePath))
        throw std::invalid_argument("Could not load mario texture");

    this->sprite = sf::Sprite(this->texture);
    this->sprite.setPosition(10,height);
    this->sprite.setColor(sf::Color(255));
}
Mario::Mario()
{

}