#include "game.h"


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


Game::Game(ISBZLibrary *lib, const int &size, const int &screen_size_x, const int &screen_size_y) 
  : m_screen_size_x(screen_size_x), m_screen_size_y(screen_size_y), m_score(0)
{
  m_map = new Map(size, size);

  for (int i = 0; i < size; i++)
    for (int j = 0; j < size; j++)
      m_map->setValue(i, j, (Map::Space)g_map[j][i]);

  m_map->generateTiles();
  m_map->generateFood();

  Whakman *player = new Whakman(lib);

  m_actors.push_back(player);
  m_actors.push_back(new Ghost(lib, player));

  m_font = lib->load_font("fonts/LondonBetween.ttf", 30);
}


Game::~Game() {
  for (auto actor : m_actors) {
    delete actor;
  }

  delete m_map;

  m_font->destroy();
}

void Game::update(const float & dt) {
  for (auto *actor : m_actors) {
    actor->update(dt, m_map);
  }
}

void Game::draw(const float & x_offset, const float & y_offset) {
  m_map->draw(x_offset, y_offset);

  for (auto *actor : m_actors) {
    actor->draw(x_offset, y_offset);
  }

  char scoreText[30];
  sprintf_s(scoreText, "Score: %d", m_score);
  m_font->draw(m_screen_size_x / 2 - 50, 10, scoreText, IFont::Color(255, 0, 0, 0));
}
