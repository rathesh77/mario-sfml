#ifndef MAP_PARSER_H
#define MAP_PARSER_H

#include <fstream>
#include <iostream>
#include <SFML/Graphics.hpp>

#include "Constants.hpp"
#include "Map.hpp"

class MapParser
{
public:
    
    void parse();
    static MapParser *create(std::string);
    Map *map;

private:

    MapParser(std::string);
    std::string filename;
    std::ifstream *ifstream;
    int parseNthGrid(std::string, int&);
    Element* parseElement(std::string,int&);
    std::string parseElementType(std::string, int&);
    float parseElementPosition(std::string, int&);
    void skipBlanks(std::string, int&);

};

#endif
