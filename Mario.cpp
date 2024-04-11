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

void Mario::detectCollisions(sf::Sprite *s_objects, int count) {
    int i = 0;
    bool hit = false;

    this->overlap = false;
    while (i < count) {
        sf::Vector2f objectPos = s_objects->getPosition();
        if (this->collides(this->getPosition() +
                               sf::Vector2f(this->velocityX, -this->velocityY),
                           objectPos)) {
            hit = true;
            this->overlap = true;
            sf::Vector2f oppositeForce = this->getPosition() - objectPos;
            // std::cout << "opposite force :"+ std::to_string(oppositeForce.x)
            // + "/"+ std::to_string(oppositeForce.y) << std::endl;

            if (std::abs(oppositeForce.y) > std::abs(oppositeForce.x)) {
                if (oppositeForce.y >= 0) {  // force downward
                    std::cout << "downward" << std::endl;
                    // this->sprite.move(-this->velocityX, -this->velocityY);
                    if (this->velocityY > 0)
                        this->velocityY = -0.0f;
                    else
                        this->overlap = false;
                } else {
                    std::cout << "upward" << std::endl;
                    // force upward
                    ground = s_objects->getPosition().y - TILE_DIMENSION;
                    this->velocityY = this->getY() - ground;
                }
            } else {
                this->velocityX = 0;
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

    // willCollide = false;
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
    if (this->getY() == ground) {
        // std::cout<<"on ground"<<std::endl;
    }
    if (this->isJumping) {
        // std::cout<<"ground:"+ std::to_string(ground) <<std::endl;
        // std::cout<<"velocity1:"+ std::to_string(this->velocityY)<<std::endl;

        if (this->getY() - (this->velocityY - this->gravity) >= ground) {
            // sf::sleep(sf::seconds(10));
            this->velocityY = this->getY() - ground;
            this->overlap = true;
        } else
            this->velocityY -= this->gravity;

        if (this->getY() - this->velocityY >= ground) {
            // this->velocityY = this->getY() - ground;
            //  this->velocityY = initialVelocityY;
            //  this->isJumping = false;
            //  this->gravity = initialGravity;
        }
    }
    // std::cout<<"velocity2:"+ std::to_string(this->velocityY)<<std::endl;
}

void Mario::jump() {
    // if (this->getY() >= ground)
    this->isJumping = true;
}

void Mario::resetY() {
    if (this->getY() == ground) {
        /// std::cout<<"reset"<<std::endl;
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

sf::Sprite Mario::getSprite() { return this->sprite; }
