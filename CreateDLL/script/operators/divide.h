#pragma once
#ifndef DIVIDE_H_INCLUDED
#define DIVIDE_H_INCLUDED

#include "oper_t.h"

#include "../is_script_string.h"
#include "../destringify.h"

#include "../../core/stringUtils/eval_string.h"

namespace script
{
    namespace oper
    {
        template <typename CHAR>
        class divide : public oper_t<CHAR>
        {
        public:
            ///Addition uses '+' and has a priority of 3.
            divide() : oper_t<CHAR>(core::string<char>("/"), 3) {}


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

#endif // DIVIDE_H_INCLUDED
