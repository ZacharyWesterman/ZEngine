#pragma once
#ifndef PREPARSER_H_INCLUDED
#define PREPARSER_H_INCLUDED

#include "../core/array.h"
#include "../core/string.h"

#include "../core/stringUtils/char_is_whiteSpace.h"
#include "../core/stringUtils/char_is_alphanumeric.h"

#include "operators/oper_t.h"

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
            //if (core::is_alpha())



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
}

#endif // PREPARSER_H_INCLUDED
