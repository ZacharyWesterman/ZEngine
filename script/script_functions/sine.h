#pragma once
#ifndef SINE_H_INCLUDED
#define SINE_H_INCLUDED

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
        //sine function must take 1 parameter.
        //units are assumed to be in radians.
        template <typename CHAR>
        uint sine(const core::array< core::string<CHAR> >& params, core::string<CHAR>& output)
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
                double angle = core::value(params.at(0));

                output = sin(angle);
            }

            return func_error;
        }
        //end of sine

    }
}

#endif // SINE_H_INCLUDED
