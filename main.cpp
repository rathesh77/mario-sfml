#include <SFML/Graphics.hpp>
#include <iostream>

#include "Constants.hpp"
#include "Game.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML window");
    window.setFramerateLimit(30);
    window.setKeyRepeatEnabled(true); 
    window.setSize(sf::Vector2u(1200, 400));
    Game game = Game(&window);
    
    while (window.isOpen())
    {
        window.clear();
        game.tick();

        window.display();
    }

    return EXIT_SUCCESS;
}