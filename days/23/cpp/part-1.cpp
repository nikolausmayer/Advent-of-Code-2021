#include <cstdlib>
#include <array>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <queue>
#include <unordered_set>
#include <string>
#include <vector>


/// #############
/// #12 3 4 5 67#
/// ###A#B#C#D###
///   #A#B#C#D#
///   #########

struct State {
  /// 7 hallway tiles + 8 room tiles
  std::map<std::string, char> map;
};

/// Path between tiles, its base cost and potential blocking tiles
struct Connection {
  const std::string from, to;
  const int cost;
  const std::vector<std::string> blockers;
};
const std::vector<Connection> all_connections{
  {"H1", "A1",  3, {"H2"}},
  {"H2", "A1",  2, {}},
  {"H3", "A1",  2, {}},
  {"H4", "A1",  4, {"H3"}},
  {"H5", "A1",  6, {"H3", "H4"}},
  {"H6", "A1",  8, {"H3", "H4", "H5"}},
  {"H7", "A1",  9, {"H3", "H4", "H5", "H6"}},
  {"H1", "A2",  4, {"A1", "H2"}},
  {"H2", "A2",  3, {"A1"}},
  {"H3", "A2",  3, {"A1"}},
  {"H4", "A2",  5, {"A1", "H3"}},
  {"H5", "A2",  7, {"A1", "H3", "H4"}},
  {"H6", "A2",  9, {"A1", "H3", "H4", "H5"}},
  {"H7", "A2", 10, {"A1", "H3", "H4", "H5", "H6"}},

  {"H1", "B1",  5, {"H2", "H3"}},
  {"H2", "B1",  4, {"H3"}},
  {"H3", "B1",  2, {}},
  {"H4", "B1",  2, {}},
  {"H5", "B1",  4, {"H4"}},
  {"H6", "B1",  6, {"H4", "H5"}},
  {"H7", "B1",  7, {"H4", "H5", "H6"}},
  {"H1", "B2",  6, {"B1", "H2", "H3"}},
  {"H2", "B2",  5, {"B1", "H3"}},
  {"H3", "B2",  3, {"B1"}},
  {"H4", "B2",  3, {"B1"}},
  {"H5", "B2",  5, {"B1", "H4"}},
  {"H6", "B2",  7, {"B1", "H4", "H5"}},
  {"H7", "B2",  8, {"B1", "H4", "H5", "H6"}},

  {"H1", "C1",  7, {"H2", "H3", "H4"}},
  {"H2", "C1",  6, {"H3", "H4"}},
  {"H3", "C1",  4, {"H4"}},
  {"H4", "C1",  2, {}},
  {"H5", "C1",  2, {}},
  {"H6", "C1",  4, {"H5"}},
  {"H7", "C1",  5, {"H5", "H6"}},
  {"H1", "C2",  8, {"C1", "H2", "H3", "H4"}},
  {"H2", "C2",  7, {"C1", "H3", "H4"}},
  {"H3", "C2",  5, {"C1", "H4"}},
  {"H4", "C2",  3, {"C1"}},
  {"H5", "C2",  3, {"C1"}},
  {"H6", "C2",  5, {"C1", "H5"}},
  {"H7", "C2",  6, {"C1", "H5", "H6"}},

  {"H1", "D1",  9, {"H2", "H3", "H4", "H5"}},
  {"H2", "D1",  8, {"H3", "H4", "H5"}},
  {"H3", "D1",  6, {"H4", "H5"}},
  {"H4", "D1",  4, {"H5"}},
  {"H5", "D1",  2, {}},
  {"H6", "D1",  2, {}},
  {"H7", "D1",  3, {"H6"}},
  {"H1", "D2", 10, {"D1", "H2", "H3", "H4", "H5"}},
  {"H2", "D2",  9, {"D1", "H3", "H4", "H5"}},
  {"H3", "D2",  7, {"D1", "H4", "H5"}},
  {"H4", "D2",  5, {"D1", "H5"}},
  {"H5", "D2",  3, {"D1"}},
  {"H6", "D2",  3, {"D1"}},
  {"H7", "D2",  4, {"D1", "H6"}},
};

/// Movement cost per tile per pod type
const std::map<char, int> cost_multiplier{
  {'A',    1},
  {'B',   10},
  {'C',  100},
  {'D', 1000},
};

template <>
struct std::hash<std::map<std::string, char>>
{
  std::size_t operator()(const std::map<std::string, char>& m) const noexcept
  {
    return   (m.at("H1")-'@')
           + (m.at("H2")-'@') * 5
           + (m.at("H3")-'@') * 5*5
           + (m.at("H4")-'@') * 5*5*5
           + (m.at("H5")-'@') * 5*5*5*5
           + (m.at("H6")-'@') * 5*5*5*5*5
           + (m.at("H7")-'@') * 5*5*5*5*5*5
           + (m.at("A1")-'@') * 5*5*5*5*5*5*5
           + (m.at("B1")-'@') * 5*5*5*5*5*5*5*5
           + (m.at("C1")-'@') * 5*5*5*5*5*5*5*5*5
           + (m.at("D1")-'@') * 5*5*5*5*5*5*5*5*5*5
           + (m.at("A2")-'@') * 5*5*5*5*5*5*5*5*5*5*5
           + (m.at("B2")-'@') * 5*5*5*5*5*5*5*5*5*5*5*5
           + (m.at("C2")-'@') * 5*5*5*5*5*5*5*5*5*5*5*5*5
           + (m.at("D2")-'@') * 5*5*5*5*5*5*5*5*5*5*5*5*5*5;
  }
};


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


  State initial_state;

  /// Read map and initial pod positions
  {
    std::vector<char> pods_in_rooms;

    std::string line;
    std::getline(infile, line);
    std::getline(infile, line);
    for (auto i{0}; i < 2; ++i) {
      std::getline(infile, line);
      for (auto c: line)
        if (c != '#' and c != ' ')
          pods_in_rooms.push_back(c);
    }
    /// "@" for "." tiles because "@" directly precedes "A" in ASCII
    initial_state.map["H1"] = '@';
    initial_state.map["H2"] = '@';
    initial_state.map["H3"] = '@';
    initial_state.map["H4"] = '@';
    initial_state.map["H5"] = '@';
    initial_state.map["H6"] = '@';
    initial_state.map["H7"] = '@';
    auto idx{0u};
    initial_state.map["A1"] = pods_in_rooms[idx++];
    initial_state.map["B1"] = pods_in_rooms[idx++];
    initial_state.map["C1"] = pods_in_rooms[idx++];
    initial_state.map["D1"] = pods_in_rooms[idx++];
    initial_state.map["A2"] = pods_in_rooms[idx++];
    initial_state.map["B2"] = pods_in_rooms[idx++];
    initial_state.map["C2"] = pods_in_rooms[idx++];
    initial_state.map["D2"] = pods_in_rooms[idx++];
    std::getline(infile, line);
  }

  const State goal_state{{
    {"H1", '@'}, {"H2", '@'}, {"H3", '@'}, {"H4", '@'},
    {"H5", '@'}, {"H6", '@'}, {"H7", '@'},
    {"A1", 'A'}, {"A2", 'A'},
    {"B1", 'B'}, {"B2", 'B'},
    {"C1", 'C'}, {"C2", 'C'},
    {"D1", 'D'}, {"D2", 'D'},
  }};


  struct AstarNode {
    State state;
    int cost;
    int heuristic;
  };

  std::unordered_set<std::map<std::string, char>> visited;

  auto get_heuristic = [&](const State& s) -> int {
    auto result{0};
    for (const auto& [pos,occ]: s.map) {
      if (occ == '@')
        continue;

      if (pos[0] == 'H') {
        for (const auto& c: all_connections) {
          if (pos != c.from)
            continue;
          if (occ == c.to[0]) {
            result += c.cost * cost_multiplier.at(occ);
            break;
          }
        }
      } else if (pos[0] != occ) {
        result += (2 + (1 + (pos[1] - '0'))) * cost_multiplier.at(occ);
      }
    }
    return result;
  };

  auto cmp = [](const AstarNode& a, const AstarNode& b) {
    return (a.cost + a.heuristic) > (b.cost + b.heuristic);
  };
  std::priority_queue<AstarNode, 
                      std::vector<AstarNode>,
                      decltype(cmp)> queue(cmp);
  queue.push({initial_state, 0, 1});

  int maxcost{0};

  while (queue.size()) {
    const auto node = queue.top();
    queue.pop();

    if (node.heuristic == 0) {
      std::cout << "goal state reached " << node.cost << std::endl;
      break;
    }
    if (visited.count(node.state.map)) {
      //std::cout << "already visited\n";
      continue;
    }
    visited.insert(node.state.map);

    if (node.cost > maxcost) {
      maxcost = node.cost;
      std::cout << maxcost << std::endl;
      std::cout << "#############\n#"
                << node.state.map.at("H1")
                << node.state.map.at("H2")
                << '.'
                << node.state.map.at("H3")
                << '.'
                << node.state.map.at("H4")
                << '.'
                << node.state.map.at("H5")
                << '.'
                << node.state.map.at("H6")
                << node.state.map.at("H7")
                << "#\n  #"
                << node.state.map.at("A1") << '#'
                << node.state.map.at("B1") << '#'
                << node.state.map.at("C1") << '#'
                << node.state.map.at("D1") << "#\n  #"
                << node.state.map.at("A2") << '#'
                << node.state.map.at("B2") << '#'
                << node.state.map.at("C2") << '#'
                << node.state.map.at("D2") << "#\n  #########\n\n";
    }

    for (const auto& [pos,occ]: node.state.map) {
      if (occ == '@')
        continue;

      if (pos[0] == 'H') {
        for (const auto& c: all_connections) {
          if (c.from != pos)
            continue;
          if (c.to[0] != occ)
            continue;
          if (node.state.map.at(c.to) != '@')
            continue;
          bool blocked{false};
          for (const auto& b: c.blockers)
            blocked |= (node.state.map.at(b) != '@');
          if (blocked)
            continue;
          bool others_in_my_room{false};
          for (auto depth{1}; depth <= 2; ++depth) {
            const std::string room{occ, (char)('0'+depth)};
            const auto in{node.state.map.at(room)};
            if (in != '@' and in != occ)
              others_in_my_room = true;
          }
          if (others_in_my_room)
            continue;
          
          auto next{node};
          next.state.map[c.to] = occ;
          next.state.map[c.from] = '@';
          if (visited.count(next.state.map))
            continue;
          next.cost += c.cost * cost_multiplier.at(occ);
          next.heuristic = get_heuristic(next.state);
          queue.push(next);
        }
      } else {
        for (const auto& c: all_connections) {
          if (c.to != pos)
            continue;
          if (node.state.map.at(c.from) != '@')
            continue;
          bool blocked{false};
          for (const auto& b: c.blockers)
            blocked |= (node.state.map.at(b) != '@');
          if (blocked)
            continue;

          auto next{node};
          next.state.map[c.from] = occ;
          next.state.map[c.to] = '@';
          if (visited.count(next.state.map))
            continue;
          next.cost += c.cost * cost_multiplier.at(occ);
          next.heuristic = get_heuristic(next.state);
          queue.push(next);
        }
      }
    }
  }



  return EXIT_SUCCESS;
}

