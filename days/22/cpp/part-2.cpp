#include <cstdlib>
#include <fstream>
#include <iostream>
#include <list>
#include <string>
#include <vector>

constexpr bool PRINT{false};

struct Region {
  int xa, xb, ya, yb, za, zb;

  bool operator==(const Region& other) const 
  {
    return (xa == other.xa and xb == other.xb and
            ya == other.ya and yb == other.yb and
            za == other.za and zb == other.zb);
  }

  friend std::ostream& operator<<(std::ostream& os, 
                                  const Region& r) 
  {
    std::cout << r.xa << ".." << r.xb << ","
              << r.ya << ".." << r.yb << ","
              << r.za << ".." << r.zb;
    return os;
  }

  unsigned long getSize() const
  {
    return (unsigned long)(xb - xa + 1) *
           (unsigned long)(yb - ya + 1) *
           (unsigned long)(zb - za + 1);
  }

  bool isContainedIn(const Region& other) const
  {
    return (xa >= other.xa and xb <= other.xb and
            ya >= other.ya and yb <= other.yb and
            za >= other.za and zb <= other.zb);
  }

  bool isDisjointWith(const Region& other) const
  {
    return (xa > other.xb or xb < other.xa or
            ya > other.yb or yb < other.ya or
            za > other.zb or zb < other.za);
  }

  std::list<Region> subtract(const Region& other) const
  {
    std::list<Region> results;
    /// If disjoint, return self
    if (isDisjointWith(other)) {
      results.push_back(*this);
      return results;
    }
    /// If self is contained in other, return empty result
    if (isContainedIn(other)) {
      return results;
    }

    const int xai{std::max(xa, other.xa)};
    const int xao{std::min(xa, other.xa)};
    const int yai{std::max(ya, other.ya)};
    const int yao{std::min(ya, other.ya)};
    const int zai{std::max(za, other.za)};
    const int zao{std::min(za, other.za)};
    const int xbi{std::min(xb, other.xb)};
    const int xbo{std::max(xb, other.xb)};
    const int ybi{std::min(yb, other.yb)};
    const int ybo{std::max(yb, other.yb)};
    const int zbi{std::min(zb, other.zb)};
    const int zbo{std::max(zb, other.zb)};
    /// "A" shapes exist in every configuration
    if (xa < other.xa) {
      results.push_back({xao, xai-1, yai, ybi, zai, zbi});
      if (PRINT) std::cout << "  A " << results.back() << "\n";
    }
    if (xb > other.xb) {
      results.push_back({xbi+1, xbo, yai, ybi, zai, zbi});
      if (PRINT) std::cout << "  B " << results.back() << "\n";
    }
    if (ya < other.ya) {
      results.push_back({xai, xbi, yao, yai-1, zai, zbi});
      if (PRINT) std::cout << "  C " << results.back() << "\n";
    }
    if (yb > other.yb) {
      results.push_back({xai, xbi, ybi+1, ybo, zai, zbi});
      if (PRINT) std::cout << "  D " << results.back() << "\n";
    }
    if (za < other.za) {
      results.push_back({xai, xbi, yai, ybi, zao, zai-1});
      if (PRINT) std::cout << "  E " << results.back() << "\n";
    }
    if (zb > other.zb) {
      results.push_back({xai, xbi, yai, ybi, zbi+1, zbo});
      if (PRINT) std::cout << "  F " << results.back() << "\n";
    }
    /// "B" shapes
    if (xa < other.xa) {
      if (ya < other.ya) {
        results.push_back({xao, xai-1, yao, yai-1, zai, zbi});
        if (PRINT) std::cout << "  G " << results.back() << "\n";
        /// "C"
        if (za < other.za) {
          results.push_back({xao, xai-1, yao, yai-1, zao, zai-1});
          if (PRINT) std::cout << "  H " << results.back() << "\n";
        }
        if (zb > other.zb) {
          results.push_back({xao, xai-1, yao, yai-1, zbi+1, zbo});
          if (PRINT) std::cout << "  I " << results.back() << "\n";
        }
      }
      if (yb > other.yb) {
        results.push_back({xao, xai-1, ybi+1, ybo, zai, zbi});
        if (PRINT) std::cout << "  J " << results.back() << "\n";
        /// "C"
        if (za < other.za) {
          results.push_back({xao, xai-1, ybi+1, ybo, zao, zai-1});
          if (PRINT) std::cout << "  K " << results.back() << "\n";
        }
        if (zb > other.zb) {
          results.push_back({xao, xai-1, ybi+1, ybo, zbi+1, zbo});
          if (PRINT) std::cout << "  L " << results.back() << "\n";
        }
      }

      if (za < other.za) {
        results.push_back({xao, xai-1, yai, ybi, zao, zai-1});
        if (PRINT) std::cout << "  M " << results.back() << "\n";
      }
      if (zb > other.zb) {
        results.push_back({xao, xai-1, yai, ybi, zbi+1, zbo});
        if (PRINT) std::cout << "  N " << results.back() << "\n";
      }
    }

    if (xb > other.xb) {
      if (ya < other.ya) {
        results.push_back({xbi+1, xbo, yao, yai-1, zai, zbi});
        if (PRINT) std::cout << "  O " << results.back() << "\n";
        if (za < other.za) {
          results.push_back({xbi+1, xbo, yao, yai-1, zao, zai-1});
          if (PRINT) std::cout << "  P " << results.back() << "\n";
        }
        if (zb > other.zb) {
          results.push_back({xbi+1, xbo, yao, yai-1, zbi+1, zbo});
          if (PRINT) std::cout << "  Q " << results.back() << "\n";
        }
      }
      if (yb > other.yb) {
        results.push_back({xbi+1, xbo, ybi+1, ybo, zai, zbi});
        if (PRINT) std::cout << "  R " << results.back() << "\n";
        if (za < other.za) {
          results.push_back({xbi+1, xbo, ybi+1, ybo, zao, zai-1});
          if (PRINT) std::cout << "  S " << results.back() << "\n";
        }
        if (zb > other.zb) {
          results.push_back({xbi+1, xbo, ybi+1, ybo, zbi+1, zbo});
          if (PRINT) std::cout << "  T " << results.back() << "\n";
        }
      }

      if (za < other.za) {
        results.push_back({xbi+1, xbo, yai, ybi, zao, zai-1});
        if (PRINT) std::cout << "  U " << results.back() << "\n";
      }
      if (zb > other.zb) {
        results.push_back({xbi+1, xbo, yai, ybi, zbi+1, zbo});
        if (PRINT) std::cout << "  V " << results.back() << "\n";
      }
    }

    if (ya < other.ya) {
      if (za < other.za) {
        results.push_back({xai, xbi, yao, yai-1, zao, zai-1});
        if (PRINT) std::cout << "  W " << results.back() << "\n";
      }
      if (zb > other.zb) {
        results.push_back({xai, xbi, yao, yai-1, zbi+1, zbo});
        if (PRINT) std::cout << "  X " << results.back() << "\n";
      }
    }
    if (yb > other.yb) {
      if (za < other.za) {
        results.push_back({xai, xbi, ybi+1, ybo, zao, zai-1});
        if (PRINT) std::cout << "  Y " << results.back() << "\n";
      }
      if (zb > other.zb) {
        results.push_back({xai, xbi, ybi+1, ybo, zbi+1, zbo});
        if (PRINT) std::cout << "  Z " << results.back() << "\n";
      }
    }

    return results;
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



  std::vector<Region> disjoint_on_regions;

  for (;;) {
    bool on_step;
    int xa, xb, ya, yb, za, zb;
    infile.get();
    if (infile.eof())
      break;

    auto c = infile.get();
    if (c == 'n') {
      on_step = true;
    } else {
      on_step = false;
      infile.get();
    }

    infile.get();
    infile.get();
    infile.get();
    int n{0};
    bool neg{false};
    for (;;) {
      c = infile.get();
      if (c == '.') { infile.get(); xa = n * (neg ? -1 : 1); n = 0; neg = false; break; }
      if (c == '-') { neg = true; continue; }
      n = 10*n + (c - '0');
    }
    for (;;) {
      c = infile.get();
      if (c == ',') { infile.get(); xb = n * (neg ? -1 : 1); n = 0; neg = false; break; }
      if (c == '-') { neg = true; continue; }
      n = 10*n + (c - '0');
    }
    infile.get();
    for (;;) {
      c = infile.get();
      if (c == '.') { infile.get(); ya = n * (neg ? -1 : 1); n = 0; neg = false; break; }
      if (c == '-') { neg = true; continue; }
      n = 10*n + (c - '0');
    }
    for (;;) {
      c = infile.get();
      if (c == ',') { infile.get(); yb = n * (neg ? -1 : 1); n = 0; neg = false; break; }
      if (c == '-') { neg = true; continue; }
      n = 10*n + (c - '0');
    }
    infile.get();
    for (;;) {
      c = infile.get();
      if (c == '.') { infile.get(); za = n * (neg ? -1 : 1); n = 0; neg = false; break; }
      if (c == '-') { neg = true; continue; }
      n = 10*n + (c - '0');
    }
    for (;;) {
      c = infile.get();
      if (c == '\n') { zb = n * (neg ? -1 : 1); n = 0; neg = false; break; }
      if (c == '-') { neg = true; continue; }
      n = 10*n + (c - '0');
    }
    if (PRINT) 
      std::cout << xa << '|' << xb << '|'
                << ya << '|' << yb << '|'
                << za << '|' << zb << (on_step ? "  ON" : "  OFF") << "\n";
  
    const Region new_region{xa,xb,ya,yb,za,zb};
    decltype(disjoint_on_regions) next_regions;

    for (const auto& r: disjoint_on_regions)
      for (auto nr: r.subtract(new_region))
        next_regions.push_back(nr);
    if (on_step)
      next_regions.push_back(new_region);

    disjoint_on_regions = next_regions;

    unsigned long result{0};
    for (const auto& r: disjoint_on_regions)
      result += r.getSize();
    if (PRINT)
      std::cout << result << " cubes are ON in " << disjoint_on_regions.size() 
                << " elements" << std::endl;
  }

  unsigned long result{0};
  for (const auto& r: disjoint_on_regions)
    result += r.getSize();
  std::cout << result << std::endl;


  return EXIT_SUCCESS;
}

