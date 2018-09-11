#pragma once
#include <vector>
#include <map>
#include "sbzwhakman.h"
#include "tile.h"

class Map {
public:
  enum Wall {
    WALL_CROSS,
    WALL_END,
    WALL_STRAIGHT,
    WALL_T,
    WALL_TURN,
    WALL_NONE
  };


  Map(const int &width, const int &height);
  ~Map();

  void draw();

  int getValue(const int &x, const int &y);
  void setValue(const int &x, const int &y, const int &value);

  void generateTiles();

  void wallFromNeighbors(const int &x, const int &y, Wall &wall, int &rot);

private:
  int m_width;
  int m_height;

  int **m_map;
  Tile **m_tiles;

  std::vector<IImage*> wallImages;


};

