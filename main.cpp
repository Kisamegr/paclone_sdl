#include <iostream>
#include <vector>
#include <Windows.h>
#include "sbzwhakman.h"
#include "SBZLibraryScope.h"
#include "tile.h"
#include "whakman.h"
#include "map.h"

#define SIZE 5

//int g_map[10][10] =
//{
//  {0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
//  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//  {0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
//  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
//};

//int g_map[SIZE][SIZE] =
//{
//  {1, 1, 1, 1, 0},
//  {0, 0, 1, 1, 0},
//  {0, 0, 1, 0, 1},
//  {0, 0, 1, 1, 1},
//  {0, 0, 0, 0, 1}
//};

int g_map[SIZE][SIZE] =
{
  {0, 1, 0, 0, 0},
  {0, 1, 1, 1, 0},
  {1, 1, 1, 1, 1},
  {0, 1, 1, 1, 0},
  {0, 1, 1, 1, 0}
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR cmd, int nshow) {
  auto *lib = SBZLibraryScope::shared()->library();
  lib->init(10 * 64, 10 * 64);

  IFont *font = lib->load_font("fonts/LondonBetween.ttf", 30);

  std::vector<IImage*> images;

  Map map(SIZE, SIZE);


  for (int i = 0; i < SIZE; i++)
    for (int j = 0; j < SIZE; j++)
      map.setValue(i, j, g_map[j][i]);

  map.generateTiles();


  std::vector<Actor*> actors;
  actors.push_back(new Whakman(lib));

  float prev_time = lib->time();
  while (lib->update()) {
    float cur_time = lib->time();
    float dt = cur_time - prev_time;
    prev_time = cur_time;

    for (auto *actor : actors) {
      actor->update(dt);
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
