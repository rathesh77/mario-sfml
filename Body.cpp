#include "Body.hpp"

#include <iostream>

Body::Body() {}

Body::Body(std::string spritePath, float x, float y) {
    this->spritePath = spritePath;

    if (!this->texture.loadFromFile(
            this->spritePath, sf::IntRect(0, 16, this->width, this->height)))
        throw std::invalid_argument("Could not load mario texture");

    this->sprite = sf::Sprite(this->texture);
    this->sprite.setPosition(x, y);
    this->realCoordinates.x += this->getX();
    ground = BOUNDING_Y_BOTTOM;
}

void Body::loop(SpriteObject *s_objects, int NB_SPRITES) {
    if (this->isOverlaping()) {
        // this->overlap = false;
    } else {
        this->updateHorizontalVelocity();
        this->updateVerticalVelocity();
        this->postCollisionsDetection();

        this->detectCollisions(s_objects);
    }
    this->moveX();
    this->moveY();

    this->resetY();
    if (this->isOverlaping()) {
        this->velocityX = -1.0;
        this->overlap = false;
    }
}

sf::Vector2f Body::getPosition() { return this->sprite.getPosition(); }

void Body::setDirectionX(int direction) {
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

int Body::getDirection() { return this->direction; }

void Body::detectCollisions(SpriteObject *s_objects) {
    int i = 0;
    bool hit = false;

    this->overlap = false;
    while (true) {
        if (s_objects->type == "NULL" || s_objects->type == "" )
            break;
        if (s_objects->type == "brick") {
            sf::Vector2f objectPos = s_objects->sprite->getPosition();
            if (this->collides(
                    this->getPosition() +
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
                    } else {
                        std::cout << "upward" << std::endl;
                        // force upward
                        ground =
                            s_objects->sprite->getPosition().y - TILE_DIMENSION;
                        this->velocityY = this->getY() - ground;
                    }
                } else {
                    this->velocityX = 0;
                }
            }
        }
        s_objects++;
        i++;
    }
}

void Body::postCollisionsDetection() {
    if (!this->isJumping) {
        //std::cout << "not hit" << std::endl;
        this->velocityY = 0;
        this->isJumping = true;
    }

    ground = BOUNDING_Y_BOTTOM;
}

bool Body::collides(sf::Vector2f a, sf::Vector2f b) {
    return a.x >= b.x - TILE_DIMENSION + 2 && a.x <= b.x + TILE_DIMENSION - 2 &&
           a.y >= b.y - TILE_DIMENSION && a.y <= b.y + TILE_DIMENSION;
}
void Body::moveX()  // no collisions handling here. Only moving sprite
{
    this->realCoordinates.x += this->velocityX;

    this->sprite.move(sf::Vector2f(this->velocityX, 0));
}

void Body::moveY() {
    if (this->isJumping) {
        this->sprite.move(sf::Vector2f(0, -this->velocityY));
    }
}
void Body::updateHorizontalVelocity() {
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

void Body::updateVerticalVelocity() {
    if (this->isJumping) {
        if (this->getY() - (this->velocityY - this->gravity) >= ground) {
            this->velocityY = this->getY() - ground;
            this->overlap = true;
        } else
            this->velocityY -= this->gravity;
    }
}

void Body::jump() { this->isJumping = true; }

void Body::resetY() {
    if (this->getY() == ground) {
        this->isJumping = false;
        this->velocityY = initialVelocityY;
        this->gravity = initialGravity;
    }
}

float Body::lerp(float current, float goal, float dt) {
    if (current + dt < goal) return current + dt;

    if (goal < current) return current - dt;

    return goal;
}

float Body::getVelocity() { return this->velocityX; }

float Body::getX() { return this->getPosition().x; }

float Body::getY() { return this->getPosition().y; }

int Body::getWidth() { return this->width; }

int Body::getHeight() { return this->height; }

bool Body::isOverlaping() { return this->overlap; }

sf::Sprite *Body::getSprite() { return &this->sprite; }
