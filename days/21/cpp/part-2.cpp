#include <cstdlib>
#include <cstring>
#include <array>
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

  /// Universes are characterized as 4-tuples of player positions and scores
  /// Max position
  constexpr auto MP{10};
  /// Max score to consider
  constexpr auto MS{31};
  auto* universes      = new unsigned long[MP*MP*MS*MS];
  auto* next_universes = new unsigned long[MP*MP*MS*MS];
  auto g = [](unsigned long* u, int p0, int p1, int s0, int s1) -> auto& {
    //std::cout << "index " << p0 << " " << p1 << " " << s0 << " " << s1 << "\n";
    //std::cout << "address " << (p0*MP*MS*MS + p1   *MS*MS + s0      *MS + s1)*sizeof(unsigned long) << "\n";
    return u[p0*MP*MS*MS +
             p1   *MS*MS +
             s0      *MS +
             s1];
  };
  /// Initial state
  std::memset(universes,      0, MP*MP*MS*MS*sizeof(unsigned long));
  std::memset(next_universes, 0, MP*MP*MS*MS*sizeof(unsigned long));
  g(universes, positions[0], positions[1], 0, 0) = 1;

  /// Possible outcomes of 3d3 and their counts (3*3*3 = 27 total)
  constexpr std::array<std::array<int,2>,7> dice_outcomes{{
    {3, 1},
    {4, 3},
    {5, 6},
    {6, 7},
    {7, 6},
    {8, 3},
    {9, 1},
  }};

  for (auto round{1};; ++round) {
    std::cout << "round " << round << std::endl;
    /// Player 1
    for (auto p0{0}; p0 < MP; ++p0) {
    for (auto p1{0}; p1 < MP; ++p1) {
    for (auto s0{0}; s0 < 21; ++s0) {
    for (auto s1{0}; s1 < 21; ++s1) {
      g(next_universes, p0, p1, s0, s1) = 0;
    }}}}
    //std::memset(next_universes, 0, MP*MP*MS*MS*sizeof(unsigned long));
    for (auto p0{0}; p0 < MP; ++p0) {
    for (auto p1{0}; p1 < MP; ++p1) {
    for (auto s0{0}; s0 < 21; ++s0) {
    for (auto s1{0}; s1 < 21; ++s1) {
      for (const auto [dice, count]: dice_outcomes) {
        //std::cout << p0 << " " << p1 << " " << dice << " " << s0 << " " << s1 << "\n";
        g(next_universes, (p0+dice)%MP, p1, s0+((p0+dice)%MP)+1, s1) +=
        g(universes,       p0,          p1, s0,                  s1) * count;
      }
    }}}}
    std::memcpy(universes, next_universes, MP*MP*MS*MS*sizeof(unsigned long));

    ///// Check wins and TODO count and TODO mark
    //for (auto p0{0}; p0 < MP; ++p0) {
    //for (auto p1{0}; p1 < MP; ++p1) {
    //for (auto s0{21}; s0 < MS; ++s0) {
    //for (auto s1{0}; s1 < MS; ++s1) {
    //  if (g(universes, p0, p1, s0, s1) >= 21)

    //}}}}

    /// Player 2
    for (auto p0{0}; p0 < MP; ++p0) {
    for (auto p1{0}; p1 < MP; ++p1) {
    for (auto s0{0}; s0 < 21; ++s0) {
    for (auto s1{0}; s1 < 21; ++s1) {
      g(next_universes, p0, p1, s0, s1) = 0;
    }}}}
    //std::memset(next_universes, 0, MP*MP*MS*MS*sizeof(unsigned long));
    for (auto p0{0}; p0 < MP; ++p0) {
    for (auto p1{0}; p1 < MP; ++p1) {
    for (auto s0{0}; s0 < 21; ++s0) {
    for (auto s1{0}; s1 < 21; ++s1) {
      for (const auto [dice, count]: dice_outcomes) {
        g(next_universes, p0, (p1+dice)%MP, s0, s1+((p1+dice)%MP)+1) +=
        g(universes,      p0,  p1,          s0, s1                 ) * count;
      }
    }}}}
    std::memcpy(universes, next_universes, MP*MP*MS*MS*sizeof(unsigned long));

    /// Stop condition: all games are won
    bool all_games_are_over{true};
    unsigned long unwon_games{0};
    unsigned long won_games{0};
    for (auto p0{0}; p0 < MP; ++p0) {
    for (auto p1{0}; p1 < MP; ++p1) {
    for (auto s0{0}; s0 < 21; ++s0) {
    for (auto s1{0}; s1 < 21; ++s1) {
      all_games_are_over &= (g(universes, p0, p1, s0, s1) == 0);
      unwon_games += g(universes, p0, p1, s0, s1);
    }}
    for (auto s0{21}; s0 < MS; ++s0) {
    for (auto s1{0}; s1 < 21; ++s1) {
      won_games += g(universes, p0, p1, s0, s1);
    }}
    for (auto s0{0}; s0 < 21; ++s0) {
    for (auto s1{21}; s1 < MS; ++s1) {
      won_games += g(universes, p0, p1, s0, s1);
    }}
    }}
    std::cout << unwon_games << " games in flight, " << won_games << " over\n";
    if (all_games_are_over)
      break;
  }

  auto p0wins{0ul}, p1wins{1ul};
  for (auto p0{0}; p0 < MP; ++p0) {
  for (auto p1{0}; p1 < MP; ++p1) {
    for (auto s0{21}; s0 < MS; ++s0) {
    for (auto s1{0}; s1 < 21; ++s1) {
      p0wins += g(universes, p0, p1, s0, s1);
    }}
    for (auto s0{0}; s0 < 21; ++s0) {
    for (auto s1{21}; s1 < MS; ++s1) {
      p1wins += g(universes, p0, p1, s0, s1);
    }}
  }}

  std::cout << std::max(p0wins, p1wins) << std::endl;

  delete[] universes;
  delete[] next_universes;

  return EXIT_SUCCESS;
}

