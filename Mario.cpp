#include "Mario.hpp"

#include <iostream>

Mario::Mario() {
  this->m_width = TILE_DIMENSION;
  this->m_height = TILE_DIMENSION;

  if (!this->m_t_texture.loadFromFile(
          this->m_spritePath))
    throw std::invalid_argument("Could not load mario frame one texture");

  this->m_sprite = sf::Sprite(this->m_t_texture);
  this->m_sprite.setPosition(10, BOUNDING_Y_BOTTOM);
  this->m_sprite.setTextureRect(sf::IntRect(0, 8,  this->m_width, this->m_height));

  this->realCoordinates.x += this->getX();
  this->m_direction = 0;
  this->m_velocityX = 0.0f;
  this->m_accOffset = 0.1f;
  this->m_maxVelocityX = 2.0f;
  m_ground = WINDOW_HEIGHT + 16;
}

std::vector<Event> Mario::loop(SpriteObject *s_objects) {
  if (this->isOverlaping()) {
    this->m_overlap = false;
    this->resetY();
  } else {
    this->updateHorizontalVelocity();
    this->updateVerticalVelocity();
    this->postCollisionsDetection();
  }
  std::vector<Event> events = this->handleCollision(s_objects);

  this->moveX();
  this->moveY();

  if (this->getY() >= WINDOW_HEIGHT) {
    events.push_back(Event(MARIO_DIED));
  }
  return events;
}

std::vector<Event> Mario::handleCollision(SpriteObject *s_objects) {
  std::vector<Event> events = {};
  std::map<std::string, std::vector<SpriteObject *>> collidedObjects =
      this->detectCollisions(s_objects);
  for (SpriteObject *object : collidedObjects["up"]) {
    std::cout << "upward:" + object->type << std::endl;

    sf::Vector2f objectPos = object->body->getPosition();

    if (object->type == "goomba") {
      delete object->body;
      object->body = new Body();
      this->m_velocityY = 1;
      events.push_back(Event(GOOMBA_KILLED));
    } else {
      m_ground = objectPos.y;
      this->m_velocityY = this->getY() - (objectPos.y - this->m_height);
    }
  }
  for (SpriteObject *object : collidedObjects["down"]) {
    std::cout << "downward:" + object->type << std::endl;

    // if (this->m_velocityY > 0)
    this->m_velocityY = 0.0f;

    if (object->type == "goomba") {
      events.push_back(Event(MARIO_DIED));
    } else if (object->type == "unknownbrick") {
      events.push_back(Event(UNKNOWN_BRICK_HIT, object));

    }
  }
  for (SpriteObject *object : collidedObjects["side"]) {
    std::cout << "sideways:" + object->type << std::endl;
    /*

        std::cout << "MarioPosPrevious:"+
       std::to_string((this->getPosition()).x) + "/" +
       std::to_string((this->getPosition()).y) << std::endl;

        std::cout << "MarioPosNext:"+ std::to_string((this->getPosition() +
            sf::Vector2f(this->m_velocityX, -this->m_velocityY)).x) + "/" +
       std::to_string((this->getPosition() + sf::Vector2f(this->m_velocityX,
       -this->m_velocityY)).y) << std::endl;

        std::cout << "obj:"+ std::to_string(s_objects->body->getX()) + "/" +
       std::to_string(s_objects->body->getY())<< std::endl;
    */
    if (this->getX() > object->body->getX()) {
      this->m_velocityX = 0.2;
    } else {
      this->m_velocityX = -0.2;
    }
    if (object->type == "goomba") {
      events.push_back(Event(MARIO_DIED));
    }
  }
  return events;
}

void Mario::moveX() // no collisions handling here. Only moving sprite
{
  if (this->getX() + this->m_velocityX <= BOUNDING_X_LEFT &&
      (this->m_direction < 1 || this->m_decelerating)) {
    this->m_velocityX = 0.0f;
    this->m_sprite.setPosition(this->getX(), this->getY());
    std::cout << "out of map:" + std::to_string(this->getX()) << std::endl;
  } else {
    this->realCoordinates.x += this->m_velocityX;

    if ((this->getX() + this->getVelocityX() >= BOUNDING_X_MIDDLE)) {
      if (!this->m_decelerating && this->getDirection() < 1) {
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
        this->m_sprite.setTextureRect(sf::IntRect(0, 8, this->m_width, this->m_height));
      }
      if (frameCount % 2 == 0) {
        this->m_sprite.setTextureRect(sf::IntRect(20, 8, this->m_width, this->m_height));
      }
      if (frameCount % 3 == 0) {
        this->m_sprite.setTextureRect(sf::IntRect(38, 8, this->m_width, this->m_height));
        
      }
    }
}

bool Mario::marioIsFreezed() { return this->m_freezeMario; }