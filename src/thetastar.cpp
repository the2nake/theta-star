#include "thetastar.hpp"

#include <cmath>
#include <limits>
#include <queue>
#include <stdexcept>

namespace pf {

std::vector<coord> theta_star(grid &g, coord start, coord end) {
  if (g.h * g.w != g.nodes.size())
    throw std::runtime_error("mismatched grid and grid::nodes size");

  using queued = std::pair<coord, float>;
  auto cmp = [](const queued &a, const queued &b) {
    return a.second > b.second;
  };
  std::priority_queue<queued, std::vector<queued>, decltype(cmp)> q(cmp);
  q.push({start, 0.f});

  std::vector<float> cost_sums(g.nodes.size(),
                               std::numeric_limits<float>::infinity());
  cost_sums[g.as_idx(start)] = 0.f;

  while (q.size()) {
    coord curr = q.top().first;
    if (curr == end) break;
    q.pop();

    auto d = [](coord a, coord b) {
      return std::hypot(a.first - b.first, a.second - b.second);
    };
    auto h = [&](coord c) { return d(c, end); };
    auto cost_of = [&d, &g, &cost_sums](coord from, coord to) {
      return cost_sums[g.as_idx(from)] + d(from, to) * g[to].cost;
    };

    for (auto &next : g.neighbours(curr)) {
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

        if (std::isinf(cost_sums[i])) q.push({next, new_cost + h(next)});

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
    } catch (std::exception &e) {
      return path;
      {};
    }  // no path found

    path.insert(path.begin(), c);
    if (c == start) break;
    c = g[c].parent;
  }

  if (path.size() && path.front() != start) {
    throw std::length_error("max depth reached");
  }

  return path;
}

std::string to_string(const std::vector<coord> &path) {
  std::string res = "";
  for (auto &c : path) { res += to_string(c) + "\n"; }
  return res;
}

};  // namespace pf
