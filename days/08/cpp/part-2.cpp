#include <cstdlib>
#include <bitset>
#include <fstream>
#include <iostream>
#include <unordered_set>
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

  std::vector<std::vector<std::string>> inputs;
  std::vector<std::vector<std::string>> outputs;
  while (true) {
    std::vector<std::string> in, out;
    std::string digit;
    for (auto i{0}; i < 10; ++i) {
      infile >> digit;
      in.push_back(digit);
    }
    infile >> digit;
    for (auto i{0}; i < 4; ++i) {
      infile >> digit;
      out.push_back(digit);
    }
    if (infile.eof())
      break;
    inputs.push_back(in);
    outputs.push_back(out);
  }

  auto result{0};
  for (auto iidx{0u}; iidx < inputs.size(); ++iidx) {
    std::unordered_set<std::bitset<7>> pool;
    for (const auto& s: inputs[iidx]) {
      unsigned i{0};
      for (auto c: s)
        i += (1 << (c - 'a'));
      pool.insert(std::bitset<7>(i));
    }
    std::bitset<7> known[10];

    /// Find unique numbers: 1, 4, 7, 8
    for (const auto& b: pool)
      if (b.count() == 2) { known[1] = b; pool.erase(b); break; }
    for (const auto& b: pool)
      if (b.count() == 3) { known[7] = b; pool.erase(b); break; }
    for (const auto& b: pool)
      if (b.count() == 4) { known[4] = b; pool.erase(b); break; }
    for (const auto& b: pool)
      if (b.count() == 7) { known[8] = b; pool.erase(b); break; }
    /// X 1 X X 4 X X 7 8 X

    //std::cout << "1: " << known[1] << ", "
    //          << "4: " << known[4] << ", "
    //          << "7: " << known[7] << ", "
    //          << "8: " << known[8] << std::endl;

    ///// Number with 5 segments and all of "7" is "3"
    for (const auto& b: pool)
      if (b.count() == 5 and (b & known[7]) == known[7]) 
        { known[3] = b; pool.erase(b); break; }
    //std::cout << "3: " << known[3] << std::endl;
    /// X 1 X 3 4 X X 7 8 X

    ///// Number with 6 segments and 2 segments not in "4" is "9"
    for (const auto& b: pool)
      if (b.count() == 6 and (b & ~known[4]).count() == 2) 
        { known[9] = b; pool.erase(b); break; }
    //std::cout << "9: " << known[9] << std::endl;
    /// X 1 X 3 4 X X 7 8 9

    /// Number not containing a segment not in "9" is "5"
    for (const auto& b: pool)
      if ((b & ~known[9]).count() == 0) 
        { known[5] = b; pool.erase(b); break; }
    //std::cout << "5: " << known[5] << std::endl;
    /// X 1 X 3 4 5 X 7 8 9

    /// Remaining number with 5 segments is "2"
    for (const auto& b: pool)
      if (b.count() == 5) 
        { known[2] = b; pool.erase(b); break; }
    //std::cout << "2: " << known[2] << std::endl;
    /// X 1 2 3 4 5 X 7 8 9

    /// Number with all of "1" is "0"
    for (const auto& b: pool)
      if ((b & known[1]) == known[1]) 
        { known[0] = b; pool.erase(b); break; }
    //std::cout << "0: " << known[0] << std::endl;
    /// 0 1 2 3 4 5 X 7 8 9
    
    /// Last number is "6"
    known[6] = *pool.begin();
    //std::cout << "6: " << known[6] << std::endl;
    /// 0 1 2 3 4 5 6 7 8 9

    /// Now let's get this bread
    /// (this line is going to age horribly)
    int displayed{0};
    for (const auto& s: outputs[iidx]) {
      unsigned i{0};
      for (auto c: s)
        i += (1 << (c - 'a'));
      const auto b{std::bitset<7>(i)};

      for (auto i{0}; i < 10; ++i)
        if (b == known[i])
          displayed = displayed*10 + i;
    }
    result += displayed;
  }

  std::cout << result << std::endl;

  return EXIT_SUCCESS;
}

