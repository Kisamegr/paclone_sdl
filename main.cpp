#include <iostream>
#include <vector>
#include <Windows.h>
#include "sbzwhakman.h"
#include "SBZLibraryScope.h"
#include "game.h"

#define SIZE 13

#define X_OFFSET 0
#define Y_OFFSET 50

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR cmd, int nshow) {
  auto *lib = SBZLibraryScope::shared()->library();

  int screen_size_x = SIZE * 64 + X_OFFSET;
  int screen_size_y = SIZE * 64 + Y_OFFSET;

  lib->init(screen_size_x, screen_size_y);

  Game game(lib, SIZE, screen_size_x, screen_size_y);

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
