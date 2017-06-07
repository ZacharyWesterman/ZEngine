/**
 * File:            data_t.h
 * Namespace:       z::script
 *
 * Description:     A template union for a generic datatype.
 *                  Generic data can be a string, a
 *                  numeric value, a complex value, an array,
 *                  or NONE. We will want to differentiate between
 *                  types to allow for high precision.
 *                  We also want to know when a variable hasn't been set.
 *
 *                  By default, all data_t types are set to NONE.
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   7 Jun. 2017
**/

#pragma once
#ifndef DATA_T_H_INCLUDED
#define DATA_T_H_INCLUDED

#include <z/core/string.h>
#include <z/core/array.h>

#include <complex>
#include <type_traits>

namespace z
{
    namespace script
    {
        namespace data
        {
            enum DATA_TYPE
            {
                NONE = 0,

                REAL,
                COMPLEX,
                STRING,
                ARRAY,

                TYPE_COUNT
            };
        }


        template <typename CHAR>
        class data_t
        {
        public:
            int d_type;

            double d_real;

            std::complex<double> d_complex;

            core::string<CHAR> d_string;

            core::array<data_t> d_array;


        public:
            data_t() {d_type = data::NONE;}

            ~data_t() {}

            const bool operator==(const data_t& other) const
            {
                if (d_type != other.d_type)
                    return false;

                switch (d_type)
                {
                case (data::REAL):
                    return d_real == other.d_real;
                    break;

                case (data::COMPLEX):
                    return d_complex == other.d_complex;
                    break;

                case (data::STRING):
                    return d_string == other.d_string;
                    break;

                case (data::ARRAY):
                    return d_array == other.d_array;
                    break;

                default:
                    return true;
                }
            }

            inline const bool operator!=(const data_t& other) const
            {
                return !operator==(other);
            }


            const data_t& operator=(const data_t& other)
            {
                d_string.clear();
                d_array.clear();

                d_type = other.d_type;

                switch (d_type)
                {
                case (data::REAL):
                    d_real = other.d_real;
                    break;

                case (data::COMPLEX):
                    d_complex = other.d_complex;
                    break;

                case (data::STRING):
                    d_string = other.d_string;
                    break;

                case (data::ARRAY):
                    d_array = other.d_array;
                    break;
                }

                return *this;
            }



        };
    }
}

#endif // DATA_T_H_INCLUDED
