#pragma once
#ifndef ADD_H_INCLUDED
#define ADD_H_INCLUDED

#include "oper_t.h"

namespace script
{
    namespace oper
    {
        template <typename CHAR>
        class add : public oper_t
        {
        public:
            add() : oper_t<CHAR>(core::string<char>("+"), ) {}

            error_flag operate(core::dynamic_stack< core::string<CHAR> >& _stack) const
            {
                return error::INVALID_OPERATION;
            }
        };
    }
}

#endif // ADD_H_INCLUDED
