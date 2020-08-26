cmake_minimum_required(VERSION 3.1)
project(lib-time-ind)
add_library(lib-time-ind STATIC
  ./src/graph.cpp
  ./src/agent.cpp
  ./src/greedy.cpp
  ./src/causal_pibt.cpp
  ./src/causal_pibt_mapf.cpp
  ./src/fsp.cpp
  ./src/mcp.cpp
  ./src/problem.cpp
  ./src/mapf_random.cpp
  ./src/mapf_dp.cpp
  ./src/stn.cpp
  )
add_definitions(-D_MAPDIR_="${CMAKE_CURRENT_LIST_DIR}/../map/")
target_compile_options(lib-time-ind PUBLIC -O3 -Wall -mtune=native -march=native)
target_compile_features(lib-time-ind PUBLIC cxx_std_17)
target_include_directories(lib-time-ind INTERFACE ./include)
