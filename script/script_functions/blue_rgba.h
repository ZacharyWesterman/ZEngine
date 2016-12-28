#pragma once
#ifndef BLUE_RGBA_H_INCLUDED
#define BLUE_RGBA_H_INCLUDED

#include "../../core/array.h"
#include "../../core/string.h"
#include "../../core/stringUtils/eval_string.h"
#include "../script_errors.h"


namespace script
{
    namespace FUNCTION
    {
        //blue function must take 1 parameter. Gets the blue value from a color.
        template <typename CHAR>
        error_flag blue_rgba(const core::array< core::string<CHAR> >& params, core::string<CHAR>& output)
        {
            error_flag func_error = ERROR::NONE;

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

                long blue = ((long)color & 0x0000FF00) >> 8;

                output = (double)blue;
            }

            return func_error;
        }
        //end of blue_rgba

    }
}

#endif // BLUE_RGBA_H_INCLUDED
