#include "thetastar.hpp"

#include <algorithm>
#include <cmath>
#include <limits>
#include <queue>
#include <stdexcept>

namespace pf {

template <typename T, typename U>
using container = std::vector<std::pair<T, U>>;

template <typename T, typename U>
using min_queue = std::priority_queue<std::pair<T, U>, container<T, U>,
                                      decltype([](auto& a, auto& b) {
                                        return a.second > b.second;
                                      })>;

std::vector<coord> theta_star(grid& g, coord start, coord end) {
  if (g.h * g.w != g.nodes.size())
    throw std::runtime_error("mismatched grid and grid::nodes size");

  min_queue<coord, float> frontier;
  std::vector<float> cost_sums(g.nodes.size(),
                               std::numeric_limits<float>::infinity());

  frontier.push({start, 0.f});
  cost_sums[g.as_idx(start)] = 0.f;

  while (frontier.size()) {
    coord curr = frontier.top().first;
    if (curr == end) break;
    frontier.pop();

    auto d = [](coord a, coord b) {
      return std::hypot(a.first - b.first, a.second - b.second);
    };
    auto h = [&](coord c) { return d(c, end); };
    auto cost_of = [&](coord from, coord to) {
      return cost_sums[g.as_idx(from)] + d(from, to) * g[to].cost;
    };

    for (auto& next : g.neighbours(curr)) {
      if (g[next].is_wall()) continue;

      float new_cost = cost_of(curr, next);
      const float i = g.as_idx(next);

      if (new_cost < cost_sums[i]) {
        if (g.visible(g[curr].parent, next)) {
          g[next].parent = g[curr].parent;
          new_cost = cost_of(g[next].parent, next);
        } else {
          g[next].parent = curr;
        }

        frontier.push({next, new_cost + h(next)});
        cost_sums[i] = new_cost;
      }
    }
  }

  std::vector<coord> path = {};
  const int max_depth = g.nodes.size();
  coord c = end;
  for (int i = 0; i < max_depth; ++i) {
    try {
      if (g.at(c).is_wall()) {
        std::string str = "wall found in trace at " + to_string(c);
        throw std::logic_error(str);
      }
    } catch (std::exception& e) { return {}; }  // no path found

    path.emplace_back(c);
    if (c == start) break;
    c = g[c].parent;
  }

  if (path.size() && path.back() != start) {
    throw std::length_error("max depth reached");
  }

  std::reverse(path.begin(), path.end());
  return path;
}

std::string to_string(const std::vector<coord>& path) {
  std::string res = "";
  for (auto& c : path) { res += to_string(c) + "\n"; }
  return res;
}

};  // namespace pf
