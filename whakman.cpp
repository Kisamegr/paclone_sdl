#include "whakman.h"

Whakman::Whakman(const float &x, const float &y)
  : Actor(x, y), m_animationFlipTime(0.3)
{
  m_speed = 120;

  ISBZLibrary *lib = SBZLibraryScope::shared()->library();
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

void Whakman::update_movement(const float &dt, Map *map) {
  int keys[8];
  int nr_pressed = SBZLibraryScope::shared()->library()->pressed_keys(keys, 8);
  if (nr_pressed > 0) {
    for (int i = 0; i < nr_pressed; ++i) {
      switch (keys[i]) {
      case 'w':
      case ISBZLibrary::KC_UP:
        m_input_direction = UP;
        break;
      case 'a':
      case ISBZLibrary::KC_LEFT:
        m_input_direction = LEFT;
        break;
      case 's':
      case ISBZLibrary::KC_DOWN:
        m_input_direction = DOWN;
        break;
      case 'd':
      case ISBZLibrary::KC_RIGHT:
        m_input_direction = RIGHT;
        break;
      }
    }
  }

  Actor::update_movement(dt, map);
}
