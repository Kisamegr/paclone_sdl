#pragma once
#include <vector>
#include <map>
#include "sbzwhakman.h"
#include "tile.h"

class Map {
public:
  enum Space {
    SPACE_EMPTY, 
    SPACE_WALL,
    SPACE_FOOD
  };

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

  void draw(const float &x_offset, const float &y_offset);


  void setValue(const int &xCoord, const int &yCoord, const Space &value);
  Space getValue(const int &xCoord, const int &yCoord) const;
  Space getValuePosition(const float &x, const float &y) const;
  Space getValueDirection(const Direction &direction, const int &xCoord, const int &yCoord) const;

  void generateTiles();
  void generateFood();

  bool checkEatingFood(const int &xCoord, const int &yCoord);
  int get_food_count();

private:
  int m_width;
  int m_height;

  int m_food_count;

  int **m_map;

  std::vector<Tile> m_wallTiles;
  std::vector<IImage*> m_wallImages;
  IImage *m_foodImage;

  void wallFromNeighbors(const int &xCoord, const int &yCoord, Wall &wall, int &rot);

};

