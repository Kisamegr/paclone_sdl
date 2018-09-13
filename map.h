#pragma once
#include <vector>
#include <map>
#include "sbzwhakman.h"
#include "tile.h"

/// <summary>
/// Creates, updates and renders the game's map
/// Generates the wall and the food tiles
/// <summary>
class Map {
public:
  // Describes the type of data the map can store
  enum Space {
    SPACE_EMPTY, 
    SPACE_WALL,
    SPACE_FOOD
  };

  // Describes the type of walls that can be rendered
  enum Wall {
    WALL_CROSS,
    WALL_END,
    WALL_STRAIGHT,
    WALL_T,
    WALL_TURN,
    WALL_NONE
  };

private:
  int m_width;      // The map width in tiles
  int m_height;     // The map height in tiles
  int m_tile_size;  // A tile's pixel size
  int m_food_count; // The remaining food count

  int **m_map;      // The 2D array that holds the map space data 

  std::vector<Tile> m_wall_tiles;      // The created wall tiles
  std::vector<IImage*> m_wall_images;  // The unique images for each wall type
  IImage *m_food_image;                // The sole food image

  // Calculates which wall type should be rendered, and in what rotation 
  // in the given coordinates based on the neighbor spaces
  void wall_from_neighbors(const int &x_coord, const int &y_coord, Wall &wall, int &rot);

public:
  // Constructor receiving the map width & height, and the tile size as arguments
  Map(const int &width, const int &height, const int &tile_size);
  ~Map();

  // Draws the wall and the food images (the order does not matter here)
  void draw(const float &x_offset, const float &y_offset) const;

  // Sets the space value to the given coordinates
  void set_value(const int &xCoord, const int &yCoord, const Space &value);

  // Gets the space value for the given coordinates
  Space get_value(const int &xCoord, const int &yCoord) const;

  // Gets the space value for the given pixel position
  Space get_value_position(const float &x, const float &y) const;

  // Gets the space value for the given direction based on the given coordinates
  Space get_value_direction(const Direction &direction, const int &xCoord, const int &yCoord) const;

  // Reads the map data and creates the wall tiles
  void create_wall_tiles();

  // Reads the map data, and changes the empty spaces to food spaces
  void create_food();

  // Checks and returns true if the given coordinates align with a food space
  // If yes, it sets that food space as empty and decreases the remaining food
  bool check_eating_food(const int &x_coord, const int &y_coord);

  // Basic getters
  int width() const;
  int height() const;
  int food_count() const;
  int tile_size() const;
};

