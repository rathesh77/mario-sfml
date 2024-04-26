#include <SFML/Graphics.hpp>

#include "Body.hpp"
#include "Constants.hpp"
#include "Event.hpp"

class Mario : public Body {
 public:
  Mario();
  std::vector<Event> loop(SpriteObject *);
  std::vector<Event> handleCollision(SpriteObject *s_objects);

  void moveX();

  void loadSpriteForward(int);

  bool marioIsFreezed();
  bool hasHitEnnemy();

 private:
  std::string m_spritePath = "sprites/sprites_sheet.png";

  sf::Texture m_t_texture;
  bool m_freezeMario = false;
};