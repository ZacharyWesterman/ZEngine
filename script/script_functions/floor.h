#pragma once
#ifndef FLOOR_H_INCLUDED
#define FLOOR_H_INCLUDED

#include "../../core/array.h"
#include "../../core/string.h"
#include "../../core/stringUtils/eval_string.h"
#include "../script_errors.h"

#ifndef uint
    #define uint uint32_t
#endif // uint

namespace script
{
    namespace FUNCTION
    {
        //floor function must take 1 parameter.
        template <typename CHAR>
        uint floor(const core::array< core::string<CHAR> >& params, core::string<CHAR>& output)
        {
            uint func_error = ERROR::NONE;

            if (params.size() >    1)
            {
                func_error |= ERROR::TOO_MANY_PARAMS;
            }
            else if (params.size() < 1)
            {
                func_error |= ERROR::TOO_FEW_PARAMS;
            }
            else
            {
                long int number = (long int)core::value(params.at(0));

                output = (double)number;
            }

            return func_error;
        }
        //end of floor

    }
}

#endif // FLOOR_H_INCLUDED
