#include <cstdlib>
#include <fstream>
#include <iostream>
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
  
  std::vector<int> nums;
  {
    int i{0};
    bool readnum{false};
    bool negative{false};
    while (not infile.eof()) {
      auto c{infile.get()};
      if (c >= '0' and c <= '9') {
        readnum = true;
        i = 10*i + (c - '0');
      } else if (c == '-') {
        negative = true;
      } else {
        if (readnum) {
          nums.push_back(i * (negative ? -1 : 1));
          readnum = false;
        }
        i = 0;
        negative = false;
      }
    }
  }
  const int L{nums[0]}, R{nums[1]}, B{nums[2]}, T{nums[3]};

  auto minsteps{0};
  for (auto xint{0}; xint < L; xint+=(++minsteps)) { }

  unsigned hitters{0};
  for (auto oxvel{minsteps}; oxvel <= R; ++oxvel) {
    for (auto oyvel{B}; oyvel <= std::abs(B); ++oyvel) {
      int xvel{oxvel}, yvel{oyvel};
      //std::cout << "Testing (" << xvel << "," << yvel << ")\n";
      int x{0}, y{0};
      bool hitstarget{false};
      for (;;) {
        x += xvel;
        y += yvel;
        if (xvel > 0)
          --xvel;
        else if (xvel < 0)
          ++xvel;
        --yvel;
        if (x >= L and x <= R and y <= T and y >= B)
          hitstarget = true;
        if (x > R or y < B)
          break;
      }
      if (hitstarget)
        ++hitters;
    }
  }

  std::cout << hitters << std::endl;


  return EXIT_SUCCESS;
}

