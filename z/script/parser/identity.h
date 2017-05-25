/**
 * File:            identity.h
 * Namespace:       z::script
 * Description:     Template for script identities. An example
 *                  identifier could be represented as <"+",OPERATOR>.
 *                  For error reporting purposes, all identities include
 *                  the line and column of their first character from
 *                  the input stream.
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   4 Mar. 2017
**/

#pragma once
#ifndef IDENTITY_H_INCLUDED
#define IDENTITY_H_INCLUDED

#include <z/core/string.h>
#include "../errors.h"

namespace z
{
    namespace script
    {
        namespace ident
        {
            enum ident_enum
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

                IDENTIFIER,     //12

                FUNCTION,       //13
                COMMAND,        //14

                KEYWORD_MAIN,   //15

                KEYWORD_IF,     //16
                KEYWORD_ELSE,   //17

                KEYWORD_FOR,    //18
                KEYWORD_DO,     //19
                KEYWORD_LOOP,   //20
                KEYWORD_WHILE,  //21

                KEYWORD_GOTO,   //22
                KEYWORD_GOSUB,  //23

                KEYWORD_RUN,    //24
                KEYWORD_INCLUDE,//25

                KEYWORD_BREAK,  //26
                KEYWORD_RETURN, //27

                KEYWORD_EXIT,   //28

                KEYWORD_WAIT,   //29

                KEYWORD_TYPE,   //30

                KEYWORD_GLOBAL, //31
                KEYWORD_EXTERNAL,//32

                OPER_ASSIGN,    //33

                OPER_ADD,       //34
                OPER_SUB,       //35

                OPER_ADD1,      //36
                OPER_SUB1,      //37

                OPER_MUL,       //38
                OPER_DIV,       //39
                OPER_IDIV,      //40
                OPER_MOD,       //41

                OPER_POW,       //42
                OPER_FAC,       //43

                OPER_AND_LGCL,  //44
                OPER_AND_BITW,  //45

                OPER_OR_LGCL,   //46
                OPER_OR_BITW,   //47

                OPER_XOR_LGCL,  //48
                OPER_XOR_BITW,  //49

                OPER_NOT_LGCL,  //50
                OPER_NOT_BITW,  //51

                OPER_NAND_LGCL, //52
                OPER_NAND_BITW, //53

                OPER_NOR_LGCL,  //54
                OPER_NOR_BITW,  //55

                OPER_EQ,        //56
                OPER_NOT_EQ,    //57
                OPER_GT,        //58
                OPER_GT_EQ,     //59
                OPER_LT,        //60
                OPER_LT_EQ,     //61


                UNKNOWN = -1
            };

        }

        template <typename CHAR>
        class ident_t
        {
        public:
            ident::ident_enum type;

            int line;
            int column;

            error_flag err;

            core::string<CHAR>* meta;

            double value;


            ///full constructor
            ident_t (ident::ident_enum t = ident::NONE, int lin = 0, int col = 0,
                     core::string<CHAR>* symbol_ptr = NULL, error_flag _error = error::NONE)
            {
                type = t;

                line = lin;
                column = col;

                err = _error;

                meta = symbol_ptr;

                value = 0;
            }


            ///equality operators
            inline bool operator==(const ident_t& other) const
            {
                return (type == other.type);
            }

            inline bool operator!=(const ident_t& other) const
            {
                return (type != other.type);
            }

            inline bool operator>(const ident_t& other) const
            {
                return (type > other.type);
            }

            inline bool operator>=(const ident_t& other) const
            {
                return (type >= other.type);
            }

            inline bool operator<(const ident_t& other) const
            {
                return (type < other.type);
            }

            inline bool operator<=(const ident_t& other) const
            {
                return (type <= other.type);
            }
        };

    }
}

#endif // IDENTITY_H_INCLUDED
