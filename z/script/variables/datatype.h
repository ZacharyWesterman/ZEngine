/**
 * File:            datatype.h
 * Namespace:       z::script::var
 *
 * Description:     A template controller class for single variables.
 *
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   3 Mar. 2017
**/


#pragma once
#ifndef DATATYPE_H_INCLUDED
#define DATATYPE_H_INCLUDED

#include <type_traits>

#include "var_struct.h"

namespace z
{
    namespace script
    {
        namespace var
        {
            template <typename CHAR>
            class datatype
            {
            private:
                var_t<CHAR> data;

            public:
                ///Set the variable to a character (just convert to string)
                const variable& operator=(CHAR character)
                {
                    data.string = character;

                    data.value = 0.0;

                    data.type = VAR_STRING;

                    return *this;
                }

                ///Set the variable to any numerical value
                template<
                    typename T, //real type
                    typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type
                >
                const variable& operator=(const T& number)
                {
                    data.string.clear();

                    data.type = VAR_NUMERICAL;

                    data.value = (double)number;

                    return *this;
                }


                ///Set the variable to a string
                const variable& operator=(const core::string<CHAR>& text)
                {
                    data.string = text;

                    data.value = 0.0;

                    data.type = VAR_STRING;

                    return *this;
                }

                ///Set the variable to be the same as another variable
                const variable& operator=(const variable& other)
                {
                    data.type   = other.data.type;
                    data.string = other.data.string;
                    data.value  = other.data.value;

                    return *this;
                }


                ///Make the variable NULL
                void clear()
                {
                    data.string.clear();

                    data.value = 0.0;

                    data.type = VAR_NULL;
                }


                ///Get a string representation of the variable
                const core::string<char> string() const
                {
                    if (data.type == VAR_STRING)
                        return data.string;
                    else if (data.type == VAR_NUMERICAL)
                        return core::string<CHAR> (data.value);
                    else
                        return core::string<CHAR> ();
                }


                ///Get the numerical value of the variable
                ///(returns 0.0 if not a number)
                const double value() const
                {
                    return data.value;
                }
            };
        }
    }
}

#endif // DATATYPE_H_INCLUDED
