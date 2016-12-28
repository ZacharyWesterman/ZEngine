#pragma once
#ifndef RGB_H_INCLUDED
#define RGB_H_INCLUDED

#include "../../core/array.h"
#include "../../core/string.h"
#include "../../core/stringUtils/eval_string.h"
#include "../script_errors.h"


namespace script
{
    namespace FUNCTION
    {
        //rgb function must take 3 parameters. converts red, green, and blue value to an opaque color.
        template <typename CHAR>
        error_flag rgb(const core::array< core::string<CHAR> >& params, core::string<CHAR>& output)
        {
            error_flag func_error = ERROR::NONE;

            if (params.size() > 3)
            {
                func_error |= ERROR::TOO_MANY_PARAMS;
            }
            else if (params.size() < 3)
            {
                func_error |= ERROR::TOO_FEW_PARAMS;
            }
            else
            {
                long red   = (long)core::value(params.at(0)) % 256;
                long green = (long)core::value(params.at(1)) % 256;
                long blue  = (long)core::value(params.at(2)) % 256;
                long alpha = (long)255;

                if (red < 0.f)
                  red = 256 + red;

                if (green < 0.f)
                  green = 256 + green;

                if (blue < 0.f)
                  blue = 256 + blue;


                long color = (red << 24) + (green << 16) + (blue << 8) + alpha;

                output = (double)color;
            }

            return func_error;
        }
        //end of rgb

    }
}

#endif // RGB_H_INCLUDED
