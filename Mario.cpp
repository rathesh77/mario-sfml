#include "Mario.hpp"

#include <iostream>

Mario::Mario() {
    if (!this->m_marioFrameOne.loadFromFile(
            this->m_spritePath, sf::IntRect(0, 8, this->m_width, this->m_height)))
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

    m_ground = BOUNDING_Y_BOTTOM;
}

void Mario::loop(SpriteObject *s_objects, int NB_SPRITES) {

        if (this->isOverlaping()) {
            this->m_overlap = false;

        } else {
            this->updateHorizontalVelocity();
            this->updateVerticalVelocity();
            this->postCollisionsDetection();

            this->detectCollisions(s_objects);
        }
        this->moveX();
        this->moveY();

        this->resetY();
}

void Mario::detectCollisions(SpriteObject *s_objects) {
    int i = 0;
    bool hit = false;

    this->m_overlap = false;
    while (true) {
        if (s_objects->type == "NULL" || s_objects->type == "" )
            break;
        sf::Vector2f objectPos = s_objects->sprite->getPosition();
        if (s_objects->type != "brick") {
            objectPos = s_objects->body->getPosition();
        }
        if (this->collides(this->getPosition() +
                               sf::Vector2f(this->m_velocityX, -this->m_velocityY),
                           objectPos)) {
            hit = true;
            this->m_overlap = true;
            sf::Vector2f oppositeForce = this->getPosition() - objectPos;
            if (std::abs(oppositeForce.y) > std::abs(oppositeForce.x)) {
                if (oppositeForce.y >= 0) {  // force downward
                    std::cout << "downward" << std::endl;
                    if (this->m_velocityY > 0)
                        this->m_velocityY = -0.0f;
                    else
                        this->m_overlap = false;
                    if (s_objects->type == "goomba") {
                        m_hitEnnemy = true;
                    }
                } else {
                    std::cout << "upward" << std::endl;
                    // force upward
                    m_ground = objectPos.y - TILE_DIMENSION;
                    this->m_velocityY = this->getY() - m_ground;

                    if (s_objects->type == "goomba") {
                        s_objects->body = new Body;
                    }
                }
            } else {
                this->m_velocityX = 0;
                if (s_objects->type == "goomba") {
                    m_hitEnnemy = true;
                }
            }
        }
        s_objects++;
        i++;
    }
}

void Mario::moveX()  // no collisions handling here. Only moving sprite
{
    if (this->getX() + this->m_velocityX <= BOUNDING_X_LEFT &&
        (this->m_direction < 1 || this->decelerating)) {
        this->m_velocityX = 0.0f;
        this->m_sprite.setPosition(this->getX(), this->getY());
        std::cout << "out of map:" + std::to_string(this->getX()) << std::endl;
    } else {
        this->realCoordinates.x += this->m_velocityX;

        if ((this->getX() + this->getVelocity() >= BOUNDING_X_MIDDLE)) {
            if (!this->decelerating && this->getDirection() < 1) {
                this->m_sprite.move(sf::Vector2f(this->m_velocityX, 0));
                this->m_freezeMario = false;
            } else {
                this->m_freezeMario = true;
            }
        } else {
            this->m_freezeMario = false;
            this->m_sprite.move(sf::Vector2f(this->m_velocityX, 0));
        }
    }
}
void Mario::loadSpriteForward(int frameCount) {
    if (this->m_velocityX != 0) {
        if (frameCount < 2) {
            this->m_sprite.setTexture(m_marioFrameOne);
        }
        if (frameCount % 2 == 0) {
            this->m_sprite.setTexture(m_marioFrameTwo);
        }
        if (frameCount % 3 == 0) {
            this->m_sprite.setTexture(m_marioFrameThree);
        }
    }
}

bool Mario::marioIsFreezed() { return this->m_freezeMario; }

bool Mario::hasHitEnnemy() { return this->m_hitEnnemy; }
