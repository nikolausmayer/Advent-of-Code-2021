#include <cmath>
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

  const auto mean1{std::round(
      (std::accumulate(std::begin(crabmarines),
                       std::end(crabmarines), 0) /
       static_cast<float>(std::size(crabmarines))) + 0.5)};
  const auto mean2{std::round(
      (std::accumulate(std::begin(crabmarines),
                       std::end(crabmarines), 0) /
       static_cast<float>(std::size(crabmarines))) - 0.5)};

  long result1{0};
  long result2{0};
  for (auto v: crabmarines) {
    result1 += ((std::abs(v-mean1)*(std::abs(v-mean1)+1)) / 2);
    result2 += ((std::abs(v-mean2)*(std::abs(v-mean2)+1)) / 2);
  }

  std::cout << std::min(result1, result2) << std::endl;

  return EXIT_SUCCESS;
}
