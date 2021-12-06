#include <cstdlib>
#include <fstream>
#include <iostream>
#include <numeric>
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

  std::vector<int> spawn_in_days(9, 0);
  while (true) {
    int i;
    infile >> i;
    if (infile.eof())
      break;
    ++spawn_in_days[i];
    char c;
    infile >> c;
  }

  for (auto day{0}; day < 80; ++day) {
    const auto tmp{spawn_in_days[0]};
    for (auto i{0}; i < 9; ++i)
      spawn_in_days[i] = spawn_in_days[i+1];
    spawn_in_days[6] += tmp;
    spawn_in_days[8] = tmp;

    //std::cout << "Day: " << day+1;
    //for (auto v: spawn_in_days) 
    //  std::cout << " " << v; 
    //std::cout << "\n";
  }
  
  std::cout << std::accumulate(std::begin(spawn_in_days), std::end(spawn_in_days), 0) << std::endl;

  return EXIT_SUCCESS;
}

