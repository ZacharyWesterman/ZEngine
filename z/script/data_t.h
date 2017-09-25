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
 * Last modified:   24 Aug. 2017
**/

#pragma once
#ifndef DATA_T_H_INCLUDED
#define DATA_T_H_INCLUDED

#include <z/float.h>
#include <z/core/string.h>
#include <z/core/array.h>
#include <z/math/remainder.h>

#include "errors.h"

#include <complex>
#include <type_traits>


namespace z
{
    namespace script
    {
        //some equality operators for complex numbers
        template <typename T>
        const bool operator>(const std::complex<T>& arg1, const std::complex<T>& arg2)
        {
            T norm1 = arg1.real()*arg1.real() + arg1.imag()*arg1.imag();
            T norm2 = arg2.real()*arg2.real() + arg2.imag()*arg2.imag();

            return norm1 > norm2;
        }

        template <typename T>
        const bool operator<(const std::complex<T>& arg1, const std::complex<T>& arg2)
        {
            T norm1 = arg1.real()*arg1.real() + arg1.imag()*arg1.imag();
            T norm2 = arg2.real()*arg2.real() + arg2.imag()*arg2.imag();

            return norm1 < norm2;
        }

        template <typename T>
        inline const bool operator>=(const std::complex<T>& arg1, const std::complex<T>& arg2)
        { return !operator<(arg1, arg2); }

        template <typename T>
        inline const bool operator<=(const std::complex<T>& arg1, const std::complex<T>& arg2)
        { return !operator>(arg1, arg2); }




        enum data
        {
            NONE = 0,

            ERROR,

            VALUE,
            STRING,
            ARRAY,

            TYPE_COUNT
        };


        template <typename CHAR>
        class data_t
        {
        public:
            int d_type;
            errorFlag d_error;

            std::complex<Float> d_value;

            core::string<CHAR> d_string;

            core::array<data_t> d_array;


        public:
            data_t()
            {
                d_type = data::NONE;

                d_value = 0;
                d_error = 0;
            }

            data_t(errorFlag new_error)
            {

                d_type = data::ERROR;

                d_error = new_error;
            }

            template<
                typename T, //numeric type
                typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type
            >
            data_t(const T& _real)
            {
                d_type = data::VALUE;
                d_value = (Float)_real;
            }

            template<
                typename T, //numeric type
                typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type
            >
            data_t(const std::complex<T>& _complex)
            {
                d_type = data::VALUE;
                d_value = _complex;
            }

            data_t(const core::string<CHAR>&);
            data_t(const core::array< data_t<CHAR> >&);

            template <typename CHAR_2>
            data_t(const CHAR_2* cstring)
            {
                d_type = data::STRING;
                d_string = core::string<CHAR>(cstring);
            }


            ~data_t() {}

            void clear()
            {
                d_type = data::NONE;
                d_error = error::NONE;

                d_string.clear();
                d_array.clear();
            }

            const bool operator==(const data_t&) const;

            inline const bool operator!=(const data_t& other) const
            { return !operator==(other); }

            const bool operator>(const data_t&) const;
            const bool operator<(const data_t&) const;

            inline const bool operator>=(const data_t& other) const
            { return !operator<(other); }

            inline const bool operator<=(const data_t& other) const
            { return !operator>(other); }


            const data_t& operator=(const data_t&);


            inline const int type() const {return d_type;}

            inline void setType(const int new_type)
            { d_type = new_type; }

            const core::string<CHAR> string() const;
            const std::complex<Float> complex() const;
            const Float real() const;
            const Float imag() const;

            inline core::array< data_t<CHAR> >& array()
            { return d_array; }

            inline const core::array< data_t<CHAR> >& array() const
            { return d_array; }

            errorFlag error() const
            {
                if (d_type == data::ERROR)
                    return d_error;
                else
                    return error::NONE;
            }


            const data_t index(const data_t&) const;
            const data_t index(const data_t&,
                               const data_t&) const;

            const data_t subIndex(const data_t&) const;
            const data_t subIndex(const data_t&,
                                  const data_t&) const;

            const data_t& merge(const core::array< data_t >&);

            //operators
            const data_t& operator++();
            const data_t& operator--();
            const data_t operator++(int);
            const data_t operator--(int);

            const data_t operator-() const;
            const data_t operator+(const data_t&) const;
            const data_t operator-(const data_t&) const;

            const data_t operator*(const data_t&) const;
            const data_t operator/(const data_t&) const;
            const data_t operator%(const data_t&) const;
            const data_t int_divide(const data_t&) const;

            const data_t operator^(const data_t&) const;
            const data_t factorial() const;

            const data_t operator&(const data_t&) const;
            const data_t operator&&(const data_t&) const;
            const data_t operator|(const data_t&) const;
            const data_t operator||(const data_t&) const;
            const data_t operator!() const;
            const data_t operator~() const;
            const data_t xor_bitwise(const data_t&) const;
            const data_t xor_logical(const data_t&) const;
            const data_t nand_bitwise(const data_t&) const;
            const data_t nand_logical(const data_t&) const;
            const data_t nor_bitwise(const data_t&) const;
            const data_t nor_logical(const data_t&) const;
            const data_t nxor_bitwise(const data_t&) const;
            const data_t nxor_logical(const data_t&) const;
        };


        template <typename CHAR>
        const data_t<CHAR> data_t<CHAR>::index(const data_t<CHAR>& _index) const
        {
            data_t<CHAR> result;

            if ((_index.d_type != data::VALUE) ||
                (_index.d_value.imag() != (Float)0)) //bad index
            {
                result = error::ILLEGAL_INDEX;
            }
            else if (d_type <= data::VALUE)
            {
                result = error::CANNOT_INDEX;
            }
            else if (d_type == data::ARRAY)
            {
                int i_index = (int)_index.d_value.real();

                if (d_array.is_valid(i_index))
                    result = d_array.at(i_index);
                else
                {
                    result = error::INDEX_OUT_OF_BOUNDS;
                }
            }
            else //STRING
            {
                int i_index = (int)_index.d_value.real();

                result = core::string<CHAR>(d_string.at(i_index));
            }

            return result;
        }


        template <typename CHAR>
        const data_t<CHAR> data_t<CHAR>::index(const data_t<CHAR>& start,
                                               const data_t<CHAR>& stop) const
        {
            data_t<CHAR> result;

            if ((start.d_type != data::VALUE) ||
                (stop.d_type != data::VALUE) ||
                (start.d_value.imag() != (Float)0) ||
                (stop.d_value.imag() != (Float)0)) //bad index
            {
                result = error::ILLEGAL_INDEX;
            }
            else if (d_type <= data::VALUE)
            {
                result = error::CANNOT_INDEX;
            }
            else if (d_type == data::ARRAY)
            {
                int i_start = (int)start.d_value.real();
                int i_stop = (int)stop.d_value.real();

                if (d_array.is_valid(i_start) &&
                    d_array.is_valid(i_stop))
                    result = d_array.subset(i_start, i_stop);
                else
                {
                    result = error::INDEX_OUT_OF_BOUNDS;
                }
            }
            else //STRING
            {
                int i_start = (int)start.d_value.real();
                int i_stop = (int)stop.d_value.real();

                result = d_string.substr(i_start, i_stop);
            }

            return result;
        }


        template <typename CHAR>
        const data_t<CHAR> data_t<CHAR>::subIndex(const data_t<CHAR>& _index) const
        {
            data_t<CHAR> result;

            if ((_index.d_type != data::VALUE) ||
                (_index.d_value.imag() != (Float)0)) //bad index
            {
                result = error::ILLEGAL_INDEX;
            }
            else if (d_type != data::ARRAY)
            {
                result.d_type = error::CANNOT_INDEX;
            }
            else
            {
                result.d_type = data::ARRAY;

                int i_index = (int)_index.d_value.real();

                for (int i=0; i<d_array.size(); i++)
                {
                    if (d_array[i].d_type <= data::VALUE)
                    {
                        result = error::CANNOT_INDEX;
                        return result;
                    }
                    else if (d_array[i].d_type == data::ARRAY)
                    {
                        if (d_array[i].d_array.is_valid(i_index))
                        {
                            result.d_array.add(d_array[i].d_array[i_index]);
                        }
                        else
                        {
                            result = error::INDEX_OUT_OF_BOUNDS;
                            return result;
                        }
                    }
                    else //STRING
                    {
                        result.d_array.add(core::string<char>(
                                           d_array[i].d_string.at(i_index)));
                    }
                }
            }

            return result;
        }


        template <typename CHAR>
        const data_t<CHAR> data_t<CHAR>::subIndex(const data_t<CHAR>& start,
                                                  const data_t<CHAR>& stop) const
        {
            data_t<CHAR> result;

            if ((start.d_type != data::VALUE) ||
                (stop.d_type != data::VALUE) ||
                (start.d_value.imag() != (Float)0) ||
                (stop.d_value.imag() != (Float)0)) //bad index
            {
                result = error::ILLEGAL_INDEX;
            }
            else if (d_type != data::ARRAY)
            {
                result.d_type = error::CANNOT_INDEX;
            }
            else
            {
                result.d_type = data::ARRAY;

                int i_start = (int)start.d_value.real();
                int i_stop = (int)stop.d_value.real();

                for (int i=0; i<d_array.size(); i++)
                {
                    if (d_array[i].d_type <= data::VALUE)
                    {
                        result = error::CANNOT_INDEX;
                        return result;
                    }
                    else if (d_array[i].d_type == data::ARRAY)
                    {
                        if (d_array[i].d_array.is_valid(i_start) &&
                            d_array[i].d_array.is_valid(i_stop))
                        {

                            result.d_array.add(
                                data_t<CHAR>(d_array[i].
                                d_array.subset(i_start, i_stop)));
                        }
                        else
                        {
                            result = error::INDEX_OUT_OF_BOUNDS;
                            return result;
                        }
                    }
                    else //STRING
                    {
                        result.d_array.add(d_array[i].
                                           d_string.substr(i_start, i_stop));
                    }
                }
            }

            return result;
        }


        ///Assuming a 2D array list, change array grouping
        ///from horizontal to vertical.
        /**

        {a,b,c}     {a,d,g}
        {d,e,f} --> {b,e,h}
        {g,h,i}     {c,f,i}

        */
        template <typename CHAR>
        const data_t<CHAR>& data_t<CHAR>::merge(const core::array< data_t<CHAR> >& arr)
        {
            d_string.clear();
            d_array.clear();
            d_type = data::ARRAY;


            int max_width = 1;

            //find max array width
            for (int i=0; i<arr.size(); i++)
            {
                if((arr[i].d_type == data::ARRAY) &&
                   (arr[i].d_array.size() > max_width))
                {
                    max_width = arr[i].d_array.size();
                }
            }


            //fill new arrays in order
            for (int i=0; i<max_width; i++)
            {
                core::array< data_t<CHAR> > current;

                for (int j=0; j<arr.size(); j++)
                {
                    int width;
                    if (arr[j].d_type == data::ARRAY)
                        width = arr[j].d_array.size();
                    else
                        width = 1;


                    if (width > i)
                    {
                        if (arr[j].d_type == data::ARRAY)
                            current.add(arr[j].d_array[i]);
                        else
                            current.add(arr[j]);
                    }
                }

                d_array.add(data_t<CHAR>(current));
            }


            return *this;
        }


        template <typename CHAR>
        const data_t<CHAR>& data_t<CHAR>::operator++()
        {
            if (d_type == data::VALUE)
            {
                d_value = d_value + (Float)1.0;
            }
            else if (d_type == data::ARRAY)
            {
                d_type = data::ERROR;
                d_error = error::INVALID_OPER_ARRAY;
            }
            else if (d_type == data::STRING)
            {
                d_type = data::ERROR;
                d_error = error::INVALID_OPER_STRING;
            }

            return *this;
        }

        template <typename CHAR>
        const data_t<CHAR>& data_t<CHAR>::operator--()
        {
            if (d_type == data::VALUE)
            {
                d_value = d_value - (Float)1.0;
            }
            else if (d_type == data::ARRAY)
            {
                d_type = data::ERROR;
                d_error = error::INVALID_OPER_ARRAY;
            }
            else if (d_type == data::STRING)
            {
                d_type = data::ERROR;
                d_error = error::INVALID_OPER_STRING;
            }

            return *this;
        }


        template <typename CHAR>
        const data_t<CHAR> data_t<CHAR>::operator++(int)
        {
            data_t<CHAR> result;

            if (d_type == data::VALUE)
            {
                result = *this;
                d_value = d_value + (Float)1.0;
            }
            else if (d_type == data::ARRAY)
            {
                result.d_type = data::ERROR;
                result.d_error = error::INVALID_OPER_ARRAY;
            }
            else if (d_type == data::STRING)
            {
                result.d_type = data::ERROR;
                result.d_error = error::INVALID_OPER_STRING;
            }

            return result;
        }

        template <typename CHAR>
        const data_t<CHAR> data_t<CHAR>::operator--(int)
        {
            data_t<CHAR> result;

            if (d_type == data::VALUE)
            {
                result = *this;
                d_value = d_value - (Float)1.0;
            }
            else if (d_type == data::ARRAY)
            {
                result.d_type = data::ERROR;
                result.d_error = error::INVALID_OPER_ARRAY;
            }
            else if (d_type == data::STRING)
            {
                result.d_type = data::ERROR;
                result.d_error = error::INVALID_OPER_STRING;
            }

            return result;
        }


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
                return false;
            }
        }

        template <typename CHAR>
        const bool data_t<CHAR>::operator>(const data_t<CHAR>& other) const
        {
            if (d_type != other.d_type)
                return (d_type > other.d_type);

            switch (d_type)
            {
            case (data::VALUE):
                return d_value > other.d_value;
                break;

            case (data::STRING):
                return d_string > other.d_string;
                break;

            case (data::ARRAY):
                return d_array > other.d_array;
                break;

            default:
                return false;
            }
        }

        template <typename CHAR>
        const bool data_t<CHAR>::operator<(const data_t<CHAR>& other) const
        {
            if (d_type != other.d_type)
                return (d_type < other.d_type);

            switch (d_type)
            {
            case (data::VALUE):
                return d_value < other.d_value;
                break;

            case (data::STRING):
                return d_string < other.d_string;
                break;

            case (data::ARRAY):
                return d_array < other.d_array;
                break;

            default:
                return false;
            }
        }


        template <typename CHAR>
        const data_t<CHAR>& data_t<CHAR>::operator=(const data_t<CHAR>& other)
        {
            d_type = other.d_type;
            d_error = other.d_error;

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
            else if (d_type == data::ERROR)
                return core::string<CHAR>("[ERR_")+
                        core::string<CHAR>(d_error)+
                        core::string<CHAR>("]");
            else
                return core::string<CHAR>();

        }


        template <typename CHAR>
        const std::complex<Float> data_t<CHAR>::complex() const
        {
            if (d_type == data::VALUE)
                return d_value;
            else if (d_type == data::STRING)
                return d_string.complexValue();
            else
                return std::complex<Float>();
        }


        template <typename CHAR>
        const Float data_t<CHAR>::real() const
        {
            if (d_type == data::VALUE)
                return d_value.real();
            else if (d_type == data::STRING)
                return d_string.complexValue().real();
            else
                return 0;
        }

        template <typename CHAR>
        const Float data_t<CHAR>::imag() const
        {
            if (d_type == data::VALUE)
                return d_value.imag();
            else if (d_type == data::STRING)
                return d_string.complexValue().imag();
            else
                return 0;
        }




        ///Operators

        //negation
        template <typename CHAR>
        const data_t<CHAR> data_t<CHAR>::operator-() const
        {
            data_t<CHAR> result;

            if (d_type == data::VALUE)
                result = -d_value;
            else if (d_type == data::STRING)
            {
                result.d_type = data::ERROR;
                result.d_error = error::INVALID_OPER_STRING;
            }
            else if (d_type == data::ARRAY)
            {
                result.d_type = data::ERROR;
                result.d_error = error::INVALID_OPER_ARRAY;
            }

            return result;
        }

        //addition
        template <typename CHAR>
        const data_t<CHAR> data_t<CHAR>::operator+(const data_t<CHAR>& other) const
        {
            data_t<CHAR> result;

            if ((d_type == data::ARRAY) || (other.d_type == data::ARRAY))
            {
                result.d_type = data::ERROR;
                result.d_error = error::INVALID_OPER_ARRAY;
            }
            else if ((d_type == data::STRING) || (other.d_type == data::STRING))
            {
                if (d_type == data::STRING)
                    result = d_string;
                else
                    result = this->string();

                if (other.d_type == data::STRING)
                    result.d_string += other.d_string;
                else
                    result.d_string += other.string();
            }
            else if ((d_type == data::VALUE) && (other.d_type == data::VALUE))
            {
                result = d_value + other.d_value;
            }

            return result;
        }

        //subtraction
        template <typename CHAR>
        const data_t<CHAR> data_t<CHAR>::operator-(const data_t<CHAR>& other) const
        {
            data_t<CHAR> result;

            if ((d_type == data::ARRAY) || (other.d_type == data::ARRAY))
            {
                result.d_type = data::ERROR;
                result.d_error = error::INVALID_OPER_ARRAY;
            }
            else if ((d_type == data::STRING) || (other.d_type == data::STRING))
            {
                result.d_type = data::ERROR;
                result.d_error = error::INVALID_OPER_STRING;
            }
            else if ((d_type == data::VALUE) && (other.d_type == data::VALUE))
            {
                result = d_value - other.d_value;
            }

            return result;
        }

        //multiplication
        template <typename CHAR>
        const data_t<CHAR> data_t<CHAR>::operator*(const data_t<CHAR>& other) const
        {
            data_t<CHAR> result;

            if ((d_type == data::ARRAY) || (other.d_type == data::ARRAY))
            {
                result.d_type = data::ERROR;
                result.d_error = error::INVALID_OPER_ARRAY;
            }
            else if ((d_type == data::STRING) || (other.d_type == data::STRING))
            {
                result.d_type = data::ERROR;
                result.d_error = error::INVALID_OPER_STRING;
            }
            else if ((d_type == data::VALUE) && (other.d_type == data::VALUE))
            {
                result = d_value * other.d_value;
            }

            return result;
        }

        //floating-point division
        template <typename CHAR>
        const data_t<CHAR> data_t<CHAR>::operator/(const data_t<CHAR>& other) const
        {
            data_t<CHAR> result;

            if ((d_type == data::ARRAY) || (other.d_type == data::ARRAY))
            {
                result.d_type = data::ERROR;
                result.d_error = error::INVALID_OPER_ARRAY;
            }
            else if ((d_type == data::STRING) || (other.d_type == data::STRING))
            {
                result.d_type = data::ERROR;
                result.d_error = error::INVALID_OPER_STRING;
            }
            else if ((d_type == data::VALUE) && (other.d_type == data::VALUE))
            {
                if (other.d_value.real() || other.d_value.imag())
                    result = d_value / other.d_value;
                else
                {
                    result.d_type = data::ERROR;
                    result.d_error = error::DIV_BY_ZERO;
                }

            }

            return result;
        }

        //remainder of integer division
        template <typename CHAR>
        const data_t<CHAR> data_t<CHAR>::operator%(const data_t<CHAR>& other) const
        {
            data_t<CHAR> result;

            if ((d_type == data::ARRAY) || (other.d_type == data::ARRAY))
            {
                result.d_type = data::ERROR;
                result.d_error = error::INVALID_OPER_ARRAY;
            }
            else if ((d_type == data::STRING) || (other.d_type == data::STRING))
            {
                result.d_type = data::ERROR;
                result.d_error = error::INVALID_OPER_STRING;
            }
            else if ((d_type == data::VALUE) && (other.d_type == data::VALUE))
            {
                if (other.d_value.real() || other.d_value.imag())
                {
                    result = math::remainder(d_value, other.d_value);
                }
                else
                {
                    result.d_type = data::ERROR;
                    result.d_error = error::DIV_BY_ZERO;
                }

            }

            return result;
        }

        //integer division
        template <typename CHAR>
        const data_t<CHAR> data_t<CHAR>::int_divide(const data_t<CHAR>& other) const
        {
            data_t<CHAR> result;

            if ((d_type == data::ARRAY) || (other.d_type == data::ARRAY))
            {
                result.d_type = data::ERROR;
                result.d_error = error::INVALID_OPER_ARRAY;
            }
            else if ((d_type == data::STRING) || (other.d_type == data::STRING))
            {
                result.d_type = data::ERROR;
                result.d_error = error::INVALID_OPER_STRING;
            }
            else if ((d_type == data::VALUE) && (other.d_type == data::VALUE))
            {
                if (other.d_value.real() || other.d_value.imag())
                {
                    result = std::complex<long>(d_value) /
                             std::complex<long>(other.d_value);
                }
                else
                {
                    result.d_type = data::ERROR;
                    result.d_error = error::DIV_BY_ZERO;
                }

            }

            return result;
        }

        //power
        template <typename CHAR>
        const data_t<CHAR> data_t<CHAR>::operator^(const data_t<CHAR>& other) const
        {
            data_t<CHAR> result;

            if ((d_type == data::ARRAY) || (other.d_type == data::ARRAY))
            {
                result.d_type = data::ERROR;
                result.d_error = error::INVALID_OPER_ARRAY;
            }
            else if ((d_type == data::STRING) || (other.d_type == data::STRING))
            {
                result.d_type = data::ERROR;
                result.d_error = error::INVALID_OPER_STRING;
            }
            else if ((d_type == data::VALUE) && (other.d_type == data::VALUE))
            {
                result = pow(d_value,other.d_value);

            }

            return result;
        }

        //factorial
        template <typename CHAR>
        const data_t<CHAR> data_t<CHAR>::factorial() const
        {
            data_t<CHAR> result;

            if (d_type == data::ARRAY)
            {
                result.d_type = data::ERROR;
                result.d_error = error::INVALID_OPER_ARRAY;
            }
            else if (d_type == data::STRING)
            {
                result.d_type = data::ERROR;
                result.d_error = error::INVALID_OPER_STRING;
            }
            else if (d_type == data::VALUE)
            {
                if (d_value.imag() != 0)
                {
                    result.d_type = data::ERROR;
                    result.d_error = error::FACTORIAL_COMPLEX;
                }
                else if (d_value.real() < 0)
                {
                    result.d_type = data::ERROR;
                    result.d_error = error::FACTORIAL_NEGATIVE;
                }
                else
                {
                    Float _rslt = math::factorial((long)d_value.real());

                    if (_rslt == 0)
                    {
                        result.d_type = data::ERROR;
                        result.d_error = error::FACTORIAL_OVERFLOW;
                    }
                    else
                        result = _rslt;
                }
            }

            return result;
        }

        //logical AND
        template <typename CHAR>
        const data_t<CHAR> data_t<CHAR>::operator&&(const data_t<CHAR>& other) const
        {
            data_t<CHAR> result;

            if ((d_type == data::VALUE) && (other.d_type == data::VALUE))
            {
                result = (long)d_value.real() && (long)other.d_value.real();
            }
            else if ((d_type == data::ARRAY) || (other.d_type == data::ARRAY))
            {
                result.d_type = data::ERROR;
                result.d_error = error::INVALID_OPER_ARRAY;
            }
            else if ((d_type == data::STRING) || (other.d_type == data::STRING))
            {
                result.d_type = data::ERROR;
                result.d_error = error::INVALID_OPER_STRING;
            }

            return result;
        }


        //bitwise AND
        template <typename CHAR>
        const data_t<CHAR> data_t<CHAR>::operator&(const data_t<CHAR>& other) const
        {
            data_t<CHAR> result;

            if ((d_type == data::VALUE) && (other.d_type == data::VALUE))
            {
                result = (long)d_value.real() & (long)other.d_value.real();
            }
            else if ((d_type == data::ARRAY) || (other.d_type == data::ARRAY))
            {
                result.d_type = data::ERROR;
                result.d_error = error::INVALID_OPER_ARRAY;
            }
            else if ((d_type == data::STRING) || (other.d_type == data::STRING))
            {
                result.d_type = data::ERROR;
                result.d_error = error::INVALID_OPER_STRING;
            }

            return result;
        }


        //logical OR
        template <typename CHAR>
        const data_t<CHAR> data_t<CHAR>::operator||(const data_t<CHAR>& other) const
        {
            data_t<CHAR> result;

            if ((d_type == data::VALUE) && (other.d_type == data::VALUE))
            {
                result = (long)d_value.real() || (long)other.d_value.real();
            }
            else if ((d_type == data::ARRAY) || (other.d_type == data::ARRAY))
            {
                result.d_type = data::ERROR;
                result.d_error = error::INVALID_OPER_ARRAY;
            }
            else if ((d_type == data::STRING) || (other.d_type == data::STRING))
            {
                result.d_type = data::ERROR;
                result.d_error = error::INVALID_OPER_STRING;
            }

            return result;
        }


        //bitwise OR
        template <typename CHAR>
        const data_t<CHAR> data_t<CHAR>::operator|(const data_t<CHAR>& other) const
        {
            data_t<CHAR> result;

            if ((d_type == data::VALUE) && (other.d_type == data::VALUE))
            {
                result = (long)d_value.real() | (long)other.d_value.real();
            }
            else if ((d_type == data::ARRAY) || (other.d_type == data::ARRAY))
            {
                result.d_type = data::ERROR;
                result.d_error = error::INVALID_OPER_ARRAY;
            }
            else if ((d_type == data::STRING) || (other.d_type == data::STRING))
            {
                result.d_type = data::ERROR;
                result.d_error = error::INVALID_OPER_STRING;
            }

            return result;
        }


        //logical NOT
        template <typename CHAR>
        const data_t<CHAR> data_t<CHAR>::operator!() const
        {
            data_t<CHAR> result;

            if (d_type == data::VALUE)
                result = !(long)d_value.real();
            else if (d_type == data::STRING)
            {
                result.d_type = data::ERROR;
                result.d_error = error::INVALID_OPER_STRING;
            }
            else if (d_type == data::ARRAY)
            {
                result.d_type = data::ERROR;
                result.d_error = error::INVALID_OPER_ARRAY;
            }

            return result;
        }


        //bitwise NOT
        template <typename CHAR>
        const data_t<CHAR> data_t<CHAR>::operator~() const
        {
            data_t<CHAR> result;

            if (d_type == data::VALUE)
                result = ~(long)d_value.real();
            else if (d_type == data::STRING)
            {
                result.d_type = data::ERROR;
                result.d_error = error::INVALID_OPER_STRING;
            }
            else if (d_type == data::ARRAY)
            {
                result.d_type = data::ERROR;
                result.d_error = error::INVALID_OPER_ARRAY;
            }

            return result;
        }

        //logical XOR
        template <typename CHAR>
        const data_t<CHAR> data_t<CHAR>::xor_logical(const data_t<CHAR>& other) const
        {
            data_t<CHAR> result;

            if ((d_type == data::VALUE) && (other.d_type == data::VALUE))
            {
                result = !(long)d_value.real() != !(long)other.d_value.real();
            }
            else if ((d_type == data::ARRAY) || (other.d_type == data::ARRAY))
            {
                result.d_type = data::ERROR;
                result.d_error = error::INVALID_OPER_ARRAY;
            }
            else if ((d_type == data::STRING) || (other.d_type == data::STRING))
            {
                result.d_type = data::ERROR;
                result.d_error = error::INVALID_OPER_STRING;
            }

            return result;
        }


        //bitwise XOR
        template <typename CHAR>
        const data_t<CHAR> data_t<CHAR>::xor_bitwise(const data_t<CHAR>& other) const
        {
            data_t<CHAR> result;

            if ((d_type == data::VALUE) && (other.d_type == data::VALUE))
            {
                result = (long)d_value.real() ^ (long)other.d_value.real();
            }
            else if ((d_type == data::ARRAY) || (other.d_type == data::ARRAY))
            {
                result.d_type = data::ERROR;
                result.d_error = error::INVALID_OPER_ARRAY;
            }
            else if ((d_type == data::STRING) || (other.d_type == data::STRING))
            {
                result.d_type = data::ERROR;
                result.d_error = error::INVALID_OPER_STRING;
            }

            return result;
        }


        //logical NAND
        template <typename CHAR>
        const data_t<CHAR> data_t<CHAR>::nand_logical(const data_t<CHAR>& other) const
        {
            data_t<CHAR> result;

            if ((d_type == data::VALUE) && (other.d_type == data::VALUE))
            {
                result = !((long)d_value.real() && (long)other.d_value.real());
            }
            else if ((d_type == data::ARRAY) || (other.d_type == data::ARRAY))
            {
                result.d_type = data::ERROR;
                result.d_error = error::INVALID_OPER_ARRAY;
            }
            else if ((d_type == data::STRING) || (other.d_type == data::STRING))
            {
                result.d_type = data::ERROR;
                result.d_error = error::INVALID_OPER_STRING;
            }

            return result;
        }


        //bitwise NAND
        template <typename CHAR>
        const data_t<CHAR> data_t<CHAR>::nand_bitwise(const data_t<CHAR>& other) const
        {
            data_t<CHAR> result;

            if ((d_type == data::VALUE) && (other.d_type == data::VALUE))
            {
                result = ~((long)d_value.real() & (long)other.d_value.real());
            }
            else if ((d_type == data::ARRAY) || (other.d_type == data::ARRAY))
            {
                result.d_type = data::ERROR;
                result.d_error = error::INVALID_OPER_ARRAY;
            }
            else if ((d_type == data::STRING) || (other.d_type == data::STRING))
            {
                result.d_type = data::ERROR;
                result.d_error = error::INVALID_OPER_STRING;
            }

            return result;
        }


        //logical NOR
        template <typename CHAR>
        const data_t<CHAR> data_t<CHAR>::nor_logical(const data_t<CHAR>& other) const
        {
            data_t<CHAR> result;

            if ((d_type == data::VALUE) && (other.d_type == data::VALUE))
            {
                result = !((long)d_value.real() || (long)other.d_value.real());
            }
            else if ((d_type == data::ARRAY) || (other.d_type == data::ARRAY))
            {
                result.d_type = data::ERROR;
                result.d_error = error::INVALID_OPER_ARRAY;
            }
            else if ((d_type == data::STRING) || (other.d_type == data::STRING))
            {
                result.d_type = data::ERROR;
                result.d_error = error::INVALID_OPER_STRING;
            }

            return result;
        }


        //bitwise NOR
        template <typename CHAR>
        const data_t<CHAR> data_t<CHAR>::nor_bitwise(const data_t<CHAR>& other) const
        {
            data_t<CHAR> result;

            if ((d_type == data::VALUE) && (other.d_type == data::VALUE))
            {
                result = ~((long)d_value.real() | (long)other.d_value.real());
            }
            else if ((d_type == data::ARRAY) || (other.d_type == data::ARRAY))
            {
                result.d_type = data::ERROR;
                result.d_error = error::INVALID_OPER_ARRAY;
            }
            else if ((d_type == data::STRING) || (other.d_type == data::STRING))
            {
                result.d_type = data::ERROR;
                result.d_error = error::INVALID_OPER_STRING;
            }

            return result;
        }


        //logical NXOR
        template <typename CHAR>
        const data_t<CHAR> data_t<CHAR>::nxor_logical(const data_t<CHAR>& other) const
        {
            data_t<CHAR> result;

            if ((d_type == data::VALUE) && (other.d_type == data::VALUE))
            {
                result = !(long)d_value.real() == !(long)other.d_value.real();
            }
            else if ((d_type == data::ARRAY) || (other.d_type == data::ARRAY))
            {
                result.d_type = data::ERROR;
                result.d_error = error::INVALID_OPER_ARRAY;
            }
            else if ((d_type == data::STRING) || (other.d_type == data::STRING))
            {
                result.d_type = data::ERROR;
                result.d_error = error::INVALID_OPER_STRING;
            }

            return result;
        }


        //bitwise NXOR
        template <typename CHAR>
        const data_t<CHAR> data_t<CHAR>::nxor_bitwise(const data_t<CHAR>& other) const
        {
            data_t<CHAR> result;

            if ((d_type == data::VALUE) && (other.d_type == data::VALUE))
            {
                result = ~((long)d_value.real() ^ (long)other.d_value.real());
            }
            else if ((d_type == data::ARRAY) || (other.d_type == data::ARRAY))
            {
                result.d_type = data::ERROR;
                result.d_error = error::INVALID_OPER_ARRAY;
            }
            else if ((d_type == data::STRING) || (other.d_type == data::STRING))
            {
                result.d_type = data::ERROR;
                result.d_error = error::INVALID_OPER_STRING;
            }

            return result;
        }



    }
}

#endif // DATA_T_H_INCLUDED
