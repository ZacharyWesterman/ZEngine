#pragma once
#ifndef IS_VALID_SCRIPT_NAME_H_INCLUDED
#define IS_VALID_SCRIPT_NAME_H_INCLUDED

#include "../core/string.h"
#include "../core/stringUtils/char_is_alphanumeric.h"

namespace script
{
    template <typename CHAR>
    bool is_valid_script_name(const core::string<CHAR>& script_name)
    {
        if (!core::is_alpha(script_name.at(0)) &&  //if the first character is not alphabetical,
            (script_name.at(0) != (CHAR)95))      //and it is not an underscore,
        {
            return false; //then it is not valid
        }

        //for every character in the name after the first,
        for (int i=0; i<script_name.length(); i++)
        {
            if (!core::is_alphanumeric(script_name.at(i)) &&  //if this character is not alphanumeric,
                (script_name.at(i) != (CHAR)95) )      //and it is not an underscore,
            {
                return false; //then it is not valid
            }
        }

        return true;
    }
}

#endif // IS_VALID_SCRIPT_NAME_H_INCLUDED
