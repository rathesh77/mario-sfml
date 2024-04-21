#include "Mario.hpp"

#include <iostream>

Mario::Mario()
{
    this->m_width = TILE_DIMENSION;
    this->m_height = TILE_DIMENSION;
    if (!this->m_marioFrameOne.loadFromFile(
            this->m_spritePath,
            sf::IntRect(0, 8, this->m_width, this->m_height)))
        throw std::invalid_argument("Could not load mario texture");

    if (!this->m_marioFrameTwo.loadFromFile(
            this->m_spritePath,
            sf::IntRect(20, 8, 20 + this->m_width, this->m_height)))
        throw std::invalid_argument("Could not load mario frame one texture");

    if (!this->m_marioFrameThree.loadFromFile(
            this->m_spritePath,
            sf::IntRect(38, 8, 38 + this->m_width, this->m_height)))
        throw std::invalid_argument("Could not load mario frame one texture");

    if (!this->m_marioFrameFour.loadFromFile(
            this->m_spritePath,
            sf::IntRect(56, 8, 56 + this->m_width, this->m_height)))
        throw std::invalid_argument("Could not load mario frame one texture");

    this->m_sprite = sf::Sprite(this->m_marioFrameOne);
    this->m_sprite.setPosition(10, BOUNDING_Y_BOTTOM);
    this->realCoordinates.x += this->getX();
    this->m_direction = 0;
    this->m_velocityX = 0.0f;
    this->m_accOffset = 0.1f;
    this->m_maxVelocityX = 2.0f;
    m_ground = WINDOW_HEIGHT + 16;
}

void Mario::loop(SpriteObject *s_objects)
{
    if (this->isOverlaping())
    {
        this->m_overlap = false;
        this->resetY();
    }
    else
    {
        this->updateHorizontalVelocity();
        this->updateVerticalVelocity();
        this->postCollisionsDetection();
    }
    this->handleCollision(s_objects);

    this->moveX();
    this->moveY();
}

void Mario::handleCollision(SpriteObject *s_objects)
{

    std::map<std::string, std::vector<SpriteObject *>> collidedObjects = this->detectCollisions(s_objects);
    std::cout << "begin" << std::endl;
    for (SpriteObject *object : collidedObjects["up"])
    {
        std::cout << "upward:" + object->type << std::endl;

        sf::Vector2f objectPos = object->body->getPosition();

        if (object->type == "goomba")
        {
            delete object->body;
            object->body = new Body();
            this->m_velocityY = 1;
        }
        else
        {
            m_ground = objectPos.y;
            this->m_velocityY = this->getY() - (objectPos.y - this->m_height);
        }
    }
    for (SpriteObject *object : collidedObjects["down"])
    {
        std::cout << "downward:" + object->type << std::endl;

        // if (this->m_velocityY > 0)
        this->m_velocityY = 0.0f;

        if (object->type == "goomba")
        {
            m_hitEnnemy = true;
        }
    }
    for (SpriteObject *object : collidedObjects["side"])
    {
        std::cout << "sideways:" + object->type << std::endl;
        /*

            std::cout << "MarioPosPrevious:"+ std::to_string((this->getPosition()).x) + "/" + std::to_string((this->getPosition()).y) << std::endl;

            std::cout << "MarioPosNext:"+ std::to_string((this->getPosition() +
                sf::Vector2f(this->m_velocityX, -this->m_velocityY)).x) + "/" + std::to_string((this->getPosition() +
                sf::Vector2f(this->m_velocityX, -this->m_velocityY)).y) << std::endl;

            std::cout << "obj:"+ std::to_string(s_objects->body->getX()) + "/" + std::to_string(s_objects->body->getY())<< std::endl;
        */
        if (this->getX() > object->body->getX())
        {
            this->m_velocityX = 0.2;
        }
        else
        {
            this->m_velocityX = -0.2;
        }
        if (object->type == "goomba")
        {
            m_hitEnnemy = true;
        }
    }
}

void Mario::moveX() // no collisions handling here. Only moving sprite
{
    if (this->getX() + this->m_velocityX <= BOUNDING_X_LEFT &&
        (this->m_direction < 1 || this->m_decelerating))
    {
        this->m_velocityX = 0.0f;
        this->m_sprite.setPosition(this->getX(), this->getY());
        std::cout << "out of map:" + std::to_string(this->getX()) << std::endl;
    }
    else
    {
        this->realCoordinates.x += this->m_velocityX;

        if ((this->getX() + this->getVelocityX() >= BOUNDING_X_MIDDLE))
        {
            if (!this->m_decelerating && this->getDirection() < 1)
            {
                this->m_sprite.move(sf::Vector2f(this->m_velocityX, 0));
                this->m_freezeMario = false;
            }
            else
            {
                this->m_freezeMario = true;
            }
        }
        else
        {
            this->m_freezeMario = false;
            this->m_sprite.move(sf::Vector2f(this->m_velocityX, 0));
        }
    }
}
void Mario::loadSpriteForward(int frameCount)
{
    if (this->m_velocityX != 0)
    {
        if (frameCount < 2)
        {
            this->m_sprite.setTexture(m_marioFrameOne);
        }
        if (frameCount % 2 == 0)
        {
            this->m_sprite.setTexture(m_marioFrameTwo);
        }
        if (frameCount % 3 == 0)
        {
            this->m_sprite.setTexture(m_marioFrameThree);
        }
    }
}

bool Mario::marioIsFreezed() { return this->m_freezeMario; }

bool Mario::hasHitEnnemy() { return this->m_hitEnnemy; }
