#pragma once

#ifndef RAND_F_H_INCLUDED
#define RAND_F_H_INCLUDED


#include "../../core/array.h"
#include "../../core/string.h"
#include "../../core/stringUtils/eval_string.h"
#include "../script_errors.h"

///Important note: the random generator requires C++11 compiler.
/// Be sure to enable -std=c++11 or -std=gnu++11 compiler options.
///
/// you may need to add -U__STRICT_ANSI__ to compile options

#include <random>
#include <time.h>

#ifndef RANDOM_GENERATOR
    #define RANDOM_GENERATOR
    std::mt19937 generator(time(NULL));
#endif // RANDOM_GENERATOR


namespace script
{
    namespace FUNCTION
    {
        //rand_f function must take 2 parameters.
        //low range and high range, inclusive.
        template <typename CHAR>
        error_flag rand_f(const core::array< core::string<CHAR> >& params, core::string<CHAR>& output)
        {
            error_flag func_error = ERROR::NONE;

            if (params.size() > 2)
            {
                func_error |= ERROR::TOO_MANY_PARAMS;
            }
            else if (params.size() < 2)
            {
                func_error |= ERROR::TOO_FEW_PARAMS;
            }
            else
            {
                double low_range = core::value(params.at(0));
                double high_range = core::value(params.at(1));

                if (low_range > high_range)
                {
                    std::uniform_real_distribution<double> distribution(high_range, low_range);

                    output = (double)distribution(generator);
                }
                else if (low_range == high_range)
                {
                    output = low_range;
                }
                else
                {
                    std::uniform_real_distribution<double> distribution(low_range, high_range);

                    output = (double)distribution(generator);
                }
            }

            return func_error;
        }
        //end of rand_f

    }
}


#endif // RAND_F_H_INCLUDED
