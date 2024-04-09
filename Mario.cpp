#include "Mario.hpp"
#include <iostream>

Mario::Mario()
{

    if (!this->marioFrameOne.loadFromFile(this->spritePath, sf::IntRect(0, 8, this->width, this->height)))
        throw std::invalid_argument("Could not load mario texture");

    if (!this->marioFrameTwo.loadFromFile(this->spritePath, sf::IntRect(20, 8, 20 + this->width, this->height)))
        throw std::invalid_argument("Could not load mario frame one texture");

    if (!this->marioFrameThree.loadFromFile(this->spritePath, sf::IntRect(38, 8, 38 + this->width, this->height)))
        throw std::invalid_argument("Could not load mario frame one texture");

    if (!this->marioFrameFour.loadFromFile(this->spritePath, sf::IntRect(56, 8, 56 + this->width, this->height)))
        throw std::invalid_argument("Could not load mario frame one texture");

    this->sprite = sf::Sprite(this->marioFrameOne);
    this->sprite.setPosition(10, BOUNDING_Y_BOTTOM);
    this->realCoordinates.x += this->getX();
}

sf::Vector2f Mario::getPosition()
{
    return this->sprite.getPosition();
}

void Mario::setDirectionX(int direction)
{
    if (direction != 0)
    {
        this->lookingRight = direction == 1 ? true : false;
    }

    if (!this->lookingRight && !this->flipped)
    {
        // this->sprite.scale(-1.0f, 1.0f);
        // this->sprite.move(this->width, 0);

        this->flipped = true;
    }
    else if (this->flipped && this->lookingRight)
    {
        // this->sprite.scale(-1.0f, 1.0f);
        // this->sprite.move(- this->width,0);

        this->flipped = false;
    }

    this->direction = direction;
}

int Mario::getDirection()
{
    return this->direction;
}

void Mario::detectCollisions(sf::Sprite *s_objects, int count)
{
    int i = 0;
    while (i < count)
    {
        if (this->collides(this->getPosition(), s_objects->getPosition()))
        {
            sf::Vector2f oppositeForce = this->getPosition()-s_objects->getPosition();
            std::cout << "opposite force :"+ std::to_string(oppositeForce.x) + "/"+ std::to_string(oppositeForce.y) << std::endl;
        }
        s_objects++;
        i++;
    }
}

bool Mario::collides(sf::Vector2f a, sf::Vector2f b)
{

    return a.x >= b.x - TILE_DIMENSION && a.x <= b.x + TILE_DIMENSION && a.y >= b.y - TILE_DIMENSION && a.y <= b.y + TILE_DIMENSION;
}
void Mario::moveX()
{

    if (this->getX() + this->velocity <= BOUNDING_X_LEFT && (this->direction < 1 || this->decelerating))
    {
        this->velocity = 0.0f;
        this->sprite.setPosition(this->getX(), BOUNDING_Y_BOTTOM);
        std::cout << "out of map:" + std::to_string(this->getX()) << std::endl;
    }
    else
    {
        this->realCoordinates.x += this->velocity;

        if ((this->getX() + this->getVelocity() >= BOUNDING_X_MIDDLE))
        {
            if (!this->decelerating && this->getDirection() < 1)
            {
                this->sprite.move(sf::Vector2f(this->velocity, 0));
                this->freezeMario = false;
            }
            else
            {
                this->freezeMario = true;
            }
        }
        else
        {
            this->freezeMario = false;
            this->sprite.move(sf::Vector2f(this->velocity, 0));
        }
    }
}

void Mario::moveY()
{
    if (this->isJumping)
    {
        this->sprite.move(sf::Vector2f(0, -this->vertVelocity));
    }
}
void Mario::updateHorizontalVelocity()
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
}

void Mario::updateVerticalVelocity()
{
    if (this->isJumping)
    {
        // std::cout << "vertical velocity: " + std::to_string(this->vertVelocity) << std::endl;
        if (this->getY() - this->vertVelocity >= BOUNDING_Y_BOTTOM)
        {
            this->vertVelocity = initialVertVelocity;
            this->isJumping = false;
            this->gravity = initialGravity;
        }
        else
        {
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

float Mario::getX()
{
    return this->getPosition().x;
}

float Mario::getY()
{
    return this->getPosition().y;
}

int Mario::getWidth()
{
    return this->width;
}

int Mario::getHeight()
{
    return this->height;
}

bool Mario::marioIsFreezed()
{
    return this->freezeMario;
}

sf::Sprite Mario::getSprite()
{
    return this->sprite;
}
