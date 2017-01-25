#pragma once
#ifndef LESS_THAN_EQUAL_H_INCLUDED
#define LESS_THAN_EQUAL_H_INCLUDED

#include "oper_t.h"

#include "../is_script_string.h"
#include "../destringify.h"

#include "../../core/stringUtils/eval_string.h"

namespace script
{
    namespace oper
    {
        template <typename CHAR>
        class less_than_equal : public oper_t<CHAR>
        {
        public:
            ///LT equality uses '<=' and has a priority of 2.
            less_than_equal() : oper_t<CHAR>(core::string<char>("<="), 2) {}


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

#endif // LESS_THAN_EQUAL_H_INCLUDED
