#pragma once
#ifndef LANG_OPERATORS_H_INCLUDED
#define LANG_OPERATORS_H_INCLUDED

#include <z/core/array.h>
#include <z/script/compiler/operator.h>

enum OPERATORS
{
    OPER_NONE = 0,
    ASSIGN,

    ADD_ASSIGN,
    SUB_ASSIGN,

    MUL_ASSIGN,
    DIV_ASSIGN,
    IDIV_ASSIGN,
    MOD_ASSIGN,

    ADD1,      //39
    SUB1,      //40

    SIZEOF,

    POW,       //45
    FAC,       //46

    ADD,       //37
    SUB,       //38

    MUL,       //41
    DIV,       //42
    IDIV,      //43
    MOD,       //44

    AND_LGCL,  //47
    AND_BITW,  //48

    OR_LGCL,   //49
    OR_BITW,   //50

    XOR_LGCL,  //51
    XOR_BITW,  //52

    NAND_LGCL, //55
    NAND_BITW, //56

    NOR_LGCL,  //57
    NOR_BITW,  //58

    NXOR_LGCL, //59
    NXOR_BITW, //60

    EQ,        //61
    NOT_EQ,    //62
    GT,        //63
    GT_EQ,     //64
    LT,        //65
    LT_EQ,     //66

    NOT_LGCL,  //53
    NOT_BITW,  //54

    R_ARROW,   //67
    L_ARROW,   //68
};

#define OPER z::script::compiler::oper

z::core::array<OPER> genOperators()
{
    z::core::array<OPER> opers =
    {
        OPER("=",   ASSIGN),
        OPER("+",   ADD),
        OPER("-",   SUB),
        OPER("*",   MUL),
        OPER("/",   DIV),
        OPER("//",  IDIV),
        OPER("%",   MOD),
    };

    return opers;
}

/*if (input.foundAt("==", x_offset))
                {
                    curr_oper = ident::OPER_EQ;
                    oper_length = 2;
                }
                else if (input.foundAt("=", x_offset))
                {
                    curr_oper = ident::OPER_ASSIGN;
                    oper_length = 1;
                }
                else if (input.foundAt("<-", x_offset))
                {
                    curr_oper = ident::OPER_L_ARROW;
                    oper_length = 2;
                }
                else if (input.foundAt("<>", x_offset))
                {
                    curr_oper = ident::OPER_NOT_EQ;
                    oper_length = 2;
                }
                else if (input.foundAt("<=", x_offset))
                {
                    curr_oper = ident::OPER_LT_EQ;
                    oper_length = 2;
                }
                else if (input.foundAt("<", x_offset))
                {
                    curr_oper = ident::OPER_LT;
                    oper_length = 1;
                }
                else if (input.foundAt(">=", x_offset))
                {
                    curr_oper = ident::OPER_GT_EQ;
                    oper_length = 2;
                }
                else if (input.foundAt(">", x_offset))
                {
                    curr_oper = ident::OPER_GT;
                    oper_length = 1;
                }
                else if (input.foundAt("++", x_offset))
                {
                    curr_oper = ident::OPER_ADD1;
                    oper_length = 2;
                }
                else if (input.foundAt("+=", x_offset))
                {
                    curr_oper = ident::OPER_ADD_ASSIGN;
                    oper_length = 2;
                }
                else if (input.foundAt("+", x_offset))
                {
                    curr_oper = ident::OPER_ADD;
                    oper_length = 1;
                }
                else if (input.foundAt("->", x_offset))
                {
                    curr_oper = ident::OPER_R_ARROW;
                    oper_length = 2;
                }
                else if (input.foundAt("--", x_offset))
                {
                    curr_oper = ident::OPER_SUB1;
                    oper_length = 2;
                }
                else if (input.foundAt("-=", x_offset))
                {
                    curr_oper = ident::OPER_SUB_ASSIGN;
                    oper_length = 2;
                }
                else if (input.foundAt("-", x_offset))
                {
                    curr_oper = ident::OPER_SUB;
                    oper_length = 1;
                }
                else if (input.foundAt("*=", x_offset))
                {
                    curr_oper = ident::OPER_MUL_ASSIGN;
                    oper_length = 2;
                }
                else if (input.foundAt("*", x_offset))
                {
                    curr_oper = ident::OPER_MUL;
                    oper_length = 1;
                }
                else if (input.foundAt("//=", x_offset))
                {
                    curr_oper = ident::OPER_IDIV_ASSIGN;
                    oper_length = 3;
                }
                else if (input.foundAt("//", x_offset))
                {
                    curr_oper = ident::OPER_IDIV;
                    oper_length = 2;
                }
                else if (input.foundAt("/=", x_offset))
                {
                    curr_oper = ident::OPER_DIV_ASSIGN;
                    oper_length = 2;
                }
                else if (input.foundAt("/", x_offset))
                {
                    curr_oper = ident::OPER_DIV;
                    oper_length = 1;
                }
                else if (input.foundAt("%=", x_offset))
                {
                    curr_oper = ident::OPER_MOD_ASSIGN;
                    oper_length = 2;
                }
                else if (input.foundAt("%", x_offset))
                {
                    curr_oper = ident::OPER_MOD;
                    oper_length = 1;
                }
                else if (input.foundAt("^", x_offset))
                {
                    curr_oper = ident::OPER_POW;
                    oper_length = 1;
                }
                else if (input.foundAt("!", x_offset))
                {
                    curr_oper = ident::OPER_FAC;
                    oper_length = 1;
                }
                else if (input.foundAt("~&", x_offset))
                {
                    curr_oper = ident::OPER_NAND_BITW;
                    oper_length = 2;
                }
                else if (input.foundAt("~|", x_offset))
                {
                    curr_oper = ident::OPER_NOR_BITW;
                    oper_length = 2;
                }
                else if (input.foundAt("~:", x_offset))
                {
                    curr_oper = ident::OPER_NXOR_BITW;
                    oper_length = 2;
                }
                else if (input.foundAt("~", x_offset))
                {
                    curr_oper = ident::OPER_NOT_BITW;
                    oper_length = 1;
                }
                else if (input.foundAt("&", x_offset))
                {
                    curr_oper = ident::OPER_AND_BITW;
                    oper_length = 1;
                }
                else if (input.foundAt("|", x_offset))
                {
                    curr_oper = ident::OPER_OR_BITW;
                    oper_length = 1;
                }
                else if (input.foundAt(":", x_offset))
                {
                    curr_oper = ident::OPER_XOR_BITW;
                    oper_length = 1;
                }
                else
                {
                    found = false;
                }*/

#undef OPER

#endif // LANG_OPERATORS_H_INCLUDED
