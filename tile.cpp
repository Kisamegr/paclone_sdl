#include "Tile.h"

Tile::Tile(int image, int x, int y, float rot)
  : m_image(image)
  , m_x(x)
  , m_y(y)
  , m_rot(rot)
{
}

int Tile::image() const {
  return m_image;
}

int Tile::x() const {
  return m_x;
}

int Tile::y() const {
  return m_y;
}

float Tile::rot() const {
  return m_rot;
}
