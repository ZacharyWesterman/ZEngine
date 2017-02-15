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


            error_flag scan(const core::string<CHAR>&);
            error_flag clean();

            error_flag list_opers(const core::string<CHAR>&,
                                  core::array< ident_t<CHAR> >&) const;

            void check_for_operators();
        };


        template <typename CHAR>
        error_flag scanner<CHAR>::scan(const core::string<CHAR>& input)
        {
            arr.clear();

            error_flag split_errors = error::NONE;

            bool in_string = false;
            bool out_of_string = true;


            core::string<CHAR> current_string;
            ident::ident_enum current_type = ident::NONE;

            core::string<CHAR> esc_quote;
            esc_sequence_name(ESC_SEQUENCE::QUOTE, esc_quote);

            int X = 0;
            int Y = 0;

            int line = 0;
            int col = 0;

            for (int i=0; i<input.length(); i++)
            {
                if (input[i] == (CHAR)34)
                {
                    if (!in_string)
                    {
                        if (current_string.length() > 0)
                        {
                            arr.add(ident_t<CHAR>(current_string, current_type, line, col));
                            current_string.clear();

                            line = Y;
                            col = X;
                        }
                        in_string = true;
                        out_of_string = false;

                        current_type = ident::STRING_LITERAL;
                    }
                    else
                    {
                        out_of_string = true;
                        current_string += input[i];

                        arr.add(ident_t<CHAR>(current_string, current_type, line, col));
                        current_string.clear();

                        line = Y;
                        col = X;
                    }
                }


                if (!in_string)
                {
                    //split if we encounter a different type of object
                    if (core::is_white_space(input[i]))
                    {
                        if (current_string.length() > 0)
                        {
                            arr.add(ident_t<CHAR>(current_string, current_type, line, col));
                            current_string.clear();
                        }

                        line = Y;
                        col = X;
                    }
                    else if (input[i] == (CHAR)44) //commas are the default delimiters
                    {
                        if (current_string.length() > 0)
                        {
                            arr.add(ident_t<CHAR>(current_string, current_type, line, col));
                            current_string.clear();

                            line = Y;
                            col = X;
                        }

                        current_type = ident::DELIMITER;
                        arr.add(ident_t<CHAR>(input[i], current_type, line, col));

                        line = Y;
                        col = X;
                    }
                    else if ((input[i] == (CHAR)40) || //open parentheses
                             (input[i] == (CHAR)41))   //closed parentheses
                    {
                        if (current_string.length() > 0)
                        {
                            arr.add(ident_t<CHAR>(current_string, current_type, line, col));
                            current_string.clear();

                            line = Y;
                            col = X;
                        }

                        current_type = ident::PARENTH;
                        arr.add(ident_t<CHAR>(input[i], current_type, line, col));
                    }
                    else if ((input[i] == (CHAR)91) || //open bracket
                             (input[i] == (CHAR)93))   //closed bracket
                    {
                        if (current_string.length() > 0)
                        {
                            arr.add(ident_t<CHAR>(current_string, current_type, line, col));
                            current_string.clear();

                            line = Y;
                            col = X;
                        }

                        current_type = ident::ELEMENT;
                        arr.add(ident_t<CHAR>(input[i], current_type, line, col));

                        line = Y;
                        col = X;
                    }
                    else if ((input[i] == (CHAR)123) || //open curly brace
                             (input[i] == (CHAR)125))   //closed curly brace
                    {
                        if (current_string.length() > 0)
                        {
                            arr.add(ident_t<CHAR>(current_string, current_type, line, col));
                            current_string.clear();

                            line = Y;
                            col = X;
                        }

                        current_type = ident::BRACE;
                        arr.add(ident_t<CHAR>(input[i], current_type, line, col));

                        line = Y;
                        col = X;
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

                            line = Y;
                            col = X;
                        }

                        current_string += input[i];
                        current_type = ident::IDENTIFIER;
                    }
                    else //assume we have an operator
                    {
                        if ((current_type != ident::OPERATOR) &&
                            (current_string.length() > 0))
                        {
                            arr.add(ident_t<CHAR>(current_string, current_type, line, col));
                            current_string.clear();

                            line = Y;
                            col = X;
                        }


                        current_string += input[i];
                        current_type = ident::OPERATOR;
                    }
                }
                else
                {
                    if (!out_of_string)
                    {
                        if (input.foundAt(esc_quote, i))
                        {
                            current_string += esc_quote;
                            i += esc_quote.length() - 1;
                        }
                        else
                        {
                            current_string += input[i];
                        }
                    }
                    else
                    {
                        in_string = false;
                    }
                }



                X++;

                if (input[i] == (CHAR)10)
                {
                    if (input[i+1] == (CHAR)13)
                        i++;

                    Y++;
                    X = 0;
                }
                else if (input[i] == (CHAR)13)
                {
                    if (input[i+1] == (CHAR)10)
                        i++;

                    Y++;
                    X = 0;
                }
            }


            if (current_string.length() > 0)
            {
                if (current_type == ident::OPERATOR)
                    split_errors |= list_opers(current_string, arr);
                else
                    arr.add(ident_t<CHAR>(current_string, current_type, line, col));
            }


            return split_errors;
        }



        template <typename CHAR>
        error_flag scanner<CHAR>::clean()
        {
            check_for_operators();


            return error::NONE;
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
                        if (arr[i].str == opers->at(o)->str())
                            arr[i].type = ident::OPERATOR;
                    }
                }
            }
        }
    }
}

#endif // SCANNER_H_INCLUDED
