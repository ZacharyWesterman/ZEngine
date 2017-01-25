#pragma once
#ifndef GREATER_THAN_H_INCLUDED
#define GREATER_THAN_H_INCLUDED

#include "oper_t.h"

#include "../is_script_string.h"
#include "../destringify.h"

#include "../../core/stringUtils/eval_string.h"

namespace script
{
    namespace oper
    {
        template <typename CHAR>
        class greater_than : public oper_t<CHAR>
        {
        public:
            ///Greater than uses '>' and has a priority of 2.
            greater_than() : oper_t<CHAR>(core::string<char>(">"), 2) {}


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

#endif // GREATER_THAN_H_INCLUDED
