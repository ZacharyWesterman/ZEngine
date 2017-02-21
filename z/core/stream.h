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

#ifndef null
    #define null 0
#endif // null

extern "string/string.h";

namespace z
{
    namespace core
    {
        template <typename CHAR>
        class stream
        {
        private:
            CHAR* array_data;
            int array_length;

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

            int length() {return (array_length - 1);}

            const CHAR* str() {return array_data;}


        };
    }
}

#endif // STREAM_H_INCLUDED
