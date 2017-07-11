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
 * Last modified:   12 Jun. 2017
**/

#pragma once
#ifndef SCANNER_H_INCLUDED
#define SCANNER_H_INCLUDED

#include <z/core/sorted_array.h>
#include <z/core/string.h>

#include <z/core/timeout.h>
#include <z/core/dynamicStack.h>

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
            core::string<CHAR>* input;
            core::array< ident_t<CHAR> >* identifiers;

            core::dynamic_stack< ident_t<CHAR> > open_symbol_indices;

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

            core::sorted_ref_array< core::string<CHAR>* >* sym_table;

        public:
            //keep track of the current file
            int file;

            core::array< parser_error<CHAR> > error_buffer;

            //constructor allows operators, commands, and functions be set
            scanner(core::sorted_ref_array< core::string<CHAR>* >* symbol_table)
            {
                input = NULL;
                identifiers = NULL;

                sym_table = symbol_table;

                clear();

                file = -1;
            }

            inline void setInput(core::string<CHAR>& string_input)
            {
                input = &string_input;
            }

            inline void setOutput(core::array< ident_t<CHAR> >& ident_output)
            {
                identifiers = &ident_output;
            }

            bool scan(const core::timeout&);

            void clear()
            {
                current_symbol.clear();
                open_symbol_indices.dump();

                index = 0;

                in_string = false;
                in_comment = false;
                multiline_comment = false;

                line = 0;
                column = 0;

                current_ident = ident_t<CHAR>(ident::NONE, 0, 0);
                newIdent = ident::NONE;
                current_symbol.clear();

                error_buffer.clear();
                done = false;

                file = -1;
            }

            inline bool error()
            {
                return error_buffer.size() > 0;
            }

        private:
            bool list_opers(core::string<CHAR>&);

            void get_this_keyword();
            void get_this_operator();

            void check_this_number();

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
            if (done || !input || !identifiers)
                return true;


            while (!time.timedOut() && (index < input->length()))
            {
                if (in_string)
                {
                    if (input->at(index) == (CHAR)34)
                    {
                        in_string = false;
                        index++;
                    }
                    else
                    {
                        int esc_seq = (int)what_esc_sequence(*input, index);

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
                            current_symbol += input->at(index);

                            if (input->at(index) == (CHAR)92) //we have some unknown escape sequence
                            {
                                core::string<CHAR> bad_esc_str = (CHAR)92;
                                bad_esc_str += input->at(index+1);

                                error_buffer.add(
                                     parser_error<CHAR>(current_ident.line,
                                                        current_ident.column,
                                                        error::UNKNOWN_ESCAPE_SEQUENCE,
                                                        bad_esc_str, file));
                            }
                        }
                    }
                }

                if (in_comment)
                {
                    if (input->foundAt("}\\", index))
                    {
                        in_comment = false;
                        multiline_comment = false;
                        index+=2;
                    }
                }

                if (!in_string && !in_comment)
                {
                    if (input->at(index) == (CHAR)34)
                    {
                        newIdent = ident::STRING_LITERAL;
                        in_string = true;
                        in_comment = false;
                        multiline_comment = false;

                        if (current_ident.type)
                        {
                            bool addmeta = false;

                            if (current_ident.type == ident::IDENTIFIER)
                            {
                                get_this_keyword();
                                get_this_operator();
                                //get_this_function();
                                //get_this_command();

                                addmeta = (current_ident.type == ident::IDENTIFIER);
                            }
                            else if (current_ident.type == ident::NUMERIC_LITERAL)
                            {
                               check_this_number();
                            }
                            else if (current_ident.type == ident::STRING_LITERAL)
                            {
                                addmeta = true;
                            }

                            if (addmeta)
                                current_ident.meta = addToSymTable(&current_symbol);
                            identifiers->add(current_ident);
                        }

                        current_symbol.clear();
                        current_ident.type = newIdent;

                        current_ident.line = line;
                        current_ident.column = column;

                        current_ident.meta = NULL;

                        current_ident.file = file;
                    }
                    ///in some kind of comment
                    else if (input->foundAt("\\{", index) || //  multiline comment "\{"
                             input->foundAt("\\\\", index))  //single line comment "\\"
                    {
                        newIdent = ident::NONE;
                        in_comment = true;
                        in_string = false;

                        multiline_comment = input->foundAt("\\{",index);

                        if (current_ident.type)
                        {
                            bool addmeta = false;

                            if (current_ident.type == ident::IDENTIFIER)
                            {
                                get_this_keyword();
                                get_this_operator();
                                //get_this_function();
                                //get_this_command();

                                addmeta = (current_ident.type == ident::IDENTIFIER);
                            }
                            else if (current_ident.type == ident::NUMERIC_LITERAL)
                            {
                                check_this_number();
                            }
                            else if (current_ident.type == ident::STRING_LITERAL)
                            {
                                addmeta = true;
                            }

                            if (addmeta)
                                current_ident.meta = addToSymTable(&current_symbol);
                            identifiers->add(current_ident);
                        }

                        current_symbol.clear();
                        current_ident.type = newIdent;

                        current_ident.line = line;
                        current_ident.column = column;

                        current_ident.meta = NULL;

                        current_ident.file = file;

                        //index++;
                    }
                }


                if (!in_string && !in_comment)
                {
                    //white space
                    if (core::is_white_space(input->at(index)))
                    {
                        newIdent = ident::NONE;
                    }
                    //generic identifiers
                    else if (core::is_alphanumeric(input->at(index)) ||
                             (input->at(index) == (CHAR)95))
                    {
                        if ((newIdent != ident::NUMERIC_LITERAL) &&
                            (newIdent != ident::IDENTIFIER))
                        {
                            if (core::is_numeric(input->at(index)))
                                newIdent = ident::NUMERIC_LITERAL;
                            else
                                newIdent = ident::IDENTIFIER;
                        }
                    }
                    //period
                    else if (input->at(index) == (CHAR)46)
                    {
                        //if a decimal point precedes a number
                        //and no alphanumeric character directly precedes it,
                        //we can assume we have a number (e.g. ".10")
                        if (!newIdent && core::is_numeric(input->at(index+1)))
                            newIdent = ident::NUMERIC_LITERAL;
                        else if (newIdent != ident::NUMERIC_LITERAL)
                            newIdent = ident::PERIOD;
                    }
                    //parentheses
                    else if (input->at(index) == (CHAR)40)
                    {
                        newIdent = ident::LPARENTH;

                        open_symbol_indices.push(
                                ident_t<CHAR>(newIdent,
                                              line, column));
                    }
                    else if (input->at(index) == (CHAR)41)
                    {
                        newIdent = ident::RPARENTH;

                        ident_t<CHAR> op_sym;
                        if (!open_symbol_indices.pop(op_sym))
                        {
                            error_buffer.add(parser_error<CHAR>(line, column,
                                                          error::MISSING_L_PARENTH, file));
                        }
                        else if (op_sym.type == ident::LBRACKET)
                        {
                            error_buffer.add(parser_error<CHAR>(op_sym.line,
                                                          op_sym.column,
                                                          error::MISSING_R_BRACKET, file));
                        }
                        else if (op_sym.type == ident::LBRACE)
                        {
                            error_buffer.add(parser_error<CHAR>(op_sym.line,
                                                          op_sym.column,
                                                          error::MISSING_R_BRACE, file));
                        }
                    }
                    //brackets
                    else if (input->at(index) == (CHAR)91)
                    {
                        newIdent = ident::LBRACKET;

                        open_symbol_indices.push(
                                ident_t<CHAR>(newIdent,
                                              line, column));
                    }
                    else if (input->at(index) == (CHAR)93)
                    {
                        newIdent = ident::RBRACKET;

                        ident_t<CHAR> op_sym;
                        if (!open_symbol_indices.pop(op_sym))
                        {
                            error_buffer.add(parser_error<CHAR>(line, column,
                                                          error::MISSING_L_BRACKET, file));
                        }
                        else if (op_sym.type == ident::LPARENTH)
                        {
                            error_buffer.add(parser_error<CHAR>(op_sym.line,
                                                          op_sym.column,
                                                          error::MISSING_R_PARENTH, file));
                        }
                        else if (op_sym.type == ident::LBRACE)
                        {
                            error_buffer.add(parser_error<CHAR>(op_sym.line,
                                                          op_sym.column,
                                                          error::MISSING_R_BRACE, file));
                        }
                    }
                    //curly braces
                    else if (input->at(index) == (CHAR)123)
                    {
                        newIdent = ident::LBRACE;

                        open_symbol_indices.push(
                                ident_t<CHAR>(newIdent,
                                              line, column));
                    }
                    else if (input->at(index) == (CHAR)125)
                    {
                        newIdent = ident::RBRACE;

                        ident_t<CHAR> op_sym;
                        if (!open_symbol_indices.pop(op_sym))
                        {
                            error_buffer.add(parser_error<CHAR>(line, column,
                                                          error::MISSING_L_BRACE, file));
                        }
                        else if (op_sym.type == ident::LBRACKET)
                        {
                            error_buffer.add(parser_error<CHAR>(op_sym.line,
                                                          op_sym.column,
                                                          error::MISSING_R_BRACKET, file));
                        }
                        else if (op_sym.type == ident::LPARENTH)
                        {
                            error_buffer.add(parser_error<CHAR>(op_sym.line,
                                                          op_sym.column,
                                                          error::MISSING_R_PARENTH, file));
                        }
                    }
                    //comma
                    else if (input->at(index) == (CHAR)44)
                    {
                        newIdent = ident::COMMA;
                    }
                    //semicolon
                    else if (input->at(index) == (CHAR)59)
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
                            list_opers(current_symbol);
                        }
                        else if (current_ident.type)
                        {
                            bool addmeta = false;

                            if (current_ident.type == ident::IDENTIFIER)
                            {
                                get_this_keyword();
                                get_this_operator();
                                //get_this_function();
                                //get_this_command();

                                addmeta = (current_ident.type == ident::IDENTIFIER);
                            }
                            else if (current_ident.type == ident::NUMERIC_LITERAL)
                            {
                                check_this_number();
                            }
                            else if (current_ident.type == ident::STRING_LITERAL)
                            {
                                addmeta = true;
                            }

                            if (addmeta)
                                current_ident.meta = addToSymTable(&current_symbol);
                            identifiers->add(current_ident);
                        }

                        current_symbol.clear();
                        current_ident.type = newIdent;

                        current_ident.line = line;
                        current_ident.column = column;

                        current_ident.meta = NULL;

                        current_ident.file = file;

                        if (current_ident.type)
                            current_symbol += input->at(index);

                        if ((current_ident.type >= ident::LPARENTH) &&
                            (current_ident.type <= ident::PERIOD))
                        {
                            identifiers->add(current_ident);
                            current_symbol.clear();

                            current_ident.type = ident::NONE;
                            newIdent = ident::NONE;
                        }
                    }
                    else if (current_ident.type)
                    {
                        current_symbol += input->at(index);
                    }
                }

                //update current line and column
                if (input->foundAt(NL, index))
                {
                    if (input->foundAt(CR, index+1))
                        index++;

                    line++;
                    column = 0;

                    in_comment = multiline_comment;
                }
                else if (input->foundAt(CR, index))
                {
                    if (input->foundAt(NL, index+1))
                        index++;

                    line++;
                    column = 0;

                    in_comment = multiline_comment;
                }
                else
                {
                    column++;
                }


                index++;
            }


            done = (index >= input->length());


            if (done)
            {
                ident_t<CHAR> op_sym;
                while (open_symbol_indices.pop(op_sym))
                {
                    if (op_sym.type == ident::LPARENTH)
                    {
                        error_buffer.add(parser_error<CHAR>(op_sym.line,
                                                    op_sym.column,
                                                    error::MISSING_R_PARENTH, file));
                    }
                    else if (op_sym.type == ident::LBRACKET)
                    {
                        error_buffer.add(parser_error<CHAR>(op_sym.line,
                                                    op_sym.column,
                                                    error::MISSING_R_BRACKET, file));
                    }
                    else if (op_sym.type == ident::LBRACE)
                    {
                        error_buffer.add(parser_error<CHAR>(op_sym.line,
                                                    op_sym.column,
                                                    error::MISSING_R_BRACE, file));
                    }
                }


                if (current_ident.type == ident::UNKNOWN)
                {
                    list_opers(current_symbol);
                }
                else if (current_ident.type)
                {
                    bool addmeta = false;

                    if (current_ident.type == ident::IDENTIFIER)
                    {
                        get_this_keyword();
                        get_this_operator();
                        //get_this_function();
                        //get_this_command();

                        addmeta = (current_ident.type == ident::IDENTIFIER);
                    }
                    else if (current_ident.type == ident::NUMERIC_LITERAL)
                    {
                        check_this_number();
                    }
                    else if (current_ident.type == ident::STRING_LITERAL)
                    {
                        addmeta = true;
                    }

                    if (addmeta)
                        current_ident.meta = addToSymTable(&current_symbol);
                    identifiers->add(current_ident);
                }

                //for (int i=0; i<identifiers->size(); i++)
                    //cout << identifiers->at(i).type << endl;
            }


            return done;
        }



        ///list all the possible operators in the string
        ///the string must contain ONLY operators and NO spaces
        //returns false if an error was found.
        template <typename CHAR>
        bool scanner<CHAR>::list_opers(core::string<CHAR>& input)
        {
            error_flag oper_error = error::NONE;

            core::array< ident_t<CHAR> > temp_opers;


            ident::ident_enum curr_oper;

            int x_offset = 0;


            while ((x_offset < input.length()) && !oper_error)
            {
                bool found = true;
                int oper_length;

                if (input.foundAt("==", x_offset))
                {
                    curr_oper = ident::OPER_EQ;
                    oper_length = 2;
                }
                else if (input.foundAt("=", x_offset))
                {
                    curr_oper = ident::OPER_ASSIGN;
                    oper_length = 1;
                }
                else if (input.foundAt("<-", x_offset))
                {
                    curr_oper = ident::OPER_L_ARROW;
                    oper_length = 2;
                }
                else if (input.foundAt("<>", x_offset))
                {
                    curr_oper = ident::OPER_NOT_EQ;
                    oper_length = 2;
                }
                else if (input.foundAt("<=", x_offset))
                {
                    curr_oper = ident::OPER_LT_EQ;
                    oper_length = 2;
                }
                else if (input.foundAt("<", x_offset))
                {
                    curr_oper = ident::OPER_LT;
                    oper_length = 1;
                }
                else if (input.foundAt(">=", x_offset))
                {
                    curr_oper = ident::OPER_GT_EQ;
                    oper_length = 2;
                }
                else if (input.foundAt(">", x_offset))
                {
                    curr_oper = ident::OPER_GT;
                    oper_length = 1;
                }
                else if (input.foundAt("++", x_offset))
                {
                    curr_oper = ident::OPER_ADD1;
                    oper_length = 2;
                }
                else if (input.foundAt("+=", x_offset))
                {
                    curr_oper = ident::OPER_ADD_ASSIGN;
                    oper_length = 2;
                }
                else if (input.foundAt("+", x_offset))
                {
                    curr_oper = ident::OPER_ADD;
                    oper_length = 1;
                }
                else if (input.foundAt("->", x_offset))
                {
                    curr_oper = ident::OPER_R_ARROW;
                    oper_length = 2;
                }
                else if (input.foundAt("--", x_offset))
                {
                    curr_oper = ident::OPER_SUB1;
                    oper_length = 2;
                }
                else if (input.foundAt("-=", x_offset))
                {
                    curr_oper = ident::OPER_SUB_ASSIGN;
                    oper_length = 2;
                }
                else if (input.foundAt("-", x_offset))
                {
                    curr_oper = ident::OPER_SUB;
                    oper_length = 1;
                }
                else if (input.foundAt("*=", x_offset))
                {
                    curr_oper = ident::OPER_MUL_ASSIGN;
                    oper_length = 2;
                }
                else if (input.foundAt("*", x_offset))
                {
                    curr_oper = ident::OPER_MUL;
                    oper_length = 1;
                }
                else if (input.foundAt("//=", x_offset))
                {
                    curr_oper = ident::OPER_IDIV_ASSIGN;
                    oper_length = 3;
                }
                else if (input.foundAt("//", x_offset))
                {
                    curr_oper = ident::OPER_IDIV;
                    oper_length = 2;
                }
                else if (input.foundAt("/=", x_offset))
                {
                    curr_oper = ident::OPER_DIV_ASSIGN;
                    oper_length = 2;
                }
                else if (input.foundAt("/", x_offset))
                {
                    curr_oper = ident::OPER_DIV;
                    oper_length = 1;
                }
                else if (input.foundAt("%=", x_offset))
                {
                    curr_oper = ident::OPER_MOD_ASSIGN;
                    oper_length = 2;
                }
                else if (input.foundAt("%", x_offset))
                {
                    curr_oper = ident::OPER_MOD;
                    oper_length = 1;
                }
                else if (input.foundAt("^", x_offset))
                {
                    curr_oper = ident::OPER_POW;
                    oper_length = 1;
                }
                else if (input.foundAt("!", x_offset))
                {
                    curr_oper = ident::OPER_FAC;
                    oper_length = 1;
                }
                else if (input.foundAt("~&", x_offset))
                {
                    curr_oper = ident::OPER_NAND_BITW;
                    oper_length = 2;
                }
                else if (input.foundAt("~|", x_offset))
                {
                    curr_oper = ident::OPER_NOR_BITW;
                    oper_length = 2;
                }
                else if (input.foundAt("~:", x_offset))
                {
                    curr_oper = ident::OPER_NXOR_BITW;
                    oper_length = 2;
                }
                else if (input.foundAt("~", x_offset))
                {
                    curr_oper = ident::OPER_NOT_BITW;
                    oper_length = 1;
                }
                else if (input.foundAt("&", x_offset))
                {
                    curr_oper = ident::OPER_AND_BITW;
                    oper_length = 1;
                }
                else if (input.foundAt("|", x_offset))
                {
                    curr_oper = ident::OPER_OR_BITW;
                    oper_length = 1;
                }
                else if (input.foundAt(":", x_offset))
                {
                    curr_oper = ident::OPER_XOR_BITW;
                    oper_length = 1;
                }
                else
                {
                    found = false;
                }


                if (found)
                {
                    temp_opers.add(ident_t<CHAR>(curr_oper, line, column + x_offset,
                                                 null, file));

                    x_offset += oper_length;
                }
                else //that operator was not found
                {
                    if (x_offset > 0)
                        oper_error = error::AMBIGUOUS_EXPR;
                    else
                        oper_error = error::UNKNOWN_OPERATOR;
                }
            }


            if (oper_error == error::NONE)
            {
                for (int i=0; i<temp_opers.size(); i++)
                {
                    identifiers->add(temp_opers[i]);
                }
            }
            else
            {
                ident::ident_enum this_type;

                this_type = ident::UNKNOWN;


                identifiers->add(ident_t<CHAR>(this_type,
                                               line, column-input.length()));

                error_buffer.add(parser_error<CHAR>(line, column-input.length(),
                                              oper_error, input, file));
            }

            return (oper_error != error::NONE);
        }


        ///If the current symbol matches a keyword, change the type to the appropriate keyword.
        template <typename CHAR>
        void scanner<CHAR>::get_this_keyword()
        {
            if (current_ident.type == ident::IDENTIFIER)
            {
                if (current_symbol == "if")
                    current_ident.type = ident::KEYWORD_IF;
                else if (current_symbol == "else")
                    current_ident.type = ident::KEYWORD_ELSE;
                else if (current_symbol == "for")
                    current_ident.type = ident::KEYWORD_FOR;
                else if (current_symbol == "each")
                    current_ident.type = ident::KEYWORD_EACH;
                else if (current_symbol == "in")
                    current_ident.type = ident::KEYWORD_IN;
                else if (current_symbol == "loop")
                    current_ident.type = ident::KEYWORD_LOOP;
                else if (current_symbol == "while")
                    current_ident.type = ident::KEYWORD_WHILE;
                else if (current_symbol == "goto")
                    current_ident.type = ident::KEYWORD_GOTO;
                else if (current_symbol == "gosub")
                    current_ident.type = ident::KEYWORD_GOSUB;
                else if (current_symbol == "label")
                    current_ident.type = ident::KEYWORD_LABEL;
                else if (current_symbol == "sub")
                    current_ident.type = ident::KEYWORD_SUB;
                else if (current_symbol == "run")
                    current_ident.type = ident::KEYWORD_RUN;
                else if (current_symbol == "include")
                    current_ident.type = ident::KEYWORD_INCLUDE;
                else if (current_symbol == "break")
                    current_ident.type = ident::KEYWORD_BREAK;
                else if (current_symbol == "return")
                    current_ident.type = ident::KEYWORD_RETURN;
                else if (current_symbol == "dim")
                    current_ident.type = ident::KEYWORD_DIM;
                else if (current_symbol == "exit")
                    current_ident.type = ident::KEYWORD_EXIT;
                else if (current_symbol == "wait")
                    current_ident.type = ident::KEYWORD_WAIT;
                else if (current_symbol == "until")
                    current_ident.type = ident::KEYWORD_UNTIL;
                else if (current_symbol == "var")
                    current_ident.type = ident::KEYWORD_VAR;
                else if (current_symbol == "type")
                    current_ident.type = ident::KEYWORD_TYPE;
                else if (current_symbol == "function")
                    current_ident.type = ident::KEYWORD_FUNCTION;
                else if (current_symbol == "global")
                    current_ident.type = ident::KEYWORD_GLOBAL;
                else if (current_symbol == "external")
                    current_ident.type = ident::KEYWORD_EXTERNAL;
                else if (current_symbol == "shared")
                    current_ident.type = ident::KEYWORD_SHARED;
            }
        }


        ///If the current identifier matches a number form, check what form the number is in
        ///(e.g. decimal, binary, octal or hexadecimal) and convert to a number.
        //returns false if an error was found.
        template <typename CHAR>
        void scanner<CHAR>::check_this_number()
        {
            bool return_good = true;

            if (current_symbol.endsWith("i"))
            {
                current_ident.type = ident::COMPLEX_LITERAL;
                current_symbol.remove(current_symbol.length()-1, current_symbol.length());
            }

            bool found_decimal = false;

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
                        error_buffer.add(
                                parser_error<CHAR>(current_ident.line,
                                             current_ident.column,
                                             error::INVALID_NUMBER_BASE2,
                                             current_symbol, file));

                        return_good = false;
                    }
                    else if (_char == (CHAR)46)
                    {
                        if (found_decimal)
                        {
                            error_buffer.add(
                                    parser_error<CHAR>(current_ident.line,
                                                 current_ident.column,
                                                 error::NUMBER_EXCESS_DECIMALS,
                                                 current_symbol, file));

                            return_good = false;
                        }

                        found_decimal = true;
                    }
                }

                if (return_good)
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
                        error_buffer.add(
                                parser_error<CHAR>(current_ident.line,
                                             current_ident.column,
                                             error::INVALID_NUMBER_BASE8,
                                             current_symbol, file));

                        return_good = false;
                    }
                    else if (_char == (CHAR)46)
                    {
                        if (found_decimal)
                        {
                            error_buffer.add(
                                    parser_error<CHAR>(current_ident.line,
                                                 current_ident.column,
                                                 error::NUMBER_EXCESS_DECIMALS,
                                                 current_symbol, file));

                            return_good = false;
                        }

                        found_decimal = true;
                    }
                }

                if (return_good)
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
                        error_buffer.add(
                                parser_error<CHAR>(current_ident.line,
                                             current_ident.column,
                                             error::INVALID_NUMBER_BASE16,
                                             current_symbol, file));

                        return_good = false;
                    }
                    else if (_char == (CHAR)46)
                    {
                        if (found_decimal)
                        {
                            error_buffer.add(
                                    parser_error<CHAR>(current_ident.line,
                                                 current_ident.column,
                                                 error::NUMBER_EXCESS_DECIMALS,
                                                 current_symbol, file));

                            return_good = false;
                        }

                        found_decimal = true;
                    }
                }

                if (return_good)
                    current_ident.value = eval_hexadecimal_str(&current_symbol);
            }
            else
            {
                //Error check for decimal numbers
                for (int e=0; e<(current_symbol.length()); e++)
                {
                    CHAR _char = current_symbol.at(e);

                    if (core::is_alpha(_char) || //any letter
                        (_char == (CHAR)95)) //'_'
                    {
                        error_buffer.add(
                                parser_error<CHAR>(current_ident.line,
                                             current_ident.column,
                                             error::INVALID_NUMBER_BASE10,
                                             current_symbol, file));

                        return_good = false;
                    }
                    else if (_char == (CHAR)46)
                    {
                        if (found_decimal)
                        {
                            error_buffer.add(
                                    parser_error<CHAR>(current_ident.line,
                                                 current_ident.column,
                                                 error::NUMBER_EXCESS_DECIMALS,
                                                 current_symbol, file));

                            return_good = false;
                        }

                        found_decimal = true;
                    }
                }

                if(return_good)
                    current_ident.value = core::value(current_symbol);
            }
        }

        ///Some operators may have alphanumeric characters in them.
        ///if the current identifier matches an operator, change the type to OPERATOR.
        //does not produce any errors, so always returns true.
        template <typename CHAR>
        void scanner<CHAR>::get_this_operator()
        {
            if (current_ident.type == ident::IDENTIFIER)
            {
                if (current_symbol == "and")
                    current_ident.type = ident::OPER_AND_LGCL;
                else if (current_symbol == "or")
                    current_ident.type = ident::OPER_OR_LGCL;
                else if (current_symbol == "xor")
                    current_ident.type = ident::OPER_XOR_LGCL;
                else if (current_symbol == "nxor")
                    current_ident.type = ident::OPER_NXOR_LGCL;
                else if (current_symbol == "nand")
                    current_ident.type = ident::OPER_NAND_LGCL;
                else if (current_symbol == "nor")
                    current_ident.type = ident::OPER_NOR_LGCL;
                else if (current_symbol == "not")
                    current_ident.type = ident::OPER_NOT_LGCL;
                else if (current_symbol == "sizeof")
                    current_ident.type = ident::OPER_SIZEOF;
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
