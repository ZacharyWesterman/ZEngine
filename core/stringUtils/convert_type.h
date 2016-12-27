#ifndef CONVERT_TYPE_H_INCLUDED
#define CONVERT_TYPE_H_INCLUDED

#include "../string.h"

namespace core
{
    const string<char> narrow(const string<wchar_t>& other)
    {
        string<char> output;

        for (int i=0; i<other.length(); i++)
        {
            wchar_t code = other[i];

            if (code < 128)
                output += (char)code;
            else
            {
                output += '?';

                if ((code >= 0xD800) && (code <= 0xD8FF))
                    i++;
            }
        }



        return output;
    }

    const string<char>& narrow(const string<char>& other)
    {
        return other;
    }



    const string<wchar_t> widen(const string<char>& other)
    {
        string<wchar_t> output;

        for (int i=0; i<other.length(); i++)
        {
            output += (wchar_t)other[i];
        }


        return output;
    }

    const string<wchar_t>& widen(const string<wchar_t>& other)
    {
        return other;
    }
}

#endif // CONVERT_TYPE_H_INCLUDED
