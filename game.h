#pragma once
#include "sbzwhakman.h"
#include "SBZLibraryScope.h"
#include "tile.h"
#include "whakman.h"
#include "map.h"
#include "ghost.h"

class Game
{
  bool m_running;
  bool m_won;

  int m_screen_size_x;
  int m_screen_size_y;
  int m_score;

  Map *m_map;

  Whakman *m_player;
  std::vector<Actor*> m_ghosts;

  IFont *m_small_font;
  IFont *m_big_font;

public:
  Game(ISBZLibrary *lib, const int &size, const int &screen_size_x, const int &screen_size_y);
  ~Game();

  void update(const float &dt);
  void draw(const float &x_offset, const float &y_offset);

  void game_won();
  void game_lost();
};

