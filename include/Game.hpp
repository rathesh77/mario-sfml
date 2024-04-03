#include <SFML/Graphics.hpp>
#include "Mario.hpp"
#include "Constants.hpp"

class Game
{
public:
    Game(sf::RenderWindow*);
    
    int getCurrentMap();
    void tick();

private:
    int currentMap;
    std::string backgroundPath = "sprites/background.png";

    sf::Sprite sprite;
    sf::Texture backgroundTexture;
    sf::RenderWindow *window;

    Mario *mario;
    void drawSprites();

};