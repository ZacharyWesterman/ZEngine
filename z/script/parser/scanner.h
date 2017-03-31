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

            core::sorted_ref_array< core::string<CHAR>* >* sym_table;

        public:
            core::array< ident_t<CHAR> > identifiers;

            //constructor allows operators, commands, and functions be set
            scanner(core::sorted_ref_array< core::string<CHAR>* >* symbol_table,
                    core::sorted_array< core::string<CHAR> >* opers = NULL,
                    core::sorted_array< core::string<CHAR> >* cmds = NULL,
                    core::sorted_array< core::string<CHAR> >* funcs = NULL)
            {
                sym_table = symbol_table;

                operators = opers;
                commands = cmds;
                functions = funcs;
            }


            bool scan(const core::string<CHAR>&);
            bool clean();

            bool list_opers(core::string<CHAR>&,
                            core::array< ident_t<CHAR> >&) const;

            bool check_for_keywords();
            bool check_for_numbers();
            bool check_for_operators();
            bool check_for_commands();
            bool check_for_functions();

            core::string<CHAR>* addToSymTable(core::string<CHAR>*) const;
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
            core::string<CHAR> current_symbol;

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

                            current_symbol += seq_equiv;

                            i += seq_name.length() - 1;
                            column += seq_name.length() - 1;
                        }
                        else
                        {
                            current_symbol += input[i];

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
                        {
                            current_ident.meta = addToSymTable(&current_symbol);
                            identifiers.add(current_ident);
                        }

                        current_symbol.clear();
                        current_ident.type = newIdent;

                        current_ident.line = line;
                        current_ident.column = column;

                        current_ident.err = error::NONE;

                        current_ident.meta = NULL;
                    }
                    else if (input.foundAt("/*", i))
                    {
                        newIdent = ident::NONE;
                        in_comment = true;

                        if (current_ident.type)
                        {
                            current_ident.meta = addToSymTable(&current_symbol);
                            identifiers.add(current_ident);
                        }

                        current_symbol.clear();
                        current_ident.type = newIdent;

                        current_ident.line = line;
                        current_ident.column = column;

                        current_ident.err = error::NONE;

                        current_ident.meta = NULL;

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
                             (input[i] == (CHAR)95))
                    {
                        if ((newIdent != ident::NUMERIC) &&
                            (newIdent != ident::IDENTIFIER))
                        {
                            if (core::is_numeric(input[i]))
                                newIdent = ident::NUMERIC;
                            else
                                newIdent = ident::IDENTIFIER;
                        }
                    }
                    //period
                    else if (input[i] == (CHAR)46)
                    {
                        if (newIdent != ident::NUMERIC)
                            newIdent = ident::PERIOD;
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
                    else //possible operator
                    {
                        newIdent = ident::UNKNOWN;
                    }


                    ///If there is a change in the type
                    if (newIdent != current_ident.type)
                    {
                        if (current_ident.type == ident::UNKNOWN)
                        {
                            list_opers(current_symbol, identifiers);
                        }
                        else if (current_ident.type)
                        {
                            current_ident.meta = addToSymTable(&current_symbol);
                            identifiers.add(current_ident);
                        }

                        current_symbol.clear();
                        current_ident.type = newIdent;

                        current_ident.line = line;
                        current_ident.column = column;

                        current_ident.err = error::NONE;
                        current_ident.meta = NULL;

                        if (current_ident.type)
                            current_symbol += input[i];

                        if ((current_ident.type >= ident::LPARENTH) &&
                            (current_ident.type <= ident::ASSIGNMENT))
                        {
                            identifiers.add(current_ident);
                            current_symbol.clear();

                            current_ident.type = ident::NONE;
                            newIdent = ident::NONE;
                        }
                    }
                    else if (current_ident.type)
                    {
                        current_symbol += input[i];
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
            {
                list_opers(current_symbol, identifiers);
            }
            else if (current_ident.type)
            {
                current_ident.meta = addToSymTable(&current_symbol);

                identifiers.add(current_ident);
            }


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
        bool scanner<CHAR>::list_opers(core::string<CHAR>& input,
                                       core::array< ident_t<CHAR> >& output) const
        {
            error_flag oper_error = error::NONE;

            core::array< ident_t<CHAR> > temp_opers;


            core::string<CHAR> curr_oper;

            int x_offset = 0;
            int line = output[output.size()-1].line;
            int column = output[output.size()-1].column;


            while ((x_offset < input.length()) && !oper_error && operators)
            {
                bool found = false;

                for (int i=0; i<(operators->size()); i++)
                {
                    if (input.foundAt(operators->at(i), x_offset))
                    {
                        curr_oper = operators->at(i);
                        found = true;
                    }
                }


                if (found)
                {
                    temp_opers.add(ident_t<CHAR>(ident::OPERATOR, line, column + x_offset,
                                             addToSymTable(&curr_oper)));

                    x_offset += curr_oper.length();
                }
                else //that operator was not found
                {
                    //assignment
                    if (input[x_offset] == (CHAR)61)//'='
                    {
                        temp_opers.add(ident_t<CHAR>(ident::ASSIGNMENT, line, column+x_offset, NULL));
                        x_offset++;
                    }
                    else if (x_offset > 0)
                        oper_error = error::AMBIGUOUS_EXPR;
                    else
                        oper_error = error::UNKNOWN_OPERATOR;
                }
            }


            if ((oper_error == error::NONE) && operators)
            {
                for (int i=0; i<temp_opers.size(); i++)
                {
                    output.add(temp_opers[i]);
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

                output.add(ident_t<CHAR>(this_type, line, column,
                                         addToSymTable(&input), oper_error));
            }

            return (oper_error != error::NONE);
        }


        ///If any identifiers match a keyword, change the type to the appropriate keyword.
        //does not produce any errors, so always returns true.
        template <typename CHAR>
        bool scanner<CHAR>::check_for_keywords()
        {
            for (int i=0; i<identifiers.size(); i++)
            {
                core::string<CHAR>* symbol = (core::string<CHAR>*)identifiers[i].meta;

                if ((identifiers[i].type == ident::IDENTIFIER) && symbol)
                {
                    if (*symbol == core::string<CHAR>(L"main"))
                    {
                        identifiers[i].type = ident::KEYWORD_MAIN;
                    }

                    else if (*symbol == L"if")
                    {
                        identifiers[i].type = ident::KEYWORD_IF;
                    }
                    else if (*symbol == L"else")
                    {
                        identifiers[i].type = ident::KEYWORD_ELSE;
                    }

                    else if (*symbol == L"for")
                    {
                        identifiers[i].type = ident::KEYWORD_FOR;
                    }
                    else if (*symbol == L"do")
                    {
                        identifiers[i].type = ident::KEYWORD_DO;
                    }
                    else if (*symbol == L"loop")
                    {
                        identifiers[i].type = ident::KEYWORD_LOOP;
                    }
                    else if (*symbol == L"while")
                    {
                        identifiers[i].type = ident::KEYWORD_WHILE;
                    }

                    else if (*symbol == L"goto")
                    {
                        identifiers[i].type = ident::KEYWORD_GOTO;
                    }
                    else if (*symbol == L"gosub")
                    {
                        identifiers[i].type = ident::KEYWORD_GOSUB;
                    }

                    else if (*symbol == L"run")
                    {
                        identifiers[i].type = ident::KEYWORD_RUN;
                    }
                    else if (*symbol == L"include")
                    {
                        identifiers[i].type = ident::KEYWORD_INCLUDE;
                    }

                    else if (*symbol == L"break")
                    {
                        identifiers[i].type = ident::KEYWORD_BREAK;
                    }
                    else if (*symbol == L"return")
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
                core::string<CHAR>* symbol = (core::string<CHAR>*)identifiers[i].meta;

                if ((identifiers[i].type == ident::NUMERIC) && symbol)
                {
                    bool isNumber = core::is_numeric(symbol->at(0));

                    if (isNumber)
                    {
                        if (symbol->beginsWith("0b"))
                        {
                            identifiers[i].type = ident::BINARY_LITERAL;

                            //Error check for binary numbers
                            for (int e=2; e<(symbol->length()); e++)
                            {
                                CHAR _char = symbol->at(e);

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
                        else if (symbol->beginsWith("0c"))
                        {
                            identifiers[i].type = ident::OCTAL_LITERAL;

                            //Error check for octal numbers
                            for (int e=2; e<(symbol->length()); e++)
                            {
                                CHAR _char = symbol->at(e);

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
                        else if (symbol->beginsWith("0x"))
                        {
                            identifiers[i].type = ident::HEXADEC_LITERAL;

                            //Error check for hexadecimal numbers
                            for (int e=2; e<(symbol->length()); e++)
                            {
                                CHAR _char = symbol->at(e);

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
                            for (int e=0; e<(symbol->length()); e++)
                            {
                                if (core::is_alpha(symbol->at(e)) || //any letter
                                    ((symbol->at(e)) == (CHAR)95)) //'_'
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
                core::string<CHAR>* symbol = (core::string<CHAR>*)identifiers[i].meta;

                if ((identifiers[i].type == ident::IDENTIFIER) && symbol)
                {
                    int location = operators->find(*symbol);

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
                core::string<CHAR>* symbol = (core::string<CHAR>*)identifiers[i].meta;

                if ((identifiers[i].type == ident::IDENTIFIER) && symbol)
                {

                    int location = commands->find(*symbol);

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
                core::string<CHAR>* symbol = (core::string<CHAR>*)identifiers[i].meta;

                if ((identifiers[i].type == ident::IDENTIFIER) && symbol)
                {
                    int location = functions->find(*symbol);

                    if (location > -1)
                    {
                        identifiers[i].type = ident::FUNCTION;
                    }
                }
            }

            return true;
        }



        template <typename CHAR>
        core::string<CHAR>* scanner<CHAR>::addToSymTable(core::string<CHAR>* symbol) const
        {
            if (sym_table)
            {
                int existing = sym_table->find(symbol);

                if (existing > -1)
                {
                    return sym_table->at(existing);
                }
                else
                {
                    core::string<CHAR>* new_sym = new core::string<CHAR>(*symbol);
                    sym_table->add(new_sym);
                    return new_sym;
                }
            }

            return NULL;
        }
    }
}

#endif // SCANNER_H_INCLUDED
