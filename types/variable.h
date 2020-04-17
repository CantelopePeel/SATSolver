//
// Created by Liam Heeger on 2020-04-16.
//

#ifndef SATSOLVER_VARIABLE_H
#define SATSOLVER_VARIABLE_H

#include <functional>

namespace sat_solver {

class Variable {
    public:
        explicit Variable(unsigned int variable_value);

        inline const unsigned int value() const { return variable_value_; };
        inline operator unsigned int() const { return variable_value_; };
    private:
        unsigned int variable_value_;

};

}

namespace std
{
template<> struct hash<sat_solver::Variable>
{
    std::size_t operator()(sat_solver::Variable const& variable) const noexcept
    {
        return variable.value();
    }
};
}

#endif //SATSOLVER_VARIABLE_H
