#include <iostream>
#include <vector>
#include <Windows.h>
#include "sbzwhakman.h"
#include "SBZLibraryScope.h"
#include "tile.h"
#include "whakman.h"
#include "map.h"
#include "ghost.h"

#define SIZE 13
#define X_OFFSET 0
#define Y_OFFSET 50

int g_map[13][13] =
{
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0},
  {0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0},
  {0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0},
  {0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0},
  {0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0},
  {1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1},
  {1, 0, 0, 1, 0, 1, 1, 1, 0, 1, 0, 0, 1},
  {1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1},
  {0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0},
  {0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0},
  {0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0},
  {0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0}
};



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR cmd, int nshow) {
  auto *lib = SBZLibraryScope::shared()->library();

  int screen_size_x = SIZE * 64 + X_OFFSET;
  int screen_size_y = SIZE * 64 + Y_OFFSET;

  lib->init(screen_size_x, screen_size_y);

  IFont *font = lib->load_font("fonts/LondonBetween.ttf", 30);

  std::vector<IImage*> images;

  Map map(SIZE, SIZE);


  for (int i = 0; i < SIZE; i++)
    for (int j = 0; j < SIZE; j++)
      map.setValue(i, j, (Map::Space)g_map[j][i]);

  map.generateTiles();
  map.generateFood();

  Whakman *player = new Whakman(lib);

  std::vector<Actor*> actors;
  actors.push_back(player);
  actors.push_back(new Ghost(lib, player));

  

  float prev_time = lib->time();
  while (lib->update()) {
    float cur_time = lib->time();
    float dt = cur_time - prev_time;
    prev_time = cur_time;

    for (auto *actor : actors) {
      actor->update(dt, map);
    }

    map.draw(X_OFFSET, Y_OFFSET);

    for (auto *actor : actors) {
      actor->draw(X_OFFSET, Y_OFFSET);
    }

    font->draw(screen_size_x/2 - 50, 10, "Score: 0", IFont::Color(255, 0, 0, 0));
  }

  for (auto actor : actors) {
    delete actor;
  }

  for (auto image : images) {
    image->destroy();
  }

  font->destroy();

  lib->destroy();

  return 0;
}
