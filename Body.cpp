#include "Body.hpp"
#include <iostream>

Body::Body()
{
    this->m_direction = 0;
    this->m_velocityX = 0.0f;
    this->m_accOffset = 0.0f;
    this->m_maxVelocityX = 0.0f;
    this->m_accOffset = 0;
    this->m_sprite = sf::Sprite();
    this->m_sprite.setPosition(0, WINDOW_HEIGHT + (TILE_DIMENSION * 2));
}

Body::Body(sf::Texture *texture, float posX, float posY, float rectX,
           float rectY, int width, int height, std::string type)
{
    this->m_type = type;

    this->m_texture = texture;

    this->m_width = width;
    this->m_height = height;

    this->m_sprite = sf::Sprite(*this->m_texture);
    this->m_sprite.setTextureRect(
        sf::IntRect(rectX, rectY, width, height));
    this->m_sprite.setPosition(posX, posY);
    this->realCoordinates.x += this->getX();
    m_ground = WINDOW_HEIGHT + 16;
}

void Body::loop(SpriteObject *s_objects)
{
    if (this->isOverlaping())
    {
        // this->overlap = false;
    }
    else
    {
        this->updateHorizontalVelocity();
        this->updateVerticalVelocity();
        this->postCollisionsDetection();

        this->detectCollisions(s_objects);
    }
    this->moveX();
    this->moveY();

    this->resetY();
    if (this->isOverlaping())
    {
        // this->m_velocityX = -1.0;
        this->m_overlap = false;
    }
}

sf::Vector2f Body::getPosition() { return this->m_sprite.getPosition(); }

void Body::setDirectionX(int direction)
{
    if (direction != 0)
    {
        this->m_lookingRight = direction == 1 ? true : false;
    }

    if (!this->m_lookingRight && !this->m_flipped)
    {
        // this->sprite.scale(-1.0f, 1.0f);
        // this->sprite.move(this->width, 0);

        this->m_flipped = true;
    }
    else if (this->m_flipped && this->m_lookingRight)
    {
        // this->sprite.scale(-1.0f, 1.0f);
        // this->sprite.move(- this->width,0);

        this->m_flipped = false;
    }

    this->m_direction = direction;
}

int Body::getDirection() { return this->m_direction; }

void Body::detectCollisions(SpriteObject *s_objects)
{
    int i = 0;
    bool hit = false;

    while (true)
    {
        if (s_objects->type == "NULL" || s_objects->type == "")
            break;
        if (compare(this, s_objects->body))
        {
            // two of the same type of body cannot collide each other
            s_objects++;
            continue;
        }
        sf::Vector2f objectPos = s_objects->body->getPosition();

        if (this->collides(
                this->getPosition() +
                    sf::Vector2f(this->m_velocityX, -this->m_velocityY),
                objectPos, this->m_width, this->m_height, s_objects->body->getWidth(), s_objects->body->getHeight()))
        {
            hit = true;

            if (this->getY() + this->getHeight() <= objectPos.y)
            {

                m_ground = objectPos.y;
                this->m_velocityY = this->getY() - (objectPos.y - this->m_height);
            }
            else if (this->getY() >= objectPos.y + s_objects->body->getHeight())
            {
                if (this->m_velocityY > 0)
                    this->m_velocityY = -0.0f;
            }
            else
            {
                this->m_direction = -this->m_direction;
                this->m_velocityX = 0;
            }
        }
        s_objects++;
        i++;
    }
    if (!hit)
    {
        m_overlap = false;
        m_ground = WINDOW_HEIGHT + 16;
    }
    else
    {
        m_overlap = true;
    }
}

bool Body::compare(Body *a, Body *b) { return a == b; }

void Body::postCollisionsDetection()
{
    if (!this->m_isJumping)
    {
        this->m_velocityY = 0;
        this->m_isJumping = true;
    }

    // m_ground = WINDOW_HEIGHT + 16;
}

bool Body::collides(sf::Vector2f a, sf::Vector2f b, int aw, int ah, int bw, int bh)
{

    return a.x >= b.x - aw + 2 && a.x <= b.x + bw - 2 &&
           a.y >= b.y - ah && a.y <= b.y + bh;
}
void Body::moveX() // no collisions handling here. Only moving sprite
{
    this->realCoordinates.x += this->m_velocityX;

    this->m_sprite.move(sf::Vector2f(this->m_velocityX, 0));
}

void Body::moveY()
{
    if (this->m_isJumping)
    {
        this->m_sprite.move(sf::Vector2f(0, -this->m_velocityY));
    }
}
void Body::updateHorizontalVelocity()
{
    float goal = this->m_maxVelocityX * this->m_direction;
    float dt = this->m_accOffset;
    float current = this->m_velocityX;
    this->m_velocityX = lerp(current, goal, dt);

    if (std::abs(this->m_velocityX) < std::abs(current))
    {
        this->m_decelerating = true;
    }
    else
    {
        this->m_decelerating = false;
    }
}

void Body::updateVerticalVelocity()
{
    if (this->m_velocityY > -m_initialVelocityY)
        this->m_velocityY -= this->m_gravity;
}

void Body::jump() { this->m_isJumping = true; }

void Body::resetY()
{
    if (this->getY() + this->getHeight() == m_ground)
    {
        this->m_isJumping = false;
        this->m_velocityY = m_initialVelocityY;
    }
}

float Body::lerp(float current, float goal, float dt)
{
    if (current + dt < goal)
        return current + dt;

    if (goal < current)
        return current - dt;

    return goal;
}

float Body::getVelocity() { return this->m_velocityX; }

float Body::getX() { return this->getPosition().x; }

float Body::getY() { return this->getPosition().y; }

int Body::getWidth() { return this->m_width; }

int Body::getHeight() { return this->m_height; }

bool Body::isOverlaping() { return this->m_overlap; }

sf::Sprite *Body::getSprite() { return &this->m_sprite; }
