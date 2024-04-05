#include "Mario.hpp"
#include <iostream>

Mario::Mario()
{

    if (!this->marioFrameOne.loadFromFile(this->spritePath, sf::IntRect(0, 8, this->width, this->height)))
        throw std::invalid_argument("Could not load mario texture");

    if (!this->marioFrameTwo.loadFromFile(this->spritePath, sf::IntRect(19, 8, 19 + this->width, this->height)))
        throw std::invalid_argument("Could not load mario frame one texture");

    if (!this->marioFrameThree.loadFromFile(this->spritePath, sf::IntRect(38, 8, 38 + this->width, this->height)))
        throw std::invalid_argument("Could not load mario frame one texture");

    if (!this->marioFrameFour.loadFromFile(this->spritePath, sf::IntRect(56, 8, 56 + this->width, this->height)))
        throw std::invalid_argument("Could not load mario frame one texture");

    this->sprite = sf::Sprite(this->marioFrameOne);
    this->sprite.setPosition(10, WINDOW_HEIGHT - 47);
}

sf::Vector2f Mario::getPosition()
{
    return this->sprite.getPosition();
}

void Mario::setDirection(int direction)
{
    if (direction != 0)
    {
        this->lookingRight = direction == 1 ? true : false;
    }

    if (!this->lookingRight && !this->flipped)
    {
        auto oldPosition = this->sprite.getPosition();
        this->sprite.scale(-1.0f, 1.0f);
        this->sprite.setPosition(oldPosition.x + this->width, oldPosition.y);

        this->flipped = true;
    }
    else if (this->flipped && this->lookingRight)
    {
        auto oldPosition = this->sprite.getPosition();

        this->sprite.scale(-1.0f, 1.0f);
        this->sprite.setPosition(oldPosition.x - this->width, oldPosition.y);

        this->flipped = false;
    }

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
    // std::cout << "velocity: " + std::to_string(this->vel) << std::endl;
    this->sprite.move(sf::Vector2f(this->vel, 0));
}

void Mario::loadSpriteForward(int frameCount)
{

    if (this->vel != 0)
    {
        if (frameCount < 2)
        {
            this->sprite.setTexture(marioFrameOne);
        }
        if (frameCount % 2 == 0)
        {
            this->sprite.setTexture(marioFrameTwo);
        }
        if (frameCount % 3 == 0)
        {
            this->sprite.setTexture(marioFrameThree);
        }
    }
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