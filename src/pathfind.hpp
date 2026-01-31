#pragma once

#include <set>
#include <string>
#include <string_view>
#include <vector>

namespace pf {

// row then column
using coord = std::pair<int, int>;

inline coord operator+(const coord& a, const coord& b) {
  return {a.first + b.first, a.second + b.second};
}

inline coord operator*(int n, const coord& c) {
  return {n * c.first, n * c.second};
};

struct cell {
  cell();

  bool is_wall() const;

  coord parent;  // for storing the trace
  float cost;    // cost of moving to the cell
};

struct grid {
  grid(int h, int w);

  std::vector<cell> nodes;
  const int h, w;

  void load(std::string_view str);
  std::vector<coord> neighbours(coord c) const;
  bool visible(coord a, coord b);

  inline int as_idx(coord c) const { return w * c.first + c.second; }
  inline bool in_bounds(coord c) const {
    return 0 <= c.first && c.first < h && 0 <= c.second && c.second < w;
  }
  inline cell& operator[](coord c) { return nodes[as_idx(c)]; }
  inline const cell& operator[](coord c) const { return nodes[as_idx(c)]; }

  cell& at(coord c);
  const cell& at(coord c) const;
};

std::string to_string(coord c);
std::string to_string(const grid& g, const std::vector<coord>& highlight = {});
std::string to_string(const grid& g, const std::set<coord>& highlight = {});

};  // namespace pf
