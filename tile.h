#pragma once
class Tile {

private:
  int	  m_image;
  int	  m_x;
  int   m_y;
  float m_rot;

public:
  Tile(int image, int x, int y, float rot = 0.0f);
  int image();
  int x();
  int y();
  float rot();
};

