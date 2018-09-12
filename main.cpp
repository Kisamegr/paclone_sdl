#include <iostream>
#include <vector>
#include <Windows.h>
#include "sbzwhakman.h"
#include "SBZLibraryScope.h"
#include "game.h"

#define MAP_SIZE 13
#define TILE_SIZE 64

#define X_OFFSET 0
#define Y_OFFSET 50

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR cmd, int nshow) {
  auto *lib = SBZLibraryScope::shared()->library();

  // Calculate the screen size
  int screen_size_x = MAP_SIZE * TILE_SIZE + X_OFFSET;
  int screen_size_y = MAP_SIZE * TILE_SIZE + Y_OFFSET;

  lib->init(screen_size_x, screen_size_y);

  // Create the Game object
  Game game(MAP_SIZE, TILE_SIZE, screen_size_x, screen_size_y);

  // Update...
  float prev_time = lib->time();
  while (lib->update()) {
    float cur_time = lib->time();
    float dt = cur_time - prev_time;
    prev_time = cur_time;

    game.update(dt);
    game.draw(X_OFFSET, Y_OFFSET);
  }

  lib->destroy();
  return 0;
}
