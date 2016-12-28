#pragma once
#ifndef SQUARE_ROOT_H_INCLUDED
#define SQUARE_ROOT_H_INCLUDED

#include "../../core/array.h"
#include "../../core/string.h"
#include "../../core/stringUtils/eval_string.h"
#include "../script_errors.h"

#include <math.h>

namespace script
{
    namespace FUNCTION
    {
        //square root function must take 1 parameter.
        template <typename CHAR>
        error_flag square_root(const core::array< core::string<CHAR> >& params, core::string<CHAR>& output)
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
                double number = core::value(params.at(0));

                if (number >= 0.f)
                {
                    output = sqrt(number);
                }
                else
                {
                    func_error = ERROR::NON_REAL_NUMBER;
                }

            }

            return func_error;
        }
        //end of square_root

    }
}

#endif // SQUARE_ROOT_H_INCLUDED
