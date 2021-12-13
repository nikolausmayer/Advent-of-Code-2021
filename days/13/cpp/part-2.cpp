#include <cstdlib>
#include <array>
#include <fstream>
#include <iostream>
#include <set>
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
  
  std::set<std::array<int,2>> dots;
  {
    bool stage_one{true};
    while (true) {
      std::string line;
      infile >> line;
      if (infile.eof())
        break;
      if (line[0] > '9' and stage_one) {
        stage_one = false;
      }
      if (stage_one) {
        const auto sep_at{std::distance(std::begin(line),
                                        std::find(std::begin(line), 
                                                  std::end(line), 
                                                  ','))};
        const auto x{std::atoi(line.substr(0, sep_at).c_str())};
        const auto y{std::atoi(line.substr(sep_at+1).c_str())};
        dots.insert({x,y});
        //std::cout << "x=" << x << ", y=" << y << "\n";
      } else {
        infile >> line;
        infile >> line;
        const auto sep_at{std::distance(std::begin(line),
                                        std::find(std::begin(line), 
                                                  std::end(line), 
                                                  '='))};
        const auto xy{line[sep_at-1]};
        const auto num{std::atoi(line.substr(sep_at+1).c_str())};
        //std::cout << "fold " << xy << " at " << num << "\n";

        decltype(dots) visible_dots;
        for (const auto& [x,y]: dots) {
          if (xy == 'x') {
            if (x < num) {
              visible_dots.insert({x,y});
            } else {
              visible_dots.insert({2*num - x,y});
            }
          } else {
            if (y < num) {
              visible_dots.insert({x,y});
            } else {
              visible_dots.insert({x,2*num - y});
            }
          }
        }
        dots = visible_dots;
      }
    }
  }

  int maxx{0}, maxy{0};
  for (auto [x,y]: dots) {
    maxx = std::max(maxx, x);
    maxy = std::max(maxy, y);
  }
  for (auto y{0}; y <= maxy; ++y) {
    for (auto x{0}; x <= maxx; ++x) {
      std::cout << (dots.count({x,y}) ? '#' : '.');
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;

  return EXIT_SUCCESS;
}

