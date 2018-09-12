#include "whakman.h"

Whakman::Whakman(ISBZLibrary * lib)
  : Actor(lib), m_animationFlipTime(0.3)
{
  m_speed = 120;

  m_images.push_back(lib->load_image("images/whakman_01.png"));
  m_images.push_back(lib->load_image("images/whakman_02.png"));
}


void Whakman::update_animation(const float &dt) {
  m_timer += dt;
  if (m_timer > m_animationFlipTime) {
    m_timer = 0;
    m_frame = (m_frame + 1) % m_images.size();
  }
}

void Whakman::update_movement(const float &dt, Map &map) {
  int keys[8];
  int nr_pressed = m_lib->pressed_keys(keys, 8);
  if (nr_pressed > 0) {
    float dx = 0, dy = 0;
    for (int i = 0; i < nr_pressed; ++i) {
      switch (keys[i]) {
      case 'w':
      case ISBZLibrary::KC_UP:
        m_lastInput = UP;
        break;
      case 'a':
      case ISBZLibrary::KC_LEFT:
        m_lastInput = LEFT;
        break;
      case 's':
      case ISBZLibrary::KC_DOWN:
        m_lastInput = DOWN;
        break;
      case 'd':
      case ISBZLibrary::KC_RIGHT:
        m_lastInput = RIGHT;
        break;
      }
    }


  }

  Actor::update_movement(dt, map);

  map.eatFood(m_xTile, m_yTile);
}
