#include <SFML/Graphics.hpp>

#include "Body.hpp"
#include "Constants.hpp"

class Ground: public Body {
   public:

    Ground(sf::Texture*, float, float, float, float, int, int, std::string);

    void detectCollisions(SpriteObject*);
   void postCollisionsDetection();

   private:

};