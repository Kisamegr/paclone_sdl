#include "actor.h"

Actor::Actor(ISBZLibrary * lib)
  : m_lib(lib)
  , m_x(0)
  , m_y(0)
  , m_face(RIGHT)
  , m_speed(0)
  , m_timer(0)
  , m_frame(0)
{

}

Actor::~Actor() {

}

void Actor::update(float dt) {
  update_animation(dt);
  update_movement(dt);
}

void Actor::draw() {
  float rotation = m_face * 90;
  IImage::FlipMode flip = m_face == LEFT ? IImage::VERTICAL : IImage::NONE;
  m_images[m_frame]->draw(static_cast<int>(m_x), static_cast<int>(m_y), rotation, flip);
}
