/**
 * File:            datatype.h
 * Namespace:       z::script
 *
 * Description:     A template controller class for the generic datatype.
 *
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   7 Jun. 2017
**/


#pragma once
#ifndef DATATYPE_H_INCLUDED
#define DATATYPE_H_INCLUDED

#include <type_traits>

#include "data_t.h"

namespace z
{
    namespace script
    {
        template <typename CHAR>
        class datatype
        {
        private:
            int type;

            data_t<CHAR> gen_data;

        public:
            datatype() {type = data::NONE;}

            ~datatype() {}

            ///Set the contained data to a character (just convert to string)
            /*const datatype& operator=(CHAR character)
            {
                data.string = character;

                data.value = 0.0;

                data.type = VAR_STRING;

                return *this;
            }

                ///Set the contained data to any numerical value
                template<
                    typename T, //real type
                    typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type
                >
                const datatype& operator=(const T& number)
                {
                    data.string.clear();

                    data.type = VAR_NUMERICAL;

                    data.value = (double)number;

                    return *this;
                }


                ///Set the contained data to a string
                const datatype& operator=(const core::string<CHAR>& text)
                {
                    data.string = text;

                    data.value = 0.0;

                    data.type = VAR_STRING;

                    return *this;
                }

                ///Set this datatype to be the same as another datatype
                const datatype& operator=(const datatype& other)
                {
                    data.type   = other.data.type;
                    data.string = other.data.string;
                    data.value  = other.data.value;

                    return *this;
                }


                ///Make the data NULL
                void clear()
                {
                    data.string.clear();

                    data.value = 0.0;

                    data.type = VAR_NULL;
                }


                ///Get a string representation of the data
                const core::string<char> string() const
                {
                    if (data.type == VAR_STRING)
                        return data.string;
                    else if (data.type == VAR_NUMERICAL)
                        return core::string<CHAR> (data.value);
                    else
                        return core::string<CHAR> ();
                }


                ///Get the numerical value of the data
                ///(returns 0.0 if null or a string)
                const double value() const
                {
                    return data.value;
                }*/
            };
    }
}

#endif // DATATYPE_H_INCLUDED
