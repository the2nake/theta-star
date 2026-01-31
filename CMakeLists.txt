cmake_minimum_required(VERSION 3.20)

project(ThetaStar LANGUAGES CXX)
set(DEMOEXEC "theta_star-demo")

find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)

file(GLOB SRC_FILES "src/*.cpp")
add_executable(${DEMOEXEC} ${SRC_FILES})

target_link_libraries(${DEMOEXEC} PRIVATE SDL3::SDL3)
set_target_properties(${DEMOEXEC} PROPERTIES
  CXX_STANDARD 23
  CXX_STANDARD_REQUIRED FALSE
)
