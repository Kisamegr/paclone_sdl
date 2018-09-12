#pragma once
#include "actor.h"

/// <summary>
/// Represents the enemy of the player.
/// Calculates it's next move based on the players position
/// When it reaches the player, the game ends
/// <summary>
class Ghost : public Actor {
private:
  Actor *m_player;                // Reference to the player, in order to get his position

  // Calculates intelligently the fastest route to catch the player
  // Jokes aside, it just selects the next move that brings it closer to the player.
  // If the closest is not valid, then it selects the next one closer to the player.
  // until all it's out of the 4 directions...
  void find_next_move(Map *map);

public:
  // Constructor with the initial pixel position and the player reference as arguments
  Ghost(const float &x, const float &y, Actor *player);

protected:
  void update_animation(const float &dt) override;
  void update_movement(const float &dt, Map *map) override;
  void draw(const float &x_offset, const float &y_offset) override;
};

