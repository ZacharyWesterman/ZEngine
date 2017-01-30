#pragma once
#ifndef PREPARSER_H_INCLUDED
#define PREPARSER_H_INCLUDED

#include "../core/array.h"
#include "../core/string.h"

#include "../core/stringUtils/char_is_whiteSpace.h"
#include "../core/stringUtils/char_is_alphanumeric.h"

#include "escape_sequences.h"

#include "operators/oper_t.h"

#include <iostream>
using namespace std;

namespace script
{
    namespace ident
    {
        enum ident_enum
        {
            NONE = 0,

            IDENTIFIER,
            OPERATOR,

            LITERAL,

            VARIABLE,
            SUBVARIABLE,
            ARR_ELEMENT,

            FUNCTION,
            COMMAND,
            UNKNOWN
        };

    }

    template <typename CHAR>
    class ident_t
    {
    public:
        core::string<CHAR> str;
        ident::ident_enum type;

        ident_t (core::string<CHAR> s, ident::ident_enum t)
        {
            str = s;
            type = t;
        }

        bool operator==(const ident_t& other) const
        {
            return ((type == other.type) &&
                    (str == other.str));
        }
    };
    ///End of identifiers definition



    template <typename CHAR>
    class preParser
    {
    private:
        core::array< oper::oper_t<CHAR>* >* opers;

    public:
        core::array< ident_t<CHAR> > arr;

        //constructor REQUIRES all operators be set
        preParser(core::array< oper::oper_t<CHAR>* >& operators)
        {
            opers = &operators;
        }


        error_flag split(const core::string<CHAR>&);

        error_flag list_opers(const core::string<CHAR>&,
                              core::array< ident_t<CHAR> >&) const;
    };


    template <typename CHAR>
    error_flag preParser<CHAR>::split(const core::string<CHAR>& input)
    {
        arr.clear();

        error_flag split_errors = error::NONE;

        bool in_string = false;


        core::string<CHAR> current_string;
        ident::ident_enum current_type = ident::NONE;

        for (int i=0; i<input.length(); i++)
        {
            if (input[i] == (CHAR)34)
            {
                ;
            }


            //split if we encounter a different type of object
            if (core::is_white_space(input[i]))
            {
                if (current_string.length() > 0)
                {
                    arr.add(ident_t<CHAR>(current_string, current_type));
                    current_string.clear();
                }
            }
            else if (core::is_alphanumeric(input[i]) || //alphanumeric
                    (input[i] == (CHAR)46) || //or decimal
                    (input[i] == (CHAR)95)) //or underscore
            {
                if ((current_type != ident::IDENTIFIER) &&
                    (current_string.length() > 0))
                {
                    split_errors |= list_opers(current_string, arr);
                    current_string.clear();
                }

                current_string += input[i];
                current_type = ident::IDENTIFIER;
            }
            else
            {
                if ((current_type != ident::OPERATOR) &&
                    (current_string.length() > 0))
                {
                    arr.add(ident_t<CHAR>(current_string, current_type));
                    current_string.clear();
                }


                current_string += input[i];
                current_type = ident::OPERATOR;
            }
        }


        if (current_string.length() > 0)
        {
            if (current_type == ident::OPERATOR)
                split_errors |= list_opers(current_string, arr);
            else
                arr.add(ident_t<CHAR>(current_string, current_type));
        }

        return split_errors;
    }


    ///list all the possible operators in the string
    ///the string must contain ONLY operators and NO spaces
    template <typename CHAR>
    error_flag preParser<CHAR>::list_opers(const core::string<CHAR>& input,
                                           core::array< ident_t<CHAR> >& output) const
    {
        error_flag oper_error = error::NONE;

        core::array< core::string<CHAR> > temp_opers;


        int pos = 0;
        core::string<CHAR> curr_oper;

        while ((pos < input.length()) && !oper_error)
        {
            bool found = false;

            for (int i=0; i<(opers->size()); i++)
            {
                if (input.foundAt(opers->at(i)->str(), pos))
                {
                    curr_oper = opers->at(i)->str();
                    found = true;
                }
            }


            if (found)
            {
                temp_opers.add(curr_oper);

                pos += curr_oper.length();
            }
            else //that operator was not found
            {
                if (pos > 0)
                    oper_error |= error::AMBIGUOUS_EXPR;
                else
                    oper_error |= error::UNKNOWN_OPERATOR;
            }
        }


        if (oper_error == error::NONE)
        {
            for (int i=0; i<temp_opers.size(); i++)
            {
                output.add(ident_t<CHAR>(temp_opers[i], ident::OPERATOR));
            }
        }

        return oper_error;
    }
}

#endif // PREPARSER_H_INCLUDED
