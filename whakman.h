#pragma once
#include "actor.h"


class Whakman : public Actor {
  float m_animationFlipTime;

public:
  Whakman(ISBZLibrary *lib);
  ~Whakman();


protected:
  void update_animation(float dt) override;
  void update_movement(float dt) override;



};