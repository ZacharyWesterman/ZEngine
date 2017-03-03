/**
 * File:            var_struct.h
 * Namespace:       z::script::var
 *
 * Description:     A template struct for basic variable types.
 *                  Generic single variables can either be a
 *                  character string or a numeric value. We will
 *                  want to differentiate between the two to
 *                  allow for high precision with numbers.
 *                  By default, all var_t types are numerical.
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   3 Mar. 2017
**/

#pragma once
#ifndef VAR_STRUCT_H_INCLUDED
#define VAR_STRUCT_H_INCLUDED

#include <z/core/string.h>

namespace z
{
    namespace script
    {
        namespace var
        {
            template <typename CHAR>
            struct var_t
            {
                bool is_numerical = true;

                double value = 0.0;

                core::string<CHAR> string;
            };
        }
    }
}

#endif // VAR_STRUCT_H_INCLUDED
