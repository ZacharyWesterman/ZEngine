#pragma once
#ifndef TANGENT_H_INCLUDED
#define TANGENT_H_INCLUDED

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
        //tangent function must take 1 parameter.
        //units are assumed to be in radians.
        template <typename CHAR>
        uint tangent(const core::array< core::string<CHAR> >& params, core::string<CHAR>& output)
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

                output = tan(angle);
            }

            return func_error;
        }
        //end of tangent

    }
}

#endif // TANGENT_H_INCLUDED
