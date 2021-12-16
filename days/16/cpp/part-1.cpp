#include <cstdlib>
#include <algorithm>
#include <array>
#include <fstream>
#include <functional>
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
  
  std::string input;
  infile >> input;
  //std::cout << input << std::endl;

  /// Cannot be const :(
  std::map<char, std::array<int,4>> charmap{
    {'0', {0,0,0,0}},
    {'1', {0,0,0,1}},
    {'2', {0,0,1,0}},
    {'3', {0,0,1,1}},
    {'4', {0,1,0,0}},
    {'5', {0,1,0,1}},
    {'6', {0,1,1,0}},
    {'7', {0,1,1,1}},
    {'8', {1,0,0,0}},
    {'9', {1,0,0,1}},
    {'A', {1,0,1,0}},
    {'B', {1,0,1,1}},
    {'C', {1,1,0,0}},
    {'D', {1,1,0,1}},
    {'E', {1,1,1,0}},
    {'F', {1,1,1,1}},
  };

  std::vector<int> BITS;
  for (auto c: input)
    for (auto bit: charmap[c])
      BITS.push_back(bit);
  //for (auto b: BITS)
  //  std::cout << b;
  //std::cout << std::endl;

  int sum_of_version_numbers{0};

  std::function<void(std::size_t&, std::size_t&)> parse;
  parse = [&](std::size_t& a, std::size_t& b) {
    //std::cout << "parsing from " << a << " to before " << b << "\n";
    if (b <= a+6) {
      //std::cout << "stop parse\n";
      a = b;
      return;
    }

    const int version = (BITS[a+0] << 2) +
                        (BITS[a+1] << 1) +
                        (BITS[a+2] << 0);
    a += 3;
    //std::cout << "version: " << version << std::endl;
    sum_of_version_numbers += version;
    const int type = (BITS[a+0] << 2) +
                     (BITS[a+1] << 1) +
                     (BITS[a+2] << 0);
    a += 3;

    switch (type) {
      case 4: {
        unsigned number{0};
        for (;;) {
          number = (number    << 4) +
                   (BITS[a+1] << 3) +
                   (BITS[a+2] << 2) +
                   (BITS[a+3] << 1) +
                   (BITS[a+4] << 0);
          a += 5;
          if (not BITS[a-5])
            break;
        }
        //std::cout << "number: " << number << std::endl;
        break;
      }
      default: {
        const int length_type = BITS[a++];
        if (length_type == 0) {
          unsigned num_bits{0};
          for (auto i{0}; i < 15; ++i, ++a)
            num_bits = (num_bits << 1) + BITS[a];
          //std::cout << num_bits << " sub-packet bits\n";
          std::size_t sub_end{a + num_bits};
          while (a < sub_end)
            parse(a, sub_end);
        } else {
          unsigned num_packets{0};
          for (auto i{0}; i < 11; ++i, ++a)
            num_packets = (num_packets << 1) + BITS[a];
          //std::cout << num_packets << " sub-packets\n";
          for (auto i{0u}; i < num_packets; ++i)
            parse(a, b);
        }
      }
    }
  };

  std::size_t a{0}, b{BITS.size()};
  parse(a, b);


  std::cout << sum_of_version_numbers << std::endl;

  return EXIT_SUCCESS;
}

