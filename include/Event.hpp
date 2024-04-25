
#ifndef EVENTS__H
#define EVENTS__H

enum EVENT_TYPES { GOOMBA_KILLED, MARIO_DIED, LEVEL_COMPLETED };

class Event {
 public:
  Event(EVENT_TYPES);
  EVENT_TYPES getType();

 private:
  EVENT_TYPES m_type;
};

#endif
