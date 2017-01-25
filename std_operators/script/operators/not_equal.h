#pragma once
#ifndef NOT_EQUAL_H_INCLUDED
#define NOT_EQUAL_H_INCLUDED

#include "oper_t.h"

#include "../is_script_string.h"
#include "../destringify.h"

#include "../../core/stringUtils/eval_string.h"

namespace script
{
    namespace oper
    {
        template <typename CHAR>
        class not_equal : public oper_t<CHAR>
        {
        public:
            ///Inequality than uses '<>' and has a priority of 2.
            not_equal() : oper_t<CHAR>(core::string<char>("<>"), 2) {}


            ///Perform multiplication
            error_flag operate(core::dynamic_stack< core::string<CHAR> >& operands) const
            {
                error_flag operation_error = error::NONE;

                core::string<CHAR> arg1;
                core::string<CHAR> arg2;

                if (operands.pop(arg2))
                {
                    if (operands.pop(arg1))
                    {
                        //we are simply checking string equality
                        //(even our "numbers" are strings.)

                        //so just remove any quotes from strings
                        if (is_script_string(arg1))
                            arg1 = destringify(arg1);

                        if (is_script_string(arg2))
                            arg2 = destringify(arg2);


                        double result = (double)(!(arg1 == arg2)); ///check equality

                        operands.push(result);

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

#endif // NOT_EQUAL_H_INCLUDED
