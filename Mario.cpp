#include "Mario.hpp"

#include <iostream>

Mario::Mario() {
    if (!this->marioFrameOne.loadFromFile(
            this->spritePath, sf::IntRect(0, 8, this->width, this->height)))
        throw std::invalid_argument("Could not load mario texture");

    if (!this->marioFrameTwo.loadFromFile(
            this->spritePath,
            sf::IntRect(20, 8, 20 + this->width, this->height)))
        throw std::invalid_argument("Could not load mario frame one texture");

    if (!this->marioFrameThree.loadFromFile(
            this->spritePath,
            sf::IntRect(38, 8, 38 + this->width, this->height)))
        throw std::invalid_argument("Could not load mario frame one texture");

    if (!this->marioFrameFour.loadFromFile(
            this->spritePath,
            sf::IntRect(56, 8, 56 + this->width, this->height)))
        throw std::invalid_argument("Could not load mario frame one texture");

    this->sprite = sf::Sprite(this->marioFrameOne);
    this->sprite.setPosition(10, BOUNDING_Y_BOTTOM);
    this->realCoordinates.x += this->getX();
    this->direction = 0;
    this->velocityX = 0.0f;
    this->accOffset = 0.1f;

    ground = BOUNDING_Y_BOTTOM;
}

void Mario::detectCollisions(SpriteObject *s_objects, int count) {
    int i = 0;
    bool hit = false;

    this->overlap = false;
    while (i < count) {
        sf::Vector2f objectPos = s_objects->sprite->getPosition();
        if (s_objects->type != "brick") {
            objectPos = s_objects->body->getPosition();
        }
        if (this->collides(this->getPosition() +
                               sf::Vector2f(this->velocityX, -this->velocityY),
                           objectPos)) {
            hit = true;
            this->overlap = true;
            sf::Vector2f oppositeForce = this->getPosition() - objectPos;
            if (std::abs(oppositeForce.y) > std::abs(oppositeForce.x)) {
                if (oppositeForce.y >= 0) {  // force downward
                    std::cout << "downward" << std::endl;
                    if (this->velocityY > 0)
                        this->velocityY = -0.0f;
                    else
                        this->overlap = false;
                    if (s_objects->type == "goomba") {
                        hitEnnemy = true;
                    }
                } else {
                    std::cout << "upward" << std::endl;
                    // force upward
                    ground = objectPos.y - TILE_DIMENSION;
                    this->velocityY = this->getY() - ground;

                    if (s_objects->type == "goomba") {
                        s_objects->body = new Body;
                    }
                }
            } else {
                this->velocityX = 0;
                if (s_objects->type == "goomba") {
                    hitEnnemy = true;
                }
            }
        }
        s_objects++;
        i++;
    }
}

void Mario::moveX()  // no collisions handling here. Only moving sprite
{
    if (this->getX() + this->velocityX <= BOUNDING_X_LEFT &&
        (this->direction < 1 || this->decelerating)) {
        this->velocityX = 0.0f;
        this->sprite.setPosition(this->getX(), this->getY());
        std::cout << "out of map:" + std::to_string(this->getX()) << std::endl;
    } else {
        this->realCoordinates.x += this->velocityX;

        if ((this->getX() + this->getVelocity() >= BOUNDING_X_MIDDLE)) {
            if (!this->decelerating && this->getDirection() < 1) {
                this->sprite.move(sf::Vector2f(this->velocityX, 0));
                this->freezeMario = false;
            } else {
                this->freezeMario = true;
            }
        } else {
            this->freezeMario = false;
            this->sprite.move(sf::Vector2f(this->velocityX, 0));
        }
    }
}
void Mario::loadSpriteForward(int frameCount) {
    if (this->velocityX != 0) {
        if (frameCount < 2) {
            this->sprite.setTexture(marioFrameOne);
        }
        if (frameCount % 2 == 0) {
            this->sprite.setTexture(marioFrameTwo);
        }
        if (frameCount % 3 == 0) {
            this->sprite.setTexture(marioFrameThree);
        }
    }
}

bool Mario::marioIsFreezed() { return this->freezeMario; }

bool Mario::hasHitEnnemy() { return this->hitEnnemy; }
