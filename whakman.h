#pragma once
#include "actor.h"


class Whakman : public Actor {
  float m_animationFlipTime;

public:
  Whakman(ISBZLibrary *lib);


protected:
  void update_animation(const float &dt) override;
  void update_movement(const float &dt, Map &map) override;



};