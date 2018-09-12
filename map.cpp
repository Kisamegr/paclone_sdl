#include "map.h"
#include "sbzlibraryscope.h"
#include <iostream>
#include <numeric>

#define TILE_SIZE 64

Map::Map(const int &width, const int &height) : m_width(width), m_height(height) {
  m_map = new int*[height];

  for (int x = 0; x < width; x++) {
    m_map[x] = new int[width];
    for (int y = 0; y < height; y++)
      m_map[x][y] = 0;
  }

  auto *lib = SBZLibraryScope::shared()->library();

  m_wallImages.push_back(lib->load_image("images/wall_cross.png"));
  m_wallImages.push_back(lib->load_image("images/wall_end.png"));
  m_wallImages.push_back(lib->load_image("images/wall_straight.png"));
  m_wallImages.push_back(lib->load_image("images/wall_t.png"));
  m_wallImages.push_back(lib->load_image("images/wall_turn.png"));

  m_foodImage = lib->load_image("images/rock.png");
}

Map::~Map() {
  for (int i = 0; i < m_width; i++)
    delete m_map[i];
  delete m_map;

  for (auto image : m_wallImages)
    image->destroy();
}

void Map::draw(const float &x_offset, const float &y_offset) {
  for (auto tile : m_wallTiles) {
    m_wallImages[tile.image()]->draw(tile.x() + x_offset, tile.y() + y_offset, tile.rot());
  }

  for (int x = 0; x < m_width; x++) {
    for (int y = 0; y < m_height; y++) {
      if (m_map[x][y] == SPACE_FOOD)
        m_foodImage->draw(x*TILE_SIZE + x_offset, y*TILE_SIZE + y_offset, 0);
    }
  }

}

Map::Space Map::getValue(const int &x, const int &y) const {
  if (m_map != nullptr) {
    if (x >= 0 && x < m_width && y >= 0 && y < m_height) {
      if (m_map[x] != nullptr) {
        return (Space)m_map[x][y];
      }
    }
  }

  return SPACE_WALL;
}

void Map::setValue(const int &x, const int &y, const Map::Space &value) {
  if (m_map != nullptr) {
    if (x >= 0 && x < m_width && y >= 0 && y < m_height) {
      if (m_map[x] != nullptr) {
        m_map[x][y] = value;
      }
    }
  }
}

Map::Space Map::getValuePosition(const float & x, const float & y) const
{
  int xTile = round(x) / TILE_SIZE;
  int yTile = round(y) / TILE_SIZE;

  return getValue(xTile, yTile);
}

Map::Space Map::getValueDirection(const Direction &direction, const int &xCoord, const int &yCoord) const {
  switch (direction)
  {
  case RIGHT:
    return getValue(xCoord + 1, yCoord);
  case DOWN:
    return getValue(xCoord, yCoord + 1);
  case LEFT:
    return getValue(xCoord - 1, yCoord);
  case UP:
    return getValue(xCoord, yCoord - 1);
  }
}


void Map::generateTiles() {
  Wall wall;
  int rot;

  for (int x = 0; x < m_width; x++) {
    for (int y = 0; y < m_height; y++) {
      if (m_map[x][y] == SPACE_WALL) {
        wallFromNeighbors(x, y, wall, rot);
        m_wallTiles.push_back(Tile(wall, x * TILE_SIZE, y * TILE_SIZE, rot));
      }
    }
  }
}

void Map::generateFood() {
  for (int x = 0; x < m_width; x++) {
    for (int y = 0; y < m_height; y++) {
      if (m_map[x][y] == SPACE_EMPTY)
        m_map[x][y] = SPACE_FOOD;
    }
  }
}

void Map::eatFood(const int & xCoord, const int & yCoord) {
  if (getValue(xCoord, yCoord) == SPACE_FOOD) {
    setValue(xCoord, yCoord, SPACE_EMPTY);
  }
}

void Map::wallFromNeighbors(const int &x, const int &y, Wall &wall, int &rot) {
  std::vector<int> neighborWalls; // Right, Down, Left, Top

  neighborWalls.push_back(x < m_width - 1 ? m_map[x + 1][y] : SPACE_EMPTY);  // Right
  neighborWalls.push_back(y < m_height - 1 ? m_map[x][y + 1] : SPACE_EMPTY); // Down
  neighborWalls.push_back(x > 0 ? m_map[x - 1][y] : SPACE_EMPTY);            // Left
  neighborWalls.push_back(y > 0 ? m_map[x][y - 1] : SPACE_EMPTY);            // Top

  int neighborCount = std::accumulate(neighborWalls.begin(), neighborWalls.end(), 0);

  switch (neighborCount)
  {
  case 1: {
    auto it = std::find(neighborWalls.begin(), neighborWalls.end(), SPACE_WALL);
    int index = std::distance(neighborWalls.begin(), it);

    wall = WALL_END;
    rot = index * 90;
    break;
  }
  case 2: {
    auto firstIt = std::find(neighborWalls.begin(), neighborWalls.end(), SPACE_WALL);
    auto secondIt = std::find(firstIt+1, neighborWalls.end(), SPACE_WALL);
    int firstIndex = std::distance(neighborWalls.begin(), firstIt);
    int secondIndex = std::distance(neighborWalls.begin(), secondIt);

    int diff = secondIndex - firstIndex;
    wall = diff == 2 ? WALL_STRAIGHT : WALL_TURN;
    rot = diff != 3 ? firstIndex * 90 : diff * 90;
    break;
  }

  case 3: {
    auto it = std::find(neighborWalls.begin(), neighborWalls.end(), SPACE_EMPTY);
    int index = std::distance(neighborWalls.begin(), it);

    wall = WALL_T;
    rot = (index - 2) * 90;
    break;
  }
  case 4:
    wall = WALL_CROSS;
    rot = 0;
    break;
  default:
    wall = WALL_NONE;
    rot = 0;
    break;
  }
}
