#pragma once
#ifndef NOT_H_INCLUDED
#define NOT_H_INCLUDED

#include "oper_t.h"

#include "../is_script_string.h"
#include "../destringify.h"

#include "../../core/stringUtils/eval_string.h"

namespace script
{
    namespace oper
    {
        template <typename CHAR>
        class _not : public oper_t<CHAR>
        {
        public:
            ///Not uses 'not' and has a priority of 5.
            _not() : oper_t<CHAR>(core::string<char>("not"), 5, true) {}


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

                        double result = (double)(!val1); ///not a

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

#endif // NOT_H_INCLUDED
