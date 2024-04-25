#include "Event.hpp"


Event::Event(EVENT_TYPES type) {

    this->m_type = type;
}


EVENT_TYPES Event::getType() {

    return this->m_type;
}

