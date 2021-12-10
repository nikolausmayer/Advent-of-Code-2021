#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <stack>

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
    {')',     3}, 
    {']',    57}, 
    {'}',  1197}, 
    {'>', 25137}, 
  };
  std::map<char, char> matches{
    {')', '('},
    {']', '['},
    {'}', '{'},
    {'>', '<'},
  };

  int total_syntax_error_score{0};

  std::stack<char> open;
  while (true) {
    const char c = infile.get();
    if (infile.eof()) {
      break;
    } else if (c == '\n') {
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
            //std::cout << "Illegal '" << c << "'\n";
            total_syntax_error_score += scores[c];
            while (infile.peek() != '\n')
              infile.get();
          }
          break;
        }
      }
    }
  }

  std::cout << total_syntax_error_score << std::endl;


  return EXIT_SUCCESS;
}

