#include <iostream>
#include <vector>
#include <Windows.h>
#include "sbzwhakman.h"
#include "SBZLibraryScope.h"
#include "tile.h"
#include "whakman.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR cmd, int nshow) {
  SBZLibraryScope library_scope;

  auto *lib = library_scope.library();
  lib->init(640, 640);

  IFont *font = lib->load_font("fonts/LondonBetween.ttf", 30);

  std::vector<IImage*> images;
  images.push_back(lib->load_image("images/wall_cross.png"));
  images.push_back(lib->load_image("images/wall_straight.png"));

  std::vector<Tile> tiles;
  for (int y = 0; y < 10; ++y) {
    for (int x = 0; x < 10; ++x) {
      bool xtile = (y % 2) == 0;
      bool ytile = (x % 3) == 0;
      if (xtile && ytile) {
        tiles.push_back(Tile(0, x * 64, y * 64, 0));
      }
      else if (xtile) {
        tiles.push_back(Tile(1, x * 64, y * 64, 0));
      }
      else if (ytile) {
        tiles.push_back(Tile(1, x * 64, y * 64, 90));
      }
    }
  }

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

    for (auto &tile : tiles) {
      images[tile.image]->draw(tile.x, tile.y, tile.rot);
    }

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
