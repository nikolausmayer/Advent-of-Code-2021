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

  auto result{0};
  while (true) {
    std::string digit;
    for (auto i{0}; i < 11; ++i)
      infile >> digit;
    if (infile.eof())
      break;
    for (auto i{0}; i < 4; ++i) {
      infile >> digit;
      const auto l{digit.size()};
      if (l == 2 or l == 3 or l == 4 or l == 7)
        ++result;
    }
  }

  std::cout << result << std::endl;


  return EXIT_SUCCESS;
}

