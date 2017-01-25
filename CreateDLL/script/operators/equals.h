#pragma once
#ifndef EQUALS_H_INCLUDED
#define EQUALS_H_INCLUDED

#include "oper_t.h"

#include "../is_script_string.h"
#include "../destringify.h"

#include "../../core/stringUtils/eval_string.h"

namespace script
{
    namespace oper
    {
        template <typename CHAR>
        class factorial : public oper_t<CHAR>
        {
        public:
            ///Factorial uses '!' and has a priority of 7.
            factorial() : oper_t<CHAR>(core::string<char>("^"), 6) {}


            ///Perform multiplication
            error_flag operate(core::dynamic_stack< core::string<CHAR> >& operands) const
            {
                error_flag operation_error = error::NONE;

                /** Stuff goes here **/

                return operation_error;
            }

        };
    }
}

#endif // EQUALS_H_INCLUDED
