#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <stack>
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

  std::map<char, int> scores{
    {'(', 1}, 
    {'[', 2}, 
    {'{', 3}, 
    {'<', 4}, 
  };
  std::map<char, char> matches{
    {')', '('},
    {']', '['},
    {'}', '{'},
    {'>', '<'},
  };

  std::vector<unsigned long> completion_string_scores;

  std::stack<char> open;
  while (true) {
    const char c = infile.get();
    if (infile.eof()) {
      break;
    } else if (c == '\n') {
      if (open.size()) {
        //std::cout << "Incomplete line\n";
        auto score{0ul};
        while (open.size()) {
          score = score*5 + scores[open.top()];
          open.pop();
        }
        completion_string_scores.push_back(score);
        //std::cout << "  score: " << score << std::endl;
      }
      open = {};
    } else {
      switch (c) {
        case '(': 
        case '[': 
        case '{': 
        case '<': {
          open.push(c);
          break;
        }
        default: {
          if (open.size() and open.top() == matches[c]) {
            open.pop();
          } else {
            //std::cout << "Corrupted line\n";
            while (infile.peek() != '\n')
              infile.get();
            infile.get();
            open = {};
          }
          break;
        }
      }
    }
  }

  std::sort(std::begin(completion_string_scores),
            std::end(completion_string_scores));
  std::cout << completion_string_scores[completion_string_scores.size()/2] << std::endl;


  return EXIT_SUCCESS;
}

