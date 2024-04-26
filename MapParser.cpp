#include "MapParser.hpp"

MapParser *MapParser::create(std::string filename) {
  std::ifstream file;
  file.open(filename);

  if (file.is_open()) {
    file.close();
    return new MapParser(filename);
  }
  return NULL;
}
MapParser::MapParser(std::string filename) {
  this->map = new Map(20);
  this->filename = filename;
}

void MapParser::parse() {
  std::ifstream file(this->filename);
  std::string line;
  Grid *currentGrid;
  while (std::getline(file, line)) {
    int start = 0;
    skipBlanks(line, start);
    if (line.length() == 0 || line[start] == 0) {
      continue;
    }
    if (line[start] == '#') {
      int currentGridId = parseNthGrid(line, start);
      currentGrid = this->map->getNthGrid(currentGridId);
    } else if (line[start] != '-') {
      Object *e = new Object;
      e = parseObject(line, start);
      this->map->addObjectToCurrentGrid(currentGrid, e);
    }
  }
  file.close();
}

int MapParser::parseNthGrid(std::string line, int &start) {
  std::string str = "";
  int num = 0;
  for (int i = start + 1; i < line.length(); i++) {
    char current = line[i];
    if (current >= 48 && current <= 57) {
      str += current;
      num = (num * 10) + ((int)current - 48);
    } else {
      throw std::invalid_argument("Invalid file header");
    }
  }

  return num;
}

Object *MapParser::parseObject(std::string line, int &start) {
  Object *object = new Object;

  skipBlanks(line, start);

  object->type = parseObjectType(line, start);

  float x = parseObjectPosition(line, start);
  float y = parseObjectPosition(line, start);

  object->position = sf::Vector2f(x, y);
  return object;
}

std::string MapParser::parseObjectType(std::string line, int &start) {
  skipBlanks(line, start);

  std::string type = "";
  while (line[start] != ' ') {
    char current = line[start];
    start++;
    if (current >= 97 && current <= 122) {
      type += current;
    } else {
      throw std::invalid_argument("Invalid type header");
    }
  };
  return type;
}

float MapParser::parseObjectPosition(std::string line, int &start) {
  skipBlanks(line, start);

  std::string str = "";
  float a = 0, b = 0;

  while (line[start] != ' ' && line[start] != '\0') {
    char current = line[start];
    start++;
    if (current >= 48 && current <= 57) {
      str += current;
    } else if (current == '*') {
      a = std::stof(str);
      str = "";
    } else {
      throw std::invalid_argument("Invalid position header");
    }
  };
  b = std::stof(str);

  return b * a;
}

void MapParser::skipBlanks(std::string line, int &start) {
  while (line[start] == ' ') {
    start++;
  };
}