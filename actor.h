#pragma once

class Actor {
public:
  virtual ~Actor();
  virtual void update(float dt) = 0;
  virtual void draw() = 0;
private:
};
