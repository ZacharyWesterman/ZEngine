/**
 * File:            function_t.h
 * Namespace:       z::script
 *
 * Description:     The base class for all script functions.
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   27 Jul. 2017
**/

#pragma once
#ifndef FUNCTION_T_H_INCLUDED
#define FUNCTION_T_H_INCLUDED

#include <z/core/string.h>

namespace z
{
    namespace script
    {

        template <typename CHAR>
        class function_t
        {
        private:
            core::string<CHAR> func_name;

            int params_min;
            int params_max;

            bool is_constant;

        public:
            function_t(core::string<CHAR> _name,
                       int min_params = 0,
                       int max_params = 0,
                       bool constant = false)
            {
                func_name = _name;
                params_min = min_params;
                params_max = max_params;
                is_constant = constant;
            }

            ~function_t() {}



        };


    }
}

#endif // FUNCTION_T_H_INCLUDED
