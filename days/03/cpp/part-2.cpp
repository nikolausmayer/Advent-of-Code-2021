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

  auto oxygen{input};
  for (auto pos{0u}; 
       pos < input.front().size() and oxygen.size() > 1;
       ++pos) {
    auto ones{0u};
    for (auto num : oxygen)
      ones += (num[pos] == '1');
    //std::cout << ones << " " << oxygen.size() << "\n";
    //if (2*ones >= oxygen.size())
    //  std::cout << "its a 1\n";
    decltype(oxygen) tmp;
    for (auto num : oxygen)
      if ((2*ones >= oxygen.size()) == (num[pos] == '1'))
        tmp.push_back(num);
    //for (auto i : tmp)
    //  std::cout << i << "\n";
    //std::cout << "\n";
    oxygen = tmp;
  }

  auto co2{input};
  for (auto pos{0u}; 
       pos < input.front().size() and co2.size() > 1;
       ++pos) {
    auto ones{0u};
    for (auto num : co2)
      ones += (num[pos] == '0');
    decltype(co2) tmp;
    for (auto num : co2)
      if ((2*ones <= co2.size()) == (num[pos] == '0'))
        tmp.push_back(num);
    co2 = tmp;
  }

  auto oxygenrating{0};
  auto co2rating{0};
  for (auto pos{0u}; pos < input.front().size(); ++pos) {
    oxygenrating += ((oxygen.front()[pos] == '1') << (input.front().size() - pos - 1));
    co2rating += ((co2.front()[pos] == '1') << (input.front().size() - pos - 1));
  }

  std::cout << oxygenrating * co2rating << std::endl;



  return EXIT_SUCCESS;
}

