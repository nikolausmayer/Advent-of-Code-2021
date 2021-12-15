#include <cstdlib>
#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <map>
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
  /// Expand 5 tiles in x-direction
  for (auto& line: risk_level_map) {
    const auto w{(int)line.size()};
    for (auto xtile{1}; xtile < 5; ++xtile) {
      for (auto x{0}; x < w; ++x) {
        auto n{line[x] + xtile};
        line.push_back((n <= 9 ? n : n - 9));
      }
    }
  }
  /// Expand 5 tiles in y-direction
  {
    const auto h{(int)risk_level_map.size()};
    for (auto ytile{1}; ytile < 5; ++ytile) {
      for (auto y{0}; y < h; ++y) {
        auto newline{risk_level_map[y]};
        for (auto& v: newline)
          v = (v+ytile <= 9 ? v+ytile : v+ytile-9);
        risk_level_map.push_back(newline);
      }
    }
  }
  const auto H{(int)risk_level_map.size()};
  const auto W{(int)risk_level_map[0].size()};

  //for (auto y{0}; y < H; ++y) {
  //  for (auto x{0}; x < W; ++x)
  //    std::cout << risk_level_map[y][x];
  //  std::cout << "\n";
  //}
  //std::cout << "\n";

  /// A*
  struct AstarNode {
    int x, y, cost, heuristic;
  };
  auto AstarHeuristic = [&](int x, int y) {
    return std::abs(x - (W-1)) + std::abs(y - (H-1));
  };

  /// Keep track of reached states' best costs
  std::map<std::array<int,2>,int> bestcost;
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
    /// Only expand states that improve on already-reached states
    auto& best = bestcost[{node.x, node.y}];
    if (best == 0)
      best = node.cost;
    else if (best < node.cost)
      continue;
    else
      best = std::min(best, node.cost);

    if (node.x > 0) {
      AstarNode next{node.x-1, node.y, 
                     node.cost + risk_level_map[node.y][node.x-1], 
                     AstarHeuristic(node.x-1, node.y)};
      const auto nextbest{bestcost[{next.x, next.y}]};
      if (nextbest == 0 or nextbest > next.cost)
        queue.push(next);
    }
    if (node.x < W-1) {
      AstarNode next{node.x+1, node.y, 
                     node.cost + risk_level_map[node.y][node.x+1], 
                     AstarHeuristic(node.x+1, node.y)};
      const auto nextbest{bestcost[{next.x, next.y}]};
      if (nextbest == 0 or nextbest > next.cost)
        queue.push(next);
    }
    if (node.y > 0) {
      AstarNode next{node.x, node.y-1, 
                     node.cost + risk_level_map[node.y-1][node.x], 
                     AstarHeuristic(node.x, node.y-1)};
      const auto nextbest{bestcost[{next.x, next.y}]};
      if (nextbest == 0 or nextbest > next.cost)
        queue.push(next);
    }
    if (node.y < H-1) {
      AstarNode next{node.x, node.y+1, 
                     node.cost + risk_level_map[node.y+1][node.x], 
                     AstarHeuristic(node.x, node.y+1)};
      const auto nextbest{bestcost[{next.x, next.y}]};
      if (nextbest == 0 or nextbest > next.cost)
        queue.push(next);
    }
  }




  return EXIT_SUCCESS;
}

