#include "map.h"
#include "sbzlibraryscope.h"
#include <iostream>
#include <numeric>


Map::Map(const int &width, const int &height, const int &tile_size)
  : m_width(width), m_height(height), m_tile_size(tile_size), m_food_count(0) {

  m_map = new int*[height];

  for (int x = 0; x < width; x++) {
    m_map[x] = new int[width];
    for (int y = 0; y < height; y++)
      m_map[x][y] = 0;
  }

  auto *lib = SBZLibraryScope::shared()->library();

  m_wall_images.push_back(lib->load_image("images/wall_cross.png"));
  m_wall_images.push_back(lib->load_image("images/wall_end.png"));
  m_wall_images.push_back(lib->load_image("images/wall_straight.png"));
  m_wall_images.push_back(lib->load_image("images/wall_t.png"));
  m_wall_images.push_back(lib->load_image("images/wall_turn.png"));

  m_food_image = lib->load_image("images/rock.png");
}

Map::~Map() {
  for (int i = 0; i < m_width; i++)
    delete m_map[i];
  delete m_map;

  for (auto image : m_wall_images)
    image->destroy();
}

void Map::draw(const float &x_offset, const float &y_offset) const {
  // Draw the walls
  for (auto tile : m_wall_tiles) {
    m_wall_images[tile.image()]->draw(
      static_cast<int>(round(tile.x() + x_offset)),
      static_cast<int>(round(tile.y() + y_offset)),
      tile.rot());
  }

  // Draw the food
  for (int x = 0; x < m_width; x++) {
    for (int y = 0; y < m_height; y++) {
      if (m_map[x][y] == SPACE_FOOD)
        m_food_image->draw(
          static_cast<int>(round(x*m_tile_size + x_offset)),
          static_cast<int>(round(y*m_tile_size + y_offset)),
          0);
    }
  }

}

Map::Space Map::get_value(const int &x, const int &y) const {
  if (m_map != nullptr) {
    if (x >= 0 && x < m_width && y >= 0 && y < m_height) {
      if (m_map[x] != nullptr) {
        return (Space)m_map[x][y];
      }
    }
  }

  return SPACE_WALL;
}

void Map::set_value(const int &x, const int &y, const Map::Space &value) {
  if (m_map != nullptr) {
    if (x >= 0 && x < m_width && y >= 0 && y < m_height) {
      if (m_map[x] != nullptr) {
        m_map[x][y] = value;
      }
    }
  }
}

Map::Space Map::get_value_position(const float &x, const float &y) const {
  int xTile = static_cast<int>(round(x) / m_tile_size);
  int yTile = static_cast<int>(round(y) / m_tile_size);

  return get_value(xTile, yTile);
}

Map::Space Map::get_value_direction(const Direction &direction, const int &xCoord, const int &yCoord) const {
  switch (direction)
  {
  case RIGHT:
    return get_value(xCoord + 1, yCoord);
  case DOWN:
    return get_value(xCoord, yCoord + 1);
  case LEFT:
    return get_value(xCoord - 1, yCoord);
  case UP:
    return get_value(xCoord, yCoord - 1);
  }

  return SPACE_EMPTY;
}


void Map::create_wall_tiles() {
  Wall wall;
  int rot;
  m_wall_tiles.clear();

  for (int x = 0; x < m_width; x++) {
    for (int y = 0; y < m_height; y++) {
      if (m_map[x][y] == SPACE_WALL) {
        wall_from_neighbors(x, y, wall, rot);
        m_wall_tiles.push_back( Tile(
          wall,
          x * m_tile_size,
          y * m_tile_size,
          static_cast<float>(rot)));
      }
    }
  }
}

void Map::create_food() {
  m_food_count = 0;
  for (int x = 0; x < m_width; x++) {
    for (int y = 0; y < m_height; y++) {
      if (m_map[x][y] == SPACE_EMPTY) {
        m_map[x][y] = SPACE_FOOD;
        m_food_count++;
      }
    }
  }
}

bool Map::check_eating_food(const int & xCoord, const int & yCoord) {
  if (get_value(xCoord, yCoord) == SPACE_FOOD) {
    set_value(xCoord, yCoord, SPACE_EMPTY);
    m_food_count--;
    return true;
  }

  return false;
}

int Map::width() const {
  return m_width;
}

int Map::height() const {
  return m_height;
}

int Map::food_count() const {
  return m_food_count;
}

int Map::tile_size() const {
  return m_tile_size;
}

void Map::wall_from_neighbors(const int &x, const int &y, Wall &wall, int &rot) {
  std::vector<int> neighborWalls; // Right, Down, Left, Top

  // Set the wall neighbors
  neighborWalls.push_back(x < m_width - 1 ? m_map[x + 1][y] : SPACE_EMPTY);  // Right
  neighborWalls.push_back(y < m_height - 1 ? m_map[x][y + 1] : SPACE_EMPTY); // Down
  neighborWalls.push_back(x > 0 ? m_map[x - 1][y] : SPACE_EMPTY);            // Left
  neighborWalls.push_back(y > 0 ? m_map[x][y - 1] : SPACE_EMPTY);            // Top

  // Calculate the neighbor wall count
  int neighborCount = std::accumulate(neighborWalls.begin(), neighborWalls.end(), 0);

  // Find which wall is appropriate for the given neighbor count
  // Case 1: The only matching wall is WALL_END that has only one connection
  //         Just find the correct rotation
  // Case 2: There are two matching walls - WALL_STRAING and WALL_TURN
  //         Find which is the correct one by looking at the distance between the neighbors
  //         And then rotate it accordingly
  // Case 3: The only matching wall is WALL_T
  //         Hence, find the correct rotation and you are set to go!
  // Case 4: Four neightbors means the wall with connections in all the directions - WALL_CROSS
  //         This one does not need to be rotated :/
  switch (neighborCount)
  {
  case 1: {
    // Find the only wall neighbor
    auto it = std::find(neighborWalls.begin(), neighborWalls.end(), SPACE_WALL);
    auto index = std::distance(neighborWalls.begin(), it);

    wall = WALL_END;
    rot = static_cast<int>(index * 90);
    break;
  }
  case 2: {
    // Find the 2 wall neighbors
    auto firstIt = std::find(neighborWalls.begin(), neighborWalls.end(), SPACE_WALL);
    auto secondIt = std::find(firstIt + 1, neighborWalls.end(), SPACE_WALL);
    auto firstIndex = std::distance(neighborWalls.begin(), firstIt);
    auto secondIndex = std::distance(neighborWalls.begin(), secondIt);

    int diff = static_cast<int>(secondIndex - firstIndex);
    wall = diff == 2 ? WALL_STRAIGHT : WALL_TURN;
    rot = static_cast<int>(diff != 3 ? firstIndex * 90 : diff * 90);
    break;
  }

  case 3: {
    auto it = std::find(neighborWalls.begin(), neighborWalls.end(), SPACE_EMPTY);
    auto index = std::distance(neighborWalls.begin(), it);

    wall = WALL_T;
    rot = static_cast<int>((index - 2) * 90);
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
