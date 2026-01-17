#include "pathfind.hpp"

#include <cmath>
#include <limits>
#include <stdexcept>

namespace pf {

std::string to_string(const pf::coord &c) {
  return "(" + std::to_string(c.first) + " " + std::to_string(c.second) + ")";
}

std::string to_string(const pf::grid &g, const std::vector<coord> &highlight) {
  std::set<coord> s(highlight.begin(), highlight.end());
  return to_string(g, s);
}

std::string to_string(const pf::grid &g, const std::set<coord> &highlight) {
  std::string res;
  for (int i = -1; i <= g.h; ++i) {
    if (i == -1) {
      res += "┌";
    } else if (i == g.h) {
      res += "└";
    } else {
      res += "│";
    }

    for (int j = 0; j < g.w; ++j) {
      if (i == -1 || i == g.h) {
        res += "─";
        continue;
      }

      if (g[{i, j}].is_wall()) {
        res += "#";
      } else if (highlight.contains({i, j})) {
        res += "o";
      } else {
        res += " ";
      }
    }

    if (i == -1) {
      res += "┐";
    } else if (i == g.h) {
      res += "┘";
    } else {
      res += "│";
    }
    res += "\n";
  }
  return res;
}

cell::cell() : parent({-1, -1}), cost(1.0) {}
bool cell::is_wall() const { return std::isinf(cost); }

grid::grid(int h, int w) : h(h), w(w), nodes(h * w) {}

void grid::load(std::string_view str) {
  if (str.size() != nodes.size())
    throw std::logic_error("string does not match grid size");

  auto it = nodes.begin();
  for (auto &c : str) {
    switch (c) {
      case ' ':
        it->cost = 1.f;
        break;
      case '#':
        it->cost = std::numeric_limits<float>::infinity();
        break;
      default:
        it->cost = 1.f;
        break;
    }
    ++it;
  }
}

cell &grid::operator[](coord c) { return nodes[w * c.first + c.second]; }

const cell &grid::operator[](coord c) const { return nodes[as_idx(c)]; }

bool grid::in_bounds(coord c) const {
  return 0 <= c.first && c.first < h || 0 <= c.second || c.second < w;
}

cell &grid::at(coord c) {
  if (!in_bounds(c)) {
    std::string str = to_string(c) + " not inside grid";
    throw std::out_of_range(str);
  }
  return operator[](c);
}

const cell &grid::at(coord c) const {
  if (!in_bounds(c)) {
    std::string str = to_string(c) + " not inside grid";
    throw std::out_of_range(str);
  }
  return operator[](c);
}

std::vector<coord> grid::neighbours(coord c) const {
  if (!in_bounds(c)) return {};

  std::vector<coord> res;
  res.reserve(8);

  int i0 = std::max(c.first - 1, 0), i1 = std::min(c.first + 1, h - 1);
  int j0 = std::max(c.second - 1, 0), j1 = std::min(c.second + 1, w - 1);

  for (int i = i0; i <= i1; ++i) {
    for (int j = j0; j <= j1; ++j) {
      if (nodes[w * i + j].cost >= 0.f) res.emplace_back(i, j);
    }
  }

  return res;
}

};  // namespace pf
