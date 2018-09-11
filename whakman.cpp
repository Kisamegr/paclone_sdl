#include "whakman.h"

Whakman::Whakman(ISBZLibrary * lib)
  : Actor(lib), m_animationFlipTime(0.3)
{
  m_speed = 120;

  m_images.push_back(lib->load_image("images/whakman_01.png"));
  m_images.push_back(lib->load_image("images/whakman_02.png"));  
}

Whakman::~Whakman() {
  for (auto image : m_images) {
    image->destroy();
  }
}

void Whakman::update_animation(float dt) {
  m_timer += dt;
  if (m_timer > m_animationFlipTime) {
    m_timer = 0;
    m_frame = (m_frame + 1) % m_images.size();
  }
}

void Whakman::update_movement(float dt) {
  int keys[8];
  int nr_pressed = m_lib->pressed_keys(keys, 8);
  if (nr_pressed > 0) {
    float dx = 0, dy = 0;
    for (int i = 0; i < nr_pressed; ++i) {
      switch (keys[i]) {
      case 'w':
      case ISBZLibrary::KC_UP:
        dy = -1;
        m_face = UP;
        break;
      case 'a':
      case ISBZLibrary::KC_LEFT:
        dx = -1;
        m_face = LEFT;
        break;
      case 's':
      case ISBZLibrary::KC_DOWN:
        dy = 1;
        m_face = DOWN;
        break;
      case 'd':
      case ISBZLibrary::KC_RIGHT:
        dx = 1;
        m_face = RIGHT;
        break;
      }
    }



    m_x += m_speed * dx * dt;
    m_y += m_speed * dy * dt;
  }

}
