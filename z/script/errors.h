/**
 * File:            script_errors.h
 * Namespace:       z::script::error
 * Description:     The z::script::error_flag type is defined here.
 *                  Additionally, all error flags are defined here.
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   6 Mar. 2017
**/

#pragma once
#ifndef SCRIPT_ERRORS_H_INCLUDED
#define SCRIPT_ERRORS_H_INCLUDED


#include <z/core/string.h>

namespace z
{
    namespace script
    {
        typedef uint32_t error_flag;

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
                MISSING_R_BRACE,

                DIV_BY_ZERO,
                INVALID_CHARACTER,
                MISSING_OPERAND,
                INVALID_OPERATION,
                TOO_FEW_PARAMS,
                TOO_MANY_PARAMS,
                INVALID_PARAM,
                NON_REAL_NUMBER,
                OUT_OF_BOUNDS,
                UNKNOWN_VARIABLE,
                WRONG_VAR_TYPE,
                BAD_FORMATTING,
                MISSING_OPERATOR,
                UNKNOWN_COMMAND,
                BAD_REDEFINITION,
                NO_ENTRY_POINT,
                BAD_SCOPING,
                UNEXP_COMMAND,
                UNEXP_END_OF_SCRIPT,
                UNKNOWN_LABEL,
                UNKNOWN_SUBROUTINE,
            };
        }

        template <typename CHAR>
        class parser_error
        {
        public:
            int line;
            int column;

            error_flag err;

            core::string<CHAR> extra_data;

            parser_error()
            {
                line = 0;
                column = 0;

                err = error::NONE;
            }

            parser_error(int Line, int Column, error_flag Error,
                         const core::string<CHAR>& extra)
            {
                line = Line;
                column = Column;

                err = Error;

                extra_data = extra;
            }

            parser_error(int Line, int Column, error_flag Error)
            {
                line = Line;
                column = Column;

                err = Error;
            }

            parser_error(const parser_error& other)
            {
                line = other.line;
                column = other.column;

                err = other.err;

                extra_data = other.extra_data;
            }

            const parser_error& operator=(const parser_error& other)
            {
                line = other.line;
                column = other.column;

                err = other.err;

                extra_data = other.extra_data;

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
