#pragma once
#ifndef AND_H_INCLUDED
#define AND_H_INCLUDED

#include "oper_t.h"

#include "../is_script_string.h"
#include "../destringify.h"

#include "../../core/stringUtils/eval_string.h"

namespace script
{
    namespace oper
    {
        template <typename CHAR>
        class and : public oper_t<CHAR>
        {
        public:
            ///And uses 'and' and has a priority of 2.
            and() : oper_t<CHAR>(core::string<char>("and"), 2) {}


            ///Perform multiplication
            error_flag operate(core::dynamic_stack< core::string<CHAR> >& operands) const
            {
                error_flag operation_error = error::NONE;



                return operation_error;
            }

        };
    }
}

#endif // AND_H_INCLUDED
