#pragma once
#ifndef LANG_KEYWORDS_H_INCLUDED
#define LANG_KEYWORDS_H_INCLUDED

#include <z/core/array.h>
#include <z/script/compiler/keyword.h>

enum KEYWORDS
{
    NONE = 0,
    IF,
    ELSE
};

#define KWD z::script::compiler::keyword

z::core::array<KWD> genKeywords()
{
    z::core::array<KWD> kwds =
    {
        KWD("if", IF),
        KWD("else", ELSE)
    };

    return kwds;
}

#undef KWD

#endif // LANG_KEYWORDS_H_INCLUDED
