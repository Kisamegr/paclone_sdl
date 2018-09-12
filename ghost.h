#pragma once
#include "actor.h"

class Ghost : public Actor {
public:
  Ghost(ISBZLibrary *lib, Actor *player);

protected:
  void update_animation(const float &dt) override;
  void update_movement(const float &dt, const Map &map) override;
  void draw() override;
private:
  Actor *m_player;


};

