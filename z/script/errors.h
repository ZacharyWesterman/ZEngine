/**
 * File:            script_errors.h
 * Namespace:       z::script::error
 * Description:     The z::script::error_flag type is defined here.
 *                  Additionally, all error flags are defined here.
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   20 Aug. 2017
**/

#pragma once
#ifndef SCRIPT_ERRORS_H_INCLUDED
#define SCRIPT_ERRORS_H_INCLUDED


#include <z/core/string.h>

namespace z
{
    namespace script
    {
        typedef uint8_t error_flag; //error range [0, 255]

        namespace error
        {
            enum ERRORS
            {
                NONE = 0,

                //scanner errors
                INVALID_IDENTIFIER,
                INVALID_NUMBER_BASE2,
                INVALID_NUMBER_BASE8,
                INVALID_NUMBER_BASE10,
                INVALID_NUMBER_BASE16,
                NUMBER_EXCESS_DECIMALS,
                UNKNOWN_ESCAPE_SEQUENCE,

                UNKNOWN_OPERATOR,
                AMBIGUOUS_EXPR,

                INVALID_INCLUDE,
                INCLUDE_LOAD_FAILED,

                MISSING_L_PARENTH,
                MISSING_R_PARENTH,

                MISSING_L_BRACKET,
                MISSING_R_BRACKET,

                MISSING_L_BRACE,
                MISSING_R_BRACE,


                //lexer errors
                SYNTAX_ERROR,
                UNEXPECTED_OPERATOR,
                STMT_OUTSIDE_FUNCTION,
                UNEXPECTED_SEMICOLON,
                EXPECTED_PARENTHS,
                EXPECTED_BRACES,
                EXPECTED_BRACKETS,
                EXPECTED_SEMICOLON,
                EXPECTED_PARAMETER,
                EXPECTED_STMT_BODY,


                //operation errors
                INVALID_OPER_STRING,
                INVALID_OPER_ARRAY,

                DIV_BY_ZERO,

                FACTORIAL_OVERFLOW,
                FACTORIAL_NEGATIVE,
                FACTORIAL_COMPLEX,



                TOO_MANY_PARAMS,
                TOO_FEW_PARAMS,

                ARRAY_OUT_OF_BOUNDS,

                PARAM_MUST_BOOLEAN,
                PARAM_MUST_INTEGER,
                PARAM_OUT_OF_BOUNDS,
                PARAM_MUST_REAL,
                PARAM_MUST_COMPLEX,
                PARAM_MUST_VALUE,
                PARAM_MUST_STRING,

                PARAM_MUST_POSITIVE,
                PARAM_MUST_NEGATIVE,
                PARAM_MUST_NONZERO,
                PARAM_MUST_NONPOSITIVE,
                PARAM_MUST_NONNEGATIVE,

                PARAM_CANNOT_VALUE,
                PARAM_CANNOT_STRING,
                PARAM_CANNOT_ARRAY,

                PARAM_MUST_ARRAY,
                PARAM_MUST_ARRAY_SAME_TYPE,


                FUNCTION_REQ_GRAPHICAL,
                COMMAND_REQ_GRAPHICAL,

                VARIABLE_REDECLARED,
                VARIABLE_UNDECLARED,

                UNDEFINED_ERROR,

                ERROR_COUNT
            };
        }

        template <typename CHAR>
        class parser_error
        {
        public:
            int line;
            int column;

            error_flag err;

            core::string<CHAR>* file;

            core::string<CHAR> extra_data;

            parser_error()
            {
                line = 0;
                column = 0;

                err = error::NONE;

                file = NULL;
            }

            parser_error(int Line, int Column, error_flag Error,
                         const core::string<CHAR>& extra, core::string<CHAR>* fileID)
            {
                line = Line;
                column = Column;

                err = Error;

                extra_data = extra;

                file = fileID;
            }

            parser_error(int Line, int Column, error_flag Error, core::string<CHAR>* fileID)
            {
                line = Line;
                column = Column;

                err = Error;

                file = fileID;
            }

            parser_error(const parser_error& other)
            {
                line = other.line;
                column = other.column;

                err = other.err;

                extra_data = other.extra_data;

                file = other.file;
            }

            const parser_error& operator=(const parser_error& other)
            {
                line = other.line;
                column = other.column;

                err = other.err;

                extra_data = other.extra_data;

                file = other.file;

                return *this;
            }

            inline bool operator==(const parser_error& other) const
            {
                return err == other.err;
            }
        };
    }
}

#endif // SCRIPT_ERRORS_H_INCLUDED
