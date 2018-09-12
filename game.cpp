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


Game::Game(const int &map_size, const int &tile_size, const int &screen_size_x, const int &screen_size_y)
  : m_screen_width(screen_size_x), m_screen_height(screen_size_y), m_score(0), m_won(false)
{
  ISBZLibrary *lib = SBZLibraryScope::shared()->library();
  
  // Create and initialize the map
  m_map = new Map(map_size, map_size, tile_size);

  for (int i = 0; i < map_size; i++)
    for (int j = 0; j < map_size; j++)
      m_map->set_value(i, j, (Map::Space)g_map[j][i]);

  m_map->create_wall_tiles();
  m_map->create_food();


  // Create the actors
  m_player = new Whakman(0, 0);

  float center = map_size * tile_size / 2 - tile_size / 2;
  m_ghosts.push_back(new Ghost(center, center, m_player));

  // Create the fonts
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
  // If the game is running...
  if (m_running) {
    // First update the player
    m_player->update(dt, m_map);

    // Then update all the ghosts
    for (auto *ghost : m_ghosts) {
      ghost->update(dt, m_map);

      // If a ghost has reached the players position, then it caught him
      if (ghost->x_coord() == m_player->x_coord()
        && ghost->y_coord() == m_player->y_coord())
      {
        game_lost();
      }
    }

    // Check if the player has eaten anything in the current tile
    if (m_map->check_eating_food(m_player->x_coord(), m_player->y_coord())) {
      m_score += 10;
    }

    // If the food count reaches zero, the player won!
    if (m_map->food_count() == 0)
      game_won();
  }
}

void Game::draw(const float & x_offset, const float & y_offset) {
  // First draw the map, as it's in the background
  m_map->draw(x_offset, y_offset);

  // Then draw the ghosts
  for (auto *ghost : m_ghosts) {
    ghost->draw(x_offset, y_offset);
  }

  // Then draw the player
  m_player->draw(x_offset, y_offset);

  // Lastly, draw the texts as they need to be at the top
  char scoreText[30];
  sprintf_s(scoreText, "Score: %d", m_score);
  m_small_font->draw(m_screen_width / 2 - 50, 10, scoreText, IFont::Color(255, 255, 255, 0));

  if (!m_running) {
    m_big_font->draw(
      m_screen_width / 2 - (m_won ? 250 : 350),
      m_screen_height / 2,
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
