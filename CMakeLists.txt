cmake_minimum_required(VERSION 3.20)

project(ThetaStar LANGUAGES CXX)

find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)

file(GLOB SRC_FILES "src/*.cpp")
add_executable(thetastar-demo ${SRC_FILES})

target_link_libraries(thetastar-demo PRIVATE SDL3::SDL3)
set_target_properties(thetastar-demo PROPERTIES
  CXX_STANDARD 23
  CXX_STANDARD_REQUIRED FALSE
)
