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

                KEYWORD_IF,     //16
                KEYWORD_ELSE,   //17

                KEYWORD_FOR,    //18
                KEYWORD_EACH,   //19
                KEYWORD_DO,     //20
                KEYWORD_LOOP,   //21
                KEYWORD_WHILE,  //22

                KEYWORD_GOTO,   //23
                KEYWORD_GOSUB,  //24

                KEYWORD_RUN,    //25
                KEYWORD_INCLUDE,//26

                KEYWORD_BREAK,  //27
                KEYWORD_RETURN, //28

                KEYWORD_EXIT,   //29

                KEYWORD_WAIT,   //30
                KEYWORD_UNTIL,  //31

                KEYWORD_TYPE,   //32

                KEYWORD_GLOBAL, //33
                KEYWORD_EXTERNAL,//34

                OPER_ASSIGN,    //35

                OPER_ADD,       //36
                OPER_SUB,       //37

                OPER_ADD1,      //38
                OPER_SUB1,      //39

                OPER_MUL,       //40
                OPER_DIV,       //41
                OPER_IDIV,      //42
                OPER_MOD,       //43

                OPER_POW,       //44
                OPER_FAC,       //45

                OPER_AND_LGCL,  //46
                OPER_AND_BITW,  //47

                OPER_OR_LGCL,   //48
                OPER_OR_BITW,   //49

                OPER_XOR_LGCL,  //50
                OPER_XOR_BITW,  //51

                OPER_NOT_LGCL,  //52
                OPER_NOT_BITW,  //53

                OPER_NAND_LGCL, //54
                OPER_NAND_BITW, //55

                OPER_NOR_LGCL,  //56
                OPER_NOR_BITW,  //57

                OPER_XNOR_LGCL, //58

                OPER_EQ,        //59
                OPER_NOT_EQ,    //60
                OPER_GT,        //61
                OPER_GT_EQ,     //62
                OPER_LT,        //63
                OPER_LT_EQ,     //64


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
