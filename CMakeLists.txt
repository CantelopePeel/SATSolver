cmake_minimum_required(VERSION 3.13)
project(SATSolver)

set(CMAKE_CXX_STANDARD 14)

add_library(types
        types/variable.h types/variable.cc
        types/literal.h types/literal.cc
        types/clause.h types/clause.cc
        types/clause_list.h types/clause_list.cc
        types/assignment.cc types/assignment.h)
target_include_directories(types PUBLIC ${CMAKE_SOURCE_DIR}/types/)

add_executable(sat_solvers main.cpp util.cc util.h solvers/backtracking_solver.cc solvers/backtracking_solver.h)
target_link_libraries(sat_solvers LINK_PUBLIC types)