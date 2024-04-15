#include "Ground.hpp"

#include <iostream>

Ground::Ground(sf::Texture*texture, float x, float y, float rectX, float rectY, int width, int height, std::string type) : Body(texture, x, y, rectX, rectY, width, height, type) {

    this->m_direction = 0;
    this->m_velocityX = 0.0f;
    this->m_accOffset = 0.0f;
    this->m_maxVelocityX = 0.0f;

}


void Ground::detectCollisions(SpriteObject *s_objects) {

}



void Ground::postCollisionsDetection() {

}