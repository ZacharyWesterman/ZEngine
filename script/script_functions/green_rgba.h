#pragma once
#ifndef GREEN_RGBA_H_INCLUDED
#define GREEN_RGBA_H_INCLUDED

#include "../../core/array.h"
#include "../../core/string.h"
#include "../../core/stringUtils/eval_string.h"
#include "../script_errors.h"


namespace script
{
    namespace FUNCTION
    {
        //green function must take 1 parameter. Gets the green value from a color.
        template <typename CHAR>
        error_flag green_rgba(const core::array< core::string<CHAR> >& params, core::string<CHAR>& output)
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

                long green = ((long)color & 0x00FF0000) >> 16;

                output = (double)green;
            }

            return func_error;
        }
        //end of green_rgba

    }
}

#endif // GREEN_RGBA_H_INCLUDED
