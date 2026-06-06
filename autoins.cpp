/**
 * @author n685
 * @date Sunday, April 13, 2025 6:18:55 PM
 */
#include <chrono>
#include <format>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <print>
#include <random>
#include <set>
#include <string>

const std::vector include_dir{"", "include/", "include/ac-library/", "include/testlib/"};

void expand(const std::string& current_file, std::ofstream& output, std::set<std::string>& visited,
            const bool angled = false) {
  if (visited.contains(current_file)) {
    return;
  }
  visited.insert(current_file);
  if (angled) {
    std::println(output, "#include <{}>", current_file);
    return;
  }
  if (current_file == "dd/debug.h" || current_file == "bits/stdc++.h") {
    std::println(output, "#include \"{}\"", current_file);
    return;
  }

  std::ifstream input;
  for (std::string dir : include_dir) {
    input.open(dir + current_file);
    if (input.is_open())
      break;
  }
  if (!input.is_open()) {
    std::println(std::cerr, "Cannot access {:?}", current_file);
    std::println(output, "#include \"{}\"", current_file);
    return;
  }
  std::println(std::cerr, "Expanding {:?}", current_file);
  std::string current_line;
  while (std::getline(input, current_line)) {
    if (current_line.size() < 8 || current_line.substr(0, 8) != "#include") {
      std::println(output, "{}", current_line);
      continue;
    }
    std::string inner_file = current_line.substr(10);
    inner_file.pop_back();
    expand(inner_file, output, visited, current_line[9] == '<');
  }
}

int main(const int argv, char** argc) {
  if (argv != 3) {
    std::println(std::cerr, "Usage: ./autoins source.cpp dest.cpp");
    return 0;
  }
  std::set<std::string> visited;
  std::ofstream output(argc[2]);
  expand(argc[1], output, visited);
  std::mt19937_64 rng(std::chrono::steady_clock::now().time_since_epoch().count());
  std::cerr << rng() << '\n';
}
