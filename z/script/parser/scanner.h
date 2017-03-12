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
 * Last modified:   6 Mar. 2017
**/

#pragma once
#ifndef SCANNER_H_INCLUDED
#define SCANNER_H_INCLUDED

#include <z/core/sorted_array.h>
#include <z/core/string.h>

#include "escape_sequences.h"
#include "identity.h"

namespace z
{
    namespace script
    {

        template <typename CHAR>
        class scanner
        {
        private:
            core::sorted_array< core::string<CHAR> >* operators;
            core::sorted_array< core::string<CHAR> >* commands;
            core::sorted_array< core::string<CHAR> >* functions;

        public:
            core::array< ident_t<CHAR> > identifiers;

            //constructor allows operators, commands, and functions be set
            scanner(core::sorted_array< core::string<CHAR> >* opers = NULL,
                    core::sorted_array< core::string<CHAR> >* cmds = NULL,
                    core::sorted_array< core::string<CHAR> >* funcs = NULL)
            {
                operators = opers;
                commands = cmds;
                functions = funcs;
            }


            bool scan(const core::string<CHAR>&);
            bool clean();

            bool list_opers(const core::string<CHAR>&,
                            core::array< ident_t<CHAR> >&) const;

            bool check_for_keywords();
            bool check_for_numbers();
            bool check_for_operators();
            bool check_for_commands();
            bool check_for_functions();
        };


        //function to scan for and separate input into separate tokens.
        //returns false if an error was found while scanning.
        template <typename CHAR>
        bool scanner<CHAR>::scan(const core::string<CHAR>& input)
        {
            identifiers.clear();

            bool no_errors = true;

            bool in_string = false;
            bool in_comment = false;


            core::string<CHAR> NL = "\n";
            core::string<CHAR> CR = "\r";

            int line = 0;
            int column = 0;


            ident_t<CHAR> current_ident (ident::NONE, 0, 0);
            ident::ident_enum newIdent = ident::NONE;


            for (int i=0; i<input.length(); i++)
            {
                if (current_ident.err)
                  no_errors = false;

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

                            if (input[i] == (CHAR)92) //we have some unknown escape sequence
                                current_ident.err = error::UNKNOWN_ESCAPE;
                        }
                    }
                }
                else if (in_comment)
                {
                    if (input.foundAt("*/", i))
                    {
                        in_comment = false;
                        i+=2;
                    }
                }
                else
                {
                    if (input[i] == (CHAR)34)
                    {
                        newIdent = ident::STRING_LITERAL;
                        in_string = true;

                        if (current_ident.type)
                            identifiers.add(current_ident);

                        current_ident.name.clear();
                        current_ident.type = newIdent;

                        current_ident.line = line;
                        current_ident.column = column;

                        current_ident.err = error::NONE;
                    }
                    else if (input.foundAt("/*", i))
                    {
                        newIdent = ident::NONE;
                        in_comment = true;

                        if (current_ident.type)
                            identifiers.add(current_ident);

                        current_ident.name.clear();
                        current_ident.type = newIdent;

                        current_ident.line = line;
                        current_ident.column = column;

                        current_ident.err = error::NONE;

                        i++;
                    }
                }


                if (!in_string && !in_comment)
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
                        newIdent = ident::SEMICOLON;
                    }
                    //assignment
                    else if (input[i] == (CHAR)61)//'='
                    {
                        newIdent = ident::ASSIGNMENT;
                    }
                    else //possible operator
                    {
                        newIdent = ident::UNKNOWN;
                    }


                    ///If there is a change in the type
                    if (newIdent != current_ident.type)
                    {
                        if (current_ident.type == ident::UNKNOWN)
                            list_opers(current_ident.name, identifiers);
                        else if (current_ident.type)
                            identifiers.add(current_ident);

                        current_ident.name.clear();
                        current_ident.type = newIdent;

                        current_ident.line = line;
                        current_ident.column = column;

                        current_ident.err = error::NONE;

                        if (current_ident.type)
                            current_ident.name += input[i];

                        if ((current_ident.type >= ident::LPARENTH) &&
                            (current_ident.type <= ident::ASSIGNMENT))
                        {
                            identifiers.add(current_ident);
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


            if (current_ident.err)
                no_errors = false;

            if (current_ident.type == ident::UNKNOWN)
                list_opers(current_ident.name, identifiers);
            else if (current_ident.type)
                identifiers.add(current_ident);


            return no_errors;
        }


        //function to reassign previously scanned tokens with more specific IDs.
        //returns false if an error was found.
        template <typename CHAR>
        bool scanner<CHAR>::clean()
        {
            bool no_errors = true;

            no_errors &= check_for_keywords();
            no_errors &= check_for_numbers();

            if (operators)
                no_errors &= check_for_operators();

            if (commands)
                no_errors &= check_for_commands();

            if (functions)
                no_errors &= check_for_functions();

            return no_errors;
        }


        ///list all the possible operators in the string
        ///the string must contain ONLY operators and NO spaces
        //returns false if an error was found.
        template <typename CHAR>
        bool scanner<CHAR>::list_opers(const core::string<CHAR>& input,
                                       core::array< ident_t<CHAR> >& output) const
        {
            error_flag oper_error = error::NONE;

            core::array< core::string<CHAR> > temp_opers;


            int pos = 0;
            core::string<CHAR> curr_oper;

            while ((pos < input.length()) && !oper_error && operators)
            {
                bool found = false;

                for (int i=0; i<(operators->size()); i++)
                {
                    if (input.foundAt(operators->at(i), pos))
                    {
                        curr_oper = operators->at(i);
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
                        oper_error = error::AMBIGUOUS_EXPR;
                    else
                        oper_error = error::UNKNOWN_OPERATOR;
                }

                return (oper_error != error::NONE);
            }


            if ((oper_error == error::NONE) && operators)
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
            else
            {
                ident::ident_enum this_type;

                if (oper_error == error::AMBIGUOUS_EXPR)
                {
                    this_type = ident::OPERATOR;
                }
                else
                {
                    this_type = ident::UNKNOWN;
                }

                int line = output[output.size()-1].line;
                int column = output[output.size()-1].column;

                output.add(ident_t<CHAR>(input, this_type,
                                         line, column, oper_error));
            }

            return (oper_error == error::NONE);
        }


        ///If any identifiers match a keyword, change the type to the appropriate keyword.
        //does not produce any errors, so always returns true.
        template <typename CHAR>
        bool scanner<CHAR>::check_for_keywords()
        {
            for (int i=0; i<identifiers.size(); i++)
            {
                if (identifiers[i].type == ident::IDENTIFIER)
                {
                    if (identifiers[i].name == L"main")
                    {
                        identifiers[i].type = ident::KEYWORD_MAIN;
                    }

                    else if (identifiers[i].name == L"if")
                    {
                        identifiers[i].type = ident::KEYWORD_IF;
                    }
                    else if (identifiers[i].name == L"else")
                    {
                        identifiers[i].type = ident::KEYWORD_ELSE;
                    }

                    else if (identifiers[i].name == L"for")
                    {
                        identifiers[i].type = ident::KEYWORD_FOR;
                    }
                    else if (identifiers[i].name == L"do")
                    {
                        identifiers[i].type = ident::KEYWORD_DO;
                    }
                    else if (identifiers[i].name == L"loop")
                    {
                        identifiers[i].type = ident::KEYWORD_LOOP;
                    }
                    else if (identifiers[i].name == L"while")
                    {
                        identifiers[i].type = ident::KEYWORD_WHILE;
                    }

                    else if (identifiers[i].name == L"goto")
                    {
                        identifiers[i].type = ident::KEYWORD_GOTO;
                    }
                    else if (identifiers[i].name == L"gosub")
                    {
                        identifiers[i].type = ident::KEYWORD_GOSUB;
                    }

                    else if (identifiers[i].name == L"run")
                    {
                        identifiers[i].type = ident::KEYWORD_RUN;
                    }
                    else if (identifiers[i].name == L"include")
                    {
                        identifiers[i].type = ident::KEYWORD_INCLUDE;
                    }

                    else if (identifiers[i].name == L"break")
                    {
                        identifiers[i].type = ident::KEYWORD_BREAK;
                    }
                    else if (identifiers[i].name == L"return")
                    {
                        identifiers[i].type = ident::KEYWORD_RETURN;
                    }
                }
            }

            return true;
        }


        ///If any identifiers match a number form, change the type to the appropriate number.
        //returns false if an error was found.
        template <typename CHAR>
        bool scanner<CHAR>::check_for_numbers()
        {
            bool no_errors = true;

            for (int i=0; i<identifiers.size(); i++)
            {
                if (identifiers[i].type == ident::IDENTIFIER)
                {
                    bool isNumber = core::is_numeric(identifiers[i].name[0]);

                    if (isNumber)
                    {
                        if (identifiers[i].name.beginsWith("0b"))
                        {
                            identifiers[i].type = ident::BINARY_LITERAL;

                            //Error check for binary numbers
                            for (int e=2; e<identifiers[i].name.length(); e++)
                            {
                                CHAR _char = identifiers[i].name[e];

                                if ((_char != (CHAR)48) && //not 0 or 1
                                    (_char != (CHAR)49) &&
                                    (_char != (CHAR)46)) //not a decimal point
                                {
                                    identifiers[i].err = error::INVALID_IDENTIFIER;
                                    no_errors = false;
                                    break;
                                }
                            }
                        }
                        else if (identifiers[i].name.beginsWith("0c"))
                        {
                            identifiers[i].type = ident::OCTAL_LITERAL;

                            //Error check for octal numbers
                            for (int e=2; e<identifiers[i].name.length(); e++)
                            {
                                CHAR _char = identifiers[i].name[e];

                                if ((_char != (CHAR)46) && //not a decimal point
                                    ((_char < (CHAR)48) || //not from 0 to 7
                                     (_char > (CHAR)55)))
                                {
                                    identifiers[i].err = error::INVALID_IDENTIFIER;
                                    no_errors = false;
                                    break;
                                }
                            }
                        }
                        else if (identifiers[i].name.beginsWith("0x"))
                        {
                            identifiers[i].type = ident::HEXADEC_LITERAL;

                            //Error check for hexadecimal numbers
                            for (int e=2; e<identifiers[i].name.length(); e++)
                            {
                                CHAR _char = identifiers[i].name[e];

                                if ((_char != (CHAR)46) && //not a decimal point
                                    ((_char == (CHAR)95) || //'_'
                                     ((_char > (CHAR)102) && //'g' to 'z'
                                      (_char <= (CHAR)122)) ||
                                     ((_char > (CHAR)70) && //'G' to 'Z'
                                      (_char <= (CHAR)90))))
                                {
                                    identifiers[i].err = error::INVALID_IDENTIFIER;
                                    no_errors = false;
                                    break;
                                }
                            }
                        }
                        else
                        {
                            identifiers[i].type = ident::DECIMAL_LITERAL;

                            //Error check for decimal numbers
                            for (int e=0; e<identifiers[i].name.length(); e++)
                            {
                                if (core::is_alpha(identifiers[i].name[e]) || //any letter
                                    (identifiers[i].name[e] == (CHAR)95)) //'_'
                                {
                                    identifiers[i].err = error::INVALID_IDENTIFIER;
                                    no_errors = false;
                                    break;
                                }
                            }
                        }
                    }
                }
            }

            return no_errors;
        }

        ///Some operators may have alphanumeric characters in them.
        ///if any identifiers match an operator, change the type to OPERATOR.
        //does not produce any errors, so always returns true.
        template <typename CHAR>
        bool scanner<CHAR>::check_for_operators()
        {
            for (int i=0; i<identifiers.size(); i++)
            {
                if (identifiers[i].type == ident::IDENTIFIER)
                {
                    int location = operators->find(identifiers[i].name);

                    if (location > -1)
                    {
                        identifiers[i].type = ident::OPERATOR;
                    }
                }
            }

            return true;
        }


        ///If any identifiers match a command, change the type to COMMAND.
        //does not produce any errors, so always returns true.
        template <typename CHAR>
        bool scanner<CHAR>::check_for_commands()
        {
            for (int i=0; i<identifiers.size(); i++)
            {
                if (identifiers[i].type == ident::IDENTIFIER)
                {

                    int location = commands->find(identifiers[i].name);

                    if (location > -1)
                    {
                        identifiers[i].type = ident::COMMAND;
                    }
                }
            }

            return true;
        }


        ///If any identifiers match a function, change the type to FUNCTION.
        //does not produce any errors, so always returns true.
        template <typename CHAR>
        bool scanner<CHAR>::check_for_functions()
        {
            for (int i=0; i<identifiers.size(); i++)
            {
                if (identifiers[i].type == ident::IDENTIFIER)
                {
                    int location = functions->find(identifiers[i].name);

                    if (location > -1)
                    {
                        identifiers[i].type = ident::FUNCTION;
                    }
                }
            }

            return true;
        }
    }
}

#endif // SCANNER_H_INCLUDED
