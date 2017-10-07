#pragma once
#ifndef LANG_OPERATORS_H_INCLUDED
#define LANG_OPERATORS_H_INCLUDED

#include <z/core/sortedArray.h>
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
#define ARRAY z::core::sortedArray<OPER>

ARRAY* genOperators()
{
    ARRAY* opers = new ARRAY
    {
        OPER("=",   ASSIGN),
        OPER("+=",  ADD_ASSIGN),
        OPER("-=",  SUB_ASSIGN),
        OPER("*=",  MUL_ASSIGN),
        OPER("/=",  DIV_ASSIGN),
        OPER("//=", IDIV_ASSIGN),
        OPER("%=",  MOD_ASSIGN),
        OPER("++",  ADD1),
        OPER("--",  SUB1),
        OPER("sizeof",  SIZEOF),
        OPER("^",   POW),
        OPER("!",   FAC),
        OPER("+",   ADD),
        OPER("-",   SUB),
        OPER("*",   MUL),
        OPER("/",   DIV),
        OPER("//",  IDIV),
        OPER("%",   MOD),
        OPER("and", AND_LGCL),
        OPER("&",   AND_BITW),
        OPER("or",  OR_LGCL),
        OPER("|",   OR_BITW),
        OPER("xor", XOR_LGCL),
        OPER(":",   XOR_BITW),
        OPER("nand",NAND_LGCL),
        OPER("~&",  NAND_BITW),
        OPER("nor", NOR_LGCL),
        OPER("~|",  NOR_BITW),
        OPER("nxor",NXOR_LGCL),
        OPER("~:",  NXOR_BITW),
        OPER("==",  EQ),
        OPER("<>",  NOT_EQ),
        OPER(">",   GT),
        OPER(">=",  GT_EQ),
        OPER("<",   LT),
        OPER("<=",  LT_EQ),
        OPER("not", NOT_LGCL),
        OPER("~",   NOT_BITW),
        OPER("->",  R_ARROW),
        OPER("<-",  L_ARROW)
    };

    return opers;
}

#undef OPER
#undef ARRAY

#endif // LANG_OPERATORS_H_INCLUDED
