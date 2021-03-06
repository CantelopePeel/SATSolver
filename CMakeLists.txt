cmake_minimum_required(VERSION 3.13)
project(SATSolver)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -mavx -msse")

add_library(types
        types/variable.h types/variable.cc
        types/literal.h types/literal.cc
        types/clause.h types/clause.cc
        types/clause_list.h types/clause_list.cc
        types/assignment.h types/assignment.cc)
include_directories(types PUBLIC ${CMAKE_SOURCE_DIR}/types/)

add_library(solvers
        solvers/solver.h solvers/solver.cc
        solvers/backtracking_solver.h solvers/backtracking_solver.cc
        solvers/cdcl_solver.h solvers/cdcl_solver.cc
        solvers/dpll_solver.cc solvers/dpll_solver.h
        solvers/schoning_solver.cc solvers/schoning_solver.h)
include_directories(solvers PUBLIC ${CMAKE_SOURCE_DIR}/solvers/)
target_link_libraries(solvers LINK_PUBLIC types)

add_executable(sat_solvers main.cpp util.h util.cc)

target_link_libraries(sat_solvers LINK_PUBLIC types solvers)
