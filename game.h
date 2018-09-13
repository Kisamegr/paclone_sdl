#pragma once
#include "sbzwhakman.h"
#include "SBZLibraryScope.h"
#include "tile.h"
#include "whakman.h"
#include "map.h"
#include "ghost.h"

/// <summary>
/// Creates, updates and draws the map, player and enemies.
/// Holds and updates the current player score.
/// Checks if the game has ended either by the player 
/// eating all the food, or the ghosts catching him 
/// <summary>
class Game
{
  bool m_running;                 // True if the game is currently running (captain obvious)
  bool m_won;                     // True if the player won, false if he lost (captain obvious x2)
  int m_score;                    // The player's current score

  int m_screen_width;             // The actual screen width in pixels
  int m_screen_height;            // The actual screen height in pixels

  Map *m_map;                     // The Map object holding the map data, tiles, food

  Whakman *m_player;              // The player playing the game
  std::vector<Actor*> m_ghosts;   // The list of player enemies

  IFont *m_small_font;            // Used to render small text on screen!
  IFont *m_big_font;              // Used to render BIG text on screen!

  // Resets the g_map data into the map and re-creates the tiles
  void initialize_map();

public:

  // Contructor 
  Game(const int &map_size, const int &tile_size, const int &screen_width, const int &screen_height);
  ~Game();

  // Updates all the components in order
  void update(const float &dt);

  // Draws all the components in order
  void draw(const float &x_offset, const float &y_offset);

  // Called when the player ate all the food
  void game_won();

  // Called when a ghost catches the player
  void game_lost();

  // Reset the map, actors and score
  void reset_game();
};

