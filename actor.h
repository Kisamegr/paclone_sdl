#pragma once
#include <vector>
#include "sbzlibraryscope.h"

class Actor {
public:
  enum Face {
    RIGHT, DOWN, LEFT, UP
  };


protected:
  float					        m_x;
  float					        m_y;
  Face                  m_face;
  float                 m_speed;
  ISBZLibrary*	        m_lib;
  float					        m_timer;
  int						        m_frame;
  std::vector<IImage*>	m_images;

  virtual void update_animation(float dt) = 0;
  virtual void update_movement(float dt) = 0;

public:
  Actor(ISBZLibrary *lib);
  virtual ~Actor();
  virtual void update(float dt);
  virtual void draw();
};
