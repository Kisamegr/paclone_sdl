#include "ghost.h"



Ghost::Ghost(const float &x, const float &y, Actor *player)
  : Actor(x, y), m_player(player) {
  m_speed = 90;
  m_images.push_back(SBZLibraryScope::shared()->library()->load_image("images/ghost_01.png"));
  m_images.push_back(SBZLibraryScope::shared()->library()->load_image("images/ghost_02.png"));
}

void Ghost::update_animation(const float &) {
}


void Ghost::update_movement(const float &dt, Map *map) {
  float x = m_x / map->tile_size();
  float y = m_y / map->tile_size();

  // Calculate the new coords
  m_x_coord = static_cast<int>(round(x));
  m_y_coord = static_cast<int>(round(y));

  // If the actor has reached a cell, find the next move
  if (abs(x - m_x_coord) == 0 || abs(y - m_y_coord) == 0) {
    find_next_move(map);
  }

  Actor::update_movement(dt, map);
}

void Ghost::draw(const float &x_offset, const float &y_offset) {
  m_images[m_frame]->draw(static_cast<int>(m_x + x_offset), static_cast<int>(m_y + y_offset));
}

void Ghost::find_next_move(Map *map) {
  // Get the pixel distance to the player
  float dx = m_player->x() - m_x;
  float dy = m_player->y() - m_y;

  // Caches the 4 directions in a particular order (see below)
  std::vector<Direction> priorityDirections;

  // Caches the opposite direction to be used later
  Direction oppositeDirection = (Direction)((m_current_direction + 2) % 4);

  // There are 4 directions, but each direction moves the ghost closer or further away from the player
  // Hence, the directions can be sorted based on their distance to the player
  // Case 0: The direction that shrinks the bigger distance in the x or y axis
  //         The ghost moves towards the player in that axis.
  // Case 1: The direction that shrinks the smaller distance in the x or y axis (opposite of case 0)
  //         The ghost moves towards the player in that axis.
  // Case 2: If the previous directions are blocked, then try the 3rd one, excluding the opposite.
  // Case 3: If all the other directions are blocked, try turning back in the opposite direction.
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

    // Avoid the opposite direction if it's not the last resort
    if (i != 3 && currentDirection == oppositeDirection)
      continue;

    // Check if the selected direction is valid
    if (map->get_value_direction(currentDirection, m_x_coord, m_y_coord) != Map::SPACE_WALL) {
      // If yes, set it as the input direction and break the loop
      m_input_direction = currentDirection;
      break;
    }

  }
}
