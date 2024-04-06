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
    this->sprite.setPosition(10, FLOOR_Y);
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
        auto oldPosition = this->getPosition();
        this->sprite.scale(-1.0f, 1.0f);
        this->sprite.setPosition(oldPosition.x + this->width, oldPosition.y);

        this->flipped = true;
    }
    else if (this->flipped && this->lookingRight)
    {
        auto oldPosition = this->getPosition();

        this->sprite.scale(-1.0f, 1.0f);
        this->sprite.setPosition(oldPosition.x - this->width, oldPosition.y);

        this->flipped = false;
    }

    this->direction = direction;
}

void Mario::setVelocity(float velocity)
{
    this->velocity = velocity;
}

int Mario::getDirection()
{
    return this->direction;
}

void Mario::move()
{

    this->sprite.move(sf::Vector2f(this->velocity, 0));
}
void Mario::updateVelocity()
{

    float goal = this->maxVelocity * this->direction;
    float dt = this->accOffset;
    float current = this->velocity;
    this->velocity = lerp(current, goal, dt);

    if (std::abs(this->velocity) < std::abs(current))
    {
        this->decelerating = true;
    }
    else
    {
        this->decelerating = false;
    }
    if (this->isJumping)
    {
        std::cout << "vertical velocity: " + std::to_string(this->vertVelocity) << std::endl;
        if (this->getPosition().y >= FLOOR_Y && this->vertVelocity < 0)
        {
            this->vertVelocity = 9.45f;
            this->isJumping = false;
        }
        else
        {
            this->sprite.move(sf::Vector2f(0, -this->vertVelocity));
            this->vertVelocity -= this->gravity;
        }
    }
}

void Mario::jump()
{
    this->isJumping = true;
}

void Mario::loadSpriteForward(int frameCount)
{

    if (this->velocity != 0)
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

float Mario::getVelocity()
{
    return this->velocity;
}

int Mario::getWidth()
{
    return this->width;
}

int Mario::getHeight()
{
    return this->height;
}