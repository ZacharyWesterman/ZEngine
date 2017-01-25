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
            ///Addition uses '+' and has a priority of 3.
            modulus() : oper_t<CHAR>(core::string<char>("/"), 4) {}


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

#endif // MODULUS_H_INCLUDED
