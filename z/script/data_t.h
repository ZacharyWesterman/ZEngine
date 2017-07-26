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
 * Last modified:   19 Jul. 2017
**/

#pragma once
#ifndef DATA_T_H_INCLUDED
#define DATA_T_H_INCLUDED

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
        namespace data
        {
            enum DATA_TYPE
            {
                NONE = 0,

                ERROR,

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
            error_flag d_error;

            std::complex<double> d_value;

            core::string<CHAR> d_string;

            core::array<data_t> d_array;


        public:
            data_t()
            {
                d_type = data::NONE;

                d_value = 0;
                d_error = 0;
            }

            data_t(error_flag new_error)
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
                d_value = (double)_real;
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
                d_string = cstring;
            }


            ~data_t() {}

            const bool operator==(const data_t&) const;

            inline const bool operator!=(const data_t& other) const
            {
                return !operator==(other);
            }


            const data_t& operator=(const data_t&);


            inline const int type() const {return d_type;}

            inline void setType(const data::DATA_TYPE new_type)
            { d_type = new_type; }

            const core::string<CHAR> string() const;
            const std::complex<double> complex() const;
            const double real() const;
            const double imag() const;

            inline core::array< data_t<CHAR> >& array()
            { return d_array; }

            error_flag error() const
            {
                if (d_type == data::ERROR)
                    return d_error;
                else
                    return error::NONE;
            }


            //operators
            const data_t operator-();
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

            const data_t floor() const;
            const data_t ceil() const;
            const data_t round() const;
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




        ///Operators

        //negation
        template <typename CHAR>
        const data_t<CHAR> data_t<CHAR>::operator-()
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
                else if (d_value.real() > FACTORIAL_MAX_INPUT_DBL)
                {
                    result.d_type = data::ERROR;
                    result.d_error = error::FACTORIAL_OVERFLOW;
                }
                else
                    result = math::factorial(d_value.real());
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
