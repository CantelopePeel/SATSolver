//
// Created by Liam Heeger on 2020-04-16.
//

#include "literal.h"

using namespace sat_solver;

Literal::
Literal(bool sign, const Variable& variable) {
    unsigned int sign_bit = sign ? 0x1 : 0x0;
    literal_value_ = (variable << 0x1) | sign_bit;
}

Literal Literal::negate() const {
    return Literal(!sign(), to_variable());
}
