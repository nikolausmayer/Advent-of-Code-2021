#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <utility>

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

  std::map<std::pair<int,int>,int> seafloor;
  int minx{9999},maxx{0},miny{9999},maxy{0};

  while (not infile.eof()) {
    int x1,y1,x2,y2;
    char c;
    infile >> x1;
    infile.read(&c, 1);
    infile >> y1;
    infile.read(&c, 1);
    infile.read(&c, 1);
    infile.read(&c, 1);
    infile.read(&c, 1);
    infile >> x2;
    infile.read(&c, 1);
    infile >> y2;
    if (infile.eof()) { break; }
    //std::cout << "(" << x1 << "x" << y1 << ")->(" << x2 << "x" << y2 << ")\n";

    if (x1 != x2 and y1 != y2)
      continue;

    minx = std::min(x1, std::min(x2, minx));
    miny = std::min(y1, std::min(y2, miny));
    maxx = std::max(x1, std::max(x2, maxx));
    maxy = std::max(y1, std::max(y2, maxy));
    const auto dx{(x1 == x2) ? 0 : (x2 > x1) ? 1 : -1};
    const auto dy{(y1 == y2) ? 0 : (y2 > y1) ? 1 : -1};
    auto x{x1};
    auto y{y1};
    while (x != x2 or y != y2) {
      //std::cout << x << "," << y << std::endl;
      ++seafloor[{x,y}];
      x += dx;
      y += dy;
    }
    ++seafloor[{x2,y2}];
  }

  int result{0};

  for (auto y{miny}; y <= maxy; ++y) {
    for (auto x{minx}; x <= maxx; ++x) {
      const auto n{seafloor[{x,y}]};
      //if (n == 0)
      //  std::cout << '.';
      //else
      //  std::cout << n;

      result += (n > 1);
    }
    //std::cout << std::endl;
  }

  std::cout << result << std::endl;

  return EXIT_SUCCESS;
}

