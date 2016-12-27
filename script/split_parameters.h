#pragma once
#ifndef SPLIT_PARAMETERS_H_INCLUDED
#define SPLIT_PARAMETERS_H_INCLUDED

#include "../core/array.h"
#include "../core/string.h"
#include "../core/stringUtils/string_remove_whiteSpace.h"

#include "operators.h"
#include "escape_sequences.h"

#ifndef uint
    #define uint uint32_t
#endif // uint


namespace script
{

    template <typename CHAR>
    void split_parameters(const core::string<CHAR>& input,
                          core::array< core::string<CHAR> >& output)
    {
        core::string<CHAR> comma((CHAR)44);
        core::string<CHAR> quote((CHAR)34);
        core::string<CHAR> q_esc;
        esc_sequence_name(ESC_SEQUENCE::QUOTE, q_esc);

        core::string<CHAR> open_parenth;
        core::string<CHAR> close_parenth;

        operator_string(OPERATOR::OPEN_PARENTH, open_parenth);
        operator_string(OPERATOR::CLOSE_PARENTH, close_parenth);


        core::array<uint> comma_positions;

        //first, get comma positions,
        //not including commas inside parentheses,
        //or commas inside quotes.
        uint level = 0;
        bool in_quote = false;

        for (int i=0; i<input.length(); i++)
        {
            if ((what_operator(input, i) == OPERATOR::OPEN_PARENTH)
                && !in_quote)
            {
                level++;
            }
            else if ((what_operator(input, i) == OPERATOR::CLOSE_PARENTH)
                     && !in_quote)
            {
                if (level > 0)
                    level--;
            }
            else if (input.foundAt(comma, i) && (level == 0) && !in_quote)
            {
                comma_positions.append(i);
            }
            else if (what_esc_sequence(input, i) == ESC_SEQUENCE::QUOTE)
            {
                i += q_esc.length()-1;
            }
            else if (input.foundAt(quote, i))
            {
                in_quote = !in_quote;
            }
        }


        //now separate into strings using the given positions
        if (comma_positions.size() >= 1)
        {
            output.append(input.substr(0, comma_positions.at(0)-1));

            for (uint i=1; i<comma_positions.size(); i++)
            {
                output.append(input.substr(comma_positions.at(i-1)+1,
                                           comma_positions.at(i)-1));
            }

            output.append(input.substr(comma_positions.at(comma_positions.size()-1)+1,
                                       input.length()-1));
        }
        else
        {
            if (core::remove_whitespace(input).length() > 0)
                output.append(input);
        }
    }
}

#endif // SPLIT_PARAMETERS_H_INCLUDED
