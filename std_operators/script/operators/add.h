#pragma once
#ifndef ADD_H_INCLUDED
#define ADD_H_INCLUDED

#include "oper_t.h"

#include "../is_script_string.h"
#include "../destringify.h"

#include "../../core/stringUtils/eval_string.h"

namespace script
{
    namespace oper
    {
        template <typename CHAR>
        class add : public oper_t<CHAR>
        {
        public:
            ///Addition uses '+' and has a priority of 3.
            add() : oper_t<CHAR>(core::string<char>("+"), 3, false) {}


            ///Perform addition
            error_flag operate(core::dynamic_stack< core::string<CHAR> >& operands) const
            {
                error_flag operation_error = error::NONE;

                core::string<CHAR> arg1;
                core::string<CHAR> arg2;

                if (operands.pop(arg2))
                {
                    if (operands.pop(arg1))
                    {
                        bool arg1_is_string = is_script_string(arg1);
                        bool arg2_is_string = is_script_string(arg2);

                        //concatenate when one or both of the operands are strings
                        if (arg1_is_string || arg2_is_string)
                        {
                            core::string<CHAR> result = (CHAR)34;

                            if (arg1_is_string)
                                arg1 = destringify(arg1);

                            if (arg2_is_string)
                                arg2 = destringify(arg2);

                            result += arg1 + arg2;
                            result += (CHAR)34;

                            operands.push(result);
                        }
                        //only add values when we have two numbers
                        else
                        {
                            double val1 = core::value(arg1);
                            double val2 = core::value(arg2);

                            double result = (val1 + val2); ///add

                            operands.push(result);
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

#endif // ADD_H_INCLUDED
