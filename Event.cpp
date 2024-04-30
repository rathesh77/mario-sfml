#include "Event.hpp"

Event::Event(EVENT_TYPES type) { this->m_type = type; }
Event::Event(EVENT_TYPES type, SpriteObject *target) {
  this->m_type = type;
  this->m_target = target;
}

EVENT_TYPES Event::getType() { return this->m_type; }

SpriteObject *Event::getTarget() { return this->m_target; }
