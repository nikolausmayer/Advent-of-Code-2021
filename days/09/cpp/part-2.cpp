#include <cstdlib>
#include <algorithm>
#include <array>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stack>
#include <string>
#include <vector>

int main(int argc, char* argv[])
{
  if (argc != 2) {
    std::cerr << "Bad arguments" << std::endl;
    return EXIT_FAILURE;
  }
  auto filename = argv[1];
  std::ifstream infile{filename};
  if (infile.bad() or not infile.is_open()) {
    std::cerr << "Cannot open \"" << argv[1] << "\"" << std::endl;
    return EXIT_FAILURE;
  }

  std::vector<std::vector<int>> height_map;
  {
    std::vector<int> v;
    while (true) {
      const char c = infile.get();
      if (infile.eof()) {
        break;
      } else if (c == '\n') {
        height_map.push_back(v);
        v.clear();
      } else {
        v.push_back(c - '0');
      }
    }
  }

  std::stack<std::array<int,3>> queue;
  auto basin_map = height_map;

  const int H = height_map.size();
  const int W = height_map[0].size();
  for (auto y{0}; y < H; ++y) {
    for (auto x{0}; x < W; ++x) {
      basin_map[y][x] = 0;
      const auto v{height_map[y][x]};
      if ((y == 0   or height_map[y-1][x] > v) and
          (y == H-1 or height_map[y+1][x] > v) and
          (x == 0   or height_map[y][x-1] > v) and
          (x == W-1 or height_map[y][x+1] > v)) {
        basin_map[y][x] = queue.size()+1;
        queue.push({x,y,(int)queue.size()+1});
      }
    }
  }

  std::vector<int> basin_sizes(queue.size(), 0);

  while (queue.size()) {
    auto [x,y,group] = queue.top();
    queue.pop();

    if (x > 0 and basin_map[y][x-1] == 0 and
        height_map[y][x-1] != 9) {
      basin_map[y][x-1] = group;
      queue.push({x-1,y,group});
    }
    if (y > 0 and basin_map[y-1][x] == 0 and
        height_map[y-1][x] != 9) {
      basin_map[y-1][x] = group;
      queue.push({x,y-1,group});
    }
    if (x < W-1 and basin_map[y][x+1] == 0 and
        height_map[y][x+1] != 9) {
      basin_map[y][x+1] = group;
      queue.push({x+1,y,group});
    }
    if (y < H-1 and basin_map[y+1][x] == 0 and
        height_map[y+1][x] != 9) {
      basin_map[y+1][x] = group;
      queue.push({x,y+1,group});
    }
  }
  
  for (const auto& row: basin_map)
    for (auto n: row)
      if (n > 0)
        ++basin_sizes[n-1];

  std::sort(std::begin(basin_sizes), std::end(basin_sizes));
  std::cout << basin_sizes[basin_sizes.size()-1] *
               basin_sizes[basin_sizes.size()-2] *
               basin_sizes[basin_sizes.size()-3]
            << std::endl;

  //for (const auto& row: height_map) {
  //  for (auto n: row)
  //    std::cout << std::setw(2) << n << " ";
  //  std::cout << std::endl;
  //}
  //std::cout << std::endl;

  //for (const auto& row: basin_map) {
  //  for (auto n: row)
  //    std::cout << std::setw(2) << n << " ";
  //  std::cout << std::endl;
  //}
  //std::cout << std::endl;

  return EXIT_SUCCESS;
}

