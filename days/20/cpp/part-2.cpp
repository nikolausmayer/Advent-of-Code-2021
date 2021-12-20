#include <cstdlib>
#include <array>
#include <fstream>
#include <iostream>
#include <map>
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
  
  std::string algorithm;
  std::map<std::array<int,2>,int> image;
  std::getline(infile, algorithm);
  int minx{0}, maxx{0}, miny{0}, maxy{0};
  {
    int y{0};
    std::string line;
    std::getline(infile, line);
    while (true) {
      std::getline(infile, line);
      if (infile.eof())
        break;
      for (auto x{0}; x < (int)line.size(); ++x) {
        image[{x,y}] = (line[x] == '#');
        maxx = x;
      }
      maxy = y;
      ++y;
    }
  }

  //auto print = [&]() {
  //  for (auto y{miny-100}; y <= maxy+100; ++y) {
  //    for (auto x{minx-100}; x <= maxx+100; ++x)
  //      std::cout << (image[{x,y}] ? '#' : '.');
  //    std::cout << std::endl;
  //  }
  //  std::cout << std::endl;
  //};

  //print();
  for (auto iter{0}; iter < 50; ++iter) {
    std::cout << "iteration " << iter+1 << "/50\n";
    decltype(image) next_image;
    for (auto y{miny-150}; y <= maxy+150; ++y) {
      for (auto x{minx-150}; x <= maxx+150; ++x) {
        next_image[{x,y}] = (algorithm[(image[{x-1,y-1}] << 8) +
                                       (image[{x  ,y-1}] << 7) +
                                       (image[{x+1,y-1}] << 6) +
                                       (image[{x-1,y  }] << 5) +
                                       (image[{x  ,y  }] << 4) +
                                       (image[{x+1,y  }] << 3) +
                                       (image[{x-1,y+1}] << 2) +
                                       (image[{x  ,y+1}] << 1) +
                                       (image[{x+1,y+1}] << 0)] == '#');
      }
    }
    image = next_image;
    //print();
  }
  
  auto result{0};
  for (auto y{miny-95}; y <= maxy+95; ++y)
    for (auto x{minx-95}; x <= maxx+95; ++x)
      result += (image[{x,y}] == 1);
  std::cout << result << std::endl;


  return EXIT_SUCCESS;
}

