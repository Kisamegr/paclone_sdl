#include "actor.h"
#include <math.h>

static bool fuzzyCompare(double a, double b) {
  return abs(a - b) < 1;
}

void Actor::update_movement(const float & dt, Map & map)
{
  float dx = 0, dy = 0;

  m_xTile = round(m_x / 64);
  m_yTile = round(m_y / 64);

  //  char s[256];
  //sprintf_s(s, "%d  -  %d\n", xTile, yTile);
  //OutputDebugString(s);
  switch (m_lastInput)
  {
  case RIGHT:
    if (map.getValue(m_xTile + 1, m_yTile) != Map::SPACE_WALL) {
      m_y = m_yTile * 64;
      m_direction = m_lastInput;
    }
    break;
  case DOWN:
    if (map.getValue(m_xTile, m_yTile + 1) != Map::SPACE_WALL) {
      m_x = m_xTile * 64;
      m_direction = m_lastInput;
    }
    break;
  case LEFT:
    if (map.getValue(m_xTile - 1, m_yTile) != Map::SPACE_WALL) {
      m_y = m_yTile * 64;
      m_direction = m_lastInput;
    }

    break;
  case UP:
    if (map.getValue(m_xTile, m_yTile - 1) != Map::SPACE_WALL) {
      m_x = m_xTile * 64;
      m_direction = m_lastInput;
    }
    break;
  }

  //if (canMove) {

  switch (m_direction) {
  case LEFT:
    if (map.getValue(m_xTile - 1, m_yTile) != Map::SPACE_WALL) {
      dx = -1;
    }
    else if (m_x > m_xTile * 64)
      dx = -1;

    break;
  case RIGHT:
    if (map.getValue(m_xTile + 1, m_yTile) != Map::SPACE_WALL) {
      dx = 1;
    }
    else if (m_x < m_xTile * 64)
      dx = 1;
    break;
  case UP:
    if (map.getValue(m_xTile, m_yTile - 1) != Map::SPACE_WALL) {
      dy = -1;
    }
    else if (m_y > m_yTile * 64)
      dy = -1;
    break;
  case DOWN:
    if (map.getValue(m_xTile, m_yTile + 1) != Map::SPACE_WALL) {
      dy = 1;
    }
    else if (m_y < m_yTile * 64)
      dy = 1;
    break;
  }


  m_x += m_speed * dx * dt;
  m_y += m_speed * dy * dt;
  //}



}

Actor::Actor(ISBZLibrary * lib)
  : m_lib(lib)
  , m_x(0)
  , m_y(0)
  , m_xTile(0)
  , m_yTile(0)
  , m_direction(RIGHT)
  , m_speed(0)
  , m_timer(0)
  , m_frame(0)
  , m_lastInput(RIGHT)
{

}

Actor::~Actor() {
  for (auto image : m_images) {
    image->destroy();
  }
}

void Actor::update(const float &dt, Map &map) {
  update_animation(dt);
  update_movement(dt, map);
}

void Actor::draw(const float &x_offset, const float &y_offset) {
  float rotation = m_direction * 90;
  IImage::FlipMode flip = m_direction == LEFT ? IImage::VERTICAL : IImage::NONE;
  m_images[m_frame]->draw(static_cast<int>(m_x+x_offset), static_cast<int>(m_y+y_offset), rotation, flip);
}

void Actor::setPosition(const float & x, const float & y) {
  m_x = x;
  m_y = y;
}

float Actor::getX() {
  return m_x;
}

float Actor::getY() {
  return m_y;
}
