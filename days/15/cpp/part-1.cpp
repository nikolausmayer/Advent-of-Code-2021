#include <cstdlib>
#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <queue>
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
  
  std::vector<std::vector<int>> risk_level_map;
  {
    std::vector<int> line;
    while (true) {
      const auto c{infile.get()};
      if (c == '\n') {
        risk_level_map.push_back(line);
        line.clear();
      } else {
        line.push_back(c - '0');
      }
      if (infile.eof())
        break;
    }
  }
  const int H{(int)risk_level_map.size()};
  const int W{(int)risk_level_map[0].size()};

  //for (auto y{0}; y < H; ++y) {
  //  for (auto x{0}; x < W; ++x)
  //    std::cout << risk_level_map[y][x] << " ";
  //  std::cout << "\n";
  //}
  //std::cout << "\n";

  struct AstarNode {
    int x, y, cost, heuristic;
  };
  auto AstarHeuristic = [&](int x, int y) {
    return std::abs(x - (W-1)) + std::abs(y - (H-1));
  };

  std::set<std::array<int,3>> visited;
  auto cmp = [](const AstarNode& a, const AstarNode& b) {
    return (a.cost + a.heuristic) > (b.cost + b.heuristic);
  };
  std::priority_queue<AstarNode, 
                      std::vector<AstarNode>,
                      decltype(cmp)> queue(cmp);
  queue.push({0, 0, 0, H+W-2});
  while (queue.size()) {
    const auto node = queue.top();
    queue.pop();
    if (node.heuristic == 0) {
      std::cout << node.cost << std::endl;
      break;
    }
    if (visited.contains({node.x, node.y, node.cost}))
      continue;
    visited.insert({node.x, node.y, node.cost});
    if (node.x > 0) {
      AstarNode next{node.x-1, node.y, 
                     node.cost + risk_level_map[node.y][node.x-1], 
                     AstarHeuristic(node.x-1, node.y)};
      if (not visited.contains({next.x, next.y, next.cost}))
        queue.push(next);
    }
    if (node.x < W-1) {
      AstarNode next{node.x+1, node.y, 
                     node.cost + risk_level_map[node.y][node.x+1], 
                     AstarHeuristic(node.x+1, node.y)};
      if (not visited.contains({next.x, next.y, next.cost}))
        queue.push(next);
    }
    if (node.y > 0) {
      AstarNode next{node.x, node.y-1, 
                     node.cost + risk_level_map[node.y-1][node.x], 
                     AstarHeuristic(node.x, node.y-1)};
      if (not visited.contains({next.x, next.y, next.cost}))
        queue.push(next);
    }
    if (node.y < H-1) {
      AstarNode next{node.x, node.y+1, 
                     node.cost + risk_level_map[node.y+1][node.x], 
                     AstarHeuristic(node.x, node.y+1)};
      if (not visited.contains({next.x, next.y, next.cost}))
        queue.push(next);
    }
  }




  return EXIT_SUCCESS;
}

