/**
 * File:            var_struct.h
 * Namespace:       z::script::var
 *
 * Description:     A template struct for basic variable data.
 *                  Generic single variables can be a character
 *                  string or a numeric value, or null. We will
 *                  want to differentiate between strings and
 *                  numbers to allow for high precision.
 *                  We also want to know when a variable hasn't been set.
 *
 *                  By default, all var_t types are set to VAR_NULL.
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
            enum VAR_TYPE
            {
                VAR_NULL = 0,

                VAR_NUMERICAL,
                VAR_STRING
            };


            template <typename CHAR>
            struct var_t
            {
                VAR_TYPE type = VAR_NULL;

                double value = 0.0;

                core::string<CHAR> string;
            };
        }
    }
}

#endif // VAR_STRUCT_H_INCLUDED
