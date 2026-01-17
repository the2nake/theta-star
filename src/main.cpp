// TODO: figure out unit testing
// handle exceptions:
// * reaching max depth (circular trace)
// * mismatched nodes size
// * coordinate outside of grid
// * wall found in trace

#include <print>

#include "pathfind.hpp"
#include "thetastar.hpp"

extern std::string maze;
extern std::string block;

void show(const pf::grid &g, const std::vector<pf::coord> &result) {
  std::print("{}", pf::to_string(g, result));
  std::println("path:\n{}", pf::to_string(result));
}

int main() {
  pf::grid g(10, 10);

  try {
    for (auto &setup : {maze, block}) {
      g.load(setup);
      show(g, pf::theta_star(g, {0, 2}, {9, 5}));
    }
  } catch (std::exception &e) { std::print("error: {}\n", e.what()); }
}

std::string maze =
    "   #   ###"
    " ###  ####"
    " # #   ###"
    " # #  ####"
    "       ###"
    "   #  ####"
    " ###    ##"
    " #      ##"
    " ###   ###"
    " ###   ###";

std::string block =
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
