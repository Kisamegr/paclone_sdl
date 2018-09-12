#pragma once
#include "actor.h"

class Ghost : public Actor {
public:
  Ghost(ISBZLibrary *lib, Actor *player);

protected:
  void update_animation(const float &dt) override;
  void update_movement(const float &dt, Map &map) override;
  void draw(const float &x_offset, const float &y_offset) override;
private:
  Actor *m_player;


};

