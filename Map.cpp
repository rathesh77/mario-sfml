#include "Map.hpp"

Map::Map()
{
}

Map::Map(int numberOfGrids)
{
    this->grids = new Grid*[numberOfGrids];
    for (int i= 0 ; i < numberOfGrids; i++) {
        this->grids[i] = new Grid;
    }
    this->NB_GRIDS = numberOfGrids;
}

Grid *Map::getNthGrid(int nth)
{
    return this->grids[nth];
}

int Map::getNumberOfGrids()
{
    return this->NB_GRIDS;
}

void Map::addObjectToCurrentGrid(Grid *grid, Object *e)
{

    if (!grid->object)
    {
        grid->object = e;
    }
    else
    {
        if (!grid->last)
        {
            grid->object->next = e;

            grid->last = e;
        }
        else
        {
            grid->last->next = e;
            grid->last = e;
        }
    }
    grid->NB_SPRITES++;
}
