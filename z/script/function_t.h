/**
 * File:            function_t.h
 * Namespace:       z::script
 *
 * Description:     The base class for all script functions.
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   2 Aug. 2017
**/

#pragma once
#ifndef FUNCTION_T_H_INCLUDED
#define FUNCTION_T_H_INCLUDED

#include <z/core/string.h>
#include <z/core/array.h>
#include <z/core/timeout.h>

#include "data_t.h"


namespace z
{
    namespace script
    {

        template <typename CHAR>
        class function_t
        {
        private:
            void* graphics_engine;

            core::string<CHAR> func_name;

            int params_min;
            int params_max;

            bool is_constant;

        protected:
            core::array< data_t<CHAR> > params;
            data_t<CHAR> return_value;

        public:

            function_t(core::string<CHAR> _name,
                       int min_params = 0,
                       int max_params = 0,
                       bool _constant = false,
                       void* _engine = NULL)
            {
                func_name = _name;

                if (max_params < min_params)
                    min_params = max_params;

                params_min = min_params;
                params_max = max_params;
                is_constant = _constant;

                graphics_engine = _engine;
            }

            virtual ~function_t() {}


            virtual error_flag addParam(const data_t<CHAR>&) = 0;

            //call this script function (with timeout).
            //should return true if finished, false otherwise
            virtual bool call(const core::timeout&) = 0;


            void clear()
            {
                return_value = data_t<CHAR>();

                params.clear();
            }


            inline const core::string<CHAR>& name() const
            { return func_name; }

            inline const int maxParams() const
            { return params_max; }

            inline const int minParams() const
            { return params_min; }

            inline const bool constant() const
            { return is_constant; }

            inline const data_t<CHAR>& result() const
            { return return_value; }


            error_flag paramCountError() const
            {
                if (params_max < 0)
                    return error::NONE;

                if (params.size() > params_max)
                    return error::FUNC_TOO_MANY_PARAMS;
                else if (params.size() < params_min)
                    return error::FUNC_TOO_FEW_PARAMS;
                else
                    return error::NONE;
            }


            bool operator==(const function_t<CHAR>& other) const
            {
                return (params_max == other.params_max) &&
                       (params_min == other.params_min) &&
                       (func_name == other.func_name);
            }

            inline bool operator!=(const function_t<CHAR>& other) const
            { return !operator==(other); }

            bool operator>(const function_t<CHAR>& other) const
            {
                return (func_name > other.func_name) ||
                       (params_max > other.params_max) ||
                       (params_min > other.params_min);
            }

            inline bool operator<=(const function_t<CHAR>& other) const
            { return !operator>(other); }

            bool operator<(const function_t<CHAR>& other) const
            {
                return (func_name < other.func_name) ||
                       (params_max < other.params_max) ||
                       (params_min < other.params_min);
            }

            inline bool operator>=(const function_t<CHAR>& other) const
            { return !operator<(other); }
        };


    }
}

#endif // FUNCTION_T_H_INCLUDED
