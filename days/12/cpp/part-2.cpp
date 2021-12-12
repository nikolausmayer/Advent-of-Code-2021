#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <stack>
#include <set>
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
    
    std::map<std::string, std::vector<std::string>> connections;
    std::map<std::string, bool> is_big;
    while (true) {
      std::string line;
      infile >> line;
      if (infile.eof())
        break;
      const auto dash_at{std::distance(std::begin(line),
                                       std::find(std::begin(line), 
                                                 std::end(line), 
                                                 '-'))};
      const auto A{line.substr(0, dash_at)};
      const auto B{line.substr(dash_at+1)};
      connections[A].push_back(B);
      connections[B].push_back(A);
      is_big[A] = (A[0] < 'a');
      is_big[B] = (B[0] < 'a');
    }

    struct Node {
      std::vector<std::string> path;
      bool double_already_used;
    };
    std::stack<Node> queue;
    std::set<std::vector<std::string>> visited;
    int result{0};
    queue.push({{"start"}, false});
    while (queue.size()) {
      auto node = queue.top();
      queue.pop();

      if (visited.count(node.path))
        continue;
      visited.insert(node.path);

      if (node.path.back() == "end") {
        //std::cout << "Path found: ";
        //for (const auto& s: node.path)
        //  std::cout << s << ",";
        //std::cout << std::endl;
        ++result;
        continue;
      }

      for (const auto& cave: connections[node.path.back()]) {
        if (cave == "start")
          continue;
        const auto doubletime{std::find(std::begin(node.path),
                                        std::end(node.path),
                                        cave) != std::end(node.path)};
        if (is_big[cave] or 
            not doubletime or
            not node.double_already_used) {
          auto next{node};
          if (doubletime and not is_big[cave])
            next.double_already_used = true;
          next.path.push_back(cave);
          queue.push(next);
        }
      }
    }

    std::cout << result << std::endl;

    return EXIT_SUCCESS;
  }

