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

  int m3{-1}, m2{-1}, m1{-1};
  int result{0};
  while (not infile.eof()) {
    int next;
    infile >> next;
    if (infile.eof()) { break; }
    const auto presum{m3+m2+m1};
    m3 = m2;
    m2 = m1;
    m1 = next;
    const auto nowsum{m3+m2+m1};
    if (m3 != -1 and nowsum > presum)
      ++result;
  }
  std::cout << result-1 << std::endl;

  return EXIT_SUCCESS;
}

