#include <cstdlib>
#include <array>
#include <fstream>
#include <iostream>
#include <map>
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

  std::map<std::array<int,3>,int> reactor;
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
    //std::cout << xa << '|' << xb << '|'
    //          << ya << '|' << yb << '|'
    //          << za << '|' << zb << (on_step ? "  ON" : "  OFF") << "\n";

    for (auto x{std::max(xa,-50)}; x <= std::min(xb,50); ++x) {
    for (auto y{std::max(ya,-50)}; y <= std::min(yb,50); ++y) {
    for (auto z{std::max(za,-50)}; z <= std::min(zb,50); ++z) {
      reactor[{x,y,z}] = (on_step ? 1 : 0);
    }}}
  }

  auto count{0};
  for (const auto& [k,v]: reactor)
    count += v;
  std::cout << count << std::endl;

  return EXIT_SUCCESS;
}

