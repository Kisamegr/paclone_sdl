#include "ghost.h"



Ghost::Ghost(ISBZLibrary * lib, Actor *player) : Actor(lib), m_player(player)
{
  m_speed = 20;

  m_images.push_back(lib->load_image("images/ghost_01.png"));
  m_images.push_back(lib->load_image("images/ghost_02.png"));

  setPosition(13 * 64 / 2 - 32, 13 * 64 / 2 - 32);
}



void Ghost::update_animation(const float &dt) {
}


void Ghost::update_movement(const float &dt, Map *map) {

  float x = m_x / 64.0;
  float y = m_y / 64.0;

  m_xCoord = round(x);
  m_yCoord = round(y);

  if (abs(x - m_xCoord) == 0 || abs(y - m_yCoord) == 0) {
    float dx = m_player->getX() - m_x;
    float dy = m_player->getY() - m_y;

    std::vector<Direction> priorityDirections;
    Direction oppositeDirection = (Direction)((m_direction + 2) % 4);

    for (int i = 0; i < 4; i++) {
      Direction currentDirection;

      switch (i)
      {
      case 0:
      case 1: {
        bool biggerDistanceOnX = abs(dx) > abs(dy);
        if (i == 1)
          biggerDistanceOnX = !biggerDistanceOnX;

        if (biggerDistanceOnX)
          if (dx > 0)
            currentDirection = RIGHT;
          else
            currentDirection = LEFT;
        else
          if (dy > 0)
            currentDirection = DOWN;
          else
            currentDirection = UP;
        break;
      }
      case 2: {

        for (int d = 0; d < 4; d++) {
          currentDirection = (Direction)d;

          if (currentDirection == oppositeDirection)
            continue;

          auto it = std::find(priorityDirections.begin(), priorityDirections.end(), currentDirection);
          if (it == priorityDirections.end()) {
            break;
          }
        }
        break;
      }
      default:
        currentDirection = oppositeDirection;
        break;
      }

      if (i != 3 && currentDirection == oppositeDirection)
        continue;

      if (map->getValueDirection(currentDirection, m_xCoord, m_yCoord) != Map::SPACE_WALL) {
        m_lastInput = currentDirection;
        break;
      }

    }

  }

  Actor::update_movement(dt, map);
}

void Ghost::draw(const float &x_offset, const float &y_offset) {
  m_images[m_frame]->draw(static_cast<int>(m_x + x_offset), static_cast<int>(m_y + y_offset));
}
