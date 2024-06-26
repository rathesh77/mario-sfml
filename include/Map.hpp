
#ifndef MAP__H
#define MAP__H
#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>

#include "Constants.hpp"

struct Object {
    std::string type;                            // brick, koopa etc...
    sf::Vector2f position = sf::Vector2f(0, 0);  // coordinates

    Object *next = NULL;
};
struct Grid {
    int id = -1;
    Object *object = NULL;
    Object *last = NULL;
    int NB_SPRITES = 0;
};

class Map {
   public:
    Map();

    Map(int);
    Grid *getNthGrid(int);
    void addObjectToCurrentGrid(Grid *, Object *);
    int getNumberOfGrids();

   private:
    Grid **grids;
    int m_nb_grids;
};

#endif
