#pragma once
#ifndef ABSOLUTE_VAL_H_INCLUDED
#define ABSOLUTE_VAL_H_INCLUDED

#include "../../core/array.h"
#include "../../core/string.h"
#include "../../core/stringUtils/eval_string.h"
#include "../script_errors.h"

#include <math.h>

#ifndef uint
    #define uint uint32_t
#endif // uint

namespace script
{
    namespace FUNCTION
    {
        //absolute value function must take 1 parameter.
        template <typename CHAR>
        uint absolute_val(const core::array< core::string<CHAR> >& params, core::string<CHAR>& output)
        {
            uint func_error = ERROR::NONE;

            if (params.size() > 1)
            {
                func_error |= ERROR::TOO_MANY_PARAMS;
            }
            else if (params.size() < 1)
            {
                func_error |= ERROR::TOO_FEW_PARAMS;
            }
            else
            {
                double number = core::value(params.at(0));

                if (number < 0.f)
                    number = -number;

                output = number;
            }

            return func_error;
        }
        //end of absolute_val

    }
}

#endif // ABSOLUTE_VAL_H_INCLUDED
