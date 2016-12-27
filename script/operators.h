#pragma once
#ifndef OPERATORS_H_INCLUDED
#define OPERATORS_H_INCLUDED

#include "../core/string.h"


namespace script
{
    namespace OPERATOR
    {
        enum OPERATOR_LIST
        {
            NONE =              0,

            ADD =               1,
            SUBTRACT =          2,
            MULTIPLY =          3,
            DIVIDE =            4,
            INTEGER_DIVIDE =    5,
            MODULUS =           6,

            POWER =             7,
            FACTORIAL =         8,

            OPEN_PARENTH =      9,
            CLOSE_PARENTH =     10,

            EQUALS =            11,
            GREATER_THAN =      12,
            LESS_THAN =         13,
            GREATER_THAN_EQUAL =14,
            LESS_THAN_EQUAL =   15,
            NOT_EQUAL =         16,

            AND =               17,
            OR =                18,
            NOT =               19,
            XOR =               20,
            NAND =              21,
            NOR =               22,
            XNOR =              23,

            OPERATOR_COUNT
        };

        const uint8_t OPERATOR_PRIORITY[] =
        {
            0, //NONE

            3,  //ADD
            3,  //SUBTRACT
            4,  //MULTIPLY
            4,  //DIVIDE
            4,  //INTEGER_DIVIDE
            4,  //MODULUS

            6,  //POWER
            7,  //FACTORIAL

            0,  //OPEN_PARENTH,
            0,  //CLOSE_PARENTH,

            2,  //EQUALS,
            2,  //GREATER_THAN,
            2,  //LESS_THAN,
            2,  //GREATER_THAN_EQUAL,
            2,  //LESS_THAN_EQUAL,
            2,  //NOT_EQUAL,

            1,  //AND,
            1,  //OR,
            5,  //NOT,
            1,  //XOR,
            1,  //NAND,
            1,  //NOR,
            1  //XNOR
        };

        const core::string<char> CHAR_OPERATOR[] =
        {
            "",

            "+",
            "-",
            "*",
            "/",
            "//",
            "%",

            "^",
            "!",

            "(",
            ")",

            "==",
            ">",
            "<",
            ">=",
            "<=",
            "!=",

            "and",
            "or",
            "not",
            "xor",
            "nand",
            "nor",
            "xnor"
        };


        const core::string<wchar_t> WCHAR_T_OPERATOR[] =
        {
            L"",

            L"+",
            L"-",
            L"*",
            L"/",
            L"//",
            L"%",

            L"^",
            L"!",

            L"(",
            L")",

            L"==",
            L">",
            L"<",
            L">=",
            L"<=",
            L"!=",

            L"and",
            L"or",
            L"not",
            L"xor",
            L"nand",
            L"nor",
            L"xnor"
        };
    }

    uint8_t priority(uint8_t oper)
    {
        if (oper < OPERATOR::OPERATOR_COUNT)
            return OPERATOR::OPERATOR_PRIORITY[oper];
        else
            return OPERATOR::OPERATOR_PRIORITY[OPERATOR::NONE];
    }


    ///<char> functions relating to operators
    uint8_t what_operator(const core::string<char>& oper, int position)
    {
        for (uint8_t i=OPERATOR::OPERATOR_COUNT-1; i>0; i--)
        {
            if (oper.foundAt(OPERATOR::CHAR_OPERATOR[i], position))
                return i;
        }

        return OPERATOR::NONE;
    }

    uint8_t what_operator(const core::string<char>& oper)
    {
        for (uint8_t i=OPERATOR::OPERATOR_COUNT-1; i>0; i--)
        {
            if (oper == OPERATOR::CHAR_OPERATOR[i])
                return i;
        }

        return OPERATOR::NONE;
    }

    uint8_t what_operator_at_end(const core::string<char>& oper)
    {
        for (uint8_t i=OPERATOR::OPERATOR_COUNT-1; i>0; i--)
        {
            if (oper.foundAt(OPERATOR::CHAR_OPERATOR[i],
                             oper.length()-OPERATOR::CHAR_OPERATOR[i].length()))
                return i;
        }

        return OPERATOR::NONE;
    }

    void operator_string(uint8_t oper, core::string<char>& output)
    {
        if (oper < OPERATOR::OPERATOR_COUNT)
            output = OPERATOR::CHAR_OPERATOR[oper];
        else
            output = OPERATOR::NONE;
    }


    ///<wchar_t> functions relating to operators
    uint8_t what_operator(const core::string<wchar_t>& oper, int position)
    {
        for (uint8_t i=OPERATOR::OPERATOR_COUNT-1; i>0; i--)
        {
            if (oper.foundAt(OPERATOR::WCHAR_T_OPERATOR[i], position))
                return i;
        }

        return OPERATOR::NONE;
    }

    uint8_t what_operator(const core::string<wchar_t>& oper)
    {
        for (uint8_t i=OPERATOR::OPERATOR_COUNT-1; i>0; i--)
        {
            if (oper == OPERATOR::WCHAR_T_OPERATOR[i])
                return i;
        }

        return OPERATOR::NONE;
    }

    uint8_t what_operator_at_end(const core::string<wchar_t>& oper)
    {
        for (uint8_t i=OPERATOR::OPERATOR_COUNT-1; i>0; i--)
        {
            if (oper.foundAt(OPERATOR::WCHAR_T_OPERATOR[i],
                             oper.length()-OPERATOR::WCHAR_T_OPERATOR[i].length()))
                return i;
        }

        return OPERATOR::NONE;
    }

    void operator_string(uint8_t oper, core::string<wchar_t>& output)
    {
        if (oper < OPERATOR::OPERATOR_COUNT)
            output = OPERATOR::WCHAR_T_OPERATOR[oper];
        else
            output = OPERATOR::NONE;
    }
}

#endif // OPERATORS_H_INCLUDED
