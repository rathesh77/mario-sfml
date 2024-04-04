#include "Mario.hpp"
#include <iostream>

Mario::Mario()
{
    this->width = 16;
    this->height = 17;
    this->speed = 0;
    this->acceleration = 0;
    this->direction = 0;
    if (!this->texture.loadFromFile(this->spritePath, sf::IntRect(0,8, this->width, this->height)))
        throw std::invalid_argument("Could not load mario texture");

    this->sprite = sf::Sprite(this->texture);
    this->sprite.setPosition(10, WINDOW_HEIGHT - 50);
    //this->sprite.setColor(sf::Color(255));
}

sf::Vector2f Mario::getPosition()
{
    return this->sprite.getPosition();
}


void Mario::move() {
    if (this->sprite.getPosition().x < WINDOW_WIDTH / 2.5)
        this->sprite.move(sf::Vector2f(this->speed * this->direction, 0));
    std::cout<<this->speed<<std::endl;
        std::cout<<this->acceleration<<std::endl;

}

void Mario::setDirection(int direction)
{
    this->direction = direction;
}

void Mario::increaseAcceleration()
{
    if (this->acceleration < this->accelerationOffset * 100 ) {
        this->acceleration = this->acceleration + this->accelerationOffset;
        this->speed = this->speed + this->acceleration;
    }
    
}

void Mario::decreaseAcceleration()
{
    if (this->speed > 0) {
        this->acceleration = this->acceleration - this->accelerationOffset;
        
    } else {
        this->acceleration = 0;
        this->speed = 0;
    }
    this->speed = this->speed + this->acceleration;

}


int Mario::getWidth()
{
    return this->width;
}

int Mario::getHeight()
{
    return this->height;
}