#pragma once

#include "pathfind.hpp"

namespace pf {

std::vector<coord> theta_star(grid& g, coord start, coord end);

std::string to_string(const std::vector<coord>& path);

};  // namespace pf
