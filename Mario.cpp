#include "Mario.hpp"
#include <iostream>

Mario::Mario()
{

    if (!this->texture.loadFromFile(this->spritePath, sf::IntRect(0, 8, this->width, this->height)))
        throw std::invalid_argument("Could not load mario texture");

    this->sprite = sf::Sprite(this->texture);
    this->sprite.setPosition(10, WINDOW_HEIGHT - 50);
    // this->sprite.setColor(sf::Color(255));
}

sf::Vector2f Mario::getPosition()
{
    return this->sprite.getPosition();
}

void Mario::setDirection(int direction)
{
    this->direction = direction;
}

void Mario::setVel(float vel)
{
    this->vel = vel;
}

int Mario::getDirection()
{
    return this->direction;
}

void Mario::move()
{

    float goal = this->maxVel * this->direction;
    float dt = this->accOffset;
    float current = this->vel;
    this->vel = lerp(current, goal, dt);
    std::cout << "velocity: " + std::to_string(this->vel) << std::endl;
    this->sprite.move(sf::Vector2f(this->vel, 0));
}

float Mario::lerp(float current, float goal, float dt)
{
    if (current + dt < goal)
        return current + dt;

    if (goal < current)
        return current - dt;

    return goal;
}

int Mario::getWidth()
{
    return this->width;
}

int Mario::getHeight()
{
    return this->height;
}