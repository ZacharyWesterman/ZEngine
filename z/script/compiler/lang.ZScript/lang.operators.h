#pragma once
#ifndef LANG_OPERATORS_H_INCLUDED
#define LANG_OPERATORS_H_INCLUDED

#include <z/core/array.h>
#include <z/script/compiler/operator.h>

enum OPERATORS
{
    NONE = 0,
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

    };

    return opers;
}

#undef OPER

#endif // LANG_OPERATORS_H_INCLUDED
