#include <cstdlib>
#include <fstream>
#include <iostream>
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


  auto risk_level_sum{0};

  const auto H{height_map.size()};
  const auto W{height_map[0].size()};
  for (auto y{0u}; y < H; ++y) {
    for (auto x{0u}; x < W; ++x) {
      const auto v{height_map[y][x]};
      if ((y == 0   or height_map[y-1][x] > v) and
          (y == H-1 or height_map[y+1][x] > v) and
          (x == 0   or height_map[y][x-1] > v) and
          (x == W-1 or height_map[y][x+1] > v))
        risk_level_sum += (v+1);
    }
  }

  std::cout << risk_level_sum << std::endl;

  return EXIT_SUCCESS;
}

