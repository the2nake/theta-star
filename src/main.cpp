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
  // std::string area =
  //     " ###   ###"
  //     " # #  ####"
  //     " # #   ###"
  //     " # #  ####"
  //     "       ###"
  //     "   #  ####"
  //     " ###    ##"
  //     " #      ##"
  //     " ###   ###"
  //     " ###   ###";

  std::string area =
      "          "
      "          "
      "          "
      " #####    "
      " #####    "
      " #####    "
      " #####    "
      "          "
      "          "
      "          ";
  g.load(area);

  try {
    auto result = pf::theta_star(g, {0, 2}, {9, 5});
    std::print("{}", pf::to_string(g, result));
    std::println("path:\n{}", pf::to_string(result));
  } catch (std::exception &e) { std::print("error: {}\n", e.what()); }
}
