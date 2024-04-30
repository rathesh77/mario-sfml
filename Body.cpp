#include "Body.hpp"

#include <iostream>

Body::Body() {
  this->m_direction = 0;
  this->m_velocityX = 0.0f;
  this->m_accOffset = 0.0f;
  this->m_maxVelocityX = 0.0f;
  this->m_accOffset = 0;
  this->m_sprite = sf::Sprite();
  this->m_sprite.setPosition(0, WINDOW_HEIGHT + (TILE_DIMENSION * 2));
}

Body::Body(sf::Texture *texture, float posX, float posY, float rectX,
           float rectY, int width, int height, std::string type) {
  this->m_type = type;

  this->m_texture = texture;

  this->m_width = width;
  this->m_height = height;

  this->m_sprite = sf::Sprite(*this->m_texture);
  this->m_sprite.setTextureRect(sf::IntRect(rectX, rectY, width, height));
  this->m_sprite.setPosition(posX, posY);
  this->realCoordinates.x += this->getX();
  m_ground = WINDOW_HEIGHT + 16;
}

void Body::loop(SpriteObject *s_objects) {
  if (this->isOverlaping()) {
    this->m_overlap = false;
  }
  this->updateHorizontalVelocity();
  this->updateVerticalVelocity();
  this->handleCollision(s_objects);

  this->moveX();
  this->moveY();
  this->resetY();
}

sf::Vector2f Body::getPosition() { return this->m_sprite.getPosition(); }

void Body::setDirectionX(int direction) {
  if (direction != 0)
    this->m_lookingRight = direction == 1 ? true : false;
  
  if (!this->m_lookingRight && !this->m_flipped) {
    this->m_sprite.scale(-1.0f, 1.0f);
    this->m_sprite.setOrigin(this->m_width, 0);
    this->m_flipped = true;
  } else if (this->m_flipped && this->m_lookingRight) {
    this->m_sprite.scale(-1.0f, 1.0f);
    this->m_sprite.setOrigin(0, 0);
    this->m_flipped = false;
  }

  this->m_direction = direction;
}

int Body::getDirection() { return this->m_direction; }

std::map<std::string, std::vector<SpriteObject *>> Body::detectCollisions(
    SpriteObject *s_objects) {
  int i = 0;
  bool hit = false;

  std::map<std::string, std::vector<SpriteObject *>> collidedObjects = {
      {"up", std::vector<SpriteObject *>{}},
      {"down", std::vector<SpriteObject *>{}},
      {"side", std::vector<SpriteObject *>{}}};

  Body nextPos = Body();
  nextPos.m_sprite.setPosition(
      this->getPosition() +
      sf::Vector2f(this->m_velocityX, -this->m_velocityY));
  nextPos.m_width = this->m_width;
  nextPos.m_height = this->m_height;

  while (true) {
    if (s_objects->type == "NULL" || s_objects->type == "") break;
    if (compare(this, s_objects->body)) {
      // two of the same type of body cannot collide each other
      s_objects++;
      continue;
    }
    sf::Vector2f objectPos = s_objects->body->getPosition();

    if (this->collides(&nextPos, s_objects->body)) {
      hit = true;

      if (this->getY() + this->m_height <= objectPos.y) {
        if (this->getX() + this->m_width >= objectPos.x + 2 &&
            this->getX() <= objectPos.x + s_objects->body->m_width - 2)
          collidedObjects["up"].push_back(s_objects);
      } else if (this->getY() >= objectPos.y + s_objects->body->m_height) {
        collidedObjects["down"].push_back(s_objects);
      } else {
        collidedObjects["side"].push_back(s_objects);
      }
    }
    s_objects++;
    i++;
  }
  if (!hit) {
    m_overlap = false;
    m_ground = WINDOW_HEIGHT + 16;
    this->m_isJumping = true;
  } else {
    m_overlap = true;
  }
  return collidedObjects;
}

void Body::handleCollision(SpriteObject *s_objects) {
  std::map<std::string, std::vector<SpriteObject *>> collidedObjects =
      this->detectCollisions(s_objects);

  for (SpriteObject *object : collidedObjects["up"]) {
    sf::Vector2f objectPos = object->body->getPosition();

    m_ground = objectPos.y;
    this->m_velocityY = this->getY() - (objectPos.y - this->m_height);
  }
  for (SpriteObject *object : collidedObjects["down"]) {
    if (this->m_velocityY > 0) this->m_velocityY = -0.0f;
  }
  for (SpriteObject *object : collidedObjects["side"]) {
    this->m_direction = -this->m_direction;
    this->m_velocityX = 0;
  }
}

bool Body::compare(Body *a, Body *b) { return a == b; }

bool Body::collides(Body *a, Body *b) {
  return a->getX() >= b->getX() - a->m_width + 2 &&
         a->getX() <= b->getX() + b->m_width - 2 &&
         a->getY() >= b->getY() - a->m_height &&
         a->getY() <= b->getY() + b->m_height;
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
  if (this->m_isJumping)
    this->m_velocityY -= this->m_gravity;
}

void Body::jump() {
  if (m_jumpEnabled) {
    this->m_isJumping = true;
    this->m_velocityY = this->m_initialVelocityY;
    this->m_jumpEnabled = false;
  }
}

void Body::resetY() {
  if (this->getY() + this->m_height == m_ground && this->m_velocityY != this->m_initialVelocityY) {
    this->m_isJumping = false;
    this->m_velocityY = 0;
    m_jumpEnabled = true;
  }
}

float Body::lerp(float current, float goal, float dt) {
  if (current + dt < goal) return current + dt;

  if (goal < current) return current - dt;

  return goal;
}

float Body::getVelocityX() { return this->m_velocityX; }

float Body::getX() { return this->getPosition().x; }

float Body::getY() { return this->getPosition().y; }

bool Body::isOverlaping() { return this->m_overlap; }

sf::Sprite *Body::getSprite() { return &this->m_sprite; }
