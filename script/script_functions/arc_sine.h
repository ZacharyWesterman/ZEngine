#pragma once
#ifndef ARC_SINE_H_INCLUDED
#define ARC_SINE_H_INCLUDED

#include "../../core/array.h"
#include "../../core/string.h"
#include "../../core/stringUtils/eval_string.h"
#include "../script_errors.h"

#include <math.h>


namespace script
{
    namespace FUNCTION
    {
        //arc sine function must take 1 parameter.
        //units are assumed to be in radians.
        template <typename CHAR>
        error_flag arc_sine(const core::array< core::string<CHAR> >& params, core::string<CHAR>& output)
        {
            error_flag func_error = error::NONE;

            if (params.size() > 1)
            {
                func_error |= error::TOO_MANY_PARAMS;
            }
            else if (params.size() < 1)
            {
                func_error |= error::TOO_FEW_PARAMS;
            }
            else
            {
                double angle = core::value(params.at(0));

                output = asin(angle);
            }

            return func_error;
        }
        //end of arc_sine

    }
}

#endif // ARC_SINE_H_INCLUDED
