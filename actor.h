#pragma once
#include <vector>
#include "sbzlibraryscope.h"
#include "map.h"

/// <summary>
/// Represents a movable entity that can navigate itself on the space avaiable
/// <summary>
class Actor {
public:


protected:
  float					        m_x;                     // Pixel position x
  float					        m_y;                     // Pixel position y
  int                   m_x_coord;               // Map coordinates x
  int                   m_y_coord;               // Map coordinates y
  Direction             m_current_direction;     // Current facing direction
  float                 m_speed;                 // Movement speed
  float					        m_timer;                 // Animation timer
  int						        m_frame;                 // Current animation frame
  std::vector<IImage*>	m_images;                // The animation images list
  Direction             m_input_direction;       // The direction the actor wants to move to

  float                 m_initial_x;             // Holds the initial x pixel position
  float                 m_initial_y;             // Holds the initial y pixel position

  // Updates the current frame being drawn
  virtual void update_animation(const float &dt) = 0;

  // Updates the actor's position based on the current direction (m_current_direction)
  // and the desired direction it wants to move to (m_input_direction)
  virtual void update_movement(const float &dt, Map *map);

public:
  // Constructor with pixel position as arguments
  Actor(const float &x, const float &y);
  virtual ~Actor();

  // Calls the update functions
  virtual void update(const float &dt, Map *map);

  // Draws the current frame
  virtual void draw(const float &x_offset, const float &y_offset);

  // Resets the actor to its original state
  virtual void reset();

  // Sets the actor's pixel position
  void setPosition(const float &x, const float &y);

  // Basic getters
  float x() const;
  float y() const;
  int x_coord() const;
  int y_coord() const;
};
