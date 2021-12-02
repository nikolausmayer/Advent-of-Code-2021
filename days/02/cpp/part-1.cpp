#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

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

  int depth{0};
  int horizontal{0};
  while (not infile.eof()) {
    std::string direction;
    int distance;
    infile >> direction >> distance;
    if (infile.eof()) { break; }

    if (direction == "forward")
      horizontal += distance;
    else if (direction == "down")
      depth += distance;
    else
      depth -= distance;
  }
  std::cout << horizontal * depth << std::endl;


  return EXIT_SUCCESS;
}

