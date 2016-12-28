#ifndef DEGREES_TO_RADIANS_H_INCLUDED
#define DEGREES_TO_RADIANS_H_INCLUDED

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
        //this function converts a number from degrees to radians.
        //as such, units are assumed to be in degrees.
        template <typename CHAR>
        error_flag deg_to_rad(const core::array< core::string<CHAR> >& params, core::string<CHAR>& output)
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
                double angle = core::value(params.at(0));

                //radians = degrees*PI / 180
                output = (angle*PI/180);
            }

            return func_error;
        }
        //end of deg_to_rad

    }
}

#endif // DEGREES_TO_RADIANS_H_INCLUDED
