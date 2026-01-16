#include <print>

#include "pathfind.hpp"
#include "thetastar.hpp"

// TODO: figure out unit testing
// handle exceptions:
// * reaching max depth (circular trace)
// * mismatched nodes size
// * coordinate outside of grid
// * wall found in trace

int main() {
  pf::grid g(10, 10);
  g.at({9, 3}).parent = {0, 0};
  g.at({0, 0}).parent = {9, 3};

  // g.at({0, 0}).cost = -1.f;

  try {
    auto result = pf::theta_star(g, {0, 0}, {9, 5});
    std::print("{}", pf::to_string(g, result));
    std::println("path:\n{}", pf::to_string(result));
  } catch (std::exception &e) { std::print("error: {}\n", e.what()); }
}
