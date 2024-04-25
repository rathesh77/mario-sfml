#ifndef MAP_PARSER_H
#define MAP_PARSER_H

#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>

#include "Constants.hpp"
#include "Map.hpp"

class MapParser {
 public:
  void parse();
  static MapParser *create(std::string);
  Map *map;

 private:
  MapParser(std::string);
  std::string filename;
  std::ifstream *ifstream;
  int parseNthGrid(std::string, int &);
  Object *parseObject(std::string, int &);
  std::string parseObjectType(std::string, int &);
  float parseObjectPosition(std::string, int &);
  void skipBlanks(std::string, int &);
};

#endif
