#include <cstdlib>
#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
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
  
  std::list<char> polymer;
  for (;;) {
    const char c = infile.get();
    if (c == '\n')
      break;
    polymer.push_back(c);
  }
  infile.get();

  std::map<std::array<char,2>, char> rules;
  while (true) {
    const char a = infile.get();
    const char b = infile.get();
    infile.get();
    infile.get();
    infile.get();
    infile.get();
    const char c = infile.get();
    infile.get();
    if (infile.eof())
      break;
    rules[{a,b}] = c;
  }

  for (auto iter{0}; iter < 10; ++iter) {
    for (auto p1{std::begin(polymer)}, p0{p1++}; 
         p1 != std::end(polymer); 
         ++p0, ++p1) {
      const auto insert{rules[{*p0,*p1}]};
      if (insert) {
        polymer.insert(p1, insert);
        ++p0;
      }
    }

    //for (auto e: polymer)
    //  std::cout << e;
    //std::cout << std::endl;
  }

  std::map<char,int> freq;
  for (auto c: polymer)
    ++freq[c];
  std::cout << (*std::max_element(std::begin(freq), std::end(freq),
                                  [](const auto& a, const auto& b) {
                                    return a.second < b.second;
                                  })).second -
               (*std::min_element(std::begin(freq), std::end(freq),
                                  [](const auto& a, const auto& b) {
                                    return a.second < b.second;
                                  })).second << std::endl;

  return EXIT_SUCCESS;
}

