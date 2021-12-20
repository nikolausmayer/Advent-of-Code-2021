#include <cstdlib>
#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>


std::ostream& operator<<(std::ostream& os, const std::array<int,3>& p)
{
  os << "[" << p[0] << "," << p[1] << "," << p[2] << "]";
  return os;
}


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
  
  std::vector<std::vector<std::array<int,3>>> scanners;
  {
    std::vector<std::array<int,3>> beacons;
    while (not infile.eof()) {
      std::string line;
      std::getline(infile, line);
      for (;;) {
        std::getline(infile, line);
        if (line == "" or infile.eof()) {
          scanners.push_back(beacons);
          beacons.clear();
          break;
        }
        int x,y,z;
        auto comma1_at{std::find(std::begin(line), std::end(line), ',')};
        x = std::stoi(line.substr(0, std::distance(std::begin(line),
                                                   comma1_at)));
        ++comma1_at;
        auto comma2_at{std::find(comma1_at, std::end(line), ',')};
        y = std::stoi(line.substr(std::distance(std::begin(line), comma1_at), 
                                  std::distance(comma1_at, comma2_at)));
        ++comma2_at;
        z = std::stoi(line.substr(std::distance(std::begin(line), comma2_at)));

        beacons.push_back({x,y,z});
      }
    }
  }

  /// choose B to match to A (A is so-far match)
  /// for all 24 possible orientations of B
  /// for all points pA in A
    /// for all points pB in B
    /// match pA to pB, seek B for matches in A
    /// if >=12 matches, merge B orientation into A

  auto rotate = [](int i, const std::array<int,3>& p) -> std::array<int,3> {
    const int a{p[0]};
    const int b{p[1]};
    const int c{p[2]};
    switch (i) {
      case  0: return { a,  b,  c};
      case  1: return { a,  c, -b};
      case  2: return { a, -b, -c};
      case  3: return { a, -c,  b};

      case  4: return {-a, -b,  c};
      case  5: return {-a,  c,  b};
      case  6: return {-a,  b, -c};
      case  7: return {-a, -c, -b};


      case  8: return { b,  c,  a};
      case  9: return { b,  a, -c};
      case 10: return { b, -c, -a};
      case 11: return { b, -a,  c};

      case 12: return {-b, -c,  a};
      case 13: return {-b,  a,  c};
      case 14: return {-b,  c, -a};
      case 15: return {-b, -a, -c};


      case 16: return { c,  a,  b};
      case 17: return { c,  b, -a};
      case 18: return { c, -a, -b};
      case 19: return { c, -b,  a};

      case 20: return {-c, -a,  b};
      case 21: return {-c,  b,  a};
      case 22: return {-c,  a, -b};
      case 23: return {-c, -b, -a};
      default: return {0,0,0};
    }
  };

  std::vector<bool> matched(scanners.size(), false);
  matched[0] = true;
  while (not std::all_of(std::begin(matched),
                         std::end(matched),
                         [](auto b){return b;})) {
    for (auto indexA{0u}; indexA < scanners.size(); ++indexA) {
      if (indexA > 0 and matched[indexA])
        continue;
      for (auto indexB{indexA+1}; indexB < scanners.size(); ++indexB) {
        if (matched[indexB])
          continue;
        bool match_found{false};
        for (auto rot{0}; rot < 24 and not match_found; ++rot) {
          //std::cout << "rot: " << rot << std::endl;
          for (const auto& pA: scanners[indexA]) {
            for (const auto& pB: scanners[indexB]) {
              const auto rpB = rotate(rot, pB);
              decltype(pA) shift{pA[0] - rpB[0], pA[1] - rpB[1], pA[2] - rpB[2]};
              int matches{0};
              for (const auto& mpB: scanners[indexB]) {
                const auto rmpB = rotate(rot, mpB);
                decltype(pA) p{rmpB[0] + shift[0],
                               rmpB[1] + shift[1],
                               rmpB[2] + shift[2]};
                if (std::find(std::begin(scanners[indexA]),
                              std::end(scanners[indexA]),
                              p) != std::end(scanners[indexA])) {
                  ++matches;
                }
                if (matches >= 12)
                  break;
              }
              if (matches >= 12) {
                std::cout << matches << " matches, merging " << indexB
                          << " into " << indexA << std::endl;
                match_found = true;
                matched[indexB] = true;
                for (const auto& mpB: scanners[indexB]) {
                  const auto rmpB = rotate(rot, mpB);
                  decltype(pA) p{rmpB[0] + shift[0],
                                 rmpB[1] + shift[1],
                                 rmpB[2] + shift[2]};
                  if (std::find(std::begin(scanners[indexA]),
                                std::end(scanners[indexA]),
                                p) == std::end(scanners[indexA])) {
                    scanners[indexA].push_back(p);
                  }
                }
                break;
              }
            }
            if (match_found)
              break;
          }
        }
      }
    }
  }

  std::cout << scanners[0].size() << std::endl;


  return EXIT_SUCCESS;
}

