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

int g_map[13][13] =
{
  {0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0},
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
  lib->init(SIZE * 64, SIZE * 64);

  IFont *font = lib->load_font("fonts/LondonBetween.ttf", 30);

  std::vector<IImage*> images;

  Map map(SIZE, SIZE);


  for (int i = 0; i < SIZE; i++)
    for (int j = 0; j < SIZE; j++)
      map.setValue(i, j, g_map[j][i]);

  map.generateTiles();

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

    map.draw();

    for (auto *actor : actors) {
      actor->draw();
    }

    font->draw(0, 640 - 30, "Score: 0", IFont::Color(255, 0, 0, 0));
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
