#pragma once
#ifndef LANG_COMMENTS_H_INCLUDED
#define LANG_COMMENTS_H_INCLUDED

#include <z/core/string.h>
#include <z/core/array.h>

#define STRING z::core::string<char>
#define ARRAY z::core::array<STRING>

ARRAY* genCommentRules()
{
    ARRAY* rules = new ARRAY[3];

    rules[0] =
    ARRAY //single-line comment rules
    {
        "\\\\", //Single-line comment
        "@"     //other acceptable SL comment
    };

    rules[1] =
    ARRAY //multiple-line comment open rules
    {
        "\\*", //open comment
    };

    rules[2] =
    ARRAY //multiple-line comment close rules
    {
        "*\\", //open comment
    };

    return rules;
}

#undef STRING
#undef ARRAY

#endif // LANG_COMMENTS_H_INCLUDED
