#pragma once
#ifndef NAND_H_INCLUDED
#define NAND_H_INCLUDED

#include "oper_t.h"

#include "../is_script_string.h"
#include "../destringify.h"

#include "../../core/stringUtils/eval_string.h"

namespace script
{
    namespace oper
    {
        template <typename CHAR>
        class _nand : public oper_t<CHAR>
        {
        public:
            ///Nand uses 'nand' and has a priority of 1.
            _nand() : oper_t<CHAR>(core::string<char>("nand"), 1, false) {}


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

                            double result = (double)(!(val1 && val2)); ///not a and b

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

#endif // NAND_H_INCLUDED
