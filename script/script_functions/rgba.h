#pragma once
#ifndef RGBA_H_INCLUDED
#define RGBA_H_INCLUDED

#include "../../core/array.h"
#include "../../core/string.h"
#include "../../core/stringUtils/eval_string.h"
#include "../script_errors.h"


namespace script
{
    namespace FUNCTION
    {
        //rgba function must take 4 parameters. converts red, green, blue, and alpha value to a color.
        template <typename CHAR>
        error_flag rgba(const core::array< core::string<CHAR> >& params, core::string<CHAR>& output)
        {
            error_flag func_error = error::NONE;

            if (params.size() > 4)
            {
                func_error |= error::TOO_MANY_PARAMS;
            }
            else if (params.size() < 4)
            {
                func_error |= error::TOO_FEW_PARAMS;
            }
            else
            {
                long red   = (long)core::value(params.at(0)) % 256;
                long green = (long)core::value(params.at(1)) % 256;
                long blue  = (long)core::value(params.at(2)) % 256;
                long alpha = (long)core::value(params.at(3)) % 256;

                if (red < 0.f)
                  red = 256 + red;

                if (green < 0.f)
                  green = 256 + green;

                if (blue < 0.f)
                  blue = 256 + blue;

                if (alpha < 0.f)
                  alpha = 256 + alpha;


                long color = (red << 24) + (green << 16) + (blue << 8) + alpha;

                output = (double)color;
            }

            return func_error;
        }
        //end of rgba

    }
}

#endif // RGBA_H_INCLUDED
