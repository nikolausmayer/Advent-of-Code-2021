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

  std::vector<std::string> input;
  while (not infile.eof()) {
    std::string line;
    infile >> line;
    if (infile.eof()) { break; }
    input.push_back(line);
  }

  auto gamma{0};
  auto epsilon{0};
  for (auto pos{0u}; pos < input.front().size(); ++pos) {
    auto ones{0u};
    for (const auto& num : input)
      ones += (num[pos] == '1');
    //std::cout << "pos " << pos << " is "
    //          << (ones > input.size()/2 ? "1" : "0")
    //          << std::endl;
    if (ones > input.size()/2)
      gamma += (1 << (input.front().size()-pos-1));
    else
      epsilon += (1 << (input.front().size()-pos-1));
  }
  std::cout << gamma * epsilon << std::endl;

  return EXIT_SUCCESS;
}

