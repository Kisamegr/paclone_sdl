#include "whakman.h"

inline Whakman::Whakman(ISBZLibrary * lib)
  :_lib(lib)
  , _x(0)
  , _y(0)
  , _timer(0)
  , _frame(0)
{
  _images.push_back(lib->load_image("images/whakman_01.png"));
  _images.push_back(lib->load_image("images/whakman_02.png"));
}

inline Whakman::~Whakman() {
  for (auto image : _images) {
    image->destroy();
  }
}

inline void Whakman::update(float dt) {
  update_animation(dt);
  update_movement(dt);
}

inline void Whakman::draw() {
  _images[_frame]->draw(static_cast<int>(_x), static_cast<int>(_y));
}

inline void Whakman::update_animation(float dt) {
  _timer += dt;
  if (_timer > k_WHAKMAN_ANIMATION_FLIP_TIME) {
    _timer = 0;
    _frame = (_frame + 1) % _images.size();
  }
}

inline void Whakman::update_movement(float dt) {
  int keys[8];
  int nr_pressed = _lib->pressed_keys(keys, 8);
  if (nr_pressed > 0) {
    float dx = 0, dy = 0;
    for (int i = 0; i < nr_pressed; ++i) {
      switch (keys[i]) {
      case 'w':
      case ISBZLibrary::KC_UP:
        dy = -1;
        break;
      case 'a':
      case ISBZLibrary::KC_LEFT:
        dx = -1;
        break;
      case 's':
      case ISBZLibrary::KC_DOWN:
        dy = 1;
        break;
      case 'd':
      case ISBZLibrary::KC_RIGHT:
        dx = 1;
        break;
      }
    }

    _x += k_WHAKMAN_SPEED * dx * dt;
    _y += k_WHAKMAN_SPEED * dy * dt;
  }

}
