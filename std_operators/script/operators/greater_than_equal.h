#pragma once
#ifndef GREATER_THAN_EQUAL_H_INCLUDED
#define GREATER_THAN_EQUAL_H_INCLUDED

#include "oper_t.h"

#include "../is_script_string.h"
#include "../destringify.h"

#include "../../core/stringUtils/eval_string.h"

namespace script
{
    namespace oper
    {
        template <typename CHAR>
        class greater_than_equal : public oper_t<CHAR>
        {
        public:
            ///GT equality than uses '>=' and has a priority of 2.
            greater_than_equal() : oper_t<CHAR>(core::string<char>(">="), 2, false) {}


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
                        bool arg1_is_string = is_script_string(arg1);
                        bool arg2_is_string = is_script_string(arg2);

                        if (arg1_is_string || arg2_is_string)
                        {
                            //we are simply checking string equality
                            //(even our "numbers" are strings.)

                            //so just remove any quotes from strings
                            if (arg1_is_string)
                                arg1 = destringify(arg1);

                            if (arg2_is_string)
                                arg2 = destringify(arg2);


                            double result = (double)(arg1 >= arg2); ///check string equality

                            operands.push(result);
                        }
                        else
                        {
                            double val1 = core::value(arg1);
                            double val2 = core::value(arg2);

                            double result = (val1 >= val2); ///check number equality

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

#endif // GREATER_THAN_EQUAL_H_INCLUDED
