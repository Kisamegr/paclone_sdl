#pragma once
#include <vector>
#include "sbzlibraryscope.h"
#include "map.h"

class Actor {
public:


protected:
  float					        m_x;
  float					        m_y;
  Direction             m_direction;
  float                 m_speed;
  ISBZLibrary*	        m_lib;
  float					        m_timer;
  int						        m_frame;
  std::vector<IImage*>	m_images;
  Direction             m_lastInput;

  virtual void update_animation(const float &dt) = 0;
  virtual void update_movement(const float &dt, const Map &map);

public:
  Actor(ISBZLibrary *lib);
  virtual ~Actor();
  virtual void update(const float &dt, const Map &map);
  virtual void draw();
  void setPosition(const float &x, const float &y);
  float getX();
  float getY();
};
