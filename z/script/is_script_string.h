/**
 * File:            is_script_string.h
 * Namespace:       z::script
 * Description:     A template function to return whether
 *                  the given text is a script string.
 *                  (it begins with a double quote)
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   3 Feb. 2017
**/

#pragma once
#ifndef IS_SCRIPT_STRING_H_INCLUDED
#define IS_SCRIPT_STRING_H_INCLUDED

#include "../core/string.h"

namespace z
{
    namespace script
    {
        template <typename CHAR>
        bool is_script_string(const core::string<CHAR>& text)
        {
            core::string<CHAR> quote = (CHAR)34;

            return text.beginsWith(quote);
        }
    }
}

#endif // IS_SCRIPT_STRING_H_INCLUDED
