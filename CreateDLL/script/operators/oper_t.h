#pragma once
#ifndef OPER_T_H_INCLUDED
#define OPER_T_H_INCLUDED

#include <stdint.h>

#include "../../core/string.h"

#include "../script_errors.h"
#include "../../core/dynamicStack.h"

namespace script
{
    namespace oper
    {
        typedef uint8_t oper_priority;

        template <typename CHAR>
        class oper_t
        {
        private:
            core::string<CHAR> _string;
            oper_priority oper_pri;

        public:
            template <typename CHAR_T>
            oper_t(core::string<CHAR_T> oper_string, oper_priority _priority)
            {
                core::convertStr(_string, oper_string);

                oper_pri = _priority;
            }



            virtual error_flag operate(core::dynamic_stack< core::string<CHAR> >& _stack) const
            {
                return error::INVALID_OPERATION;
            }

            const core::string<CHAR>& str() const
            {
                return _string;
            }

            oper_priority priority() const
            {
                return oper_pri;
            }


            ///These functions are for sorting operators
            bool operator==(const oper_t& other) const
            {
                return (_string == other._string);
            }

            bool operator>(const oper_t& other) const
            {
                return (_string > other._string);
            }

            bool operator<(const oper_t& other) const
            {
                return (_string < other._string);
            }

        };
    }
}

#endif // OPER_T_H_INCLUDED
