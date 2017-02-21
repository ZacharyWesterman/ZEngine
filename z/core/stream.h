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

            int length() {return data.length();}

            const CHAR* str() {return data.str();}


            ///Stream output operator when left operand is a string
            stream& operator<<(const string<CHAR>& arg2)
            {
                CHAR STX = 2; //start of text
                CHAR ETX = 3; //end of text

                data += STX;
                data += arg2 + ETX;

                return *this;
            }

            ///Stream output operator when both operands are streams
            stream& operator<<(const stream& arg2)
            {
                data += arg2.data;

                return *this;
            }


            friend stream<char> operator<<(const string<char>&, const stream<char>&);
            friend stream<wchar_t> operator<<(const string<wchar_t>&, const stream<wchar_t>&);

            friend stream<char> operator<<(const string<char>&, const string<char>&);
            friend stream<wchar_t> operator<<(const string<wchar_t>&, const string<wchar_t>&);
        };





        ///Stream output operator when right operand is a string (narrow chars)
        stream<char> operator<<(const string<char>& arg1, const stream<char>& arg2)
        {
            stream<char> output;

            char STX = 2; //start of text
            char ETX = 3; //end of text

            output.data = STX;
            output.data += arg1 + ETX;
            output.data += arg2.data;

            return output;
        }


        ///Stream output operator when right operand is a string (wide chars)
        stream<wchar_t> operator<<(const string<wchar_t>& arg1, const stream<wchar_t>& arg2)
        {
            stream<wchar_t> output;

            wchar_t STX = 2; //start of text
            wchar_t ETX = 3; //end of text

            output.data = STX;
            output.data += arg1 + ETX;
            output.data += arg2.data;

            return output;
        }


        ///Stream output operator when both operands are strings (narrow chars)
        stream<char> operator<<(const string<char>& arg1, const string<char>& arg2)
        {
            stream<char> output;

            char STX = 2; //start of text
            char ETX = 3; //end of text

            output.data = STX;
            output.data += arg1 + ETX;
            output.data += STX;
            output.data += arg2 + ETX;

            return output;
        }


        ///Stream output operator when both operands are strings (wide chars)
        stream<wchar_t> operator<<(const string<wchar_t>& arg1, const string<wchar_t>& arg2)
        {
            stream<wchar_t> output;

            wchar_t STX = 2; //start of text
            wchar_t ETX = 3; //end of text

            output.data = STX;
            output.data += arg1 + ETX;
            output.data += STX;
            output.data += arg2 + ETX;

            return output;
        }
    }
}

#endif // STREAM_H_INCLUDED
