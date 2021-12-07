#include <cstdlib>
#include <fstream>
#include <iostream>
#include <numeric>
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

  std::vector<int> crabmarines;
  while (true) {
    int i;
    infile >> i;
    if (infile.eof())
      break;
    crabmarines.push_back(i);
    char c;
    infile >> c;
  }

  std::sort(std::begin(crabmarines), std::end(crabmarines));

  /// The solution to this L1 problem (sum of absolute differences)
  /// is the (or "a") median.
  const auto median{crabmarines[crabmarines.size()/2]};

  auto result{0};
  for (auto v: crabmarines)
    result += std::abs(median - v);

  std::cout << result << std::endl;

  return EXIT_SUCCESS;
}

