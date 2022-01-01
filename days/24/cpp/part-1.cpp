#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
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

  std::vector<std::vector<std::string>> instructions;
  for (;;) {
    std::string line;
    std::getline(infile, line);
    if (infile.eof())
      break;
    std::istringstream splitter{line};
    std::vector<std::string> next;
    for (;;) {
      splitter >> line;
      next.push_back(line);
      if (splitter.eof())
        break;
    }
    instructions.push_back(next);
  }

  std::vector<long> inputs{9,9,9,1,9,7,6,5,9,4,9,4,9,8};


  auto input_idx{0u};
  std::map<std::string, long> registers;
  for (auto iptr{0u}; iptr < instructions.size(); ++iptr) {
    const auto& in{instructions[iptr]};

    const auto cmd{in[0]};
    if (cmd == "#")
      continue;
    const auto target{in[1]};
    if (cmd == "inp") {
      if (input_idx >= inputs.size()) {
        break;
      }
      registers[target] = inputs[input_idx++];
    } else {
      const auto source{in[2]};
      const long arg{(source[0] > '9') 
                      ? registers[source] 
                      : std::stoi(source)};
      if (cmd == "add") {
        registers[target] += arg;
      } else if (cmd == "mul") {
        registers[target] *= arg;
      } else if (cmd == "div") {
        if (arg == 0) {
          break;
        }
        registers[target] /= arg;
      } else if (cmd == "mod") {
        if (registers[target] < 0) {
          break;
        }
        if (arg <= 0) {
          break;
        }
        registers[target] %= arg;
      } else if (cmd == "eql") {
        registers[target] = (registers[target] == arg);
      }
    }

    if (iptr == instructions.size()-1 and registers["z"] == 0) {
      for (auto d: inputs)
        std::cout << d;
      std::cout << std::endl;
    }
  }


  return EXIT_SUCCESS;
}

