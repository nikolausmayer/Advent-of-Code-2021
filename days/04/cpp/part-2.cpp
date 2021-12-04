#include <cstdlib>
#include <algorithm>
#include <array>
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

  std::vector<int> input;
  {
    std::string line;
    std::getline(infile, line);
    int num{0};
    for (auto c : line) {
      if (c == ',') {
        input.push_back(num);
        num = 0;
      } else {
        num = num*10 + (c - '0');
      }
    }
    input.push_back(num);
  }
  std::vector<std::array<std::array<int,5>,5>> boards;
  while (not infile.eof()) {
    std::array<std::array<int,5>,5> board;
    for (auto row{0}; row < 5; ++row) {
      for (auto col{0}; col < 5; ++col) {
        int num;
        infile >> num;
        board[row][col] = num;
      }
    }
    if (infile.eof()) { break; }
    boards.push_back(board);
  }

  //for (auto n: input) std::cout << n << " "; std::cout << "\n\n";
  //for (auto b: boards) {
  //  for (auto row: b) {
  //    for (auto n: row)
  //      std::cout << n << " ";
  //    std::cout << "\n";
  //  }
  //  std::cout << "\n";
  //}

  std::vector<std::size_t> skip_boards;
  for (auto num: input) {
    for (auto bidx{0u}; bidx < boards.size(); ++bidx) {
      if (std::find(std::begin(skip_boards),
                    std::end(skip_boards),
                    bidx) != std::end(skip_boards))
        continue;
      auto& board{boards[bidx]};
      for (auto row{0u}; row < 5; ++row) {
        for (auto col{0u}; col < 5; ++col) {
          if (board[row][col] == num) {
            board[row][col] *= -1;
            bool won{true};
            for (auto i{0u}; i < 5; ++i)
              won &= (board[row][i] <= 0);
            if (not won) {
              won = true;
              for (auto i{0u}; i < 5; ++i)
                won &= (board[i][col] <= 0);
            }
            if (won) {
              if (skip_boards.size()+1 == boards.size()) {
                auto unmarked_sum{0};
                for (auto r: board)
                  for (auto n: r)
                    if (n > 0)
                      unmarked_sum += n;
                std::cout << unmarked_sum * num << std::endl;
                return EXIT_SUCCESS;
              } else {
                skip_boards.push_back(bidx);
              }
            }
          }
        }
      }
    }
  }


  return EXIT_SUCCESS;
}

