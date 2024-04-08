
#ifndef MAP__H
#define MAP__H
#include <SFML/Graphics.hpp>
#include "Constants.hpp"
#include <iostream>
#include <fstream>

struct Element
{
    std::string type;      // brick, koopa etc...
    sf::Vector2f position = sf::Vector2f(0,0); // coordinates

    Element *next = NULL;
};
struct Grid
{
    int id = -1;
    Element *element = NULL;
    Element *last = NULL;
    int NB_SPRITES = 0;
};

class Map
{
public:
    Map();

    Map(int);
    Grid *getNthGrid(int);
    void addElementToCurrentGrid(Grid *, Element*);
    int getNumberOfGrids();

private:
    Grid **grids;
    int NB_GRIDS;

};

#endif
