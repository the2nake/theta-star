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

#include <cmath>
#include <print>

#include "pathfind.hpp"
#include "thetastar.hpp"

const int cell_w = 16;

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

void set_draw_color(SDL_Renderer *renderer, SDL_Color c) {
  SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
}

void show(const pf::grid &g, const std::vector<pf::coord> &result) {
  std::print("{}", pf::to_string(g, result));
}

int main() {
  pf::grid g(10, 10);
  g.load(maze);
  pf::coord start = {0, 2};
  pf::coord end = {9, 5};

  auto path = pf::theta_star(g, start, end);
  std::println("path:\n{}", pf::to_string(path));

  SDL_Init(SDL_INIT_VIDEO);

  int w = cell_w * g.w, h = cell_w * g.h;
  unsigned long flags = SDL_WINDOW_HIGH_PIXEL_DENSITY;

  SDL_Window *window = SDL_CreateWindow("theta-star-demo", w, h, flags);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, nullptr);
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

  int path_range = path.size();
  int ms_since_update = 0;
  int ms_per_frame = 600;

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

    if (ms_per_frame < ms_since_update) {
      ms_since_update = 0;
      path_range = (path_range + 1) % (path.size() + 1);
    }

    auto SDL_RenderCoords = SDL_RenderCoordinatesFromWindow;
    auto SDL_SetColor = SDL_SetRenderDrawColor;

    SDL_SetColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    auto get_rect = [&](int i, int j, int t = 2) -> SDL_FRect {
      SDL_FRect rect;
      SDL_RenderCoordinatesFromWindow(renderer, cell_w * j + t, cell_w * i + t,
                                      &rect.x, &rect.y);
      SDL_RenderCoordinatesFromWindow(renderer, cell_w - t, cell_w - t, &rect.w,
                                      &rect.h);
      return rect;
    };

    for (int i = 0; i < g.h; ++i) {
      for (int j = 0; j < g.w; ++j) {
        SDL_FRect rect = get_rect(i, j);
        if (g[{i, j}].is_wall()) {
          SDL_SetColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        } else {
          SDL_SetColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        }
        SDL_RenderFillRect(renderer, &rect);
      }
    }

    SDL_Color green{0, 255, 0, SDL_ALPHA_OPAQUE};
    SDL_Color red{255, 0, 0, SDL_ALPHA_OPAQUE};

    for (int i = 0; i < path_range; ++i) {
      SDL_FRect rect = get_rect(path[i].first, path[i].second);
      set_draw_color(renderer,
                     lerp(green, red, static_cast<float>(i) / path.size()));
      SDL_RenderFillRect(renderer, &rect);
    }

    SDL_RenderPresent(renderer);

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
