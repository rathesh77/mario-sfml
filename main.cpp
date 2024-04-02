#include <SFML/Graphics.hpp>
#include <iostream>

#include "Game.hpp"

#define WINDOW_WIDTH 960
#define WINDOW_HEIGHT 600

int main()
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML window");

    Game game(&window);

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