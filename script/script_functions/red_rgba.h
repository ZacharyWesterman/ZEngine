#pragma once
#ifndef RED_RGBA_H_INCLUDED
#define RED_RGBA_H_INCLUDED

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
        //red function must take 1 parameter. Gets the red value from a color.
        template <typename CHAR>
        uint red_rgba(const core::array< core::string<CHAR> >& params, core::string<CHAR>& output)
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