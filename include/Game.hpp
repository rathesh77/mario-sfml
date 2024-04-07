#include <SFML/Graphics.hpp>
#include "Mario.hpp"
#include "Constants.hpp"

class Game
{
public:
    int frameCount = 0;
    
    Game(sf::RenderWindow*);

    int getCurrentMap();
    void tick(sf::Clock *);
    bool isInsideLimits(sf::Vector2f);

private:
    int currentMap;
    std::string backgroundPath = "sprites/background.png";

    sf::Sprite s_background;
    sf::Sprite s_brick;

    sf::Texture t_background;
    sf::Texture t_brick;



    sf::RenderWindow *window;

    Mario *mario;
    void drawSprites();

};