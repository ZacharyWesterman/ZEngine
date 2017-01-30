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


        void split(const core::string<CHAR>&);

        error_flag list_opers(const core::string<CHAR>&,
                              core::array< core::string<CHAR> >&) const;
    };


    template <typename CHAR>
    void preParser<CHAR>::split(const core::string<CHAR>& input)
    {
        arr.clear();

        ident_t<CHAR> this_object;

        this_object.type = ident::NONE;


        int beg = 0;
        int end = -1;

        bool in_string = false;


        ident::ident_enum new_type = this_object.type;

        for (int i=0; i<input.length(); i++)
        {
            if (input[i] == (CHAR)34)
            {
                ;
            }

            if (core::is_alphanumeric(input[i]) ||
                (input[i] == (CHAR)46))
            {
                ;
            }



            //split if we encounter a different type of object
            if (core::is_white_space(input[i]) ||
                (this_object.type != new_type))
            {
                //end of the string is before we encounter whitespace
                end = i - 1;

                if (end >= beg)
                {
                    this_object.str = input.substr(beg, end);
                    this_object.type = new_type;

                    arr.add(this_object);
                }

                //beginning of next string is after this whitespace
                beg = i + 1;
            }
        }

        //append any leftover characters in the input to the output
        if (input.length()-1 >= beg)
        {
            this_object.str = input.substr(beg, input.length()-1);
            this_object.type = new_type;

            arr.add(this_object);
        }
    }


    ///list all the possible operators in the string
    ///the string must contain ONLY operators and NO spaces
    template <typename CHAR>
    error_flag preParser<CHAR>::list_opers(const core::string<CHAR>& input,
                                           core::array< core::string<CHAR> >& output) const
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
                output.add(temp_opers[i]);
            }
        }

        return oper_error;
    }
}

#endif // PREPARSER_H_INCLUDED
