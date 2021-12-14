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
  
  std::string polymer;
  infile >> polymer;
  infile.get();
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

  std::map<std::array<char,2>, unsigned long> site_count;
  /// Get initial count of sites at which rules are active
  for (auto p1{std::begin(polymer)}, p0{p1++}; 
       p1 != std::end(polymer); 
       ++p0, ++p1) {
    site_count[{*p0,*p1}] += (rules[{*p0,*p1}] != 0);
  }

  /// Element counts
  std::map<char, unsigned long> element_count;
  for (auto c: polymer)
    ++element_count[c];


  /// In each step, apply rule expansion and keep track of new elements
  for (auto iter{0}; iter < 40; ++iter) {
    decltype(site_count) new_site_count;
    for (const auto& [k,v]: site_count) {
      element_count[rules[k]] += v;
      std::array<char,2> maybe_site{k[0], rules[k]};
      if (rules[maybe_site])
        new_site_count[maybe_site] += v;
      maybe_site = {rules[k], k[1]};
      if (rules[maybe_site])
        new_site_count[maybe_site] += v;
    }
    site_count = new_site_count;

    //for (const auto& [k,v]: site_count)
    //  std::cout << k[0] << k[1] << ": " << v << std::endl;
    //for (const auto& [k,v]: element_count)
    //  std::cout << k << ": " << v << ", ";
    //std::cout << std::endl;
  }

  std::cout << (*std::max_element(std::begin(element_count),
                                  std::end(element_count),
                                  [](const auto& l, const auto& r) {
                                    return l.second < r.second;
                                  })).second -
               (*std::min_element(std::begin(element_count),
                                  std::end(element_count),
                                  [](const auto& l, const auto& r) {
                                    return l.second < r.second;
                                  })).second 
            << std::endl;



  return EXIT_SUCCESS;
}

