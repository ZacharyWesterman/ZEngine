/**
 * File:            stream.h
 * Namespace:       z::core
 *
 * Description:     A template class for character streams.
 *                  Allows for condensing multiple objects
 *                  into one string.
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   21 Feb. 2017
**/

#pragma once
#ifndef STREAM_H_INCLUDED
#define STREAM_H_INCLUDED

#include "string.h"

#ifndef null
    #define null 0
#endif // null

namespace z
{
    namespace core
    {
        template <typename CHAR>
        class stream
        {
        private:
            string<CHAR> data;

        public:
            stream()
            {
                array_data = new CHAR[1];
                array_data[0] = null;

                array_length = 1;
            }

            ~stream()
            {
                delete[] array_data;
            }

            int length() {return data.length();}

            const CHAR* str() {return data.str();}


        };


        ///Stream output operator for strings
        template <typename CHAR>
        stream<CHAR> operator<<(const string<CHAR>& arg1, const string& other) const
        {
            CHAR STX = 2; //start of text
            CHAR ETX = 3; //end of text

            string output = *this + ETX;
            output += STX;
            output += other;

            return output;
        }
    }
}

#endif // STREAM_H_INCLUDED
