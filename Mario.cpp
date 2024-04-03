#include "Mario.hpp"
#include <iostream>

Mario::Mario()
{
    this->width = 16;
    this->height = 17;
    this->speed = 0;
    this->acceleration = 0;

    if (!this->texture.loadFromFile(this->spritePath, sf::IntRect(0,8, this->width, this->height)))
        throw std::invalid_argument("Could not load mario texture");

    this->sprite = sf::Sprite(this->texture);
    this->sprite.setPosition(10,WINDOW_HEIGHT - 50);
    //this->sprite.setColor(sf::Color(255));
}

sf::Vector2f Mario::getPosition()
{
    return this->sprite.getPosition();
}

sf::Vector2f Mario::moveForward()
{
    if (this->acceleration < 0.010 * 100 ) {
        this->acceleration = this->acceleration + 0.010;
        this->speed = this->speed + this->acceleration;
    std::cout<<this->acceleration<<std::endl;
    }
    
    if (this->sprite.getPosition().x < WINDOW_WIDTH / 2.5)
        this->sprite.move(sf::Vector2f(this->speed, 0));
    return this->sprite.getPosition();
}

sf::Vector2f Mario::moveBackward()
{
    if (this->sprite.getPosition().x > 0)
        this->sprite.move(sf::Vector2f(-0.02, 0));
    return this->sprite.getPosition();
}

void Mario::reduceAcceleration()
{
    if (this->acceleration) {
        this->acceleration = this->acceleration - 0.010;
    }
}



int Mario::getWidth()
{
    return this->width;
}

int Mario::getHeight()
{
    return this->height;
}