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
    ground = BOUNDING_Y_BOTTOM;
}

sf::Vector2f Mario::getPosition() { return this->sprite.getPosition(); }

void Mario::setDirectionX(int direction) {
    if (direction != 0) {
        this->lookingRight = direction == 1 ? true : false;
    }

    if (!this->lookingRight && !this->flipped) {
        // this->sprite.scale(-1.0f, 1.0f);
        // this->sprite.move(this->width, 0);

        this->flipped = true;
    } else if (this->flipped && this->lookingRight) {
        // this->sprite.scale(-1.0f, 1.0f);
        // this->sprite.move(- this->width,0);

        this->flipped = false;
    }

    this->direction = direction;
}

int Mario::getDirection() { return this->direction; }

void Mario::detectCollisions(SpriteObject *s_objects, int count) {
    int i = 0;
    bool hit = false;

    this->overlap = false;
    while (i < count) {
        sf::Vector2f objectPos = s_objects->sprite->getPosition();
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
                    ground = s_objects->sprite->getPosition().y - TILE_DIMENSION;
                    this->velocityY = this->getY() - ground;

                    if (s_objects->type == "goomba") {
                        s_objects->sprite = new sf::Sprite;
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

void Mario::postCollisionsDetection() {
    if (!this->isJumping) {
        std::cout << "not hit" << std::endl;
        this->velocityY = 0;
        this->isJumping = true;
    }

    ground = BOUNDING_Y_BOTTOM;
}

bool Mario::collides(sf::Vector2f a, sf::Vector2f b) {
    return a.x >= b.x - TILE_DIMENSION + 2 && a.x <= b.x + TILE_DIMENSION - 2 &&
           a.y >= b.y - TILE_DIMENSION && a.y <= b.y + TILE_DIMENSION;
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

void Mario::moveY() {
    if (this->isJumping) {
        this->sprite.move(sf::Vector2f(0, -this->velocityY));
    }
}
void Mario::updateHorizontalVelocity() {
    float goal = this->maxVelocityX * this->direction;
    float dt = this->accOffset;
    float current = this->velocityX;
    this->velocityX = lerp(current, goal, dt);

    if (std::abs(this->velocityX) < std::abs(current)) {
        this->decelerating = true;
    } else {
        this->decelerating = false;
    }
}

void Mario::updateVerticalVelocity() {
    if (this->isJumping) {
        if (this->getY() - (this->velocityY - this->gravity) >= ground) {
            this->velocityY = this->getY() - ground;
            this->overlap = true;
        } else
            this->velocityY -= this->gravity;
    }
}

void Mario::jump() {
    this->isJumping = true;
}

void Mario::resetY() {
    if (this->getY() == ground) {
        this->isJumping = false;
        this->velocityY = initialVelocityY;
        this->gravity = initialGravity;
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

float Mario::lerp(float current, float goal, float dt) {
    if (current + dt < goal) return current + dt;

    if (goal < current) return current - dt;

    return goal;
}

float Mario::getVelocity() { return this->velocityX; }

float Mario::getX() { return this->getPosition().x; }

float Mario::getY() { return this->getPosition().y; }

int Mario::getWidth() { return this->width; }

int Mario::getHeight() { return this->height; }

bool Mario::marioIsFreezed() { return this->freezeMario; }

bool Mario::isOverlaping() { return this->overlap; }

bool Mario::hasHitEnnemy() { return this->hitEnnemy; }

sf::Sprite Mario::getSprite() { return this->sprite; }
