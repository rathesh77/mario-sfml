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
    void shiftSceneBackward();

private:
    int currentMap;
    int NB_BACKGROUNDS = 30;
    int SINGLE_BACKGROUND_WIDTH = 254;

    std::string backgroundPath = "sprites/background2.png";
    std::string brickPath = "sprites/bricks.png";

    sf::Sprite s_background[30];
    sf::Sprite s_brick;

    sf::Texture t_background;
    sf::Texture t_brick;

    sf::RenderWindow *window;

    Mario *mario;

    void drawSprites();
};