#include <SFML/Graphics.hpp>

#include "Body.hpp"
#include "Constants.hpp"

class Ground: public Body {
   public:

    Ground(sf::Texture*, float, float, float, float, int, int, std::string);

   std::map<std::string, std::vector<SpriteObject *>>  detectCollisions(SpriteObject*);
   void handleCollision(SpriteObject *);
   void postCollisionsDetection();

   private:

};