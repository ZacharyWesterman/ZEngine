/**
 * File:            scanner.h
 * Namespace:       z::script
 * Description:     Script scanner template. The scanner
 *                  separates individual parts of the script
 *                  and labels them based on what type of
 *                  element they are (e.g. identifier, operator,
 *                  literal value, etc.).
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   5 Feb. 2017
**/

#pragma once
#ifndef SCANNER_H_INCLUDED
#define SCANNER_H_INCLUDED

#include "../core/array.h"
#include "../core/string.h"

#include "escape_sequences.h"

#include "operators/oper_t.h"

#include "identity.h"

#include <iostream>
using namespace std;

namespace z
{
    namespace script
    {

        template <typename CHAR>
        class scanner
        {
        private:
            core::array< oper::oper_t<CHAR>* >* opers;

        public:
            core::array< ident_t<CHAR> > arr;

            //constructor REQUIRES all operators be set
            scanner(core::array< oper::oper_t<CHAR>* >& operators)
            {
                opers = &operators;
            }


            void scan(const core::string<CHAR>&);
            void clean();

            error_flag list_opers(const core::string<CHAR>&,
                                  core::array< ident_t<CHAR> >&) const;

            void check_for_operators();
        };


        template <typename CHAR>
        void scanner<CHAR>::scan(const core::string<CHAR>& input)
        {
            arr.clear();

            bool in_string = false;


            core::string<CHAR> NL = "\n";
            core::string<CHAR> CR = "\r";

            int line = 0;
            int column = 0;


            ident_t<CHAR> current_ident (ident::NONE, 0, 0);
            ident::ident_enum newIdent = ident::NONE;


            for (int i=0; i<input.length(); i++)
            {
                if (in_string)
                {
                    if (input[i] == (CHAR)34)
                    {
                        in_string = false;
                        i++;
                    }
                    else
                    {
                        int esc_seq = (int)what_esc_sequence(input, i);

                        if (esc_seq)
                        {
                            core::string<CHAR> seq_name;
                            core::string<CHAR> seq_equiv;

                            esc_sequence_name(esc_seq, seq_name);
                            esc_sequence_equiv(esc_seq, seq_equiv);

                            current_ident.name += seq_equiv;

                            i += seq_name.length() - 1;
                            column += seq_name.length() - 1;
                        }
                        else
                        {
                            current_ident.name += input[i];
                        }
                    }
                }
                else
                {
                    if (input[i] == (CHAR)34)
                    {
                        newIdent = ident::STRING_LITERAL;
                        in_string = true;

                        if (current_ident.type)
                            arr.add(current_ident);

                        current_ident.name.clear();
                        current_ident.type = newIdent;

                        current_ident.line = line;
                        current_ident.column = column;
                    }
                }


                if (!in_string)
                {
                    //white space
                    if (core::is_white_space(input[i]))
                    {
                        newIdent = ident::NONE;
                    }
                    //generic identifiers
                    else if (core::is_alphanumeric(input[i]) ||
                             (input[i] == (CHAR)46) ||
                             (input[i] == (CHAR)95))
                    {
                        newIdent = ident::IDENTIFIER;
                    }
                    //parentheses
                    else if (input[i] == (CHAR)40)
                    {
                        newIdent = ident::LPARENTH;
                    }
                    else if (input[i] == (CHAR)41)
                    {
                        newIdent = ident::RPARENTH;
                    }
                    //brackets
                    else if (input[i] == (CHAR)91)
                    {
                        newIdent = ident::LBRACKET;
                    }
                    else if (input[i] == (CHAR)93)
                    {
                        newIdent = ident::RBRACKET;
                    }
                    //curly braces
                    else if (input[i] == (CHAR)123)
                    {
                        newIdent = ident::LBRACE;
                    }
                    else if (input[i] == (CHAR)125)
                    {
                        newIdent = ident::RBRACE;
                    }
                    //comma
                    else if (input[i] == (CHAR)44)
                    {
                        newIdent = ident::LBRACE;
                    }
                    //semicolon
                    else if (input[i] == (CHAR)59)
                    {
                        newIdent = ident::RBRACE;
                    }
                    else
                    {
                        newIdent = ident::UNKNOWN;
                    }


                    ///If there is a change in the type
                    if (newIdent != current_ident.type)
                    {
                        if (current_ident.type)
                            arr.add(current_ident);

                        current_ident.name.clear();
                        current_ident.type = newIdent;

                        current_ident.line = line;
                        current_ident.column = column;

                        if (current_ident.type)
                            current_ident.name += input[i];

                        if ((current_ident.type >= ident::LPARENTH) &&
                            (current_ident.type <= ident::SEMICOLON))
                        {
                            arr.add(current_ident);
                            current_ident.name.clear();

                            current_ident.type = ident::NONE;
                            newIdent = ident::NONE;
                        }
                    }
                    else if (current_ident.type)
                    {
                        current_ident.name += input[i];
                    }
                }

                //update current line and column
                if (input.foundAt(NL, i))
                {
                    if (input.foundAt(CR, i+1))
                        i++;

                    line++;
                    column = 0;
                }
                else if (input.foundAt(CR, i))
                {
                    if (input.foundAt(NL, i+1))
                        i++;

                    line++;
                    column = 0;
                }
                else
                {
                    column++;
                }

            }


            if (current_ident.type)
            {
                arr.add(current_ident);
            }
        }



        template <typename CHAR>
        void scanner<CHAR>::clean()
        {
            check_for_operators();
        }


        ///list all the possible operators in the string
        ///the string must contain ONLY operators and NO spaces
        template <typename CHAR>
        error_flag scanner<CHAR>::list_opers(const core::string<CHAR>& input,
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
                int x_offset = 0;
                int line = output[output.size()-1].line;
                int column = output[output.size()-1].column;

                for (int i=0; i<temp_opers.size(); i++)
                {
                    output.add(ident_t<CHAR>(temp_opers[i], ident::OPERATOR,
                                             line, column + x_offset));

                    x_offset += temp_opers[i].length();
                }
            }

            return oper_error;
        }


        ///Some operators may have alphanumeric characters in them.
        ///if any identifiers match an operator, change the type to OPERATOR.
        template <typename CHAR>
        void scanner<CHAR>::check_for_operators()
        {
            for (int i=0; i<arr.size(); i++)
            {
                if (arr[i].type == ident::IDENTIFIER)
                {
                    for (int o=0; o<(opers->size()); o++)
                    {
                        if (arr[i].name == opers->at(o)->str())
                            arr[i].type = ident::OPERATOR;
                    }
                }
            }
        }
    }
}

#endif // SCANNER_H_INCLUDED
