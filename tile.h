#pragma once

/// <summary>
/// Represents a tile to be rendered at a position with a rotation and image
/// <summary>
class Tile {

private:
  int	  m_image; // The saved image index (the creator should have a list of images)
  int	  m_x;     // The x pixel position 
  int   m_y;     // The y pixel position
  float m_rot;   // The rotation

public:
  Tile() = default;
  Tile(int image, int x, int y, float rot = 0.0f);

  // Basic getters
  int image() const;
  int x() const;
  int y() const;
  float rot() const;
};

