//
// Created by Liam Heeger on 2020-04-16.
//

#ifndef SATSOLVER_LITERAL_H
#define SATSOLVER_LITERAL_H

#include <cstddef>
#include <functional>
#include "variable.h"

namespace sat_solver {

/**
 * Literal memory format (for member literal_value_):
 * Bit 0: sign value (0 if normal, 1 if negated)
 * Bits 2-31: variable value.
 */

class Literal {
    public:
        Literal(bool sign, const Variable& variable);

        inline bool sign() const { return static_cast<bool>(literal_value_ & 0x1); };
        inline const unsigned int value() const { return literal_value_; };
        inline const Variable to_variable() const { return Variable(literal_value_ >> 0x1); };

        Literal negate() const;

        inline bool operator==(const Literal& other) const { return literal_value_ == other.literal_value_; }

    private:
        unsigned int literal_value_;
};

}


namespace std
{
template<> struct hash<sat_solver::Literal>
{
    std::size_t operator()(sat_solver::Literal const& literal) const noexcept
    {
        return literal.value();
    }
};
}



#endif //SATSOLVER_LITERAL_H
