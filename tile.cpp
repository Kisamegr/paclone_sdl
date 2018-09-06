#include "Tile.h"

Tile::Tile(int image, int x, int y, float rot)
    : m_image(image)
    , m_x(x)
    , m_y(y)
    , m_rot(rot)
{

}

int Tile::image() {
    return m_image;
}

int Tile::x() {
    return m_x;
}

int Tile::y() {
    return m_y;
}

float Tile::rot() {
    return m_rot;
}
