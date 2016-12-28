#ifndef RADIANS_TO_DEGREES_H_INCLUDED
#define RADIANS_TO_DEGREES_H_INCLUDED

#include "../../core/array.h"
#include "../../core/string.h"
#include "../../core/stringUtils/eval_string.h"
#include "../script_errors.h"

#ifndef PI
    #define PI 3.141592653589 //pi to the 12th decimal place should be accurate enough
#endif // PI


namespace script
{
    namespace FUNCTION
    {
        //rad_to_deg function must take 1 parameter.
        //this function converts a number from radians to degrees.
        //as such, units are assumed to be in radians.
        template <typename CHAR>
        error_flag rad_to_deg(const core::array< core::string<CHAR> >& params, core::string<CHAR>& output)
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
                double angle = core::value(params.at(0));

                //degrees = radians*180 / PI
                output = (angle*180/PI);
            }

            return func_error;
        }
        //end of rad_to_deg

    }
}

#endif // RADIANS_TO_DEGREES_H_INCLUDED
