// TODO: figure out unit testing
// handle exceptions:
// * reaching max depth (circular trace)
// * mismatched nodes size
// * coordinate outside of grid
// * wall found in trace

// write the blog post from a perspective of 1st year CS student who has not
// heard of pathfinding before.
#include <SDL3/SDL.h>
#include <SDL3/SDL_blendmode.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_video.h>

#include <chrono>
#include <cmath>
#include <cstdlib>
#include <print>
#include <random>
#include <stack>

#include "pathfind.hpp"
#include "thetastar.hpp"

const int cell_w = 8;    // 16;
const int cell_gap = 0;  // 2;

extern std::string maze;
extern std::string block;

SDL_Color lerp(SDL_Color a, SDL_Color b, float t) {
  return SDL_Color{
      static_cast<Uint8>(std::lerp(a.r, b.r, t)),
      static_cast<Uint8>(std::lerp(a.g, b.g, t)),
      static_cast<Uint8>(std::lerp(a.b, b.b, t)),
      static_cast<Uint8>(std::lerp(a.a, b.a, t)),
  };
}

void set_draw_color(SDL_Renderer* renderer, SDL_Color c) {
  SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
}

void show(const pf::grid& g, const std::vector<pf::coord>& result) {
  std::print("{}", pf::to_string(g, result));
}

template <class F>
auto elapsed(F f) {
  using namespace std::chrono;
  auto t0 = high_resolution_clock::now();
  auto result = f();
  auto t1 = high_resolution_clock::now();
  return std::pair{result, duration_cast<microseconds>(t1 - t0).count()};
}

void generate_maze(pf::grid& g, pf::coord s, pf::coord e) {
  using namespace pf;

  std::random_device dev;
  std::mt19937 eng(dev());
  std::uniform_int_distribution<> dist(0, 3);

  for (int i = 0; i < g.nodes.size(); ++i) { g.nodes[i].cost = INFINITY; }
  pf::coord dirs[] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

  std::stack<coord> prev;
  std::vector<bool> done(g.nodes.size(), false);
  auto curr = s;

  do {
    int valid = 0;
    bool possible[] = {false, false, false, false};
    for (int i = 0; i < 4; ++i) {
      try {
        auto nc = curr + 2 * dirs[i];
        if (g.at(nc).is_wall() && !done[g.as_idx(nc)]) {
          possible[i] = true;
          ++valid;
        }
      } catch (std::exception& e) {};
    }

    if (valid) {
      int picked = dist(dev);
      while (!possible[picked]) { picked = dist(dev); }

      prev.push(curr);
      curr = curr + dirs[picked];
      g.at(curr).cost = 1.0f;
      curr = curr + dirs[picked];
      g.at(curr).cost = 1.0f;
    } else {
      done[g.as_idx(curr)] = true;
      curr = prev.top();
      prev.pop();
    }
  } while (!prev.empty());
}

int main() {
  // pf::grid g(10, 10);
  // g.load(maze);
  // pf::coord start = {0, 2};
  // pf::coord end = {9, 5};

  int gw = 101;
  int gh = 101;

  pf::grid g(gh, gw);
  pf::coord start = {1, 1};
  pf::coord end = {gh - 2, gw - 2};
  // std::println("{}", bigmaze.size());
  generate_maze(g, start, end);

  g.at(start).cost = 1.0;
  g.at(end).cost = 1.0;

  auto [path, us] = elapsed([&]() { return pf::theta_star(g, start, end); });
  std::println("path:\n{}", pf::to_string(path));
  std::println("runtime: {} us", us);

  // visualisation

  char* sdl_wayland_hint = "SDL_VIDEO_DRIVER=wayland";
  putenv(sdl_wayland_hint);
  SDL_Init(SDL_INIT_VIDEO);

  int w = cell_w * g.w, h = cell_w * g.h;
  unsigned long flags = SDL_WINDOW_HIGH_PIXEL_DENSITY;

  SDL_Window* win = SDL_CreateWindow("theta-star-demo", w, h, flags);
  SDL_Renderer* ren = SDL_CreateRenderer(win, nullptr);
  SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);

  int path_range = path.size();
  int ms_since_update = 0;
  int ms_per_frame = 2000.0 / path.size();
  int ms_at_stop = 1000;

  bool running = true;
  while (running) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_EVENT_QUIT:
          running = false;
          break;
        default:
          break;
      }
    }

    if (ms_since_update > ms_per_frame) {
      ms_since_update = 0;
      ++path_range;
      if (path_range == path.size()) { ms_since_update = -ms_at_stop; }
      if (path_range > path.size()) { path_range = 1; }
    }

    auto SDL_RenderCoords = SDL_RenderCoordinatesFromWindow;
    auto SDL_SetColor = SDL_SetRenderDrawColor;

    SDL_SetColor(ren, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(ren);

    auto get_rect = [&](int i, int j, int t = cell_gap) -> SDL_FRect {
      SDL_FRect rect;
      SDL_RenderCoords(ren, cell_w * j + t, cell_w * i + t, &rect.x, &rect.y);
      SDL_RenderCoords(ren, cell_w - t, cell_w - t, &rect.w, &rect.h);
      return rect;
    };

    for (int i = 0; i < g.h; ++i) {
      for (int j = 0; j < g.w; ++j) {
        SDL_FRect rect = get_rect(i, j);
        if (g[{i, j}].is_wall()) {
          SDL_SetColor(ren, 0, 0, 0, SDL_ALPHA_OPAQUE);
        } else {
          SDL_SetColor(ren, 255, 255, 255, SDL_ALPHA_OPAQUE);
        }
        SDL_RenderFillRect(ren, &rect);
      }
    }

    SDL_Color green{0, 255, 0, SDL_ALPHA_OPAQUE};
    SDL_Color red{255, 0, 0, SDL_ALPHA_OPAQUE};

    for (int i = 0; i < path_range; ++i) {
      SDL_FRect rect = get_rect(path[i].first, path[i].second);
      set_draw_color(ren,
                     lerp(green, red, static_cast<float>(i) / path.size()));
      SDL_RenderFillRect(ren, &rect);
    }

    SDL_RenderPresent(ren);

    SDL_Delay(5);
    ms_since_update += 5;
  }

  SDL_Quit();
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
