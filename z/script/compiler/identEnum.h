#pragma once
#ifndef IDENTENUM_H_INCLUDED
#define IDENTENUM_H_INCLUDED

namespace z
{
namespace script
{
    namespace compiler
    {
        enum ident
        {
            NONE = 0,

            LPARENTH,       //1
            RPARENTH,       //2

            LBRACE,         //3
            RBRACE,         //4

            LBRACKET,       //5
            RBRACKET,       //6

            COMMA,          //7
            SEMICOLON,      //8
            PERIOD,         //9

            STRING_LITERAL, //10

            NUMERIC_LITERAL,//11
            COMPLEX_LITERAL,//12

            LITERAL,

            IDENTIFIER,     //13

            KEYWORD,
            OPERATOR,


            UNKNOWN,        //69

            ID_COUNT
        };
    }
}
}

#endif // IDENTENUM_H_INCLUDED
