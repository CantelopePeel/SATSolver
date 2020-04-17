//
// Created by Liam Heeger on 2020-04-16.
//

#ifndef SATSOLVERS_UTIL_H
#define SATSOLVERS_UTIL_H

#include "types/clause_list.h"

#include <iostream>
#include <fstream>


namespace sat_solver {
namespace util {

ClauseList read_dimacs(std::ifstream& input_stream);

}
}

#endif //SATSOLVERS_UTIL_H
