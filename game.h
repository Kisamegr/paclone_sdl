#pragma once
#include "sbzwhakman.h"
#include "SBZLibraryScope.h"
#include "tile.h"
#include "whakman.h"
#include "map.h"
#include "ghost.h"

class Game
{

  int m_screen_size_x;
  int m_screen_size_y;
  int m_score;

  Map *m_map;
  std::vector<Actor*> m_actors;

  IFont *m_font;

public:
  Game(ISBZLibrary *lib, const int &size, const int &screen_size_x, const int &screen_size_y);
  ~Game();

  void update(const float &dt);
  void draw(const float &x_offset, const float &y_offset);
};

