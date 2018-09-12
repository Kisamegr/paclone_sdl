#pragma once
#include "actor.h"

/// <summary>
/// Represents the player's actor
/// Looks at the keyboard input and moves the actor accordingly
/// <summary>
class Whakman : public Actor {
  float m_animationFlipTime; // The time it takes to change the animation image

public:
  // Constructor getting the pixel position as an argument
  Whakman(const float &x, const float &y);

protected:
  void update_animation(const float &dt) override;
  void update_movement(const float &dt, Map *map) override;
};