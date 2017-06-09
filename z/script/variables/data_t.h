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
 * Last modified:   9 Jun. 2017
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

                VALUE,
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

            std::complex<double> d_value;

            core::string<CHAR> d_string;

            core::array<data_t> d_array;


        public:
            data_t() {d_type = data::NONE;}

            ~data_t() {}

            const bool operator==(const data_t& other) const;

            inline const bool operator!=(const data_t& other) const
            {
                return !operator==(other);
            }


            const data_t& operator=(const data_t& other);

            inline int type() const {return type;}

        };


        template <typename CHAR>
        const bool data_t<CHAR>::operator==(const data_t<CHAR>& other) const
        {
            if (d_type != other.d_type)
                return false;

            switch (d_type)
            {
            case (data::VALUE):
                return d_value == other.d_value;
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


        template <typename CHAR>
        const data_t<CHAR>& data_t<CHAR>::operator=(const data_t<CHAR>& other)
        {
            d_string.clear();
            d_array.clear();

            d_type = other.d_type;

            switch (d_type)
            {
            case (data::VALUE):
                d_value = other.d_value;
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


    }
}

#endif // DATA_T_H_INCLUDED
