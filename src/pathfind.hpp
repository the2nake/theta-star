#pragma once

#include <set>
#include <string>
#include <vector>

namespace pf {

// row then column
using coord = std::pair<int, int>;

struct cell {
  cell();

  bool is_wall() const;

  coord parent;  // for storing the trace
  float cost;    // cost of moving to the cell
};

struct grid {
  grid(int h, int w);

  inline int as_idx(coord c) const { return w * c.first + c.second; }
  cell &operator[](coord c);
  const cell &operator[](coord c) const;

  bool in_bounds(coord c) const;
  cell &at(coord c);
  const cell &at(coord c) const;

  std::vector<coord> neighbours(coord c) const;

  std::vector<cell> nodes;
  const int h, w;
};

std::string to_string(const coord &c);
std::string to_string(const pf::grid &g,
                      const std::vector<coord> &highlight = {});
std::string to_string(const pf::grid &g, const std::set<coord> &highlight = {});

};  // namespace pf
