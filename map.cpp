#include "map.h"
#include "sbzlibraryscope.h"
#include <iostream>
#include <numeric>

Map::Map(const int &width, const int &height) : m_width(width), m_height(height) {
  m_map = new int*[height];

  for (int y = 0; y < height; y++) {
    m_map[y] = new int[width];
    for (int x = 0; x < width; x++) {
      m_map[y][x] = 0;
    }
  }

  auto *lib = SBZLibraryScope::shared()->library();

  wallImages.push_back(lib->load_image("images/wall_cross.png"));
  wallImages.push_back(lib->load_image("images/wall_end.png"));
  wallImages.push_back(lib->load_image("images/wall_straight.png"));
  wallImages.push_back(lib->load_image("images/wall_t.png"));
  wallImages.push_back(lib->load_image("images/wall_turn.png"));

}

Map::~Map() {
  for (int i = 0; i < m_height; i++)
    delete m_map[i];
  delete m_map;

  for (auto image : wallImages)
    image->destroy();
}

void Map::draw() {
  for (int y = 0; y < m_height; y++) {
    for (int x = 0; x < m_width; x++) {
      Tile *tile = &m_tiles[y][x];
      if (tile->image() != WALL_NONE)
        wallImages[tile->image()]->draw(tile->x(), tile->y(), tile->rot());
    }
  }

}

int Map::getValue(const int &xCoord, const int &yCoord) {
  if (m_map != nullptr) {
    if (xCoord > 0 && xCoord < m_width && yCoord >= 0 && yCoord < m_height) {
      if (m_map[yCoord] != nullptr) {
        return m_map[yCoord][xCoord];
      }
    }
  }

  return -1;
}

void Map::setValue(const int &xCoord, const int &yCoord, const int &value) {
  if (m_map != nullptr) {
    if (xCoord >= 0 && xCoord < m_width && yCoord >= 0 && yCoord < m_height) {
      if (m_map[yCoord] != nullptr) {
        m_map[yCoord][xCoord] = value;
        //char s[256];
        //int x = 4;
        //sprintf_s(s, "%d\n", m_map[x][y]);
        //OutputDebugString(s);
      }
    }
  }
}

void Map::generateTiles() {
  m_tiles = new Tile*[m_height];
  Wall wall;
  int rot;

  for (int y = 0; y < m_height; y++) {
    m_tiles[y] = new Tile[m_width];
    for (int x = 0; x < m_width; x++) {
      if (m_map[y][x] == 0) {
        wall = WALL_NONE;
        rot = 0;
      }
      else
        wallFromNeighbors(x, y, wall, rot);

      m_tiles[y][x] = Tile(wall, x * 64, y * 64, rot);
      //m_tiles[x][y] = Tile(m_map[x][y], x * 64, y * 64, 0);
    }
  }
}

void Map::wallFromNeighbors(const int &x, const int &y, Wall &wall, int &rot) {
  std::vector<int> neighbors; // Right, Down, Left, Top

  neighbors.push_back(x < m_width - 1 ? m_map[y][x + 1] : 0);  // Right
  neighbors.push_back(y < m_height - 1 ? m_map[y + 1][x] : 0); // Down
  neighbors.push_back(x > 0 ? m_map[y][x - 1] : 0);            // Left
  neighbors.push_back(y > 0 ? m_map[y - 1][x] : 0);            // Top

  int neighborCount = std::accumulate(neighbors.begin(), neighbors.end(), 0);

  switch (neighborCount)
  {
  case 1: {
    auto it = std::find(neighbors.begin(), neighbors.end(), 1);
    int index = std::distance(neighbors.begin(), it);

    wall = WALL_END;
    rot = index * 90;
    break;
  }
  case 2: {
    auto firstIt = std::find(neighbors.begin(), neighbors.end(), 1);
    auto secondIt = std::find(firstIt, neighbors.end(), 1);
    int firstIndex = std::distance(neighbors.begin(), firstIt);
    int secondIndex = std::distance(neighbors.begin(), firstIt);

    wall = secondIndex - firstIndex == 1 ? WALL_TURN : WALL_STRAIGHT;
    rot = firstIndex * 90;
    break;
  }

  case 3: {
    auto it = std::find(neighbors.begin(), neighbors.end(), 0);
    int index = std::distance(neighbors.begin(), it);

    wall = WALL_T;
    rot = (index - 2) * 180;
    break;
  }
  case 4:
    wall = WALL_CROSS;
    rot = 0;
  default:
    wall = WALL_NONE;
    rot = 0;
    break;
  }
}
