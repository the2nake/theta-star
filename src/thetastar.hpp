#pragma once

#include "pathfind.hpp"

namespace pf {

#ifndef DEMO
std::vector<coord> theta_star(grid &g, coord start, coord end);
#else
enum class step_type { queue, pop, process, check_vis };

struct step {
  step_type type;
  std::vector<coord> locations;
};

std::vector<coord> theta_star(grid &g, coord start, coord end,
                              std::vector<step> &record);
#endif

std::string to_string(const std::vector<coord> &path);

};  // namespace pf
