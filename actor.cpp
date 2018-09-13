#include "actor.h"
#include <math.h>

Actor::Actor(const float &x, const float &y)
  : m_x(x)
  , m_y(y)
  , m_x_coord(0)
  , m_y_coord(0)
  , m_current_direction(RIGHT)
  , m_speed(0)
  , m_timer(0)
  , m_frame(0)
  , m_input_direction(RIGHT)
  , m_initial_x(x)
  , m_initial_y(y)
{

}

Actor::~Actor() {
  for (auto image : m_images) {
    image->destroy();
  }
}

void Actor::update(const float &dt, Map *map) {
  update_animation(dt);
  update_movement(dt, map);
}

void Actor::update_movement(const float &dt, Map *map)
{
  int tile_size = map->tile_size();

  // Calculate the current coordinates based on the pixel position
  m_x_coord = static_cast<int>(round(m_x / map->tile_size()));
  m_y_coord = static_cast<int>(round(m_y / map->tile_size()));

  // Check if the direction the actor wants to go (m_input_direction)
  // is valid, by checking if the space in that direction is not a wall
  // If it's valid, set the current direction to the input direction
  // Also, snap the pixel position in order to stay on the grid
  switch (m_input_direction)
  {
  case RIGHT:
    if (map->get_value(m_x_coord + 1, m_y_coord) != Map::SPACE_WALL) {
      m_y = static_cast<float>(m_y_coord * tile_size);
      m_current_direction = m_input_direction;
    }
    break;
  case DOWN:
    if (map->get_value(m_x_coord, m_y_coord + 1) != Map::SPACE_WALL) {
      m_x = static_cast<float>(m_x_coord * tile_size);
      m_current_direction = m_input_direction;
    }
    break;
  case LEFT:
    if (map->get_value(m_x_coord - 1, m_y_coord) != Map::SPACE_WALL) {
      m_y = static_cast<float>(m_y_coord * tile_size);
      m_current_direction = m_input_direction;
    }

    break;
  case UP:
    if (map->get_value(m_x_coord, m_y_coord - 1) != Map::SPACE_WALL) {
      m_x = static_cast<float>(m_x_coord * tile_size);
      m_current_direction = m_input_direction;
    }
    break;
  }

  // Move the actor based on the current direction (m_current_direction)
  // Test if the space in that direction doesn't have a wall first though
  float dx = 0, dy = 0;
  switch (m_current_direction) {
  case LEFT:
    if (map->get_value(m_x_coord - 1, m_y_coord) != Map::SPACE_WALL) {
      dx = -1;
    }
    else if (m_x > m_x_coord * tile_size)
      dx = -1;
    break;
  case RIGHT:
    if (map->get_value(m_x_coord + 1, m_y_coord) != Map::SPACE_WALL) {
      dx = 1;
    }
    else if (m_x < m_x_coord * tile_size)
      dx = 1;
    break;
  case UP:
    if (map->get_value(m_x_coord, m_y_coord - 1) != Map::SPACE_WALL) {
      dy = -1;
    }
    else if (m_y > m_y_coord * tile_size)
      dy = -1;
    break;
  case DOWN:
    if (map->get_value(m_x_coord, m_y_coord + 1) != Map::SPACE_WALL) {
      dy = 1;
    }
    else if (m_y < m_y_coord * tile_size)
      dy = 1;
    break;
  }

  // Actually move the actor
  m_x += m_speed * dx * dt;
  m_y += m_speed * dy * dt;
}


void Actor::draw(const float &x_offset, const float &y_offset) {
  // Calculate the rotation based on the current facing direction
  float rotation = static_cast<float>(m_current_direction * 90);

  // Because in the LEFT direction, the actor is rotated 180 degrees
  // the sprite is flipped vertically, hence flip it back
  IImage::FlipMode flip = m_current_direction == LEFT ? IImage::VERTICAL : IImage::NONE;

  // Draw the current frame based on the rotation and flip mode
  m_images[m_frame]->draw(static_cast<int>(m_x + x_offset), static_cast<int>(m_y + y_offset), rotation, flip);
}

void Actor::reset() {
  // Reset the pixel position
  m_x = m_initial_x;
  m_y = m_initial_y;
  // Reset the directions
  m_current_direction = RIGHT;
  m_input_direction = RIGHT;
  // Reset the animation variables
  m_timer = 0;
  m_frame = 0;
}

void Actor::setPosition(const float & x, const float & y) {
  m_x = x;
  m_y = y;
}

float Actor::x() const {
  return m_x;
}

float Actor::y() const {
  return m_y;
}

int Actor::x_coord() const {
  return m_x_coord;
}

int Actor::y_coord() const {
  return m_y_coord;
}
