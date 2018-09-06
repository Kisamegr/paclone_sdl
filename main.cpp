#include <iostream>
#include <vector>
#include <Windows.h>
#include "sbzwhakman.h"

class SBZLibraryScope {
public:
    typedef ISBZLibrary*(*CreateLibraryFunc)();

    SBZLibraryScope()
        : _dll(nullptr)
        , _library(nullptr)
    {
        _dll = LoadLibraryA("SBZWhakman.dll");
        CreateLibraryFunc create_library = reinterpret_cast<CreateLibraryFunc>(GetProcAddress(_dll, "CreateLibrary"));
        _library = create_library();
    }

    ~SBZLibraryScope() {
        FreeLibrary(_dll);
    }

    ISBZLibrary	*library() const {
        return _library;
    }
private:
    HMODULE			_dll;
    ISBZLibrary*	_library;
};

struct Tile {
    Tile(int image, int x, int y, float rot = 0.0f)
        : image(image)
        , x(x)
        , y(y)
        , rot(rot)
    {}

    int		image;
    int		x, y;
    float	rot;
};

class Actor {
public:
    virtual ~Actor() {}
    virtual void update(float dt) = 0;
    virtual void draw() = 0;
private:
};

const float k_WHAKMAN_SPEED = 120;
const float k_WHAKMAN_ANIMATION_FLIP_TIME = 0.3f;

class Whakman : public Actor {
public:
    Whakman(ISBZLibrary *lib)
        :_lib(lib)
        , _x(0)
        , _y(0)
        , _timer(0)
        , _frame(0)
    {
        _images.push_back(lib->load_image("images/whakman_01.png"));
        _images.push_back(lib->load_image("images/whakman_02.png"));
    }

    ~Whakman() {
        for (auto image : _images) {
            image->destroy();
        }
    }

    void update(float dt) override {
        update_animation(dt);
        update_movement(dt);
    }

    void draw() override {
        _images[_frame]->draw(static_cast<int>(_x), static_cast<int>(_y));
    }

private:
    void update_animation(float dt) {
        _timer += dt;
        if (_timer > k_WHAKMAN_ANIMATION_FLIP_TIME) {
            _timer = 0;
            _frame = (_frame + 1) % _images.size();
        }
    }

    void update_movement(float dt) {
        int keys[8];
        int nr_pressed = _lib->pressed_keys(keys, 8);
        if (nr_pressed > 0) {
            float dx = 0, dy = 0;
            for (int i = 0; i < nr_pressed; ++i) {
                switch (keys[i]) {
                case 'w':
                case ISBZLibrary::KC_UP:
                    dy = -1;
                    break;
                case 'a':
                case ISBZLibrary::KC_LEFT:
                    dx = -1;
                    break;
                case 's':
                case ISBZLibrary::KC_DOWN:
                    dy = 1;
                    break;
                case 'd':
                case ISBZLibrary::KC_RIGHT:
                    dx = 1;
                    break;
                }
            }

            _x += k_WHAKMAN_SPEED * dx * dt;
            _y += k_WHAKMAN_SPEED * dy * dt;
        }

    }

    std::vector<IImage*>	_images;
    int						_frame;
    float					_timer;
    float					_x;
    float					_y;
    ISBZLibrary*			_lib;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR cmd, int nshow) {
    SBZLibraryScope library_scope;

    auto *lib = library_scope.library();
    lib->init(640, 640);

    IFont *font = lib->load_font("fonts/LondonBetween.ttf", 30);

    std::vector<IImage*> images;
    images.push_back(lib->load_image("images/wall_cross.png"));
    images.push_back(lib->load_image("images/wall_straight.png"));

    std::vector<Tile> tiles;
    for (int y = 0; y < 10; ++y) {
        for (int x = 0; x < 10; ++x) {
            bool xtile = (y % 2) == 0;
            bool ytile = (x % 3) == 0;
            if (xtile && ytile) {
                tiles.push_back(Tile(0, x * 64, y * 64, 0));
            }
            else if (xtile) {
                tiles.push_back(Tile(1, x * 64, y * 64, 0));
            }
            else if (ytile) {
                tiles.push_back(Tile(1, x * 64, y * 64, 90));
            }
        }
    }

    std::vector<Actor*> actors;
    actors.push_back(new Whakman(lib));

    float prev_time = lib->time();
    while (lib->update()) {
        float cur_time = lib->time();
        float dt = cur_time - prev_time;
        prev_time = cur_time;

        for (auto *actor : actors) {
            actor->update(dt);
        }

        for (auto &tile : tiles) {
            images[tile.image]->draw(tile.x, tile.y, tile.rot);
        }

        for (auto *actor : actors) {
            actor->draw();
        }

        font->draw(0, 640 - 30, "Score: 0", IFont::Color(255, 0, 0, 0));
    }

    for (auto actor : actors) {
        delete actor;
    }

    for (auto image : images) {
        image->destroy();
    }

    font->destroy();

    lib->destroy();

    return 0;
}
