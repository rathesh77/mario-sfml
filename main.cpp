#include <SFML/Graphics.hpp>
#include <iostream>

#include "Constants.hpp"
#include "Game.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML window");
    window.setFramerateLimit(30);
    Game game = Game(&window);
    
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();
        game.tick();

        window.display();
    }

    return EXIT_SUCCESS;
}