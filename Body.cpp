#include "Body.hpp"

#include <iostream>

Body::Body() {}

Body::Body(std::string spritePath, float x, float y) {
    this->m_spritePath = spritePath;

    if (!this->m_texture.loadFromFile(
            this->m_spritePath,
            sf::IntRect(0, 16, this->m_width, this->m_height)))
        throw std::invalid_argument("Could not load mario texture");

    this->m_sprite = sf::Sprite(this->m_texture);
    this->m_sprite.setPosition(x, y);
    this->realCoordinates.x += this->getX();
    m_ground = WINDOW_HEIGHT + 16;
}

void Body::loop(SpriteObject *s_objects) {
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
        this->m_velocityX = -1.0;
        this->m_overlap = false;
    }
}

sf::Vector2f Body::getPosition() { return this->m_sprite.getPosition(); }

void Body::setDirectionX(int direction) {
    if (direction != 0) {
        this->m_lookingRight = direction == 1 ? true : false;
    }

    if (!this->m_lookingRight && !this->m_flipped) {
        // this->sprite.scale(-1.0f, 1.0f);
        // this->sprite.move(this->width, 0);

        this->m_flipped = true;
    } else if (this->m_flipped && this->m_lookingRight) {
        // this->sprite.scale(-1.0f, 1.0f);
        // this->sprite.move(- this->width,0);

        this->m_flipped = false;
    }

    this->m_direction = direction;
}

int Body::getDirection() { return this->m_direction; }

void Body::detectCollisions(SpriteObject *s_objects) {
    int i = 0;
    bool hit = false;

    this->m_overlap = false;
    while (true) {
        if (s_objects->type == "NULL" || s_objects->type == "") break;
        if (s_objects->type == "brick" || s_objects->type == "ground") {
            sf::Vector2f objectPos = s_objects->sprite->getPosition();
            if (this->collides(
                    this->getPosition() +
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
                    } else {
                        std::cout << "upward" << std::endl;
                        // force upward
                        m_ground =
                            s_objects->sprite->getPosition().y - TILE_DIMENSION;
                        this->m_velocityY = this->getY() - m_ground;
                    }
                } else {
                    this->m_velocityX = 0;
                }
            }
        }
        s_objects++;
        i++;
    }
}

void Body::postCollisionsDetection() {
    if (!this->m_isJumping) {
        // std::cout << "not hit" << std::endl;
        this->m_velocityY = 0;
        this->m_isJumping = true;
    }

    m_ground = WINDOW_HEIGHT + 16;
}

bool Body::collides(sf::Vector2f a, sf::Vector2f b) {
    return a.x >= b.x - TILE_DIMENSION + 2 && a.x <= b.x + TILE_DIMENSION - 2 &&
           a.y >= b.y - TILE_DIMENSION && a.y <= b.y + TILE_DIMENSION;
}
void Body::moveX()  // no collisions handling here. Only moving sprite
{
    this->realCoordinates.x += this->m_velocityX;

    this->m_sprite.move(sf::Vector2f(this->m_velocityX, 0));
}

void Body::moveY() {
    if (this->m_isJumping) {
        this->m_sprite.move(sf::Vector2f(0, -this->m_velocityY));
    }
}
void Body::updateHorizontalVelocity() {
    float goal = this->m_maxVelocityX * this->m_direction;
    float dt = this->m_accOffset;
    float current = this->m_velocityX;
    this->m_velocityX = lerp(current, goal, dt);

    if (std::abs(this->m_velocityX) < std::abs(current)) {
        this->m_decelerating = true;
    } else {
        this->m_decelerating = false;
    }
}

void Body::updateVerticalVelocity() {
    if (this->m_isJumping) {
        if (this->getY() - (this->m_velocityY - this->m_gravity) >= m_ground) {
            this->m_velocityY = this->getY() - m_ground;
            this->m_overlap = true;
        } else
            this->m_velocityY -= this->m_gravity;
    }
}

void Body::jump() { this->m_isJumping = true; }

void Body::resetY() {
    if (this->getY() == m_ground) {
        this->m_isJumping = false;
        this->m_velocityY = m_initialVelocityY;
        this->m_gravity = m_initialGravity;
    }
}

float Body::lerp(float current, float goal, float dt) {
    if (current + dt < goal) return current + dt;

    if (goal < current) return current - dt;

    return goal;
}

float Body::getVelocity() { return this->m_velocityX; }

float Body::getX() { return this->getPosition().x; }

float Body::getY() { return this->getPosition().y; }

int Body::getWidth() { return this->m_width; }

int Body::getHeight() { return this->m_height; }

bool Body::isOverlaping() { return this->m_overlap; }

sf::Sprite *Body::getSprite() { return &this->m_sprite; }
