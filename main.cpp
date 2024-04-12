#include <SFML/Graphics.hpp>
#include <iostream>

#include "Constants.hpp"
#include "Game.hpp"
#include "MapParser.hpp"

int main() {
    MapParser *mp = MapParser::create("map1.txt");
    if (mp) {
        mp->parse();
    } else {
        return EXIT_FAILURE;
    }

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),
                            "Super Mario Bros");
    window.setFramerateLimit(40);
    window.setKeyRepeatEnabled(true);
    window.setSize(sf::Vector2u(1200, 400));
    Game game = Game(&window);
    Map *m = mp->map;
    game.loadMap(m);

    sf::Clock clock;
    while (window.isOpen()) {
        window.clear();
        game.tick(&clock);

        window.display();
    }

    return EXIT_SUCCESS;
}