cmake_minimum_required(VERSION 3.1)
project(time-independent-planning CXX)
add_subdirectory(./time_independent)
add_executable(app app.cpp)
target_compile_features(app PUBLIC cxx_std_17)
target_link_libraries(app lib-time-ind)
add_definitions(-D_PLANDIR_="${CMAKE_CURRENT_LIST_DIR}/mapf_plan/")
