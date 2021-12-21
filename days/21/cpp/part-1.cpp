#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>


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

  int positions[2] = {0};
  {
    std::string dummy;
    int playernum, startingpos;
    for (auto i{0}; i < 2; ++i) {
      infile >> dummy >> playernum;
      infile >> dummy >> dummy >> startingpos;
      positions[playernum-1] = startingpos-1;
    }
  }

  int scores[2] = {0};
  unsigned die_rolls{0};
  int deterministic_die{0};
  for (;;) {
    for (auto player{0}; player < 2; ++player) {
      //std::cout << "Player " << player+1 << " rolls";
      for (auto die_roll{0}; die_roll < 3; ++die_roll) {
        //std::cout << " " << deterministic_die+1;
        positions[player] = (positions[player] + deterministic_die+1)%10;
        deterministic_die = (deterministic_die+1)%100;
        ++die_rolls;
      }
      scores[player] += positions[player]+1;
      //std::cout << " and moves to " << positions[player]+1 << ", score now "
      //          << scores[player] << "\n";
      if (scores[player] >= 1000) {
        std::cout << "Player " << player+1 << "/2 wins with score "
                  << scores[player] << " > " << scores[1-player]
                  << " after " << die_rolls << " die rolls, result is "
                  << scores[1-player] * die_rolls << std::endl;
        return EXIT_SUCCESS;
      }
    }
  }


  return EXIT_SUCCESS;
}

