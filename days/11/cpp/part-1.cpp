#include <cstdlib>
#include <array>
#include <fstream>
#include <iostream>
#include <stack>
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
  

  std::vector<std::vector<int>> octomap;
  {
    std::vector<int> line;
    while (true) {
      const char c = infile.get();
      if (infile.eof())
        break;
      if (c == '\n') {
        octomap.push_back(line);
        line.clear();
      } else {
        line.push_back(c - '0');
      }
    }
  }

  //for (const auto& row: octomap) {
  //  for (auto v : row)
  //    std::cout << v << " ";
  //  std::cout << "\n";
  //}
  //std::cout << "\n";

  const int H{(int)octomap.size()};
  const int W{(int)octomap[0].size()};

  int total_flashes{0};
  for (auto iter{0}; iter < 100; ++iter) {
    /// Initial energy increment for all octop... squids
    for (auto y{0}; y < H; ++y)
      for (auto x{0}; x < W; ++x)
        ++octomap[y][x];
    
    /// Mark initial flashers
    std::stack<std::array<int,2>> flashers;
    std::vector<std::vector<bool>> flashed(H, std::vector<bool>(W, false));
    for (int y{0}; y < H; ++y)
      for (int x{0}; x < W; ++x)
        if (octomap[y][x] > 9)
          flashers.push({y,x});

    /// Iterate while more flashers keep being triggered
    while (flashers.size()) {
      const auto [y,x] = flashers.top();
      flashers.pop();
      if (flashed[y][x])
        continue;
      flashed[y][x] = true;
      ++total_flashes;

      for (int dy: {-1, 0, 1}) {
        for (int dx: {-1, 0, 1}) {
          if (x+dx < 0 or x+dx >= W or
              y+dy < 0 or y+dy >= H or
              (dx == 0 and dy == 0))
            continue;
          ++octomap[y+dy][x+dx];
          if (octomap[y+dy][x+dx] > 9 and not flashed[y+dy][x+dx])
            flashers.push({y+dy,x+dx});
        }
      }
    }

    /// Reset flashers to 0 energy
    for (auto y{0}; y < H; ++y)
      for (auto x{0}; x < W; ++x)
        if (flashed[y][x])
          octomap[y][x] = 0;

    //for (const auto& row: octomap) {
    //  for (auto v : row)
    //    std::cout << v << " ";
    //  std::cout << "\n";
    //}
    //std::cout << "\n";
  }

  std::cout << total_flashes << std::endl;


  return EXIT_SUCCESS;
}

