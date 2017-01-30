#pragma once
#ifndef MODULUS_H_INCLUDED
#define MODULUS_H_INCLUDED

#include "oper_t.h"

#include "../is_script_string.h"
#include "../destringify.h"

#include "../../core/stringUtils/eval_string.h"

namespace script
{
    namespace oper
    {
        template <typename CHAR>
        class modulus : public oper_t<CHAR>
        {
        public:
            ///Modulus uses '%' and has a priority of 4.
            modulus() : oper_t<CHAR>(core::string<char>("%"), 4, false) {}


            ///Perform operation
            error_flag operate(core::dynamic_stack< core::string<CHAR> >& operands) const
            {
                error_flag operation_error = error::NONE;

                core::string<CHAR> arg1;
                core::string<CHAR> arg2;

                if (operands.pop(arg2))
                {
                    if (operands.pop(arg1))
                    {
                        if (is_script_string(arg1) ||
                            is_script_string(arg2))
                        {
                            operation_error |= error::INVALID_OPERATION;
                        }
                        else
                        {
                            double val1 = core::value(arg1);
                            double val2 = core::value(arg2);

                            if (val2 == 0.f)
                            {
                                operation_error |= error::DIV_BY_ZERO;
                                operands.push(0.f);
                            }
                            else
                            {
                                double result = (double)((long int)val1 % (long int)val2); ///modulus

                                operands.push(result);
                            }
                        }
                    }
                    else
                    {
                        operation_error |= error::MISSING_OPERAND;
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

#endif // MODULUS_H_INCLUDED
