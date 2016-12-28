#pragma once
#ifndef RED_RGBA_H_INCLUDED
#define RED_RGBA_H_INCLUDED

#include "../../core/array.h"
#include "../../core/string.h"
#include "../../core/stringUtils/eval_string.h"
#include "../script_errors.h"


namespace script
{
    namespace FUNCTION
    {
        //red function must take 1 parameter. Gets the red value from a color.
        template <typename CHAR>
        error_flag red_rgba(const core::array< core::string<CHAR> >& params, core::string<CHAR>& output)
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
                double color = core::value(params.at(0));

                long red = ((long)color & 0xFF000000) >> 24;

                output = (double)red;
            }

            return func_error;
        }
        //end of red_rgba

    }
}

#endif // RED_RGBA_H_INCLUDED
