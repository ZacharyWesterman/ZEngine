/**
 * File:            script_errors.h
 * Namespace:       z::script::error
 * Description:     The z::script::error_flag type is defined here.
 *                  Additionally, all error flags are defined here.
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   2 Aug. 2017
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
                MISSING_R_BRACE, //16


                SYNTAX_ERROR,


                INVALID_OPER_STRING,
                INVALID_OPER_ARRAY,

                DIV_BY_ZERO,

                FACTORIAL_OVERFLOW,
                FACTORIAL_NEGATIVE,
                FACTORIAL_COMPLEX,


                FUNC_TOO_MANY_PARAMS,
                FUNC_TOO_FEW_PARAMS,

                ARRAY_OUT_OF_BOUNDS,

                PARAM_MUST_BOOLEAN,
                PARAM_MUST_INTEGER,
                PARAM_MUST_POS_INTEGER,
                PARAM_MUST_NEG_INTEGER,
                PARAM_OUT_OF_BOUNDS,
                PARAM_MUST_REAL,
                PARAM_MUST_COMPLEX,
                PARAM_MUST_STRING,

                PARAM_MUST_ARRAY,
                PARAM_MUST_ARRAY_SAME_TYPE,
                PARAM_MUST_ARRAY_BOOLEAN,
                PARAM_MUST_ARRAY_INTEGER,
                PARAM_MUST_ARRAY_POS_INTEGER,
                PARAM_MUST_ARRAY_NEG_INTEGER,
                PARAM_MUST_ARRAY_REAL,
                PARAM_MUST_ARRAY_COMPLEX,
                PARAM_MUST_ARRAY_STRING,

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

            int file;

            core::string<CHAR> extra_data;

            parser_error()
            {
                line = 0;
                column = 0;

                err = error::NONE;

                file = -1;
            }

            parser_error(int Line, int Column, error_flag Error,
                         const core::string<CHAR>& extra, int fileID)
            {
                line = Line;
                column = Column;

                err = Error;

                extra_data = extra;

                file = fileID;
            }

            parser_error(int Line, int Column, error_flag Error, int fileID)
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
