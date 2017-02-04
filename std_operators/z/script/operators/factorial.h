/**
 * File:            factorial.h
 * Namespace:       z::script::oper
 * Description:     '!' script operator for factorials.
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   3 Feb. 2017
**/

#pragma once
#ifndef FACTORIAL_OPER_H_INCLUDED
#define FACTORIAL_OPER_H_INCLUDED

#include "oper_t.h"

#include "../is_script_string.h"
#include "../destringify.h"

#include "../../math/factorial.h"

namespace z
{
    namespace script
    {
        namespace oper
        {
            template <typename CHAR>
            class factorial : public oper_t<CHAR>
            {
            public:
                ///Factorial uses '!' and has a priority of 7, is unary and comes after the operand.
                factorial() : oper_t<CHAR>(core::string<char>("!"), 7, true, false) {}


                ///Perform operation
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

                            double result = (double)(math::fac((int)val1)); ///factorial

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
}

#endif // FACTORIAL_H_INCLUDED
