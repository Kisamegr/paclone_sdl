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
  : m_screen_size_x(screen_size_x), m_screen_size_y(screen_size_y), m_score(0), m_won(false)
{
  m_map = new Map(size, size);

  for (int i = 0; i < size; i++)
    for (int j = 0; j < size; j++)
      m_map->setValue(i, j, (Map::Space)g_map[j][i]);

  m_map->generateTiles();
  m_map->generateFood();

  m_player = new Whakman(lib);

  m_ghosts.push_back(new Ghost(lib, m_player));

  m_small_font = lib->load_font("fonts/LondonBetween.ttf", 30);
  m_big_font = lib->load_font("fonts/LondonBetween.ttf", 120);

  m_running = true;
}


Game::~Game() {
  for (auto actor : m_ghosts) {
    delete actor;
  }
  delete m_player;
  delete m_map;
  m_small_font->destroy();
  m_big_font->destroy();
}

void Game::update(const float & dt) {
  if (m_running) {
    m_player->update(dt, m_map);

    for (auto *ghost : m_ghosts) {
      ghost->update(dt, m_map);

      if (ghost->get_x_coord() == m_player->get_x_coord()
        && ghost->get_y_coord() == m_player->get_y_coord()) 
      {
        game_lost();
      }
    }

    if (m_map->checkEatingFood(m_player->get_x_coord(), m_player->get_y_coord())) {
      m_score += 10;
    }

    if (m_map->get_food_count() == 0)
      game_won();
  }
}

void Game::draw(const float & x_offset, const float & y_offset) {
  m_map->draw(x_offset, y_offset);

  for (auto *ghost : m_ghosts) {
    ghost->draw(x_offset, y_offset);
  }

  m_player->draw(x_offset, y_offset);

  char scoreText[30];
  sprintf_s(scoreText, "Score: %d", m_score);
  m_small_font->draw(m_screen_size_x / 2 - 50, 10, scoreText, IFont::Color(255, 255, 255, 0));

  if (!m_running) {
    m_big_font->draw(
      m_screen_size_x / 2 - (m_won ? 250 : 350),
      m_screen_size_y/2, 
      m_won ? "YOU WIN" : "GAME OVER", 
      IFont::Color(255, 255, 255, 0));
  }
}

void Game::game_won() {
  m_running = false;
  m_won = true;
}

void Game::game_lost() {
  m_running = false;
  m_won = false;
}
