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
 * Last modified:   11 Jun. 2017
**/

#pragma once
#ifndef DATA_T_H_INCLUDED
#define DATA_T_H_INCLUDED

#include <z/core/string.h>
#include <z/core/array.h>

#include <complex>
#include <type_traits>

using std::complex;

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
            data_t()
            {
                d_string = core::string<CHAR>();
                d_array = core::array< data_t<CHAR> >();

                d_type = data::NONE;
            }

            template<
                typename T, //numeric type
                typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type
            >
            data_t(const T& _real)
            {
                d_type = data::VALUE;
                d_value = (double)_real;
            }

            template<
                typename T, //numeric type
                typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type
            >
            data_t(const std::complex<T>& _complex)
            {
                d_type = data::VALUE;
                d_value = std::complex<double>(_complex);
            }

            data_t(const core::string<CHAR>&);
            data_t(const core::array< data_t<CHAR> >&);

            template <typename CHAR_2>
            data_t(const CHAR_2* cstring)
            {
                d_type = data::STRING;
                d_string = cstring;
            }


            ~data_t() {}

            const bool operator==(const data_t&) const;

            inline const bool operator!=(const data_t& other) const
            {
                return !operator==(other);
            }


            const data_t& operator=(const data_t&);


            inline int type() const {return type;}

            inline void setType(const data::DATA_TYPE new_type)
            { d_type = new_type; }

            const core::string<CHAR> string() const;
            const std::complex<double> complex() const;
            const double real() const;
            const double imag() const;

            inline core::array< data_t<CHAR> >& array()
            { return d_array; }
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
            d_type = other.d_type;

            d_value = other.d_value;
            d_string = other.d_string;

            d_array = other.d_array;


            return *this;
        }



        template <typename CHAR>
        data_t<CHAR>::data_t(const core::string<CHAR>& _string)
        {
            d_type = data::STRING;
            d_string = _string;
        }

        template <typename CHAR>
        data_t<CHAR>::data_t(const core::array< data_t<CHAR> >& _array)
        {
            d_type = data::ARRAY;
            d_array = _array;
        }



        template <typename CHAR>
        const core::string<CHAR> data_t<CHAR>::string() const
        {
            if (d_type == data::VALUE)
                return core::string<CHAR>(d_value);
            else if (d_type == data::STRING)
                return d_string;
            else if (d_type == data::ARRAY)
            {
                core::string<CHAR> r_string = "{";

                for (int i=0; i<d_array.size(); i++)
                {
                    if (i > 0)
                        r_string += ",";

                    r_string += d_array[i].string();
                }

                r_string += "}";

                return r_string;
            }
            else
                return core::string<CHAR>();

        }


        template <typename CHAR>
        const std::complex<double> data_t<CHAR>::complex() const
        {
            if (d_type == data::VALUE)
                return d_value;
            else if (d_type == data::STRING)
                return core::complexValue(d_string);
            else
                return std::complex<double>();
        }


        template <typename CHAR>
        const double data_t<CHAR>::real() const
        {
            if (d_type == data::VALUE)
                return d_value.real();
            else if (d_type == data::STRING)
                return core::complexValue(d_string).real();
            else
                return 0;
        }

        template <typename CHAR>
        const double data_t<CHAR>::imag() const
        {
            if (d_type == data::VALUE)
                return d_value.imag();
            else if (d_type == data::STRING)
                return core::complexValue(d_string).imag();
            else
                return 0;
        }
    }
}

#endif // DATA_T_H_INCLUDED
