#pragma once
#ifndef FACTORIAL_H_INCLUDED
#define FACTORIAL_H_INCLUDED

#include "oper_t.h"

#include "../is_script_string.h"
#include "../destringify.h"

#include "../../core/stringUtils/eval_string.h"

namespace math
{
    long int factorial(int n)
    {
        long int output = 1;

        for (long int i=2; i<=(long int)n; i++)
        {
            output *= i;
        }

        return output;
    }
}

namespace script
{
    namespace oper
    {
        template <typename CHAR>
        class factorial : public oper_t<CHAR>
        {
        public:
            ///Factorial uses '!' and has a priority of 7.
            factorial() : oper_t<CHAR>(core::string<char>("!"), 7) {}


            ///Perform multiplication
            error_flag operate(core::dynamic_stack< core::string<CHAR> >& operands) const
            {
                error_flag operation_error = error::NONE;

                core::string<CHAR> arg1;

                if (operands.pop(arg1))
                {
                    if (is_script_string(arg1))
                    {
                        operation_error |= error::INVALID_OPERATION;
                    }
                    else
                    {
                        double val1 = core::value(arg1);

                        double result = (double)(math::factorial((int)val1)); ///factorial

                        operands.push(result);
                    }
                }
                else
                {
                    operation_error |= error::MISSING_OPERAND;
                }

                return operation_error;
            }

        };
    }
}

#endif // FACTORIAL_H_INCLUDED
