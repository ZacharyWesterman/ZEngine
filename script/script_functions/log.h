#pragma once
#ifndef LOG_H_INCLUDED
#define LOG_H_INCLUDED

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
        //log function must take 1 or 2 parameters.
        //if 1 parameter, log base 10 is assumed.
        //if 2 parameters, 1st is the base.
        template <typename CHAR>
        uint log(const core::array< core::string<CHAR> >& params, core::string<CHAR>& output)
        {
            uint func_error = ERROR::NONE;

            if (params.size() > 2)
            {
                func_error |= ERROR::TOO_MANY_PARAMS;
            }
            else if (params.size() < 1)
            {
                func_error |= ERROR::TOO_FEW_PARAMS;
            }
            else
            {
                double base = 10.f;
                double number = 1.f;

                if (params.size() == 2)
                {
                    base = core::value(params.at(0));
                    number = core::value(params.at(1));
                }
                else
                {
                    number = core::value(params.at(0));
                }


                if ((number > 0) && (base > 0))
                {
                    output = (log10(number) / log10(base));
                }
                else
                {
                    func_error |= ERROR::NON_REAL_NUMBER;
                }
            }

            return func_error;
        }
        //end of log

    }
}

#endif // LOG_H_INCLUDED
