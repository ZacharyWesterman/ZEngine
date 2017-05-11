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

#include <z/core/timeout.h>

#include "escape_sequences.h"
#include "identity.h"

#define NL '\n'
#define CR '\r'

namespace z
{
    namespace script
    {

        template <typename CHAR>
        class scanner
        {
        private:
            core::string<CHAR> input;
            int index;

            bool in_string;

            bool in_comment;
            bool multiline_comment;

            int line;
            int column;

            ident_t<CHAR> current_ident;
            ident::ident_enum newIdent;
            core::string<CHAR> current_symbol;

            bool done;

            bool found_error;


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

                done = true;
                found_error = false;
            }

            void setInput(const core::string<CHAR>& inputString)
            {
                input = inputString;
                index = 0;

                identifiers.clear();

                in_string = false;

                in_comment = false;
                multiline_comment = false;

                line = 0;
                column = 0;

                current_ident = ident_t<CHAR>(ident::NONE, 0, 0);
                newIdent = ident::NONE;
                current_symbol.clear();

                done = false;
                found_error = false;
            }

            bool scan(const core::timeout&);

            void clear()
            {
                input.clear();
                identifiers.clear();
                current_symbol.clear();

                found_error = false;
                done = true;
            }

            inline bool error()
            {
                return found_error;
            }

        private:
            bool list_opers(core::string<CHAR>&,
                            core::array< ident_t<CHAR> >&);

            void get_this_keyword();
            void get_this_operator();
            void get_this_function();
            void get_this_command();

            bool check_this_number();

            core::string<CHAR>* addToSymTable(core::string<CHAR>*) const;


            double eval_binary_str(const core::string<CHAR>*) const;
            double eval_octal_str(const core::string<CHAR>*) const;
            double eval_hexadecimal_str(const core::string<CHAR>*) const;
        };


        //function to scan for and separate input into separate tokens.
        //returns on timeout (does not mean it is finished scanning).
        //returns 1 if finished scanning,
        //returns 0 otherwise.
        template <typename CHAR>
        bool scanner<CHAR>::scan(const core::timeout& time)
        {
            if (done)
                return true;

            bool no_errors = true;


            while (!time.timedOut() && (index < input.length()))
            {
                if (current_ident.err)
                  no_errors = false;

                if (in_string)
                {
                    if (input[index] == (CHAR)34)
                    {
                        in_string = false;
                        index++;
                    }
                    else
                    {
                        int esc_seq = (int)what_esc_sequence(input, index);

                        if (esc_seq)
                        {
                            core::string<CHAR> seq_name;
                            core::string<CHAR> seq_equiv;

                            esc_sequence_name(esc_seq, seq_name);
                            esc_sequence_equiv(esc_seq, seq_equiv);

                            current_symbol += seq_equiv;

                            index += seq_name.length() - 1;
                            column += seq_name.length() - 1;
                        }
                        else
                        {
                            current_symbol += input[index];

                            if (input[index] == (CHAR)92) //we have some unknown escape sequence
                                current_ident.err = error::UNKNOWN_ESCAPE;
                        }
                    }
                }
                else if (in_comment)
                {
                    if (input.foundAt("*\\", index))
                    {
                        in_comment = false;
                        index+=2;
                    }
                }
                else
                {
                    if (input[index] == (CHAR)34)
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
                    ///in some kind of comment
                    else if ((input[index] == (CHAR)92) &&
                             ((input[index+1] == (CHAR)42) || //  multiline comment "\*"
                              (input[index+1] == (CHAR)92)))  //single line comment "\\"
                    {
                        newIdent = ident::NONE;
                        in_comment = true;

                        multiline_comment = (input[index+1] == (CHAR)42);

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

                        index++;
                    }
                }


                if (!in_string && !in_comment)
                {
                    //white space
                    if (core::is_white_space(input[index]))
                    {
                        newIdent = ident::NONE;
                    }
                    //generic identifiers
                    else if (core::is_alphanumeric(input[index]) ||
                             (input[index] == (CHAR)95))
                    {
                        if ((newIdent != ident::NUMERIC_LITERAL) &&
                            (newIdent != ident::IDENTIFIER))
                        {
                            if (core::is_numeric(input[index]))
                                newIdent = ident::NUMERIC_LITERAL;
                            else
                                newIdent = ident::IDENTIFIER;
                        }
                    }
                    //period
                    else if (input[index] == (CHAR)46)
                    {
                        //if a decimal point precedes a number
                        //and no alphanumeric character directly precedes it,
                        //we can assume we have a number (e.g. ".10")
                        if (!newIdent && core::is_numeric(input[index+1]))
                            newIdent = ident::NUMERIC_LITERAL;
                        else if (newIdent != ident::NUMERIC_LITERAL)
                            newIdent = ident::PERIOD;
                    }
                    //parentheses
                    else if (input[index] == (CHAR)40)
                    {
                        newIdent = ident::LPARENTH;
                    }
                    else if (input[index] == (CHAR)41)
                    {
                        newIdent = ident::RPARENTH;
                    }
                    //brackets
                    else if (input[index] == (CHAR)91)
                    {
                        newIdent = ident::LBRACKET;
                    }
                    else if (input[index] == (CHAR)93)
                    {
                        newIdent = ident::RBRACKET;
                    }
                    //curly braces
                    else if (input[index] == (CHAR)123)
                    {
                        newIdent = ident::LBRACE;
                    }
                    else if (input[index] == (CHAR)125)
                    {
                        newIdent = ident::RBRACE;
                    }
                    //comma
                    else if (input[index] == (CHAR)44)
                    {
                        newIdent = ident::LBRACE;
                    }
                    //semicolon
                    else if (input[index] == (CHAR)59)
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
                            bool addmeta = false;

                            if (current_ident.type == ident::IDENTIFIER)
                            {
                                get_this_keyword();
                                get_this_operator();
                                get_this_function();
                                get_this_command();

                                addmeta = (current_ident.type == ident::IDENTIFIER) ||
                                          (current_ident.type == ident::OPERATOR) ||
                                          (current_ident.type == ident::FUNCTION) ||
                                          (current_ident.type == ident::COMMAND);
                            }
                            else if (current_ident.type == ident::NUMERIC_LITERAL)
                            {
                                addmeta = !check_this_number();

                                if (addmeta)
                                    current_ident.err = error::INVALID_IDENTIFIER;
                            }

                            if (addmeta)
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
                            current_symbol += input[index];

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
                        current_symbol += input[index];
                    }
                }

                //update current line and column
                if (input.foundAt(NL, index))
                {
                    if (input.foundAt(CR, index+1))
                        index++;

                    line++;
                    column = 0;

                    if (in_comment && !multiline_comment)
                        in_comment = false;
                }
                else if (input.foundAt(CR, index))
                {
                    if (input.foundAt(NL, index+1))
                        index++;

                    line++;
                    column = 0;

                    if (in_comment && !multiline_comment)
                        in_comment = false;
                }
                else
                {
                    column++;
                }


                index++;
            }


            done = (index >= input.length());


            found_error = !no_errors;

            if (done)
            {
                if (current_ident.type == ident::UNKNOWN)
                {
                    list_opers(current_symbol, identifiers);
                }
                else if (current_ident.type)
                {
                    bool addmeta = false;

                    if (current_ident.type == ident::IDENTIFIER)
                    {
                        get_this_keyword();
                        get_this_operator();
                        get_this_function();
                        get_this_command();

                        addmeta = (current_ident.type == ident::IDENTIFIER) ||
                                  (current_ident.type == ident::OPERATOR) ||
                                  (current_ident.type == ident::FUNCTION) ||
                                  (current_ident.type == ident::COMMAND);
                    }
                    else if (current_ident.type == ident::NUMERIC_LITERAL)
                    {
                        addmeta = !check_this_number();

                        if (addmeta)
                            current_ident.err = error::INVALID_IDENTIFIER;
                    }

                    if (addmeta)
                        current_ident.meta = addToSymTable(&current_symbol);
                    identifiers.add(current_ident);
                }
            }


            return done;
        }



        ///list all the possible operators in the string
        ///the string must contain ONLY operators and NO spaces
        //returns false if an error was found.
        template <typename CHAR>
        bool scanner<CHAR>::list_opers(core::string<CHAR>& input,
                                       core::array< ident_t<CHAR> >& output)
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


        ///If the current symbol matches a keyword, change the type to the appropriate keyword.
        template <typename CHAR>
        void scanner<CHAR>::get_this_keyword()
        {
            if (current_ident.type == ident::IDENTIFIER)
            {
                if (current_symbol == "main")
                    current_ident.type = ident::KEYWORD_MAIN;
                else if (current_symbol == "if")
                    current_ident.type = ident::KEYWORD_IF;
                else if (current_symbol == "else")
                    current_ident.type = ident::KEYWORD_ELSE;
                else if (current_symbol == "for")
                    current_ident.type = ident::KEYWORD_FOR;
                else if (current_symbol == "do")
                    current_ident.type = ident::KEYWORD_DO;
                else if (current_symbol == "loop")
                    current_ident.type = ident::KEYWORD_LOOP;
                else if (current_symbol == "while")
                    current_ident.type = ident::KEYWORD_WHILE;
                else if (current_symbol == "goto")
                    current_ident.type = ident::KEYWORD_GOTO;
                else if (current_symbol == "gosub")
                    current_ident.type = ident::KEYWORD_GOSUB;
                else if (current_symbol == "run")
                    current_ident.type = ident::KEYWORD_RUN;
                else if (current_symbol == "include")
                    current_ident.type = ident::KEYWORD_INCLUDE;
                else if (current_symbol == "break")
                    current_ident.type = ident::KEYWORD_BREAK;
                else if (current_symbol == "return")
                    current_ident.type = ident::KEYWORD_RETURN;
                else if (current_symbol == "exit")
                    current_ident.type = ident::KEYWORD_EXIT;
                else if (current_symbol == "wait")
                    current_ident.type = ident::KEYWORD_WAIT;
                else if (current_symbol == "type")
                    current_ident.type = ident::KEYWORD_TYPE;
                else if (current_symbol == "global")
                    current_ident.type = ident::KEYWORD_GLOBAL;
                else if (current_symbol == "external")
                    current_ident.type = ident::KEYWORD_EXTERNAL;
            }
        }


        ///If the current identifier matches a number form, check what form the number is in
        ///(e.g. decimal, binary, octal or hexadecimal) and convert to a number.
        //returns false if an error was found.
        template <typename CHAR>
        bool scanner<CHAR>::check_this_number()
        {
            if (current_symbol.beginsWith("0b"))
            {
                //Error check for binary numbers
                for (int e=2; e<(current_symbol.length()); e++)
                {
                    CHAR _char = current_symbol.at(e);

                    if ((_char != (CHAR)48) && //not 0 or 1
                        (_char != (CHAR)49) &&
                        (_char != (CHAR)46)) //not a decimal point
                    {
                        return false;
                    }
                }

                current_ident.value = eval_binary_str(&current_symbol);
            }
            else if (current_symbol.beginsWith("0c") ||
                     current_symbol.beginsWith("0o"))
            {
                //Error check for octal numbers
                for (int e=2; e<(current_symbol.length()); e++)
                {
                    CHAR _char = current_symbol.at(e);

                    if ((_char != (CHAR)46) && //not a decimal point
                        ((_char < (CHAR)48) || //not from 0 to 7
                         (_char > (CHAR)55)))
                    {
                        return false;
                    }
                }

                current_ident.value = eval_octal_str(&current_symbol);
            }
            else if (current_symbol.beginsWith("0h") ||
                     current_symbol.beginsWith("0x"))
            {
                //Error check for hexadecimal numbers
                for (int e=2; e<(current_symbol.length()); e++)
                {
                    CHAR _char = current_symbol.at(e);

                    if ((_char != (CHAR)46) && //not a decimal point
                        ((_char == (CHAR)95) || //'_'
                         ((_char > (CHAR)102) && //'g' to 'z'
                          (_char <= (CHAR)122)) ||
                         ((_char > (CHAR)70) && //'G' to 'Z'
                          (_char <= (CHAR)90))))
                    {
                        return false;
                    }
                }

                current_ident.value = eval_hexadecimal_str(&current_symbol);
            }
            else
            {
                //Error check for decimal numbers
                for (int e=0; e<(current_symbol.length()); e++)
                {
                    if (core::is_alpha(current_symbol.at(e)) || //any letter
                        ((current_symbol.at(e)) == (CHAR)95)) //'_'
                    {
                        return false;
                    }
                }

                current_ident.value = core::value(current_symbol);
            }

            return true;
        }

        ///Some operators may have alphanumeric characters in them.
        ///if the current identifier matches an operator, change the type to OPERATOR.
        //does not produce any errors, so always returns true.
        template <typename CHAR>
        void scanner<CHAR>::get_this_operator()
        {
            if (operators && (current_ident.type == ident::IDENTIFIER))
            {
                if (operators->find(current_symbol) > -1)
                    current_ident.type = ident::OPERATOR;
            }
        }


        ///If any identifiers match a command, change the type to COMMAND.
        //does not produce any errors, so always returns true.
        template <typename CHAR>
        void scanner<CHAR>::get_this_command()
        {
            if (commands && (current_ident.type == ident::IDENTIFIER))
            {
                if (commands->find(current_symbol) > -1)
                    current_ident.type = ident::COMMAND;
            }
        }


        ///If any identifiers match a function, change the type to FUNCTION.
        //does not produce any errors, so always returns true.
        template <typename CHAR>
        void scanner<CHAR>::get_this_function()
        {
            if (functions && (current_ident.type == ident::IDENTIFIER))
            {
                if (functions->find(current_symbol) > -1)
                    current_ident.type = ident::FUNCTION;
            }
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



        ///function to evaluate binary strings
        //assumes strings begin with "0b"
        template <typename CHAR>
        double scanner<CHAR>::eval_binary_str(const core::string<CHAR>* input) const
        {
            double result = 0;

            bool reached_decimal = false;

            double frac_mul = 1;

            for (int i=2; i<(input->length()); i++)
            {
                if (input->at(i) == (CHAR)46) // '.'
                {
                    reached_decimal = true;
                }
                else
                {
                    if (!reached_decimal)
                    {
                        result *= 2;

                        result += (input->at(i) - 48);
                    }
                    else
                    {
                        frac_mul /= 2;

                        result += frac_mul * (input->at(i) - 48);
                    }

                }
            }

            return result;
        }

        ///function to evaluate octal strings
        //assumes strings begin with "0c"
        template <typename CHAR>
        double scanner<CHAR>::eval_octal_str(const core::string<CHAR>* input) const
        {
            double result = 0;

            bool reached_decimal = false;

            double frac_mul = 1;

            for (int i=2; i<(input->length()); i++)
            {
                if (input->at(i) == (CHAR)46) // '.'
                {
                    reached_decimal = true;
                }
                else
                {
                    if (!reached_decimal)
                    {
                        result *= 8;

                        result += (input->at(i) - 48);
                    }
                    else
                    {
                        frac_mul /= 8;

                        result += frac_mul * (input->at(i) - 48);
                    }
                }
            }

            return result;
        }

        ///function to evaluate hexadecimal strings
        //assumes strings begin with "0h"
        template <typename CHAR>
        double scanner<CHAR>::eval_hexadecimal_str(const core::string<CHAR>* input) const
        {
            double result = 0;

            bool reached_decimal = false;

            double frac_mul = 1;

            for (int i=2; i<(input->length()); i++)
            {
                CHAR this_char = input->at(i);

                if (this_char == (CHAR)46) // '.'
                {
                    reached_decimal = true;
                }
                else
                {
                    double char_val;

                    if (core::is_upper_alpha(this_char))
                        char_val = this_char - 65 + 10;
                    else if (core::is_lower_alpha(this_char))
                        char_val = this_char - 97 + 10;
                    else
                        char_val = this_char - 48;


                    if (!reached_decimal)
                    {
                        result *= 16;

                        result += char_val;
                    }
                    else
                    {
                        frac_mul /= 16;

                        result += frac_mul * char_val;
                    }
                }
            }

            return result;
        }
    }
}


#undef NL
#undef CR


#endif // SCANNER_H_INCLUDED