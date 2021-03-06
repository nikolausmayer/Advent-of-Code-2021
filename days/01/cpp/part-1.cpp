#include <cstdlib>
#include <fstream>
#include <iostream>

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

  int last{-1};
  int result{0};
  while (not infile.eof()) {
    int next;
    infile >> next;
    if (infile.eof()) { break; }
    if (last != -1 and next > last)
      ++result;
    last = next;
  }
  std::cout << result << std::endl;

  return EXIT_SUCCESS;
}

