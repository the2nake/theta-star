# theta-star

A visualization of the Theta\* algorithm, often used in robotics. It extends on A\* by allowing non-adjacent grid cells to be joined in a path if they have a clear line of sight, allowing for smoother outputs with fewer jagged edges.

<img alt="preview image of theta star path" src="https://github.com/the2nake/theta-star/blob/master/docs/demo.gif?raw=true"  height="600"/>

## [features]

- automatic maze generation
- cool path gradient ദ്ദി ( ᵔ ᗜ ᵔ )
- smooths created paths

## [building]

```bash
cmake . -B build
cmake --build build
```

## [usage]

Run the program with command `./theta_star-demo` or potentially `./theta_star-demo.exe` on Windows.

## [dependencies]

- SDL3
  - package may be listed with `-devel` suffix
- C23 compliant compiler
- CMake

## [credits]

- Whoever wrote the Wikipedia article
- [Burton Ma](https://www.youtube.com/watch?v=vj1IhhDRNR4)
