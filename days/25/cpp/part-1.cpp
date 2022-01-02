#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
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

  std::vector<std::string> map;
  for (;;) {
    std::string line;
    std::getline(infile, line);
    if (infile.eof())
      break;
    map.push_back(line);
  }
  const int H{(int)map.size()};
  const int W{(int)map[0].size()};

  auto newmap{map};
  bool changes;
  for (int iter{1};; ++iter) {
    changes = false;

    /// East-facing herd
    for (auto y{0}; y < H; ++y) {
      for (auto x{0}; x < W; ++x) {
        if (map[y][x] == '.' and map[y][(x-1+W)%W] == '>') {
          newmap[y][x] = '>';
          newmap[y][(x-1+W)%W] = '.';
          changes = true;
        }
      }
    }
    map = newmap;
    /// South-facing herd
    for (auto y{0}; y < H; ++y) {
      for (auto x{0}; x < W; ++x) {
        if (map[y][x] == '.' and map[(y-1+H)%H][x] == 'v') {
          newmap[y][x] = 'v';
          newmap[(y-1+H)%H][x] = '.';
          changes = true;
        }
      }
    }
    map = newmap;

    //std::cout << iter << std::endl;
    //for (const auto& row: map)
    //  std::cout << row << std::endl;
    //std::cout << std::endl;
    if (not changes) {
      std::cout << iter << std::endl;
      break;
    }
  }

  return EXIT_SUCCESS;
}

