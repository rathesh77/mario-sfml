#include <SFML/Graphics.hpp>
#include "Mario.hpp"

class Game
{
public:
    Game(sf::RenderWindow*);
    
    int getCurrentMap();
    void tick();

private:
    int current_map;
    int window_width;

    std::string backgroundPath = "sprites/background.jpg";

    sf::Sprite sprites[2];
    sf::Texture backgroundTexture;
    sf::RenderWindow *window;

    Mario mario;
    void drawSprites();

};