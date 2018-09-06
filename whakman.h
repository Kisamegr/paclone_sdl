#pragma once
#include <vector>
#include "actor.h"
#include "sbzlibraryscope.h"

class Whakman : public Actor {

    const float k_WHAKMAN_SPEED = 120;
    const float k_WHAKMAN_ANIMATION_FLIP_TIME = 0.3f;

    std::vector<IImage*>	_images;
    int						_frame;
    float					_timer;
    float					_x;
    float					_y;
    ISBZLibrary*			_lib;

public:
    Whakman(ISBZLibrary *lib);
    ~Whakman();
    void update(float dt) override;
    void draw() override;

private:
    void update_animation(float dt);
    void update_movement(float dt);
};