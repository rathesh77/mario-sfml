
#ifndef EVENTS__H
#define EVENTS__H

#include "Body.hpp"

enum EVENT_TYPES { GOOMBA_KILLED, MARIO_DIED, LEVEL_COMPLETED, COIN_COLLECTED, UNKNOWN_BRICK_HIT };

class Event {
 public:
  Event(EVENT_TYPES);
  Event(EVENT_TYPES, SpriteObject *);

  EVENT_TYPES getType();
  SpriteObject* getTarget();

 private:
  EVENT_TYPES m_type;
  SpriteObject *m_target;

};

#endif
