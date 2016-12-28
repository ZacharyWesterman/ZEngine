#pragma once
#ifndef CEILING_H_INCLUDED
#define CEILING_H_INCLUDED

#include "../../core/array.h"
#include "../../core/string.h"
#include "../../core/stringUtils/eval_string.h"
#include "../script_errors.h"


namespace script
{
    namespace FUNCTION
    {
        //floor function must take 1 parameter.
        template <typename CHAR>
        error_flag ceiling(const core::array< core::string<CHAR> >& params, core::string<CHAR>& output)
        {
            error_flag func_error = error::NONE;

            if (params.size() >    1)
            {
                func_error |= error::TOO_MANY_PARAMS;
            }
            else if (params.size() < 1)
            {
                func_error |= error::TOO_FEW_PARAMS;
            }
            else
            {
                double number = core::value(params.at(0));
                long intpart = (long int)number;

                double fracpart = number - (double)intpart;

                if (fracpart > 0.f)
                {
                    output = (double)(intpart + 1);
                }
                else
                {
                    output = (double)(intpart);
                }
            }

            return func_error;
        }
        //end of ceiling

    }
}

#endif // CEILING_H_INCLUDED
