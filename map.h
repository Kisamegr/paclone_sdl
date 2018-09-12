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


  void setValue(const int &xCoord, const int &yCoord, const int &value);
  int getValue(const int &xCoord, const int &yCoord) const;
  int getValuePosition(const float &x, const float &y) const;
  int getValueDirection(const Direction &direction, const int &xCoord, const int &yCoord) const;



  void generateTiles();

  bool reachedCell(const int &x, const int &y) const;

private:
  int m_width;
  int m_height;

  int **m_map;
  Tile **m_tiles;

  std::vector<IImage*> wallImages;

  void wallFromNeighbors(const int &xCoord, const int &yCoord, Wall &wall, int &rot);

};

