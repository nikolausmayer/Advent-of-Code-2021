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

  while (not infile.eof()) {
    std::string entry;
    infile >> entry;
    if (infile.eof()) { break; }

  }

  return EXIT_SUCCESS;
}

