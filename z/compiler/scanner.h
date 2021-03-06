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
 * Last modified:   8 Aug. 2017
**/

#pragma once
#ifndef SCANNER_H_INCLUDED
#define SCANNER_H_INCLUDED

#include <limits>

#include <z/core/sortedArray.h>
#include <z/core/string.h>

#include <z/core/timeout.h>
#include <z/core/dynamicStack.h>

#include "escapeSequences.h"
#include "identity.h"
#include "keyword.h"
#include "operator.h"

namespace z
{
namespace script
{
    namespace compiler
    {


        class scanner
        {
        private:
            core::string<CPL_CHAR>* input;
            core::array< ident_t >* identifiers;

            core::dynamicStack< ident_t > open_symbol_indices;

            int index;

            bool in_string;

            bool in_comment;
            bool multiline_comment;

            int line;
            int column;
            int indent;

            ident_t current_ident;
            ident newIdent;
            core::string<CPL_CHAR> current_symbol;

            bool done;

            core::sortedRefArray< core::string<CPL_CHAR>* >* sym_table;

            core::array<keyword>* keywords;
            core::array<oper>* operators;

            core::array< core::string<char> >* comment_rules;
            int open_comment;

            CPL_CHAR open_string;

            //keep track of the current file
            core::string<char>* file;

        public:
            core::array< error > error_buffer;


            scanner(core::sortedRefArray< core::string<CPL_CHAR>* >*,
                     core::array<keyword>*,
                     core::array<oper>*,
                     core::array< core::string<char> >*,
                     core::string<char>*,
                     core::array< ident_t >*);

            void set(core::sortedRefArray< core::string<CPL_CHAR>* >*,
                     core::array<keyword>*,
                     core::array<oper>*,
                     core::array< core::string<char> >*,
                     core::string<char>*,
                     core::array< ident_t >*);

            inline void linkInput(core::string<CPL_CHAR>*);

            bool scan(const core::timeout& time = -1);

            void clear();
            inline bool good();
            inline bool bad();

        private:
            bool list_opers(core::string<CPL_CHAR>&);

            void get_this_keyword();
            void get_this_operator();

            void check_this_number();

            core::string<CPL_CHAR>* addToSymTable(const core::string<CPL_CHAR>&) const;


            void check_open_symbols();

            void symbol_type_change();
            void update_line_column();

            void behav_identifier();
            void behav_period();

            void behav_lparenth();
            void behav_rparenth();

            void behav_lbracket();
            void behav_rbracket();

            void behav_lbrace();
            void behav_rbrace();

            void behav_in_string();
        };




        //constructor allows operators, commands, and functions be set
        scanner::scanner(core::sortedRefArray<core::string<CPL_CHAR>* >*
                                                            symbol_table,
                               core::array<keyword>* Keywords,
                               core::array<oper>* Operators,
                               core::array< core::string<char> >* commentRules,
                               core::string<char>* File,
                               core::array< ident_t >* Output)
        {
            clear();

            set(symbol_table,
                Keywords,
                Operators,
                commentRules,
                File,
                Output);
        }


        void scanner::set(core::sortedRefArray<core::string<CPL_CHAR>* >*
                                                            symbol_table,
                                core::array<keyword>* Keywords,
                                core::array<oper>* Operators,
                                core::array< core::string<char> >* commentRules,
                                core::string<char>* File,
                                core::array< ident_t >* Output)
        {
            sym_table = symbol_table;
            keywords = Keywords;
            operators = Operators;
            file = File;
            identifiers = Output;
            comment_rules = commentRules;

            done = true;
        }


        inline void scanner::linkInput(core::string<CPL_CHAR>* string_input)
        {
            input = string_input;

            clear();
        }


        void scanner::clear()
        {
            current_symbol.clear();
            open_symbol_indices.dump();

            index = 0;

            in_string = false;
            in_comment = false;
            multiline_comment = false;

            line = 0;
            column = 0;
            indent = 0;

            current_ident = ident_t(ident::NONE, 0, 0);
            newIdent = ident::NONE;
            current_symbol.clear();

            error_buffer.clear();
            done = false;
        }


        inline bool scanner::good()
        {
            return error_buffer.size() == 0;
        }


        inline bool scanner::bad()
        {
            return error_buffer.size() != 0;
        }


        //function to scan for and separate input into separate tokens.
        //returns on timeout (does not mean it is finished scanning).
        //returns 1 if finished scanning,
        //returns 0 otherwise.

        bool scanner::scan(const core::timeout& time)
        {
            if (done || !input || !identifiers)
                return true;


            while (!time.timedOut() && (index < input->length()))
            {
                if (in_string)
                    behav_in_string();


                if (in_comment && multiline_comment &&
                    input->foundAt(comment_rules[2][open_comment], index))
                {
                    //end of multiple-line comment
                    in_comment = false;
                    multiline_comment = false;
                    index += comment_rules[2][open_comment].length();
                }

                if (!in_string && !in_comment)
                {
                    //now in a string
                    if ((input->at(index) == (CPL_CHAR)'\"') ||
                        (input->at(index) == (CPL_CHAR)'\''))
                    {
                        newIdent = ident::STRING_LITERAL;
                        in_string = true;
                        in_comment = false;
                        multiline_comment = false;

                        open_string = input->at(index);

                        symbol_type_change();
                    }
                    else //check if we're in a comment
                    {
                        int i=0;
                        while((i < comment_rules[1].size()) &&
                              !multiline_comment)
                        {
                            //now in a multiple-line comment
                            if (input->foundAt(comment_rules[1][i], index))
                            {
                                open_comment = i;
                                multiline_comment = true;
                            }

                            i++;
                        }


                        if (multiline_comment)
                        {
                            newIdent = ident::NONE;
                            in_comment = true;
                            in_string = false;

                            symbol_type_change();
                        }
                        else
                        {
                            i=0;
                            while((i < comment_rules[0].size()) &&
                                  !in_comment)
                            {
                                //now in a single-line comment
                                if (input->foundAt(comment_rules[0][i], index))
                                {
                                    in_comment = true;
                                }

                                i++;
                            }

                            if (in_comment)
                            {
                                newIdent = ident::NONE;
                                in_string = false;

                                symbol_type_change();
                            }
                        }
                    }
                }


                if (!in_string && !in_comment)
                {
                    //white space
                    if (core::isWhiteSpace(input->at(index)))
                    {
                        newIdent = ident::NONE;
                        indent++;
                    }
                    //generic identifiers
                    else if (core::isAlphanumeric(input->at(index)) ||
                             (input->at(index) == (CPL_CHAR)'_'))
                        behav_identifier();
                    //period
                    else if (input->at(index) == (CPL_CHAR)'.')
                        behav_period();
                    //parentheses
                    else if (input->at(index) == (CPL_CHAR)'(')
                        behav_lparenth();
                    else if (input->at(index) == (CPL_CHAR)')')
                        behav_rparenth();
                    //brackets
                    else if (input->at(index) == (CPL_CHAR)'[')
                        behav_lbracket();
                    else if (input->at(index) == (CPL_CHAR)']')
                        behav_rbracket();
                    //curly braces
                    else if (input->at(index) == (CPL_CHAR)'{')
                        behav_lbrace();
                    else if (input->at(index) == (CPL_CHAR)'}')
                        behav_rbrace();
                    //comma
                    else if (input->at(index) == (CPL_CHAR)',')
                        newIdent = ident::COMMA;
                    //semicolon
                    else if (input->at(index) == (CPL_CHAR)';')
                        newIdent = ident::SEMICOLON;
                    else //possible operator
                        newIdent = ident::UNKNOWN;


                    ///If there is a change in the type
                    if (newIdent != current_ident.type)
                    {
                        symbol_type_change();

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

                        indent = 0;
                    }
                    else if (current_ident.type)//otherwise,
                        current_symbol += input->at(index);
                }

                //update current line and column
                update_line_column();

                index++;
            }


            done = (index >= input->length());


            if (done)
            {
                check_open_symbols();

                symbol_type_change();
            }


            return done;
        }



        void scanner::check_open_symbols()
        {
            ident_t op_sym;
            while (open_symbol_indices.pop(op_sym))
            {
                if (op_sym.type == ident::LPARENTH)
                {
                    error_buffer.add(error("Missing close parentheses",
                                       *file,
                                       op_sym.line,
                                       op_sym.column));
                }
                else if (op_sym.type == ident::LBRACKET)
                {
                    error_buffer.add(error("Missing close square bracket",
                                       *file,
                                       op_sym.line,
                                       op_sym.column));
                }
                else if (op_sym.type == ident::LBRACE)
                {
                    error_buffer.add(error("Missing close curly brace",
                                       *file,
                                       op_sym.line,
                                       op_sym.column));
                }
            }
        }


        void scanner::symbol_type_change()
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

                    addmeta = (current_ident.type == ident::IDENTIFIER);
                }
                else if (current_ident.type == ident::NUMERIC_LITERAL)
                {
                    check_this_number();
                    current_ident.type = ident::LITERAL;
                }
                else if (current_ident.type == ident::STRING_LITERAL)
                {
                    current_ident.type = ident::LITERAL;
                    current_ident.value = current_symbol;
                }

                if (addmeta)
                    current_ident.meta = addToSymTable(current_symbol);
                identifiers->add(current_ident);
            }

            current_symbol.clear();
            current_ident.type = newIdent;

            current_ident.line = line;
            current_ident.column = column;
            current_ident.indent = indent;

            current_ident.meta = NULL;
            current_ident.file = file;

            current_ident.value.clear();
        }


        void scanner::update_line_column()
        {
            if (input->at(index) == (CPL_CHAR)'\n')
            {
                if (input->at(index+1) == (CPL_CHAR)'\r')
                    index++;

                line++;
                column = 0;
                indent = 0;

                in_comment = multiline_comment;
            }
            else if (input->at(index) == (CPL_CHAR)'\r')
            {
                if (input->at(index+1) == (CPL_CHAR)'\n')
                    index++;

                line++;
                column = 0;
                indent = 0;

                in_comment = multiline_comment;
            }
            else
            {
                column++;
            }
        }



        void scanner::behav_identifier()
        {
            if ((newIdent != ident::NUMERIC_LITERAL) &&
                (newIdent != ident::IDENTIFIER))
            {
                if (core::isNumeric(input->at(index)))
                    newIdent = ident::NUMERIC_LITERAL;
                else
                    newIdent = ident::IDENTIFIER;
            }
        }


        void scanner::behav_period()
        {
            //if a decimal point precedes a number
            //and no alphanumeric character directly precedes it,
            //we can assume we have a number (e.g. ".10")
            if (!newIdent && core::isNumeric(input->at(index+1)))
                newIdent = ident::NUMERIC_LITERAL;
            else if (newIdent != ident::NUMERIC_LITERAL)
                newIdent = ident::PERIOD;
        }


        void scanner::behav_lparenth()
        {
            newIdent = ident::LPARENTH;

            open_symbol_indices.push(ident_t(newIdent,
                                                   line, column));
        }


        void scanner::behav_rparenth()
        {
            newIdent = ident::RPARENTH;

            ident_t op_sym;
            if (!open_symbol_indices.pop(op_sym))
            {
                error_buffer.add(error("Missing open parentheses",
                                       *file,
                                       line, column));
            }
            else if (op_sym.type == ident::LBRACKET)
            {
                error_buffer.add(error("Missing close square bracket",
                                       *file,
                                       op_sym.line,
                                       op_sym.column));
            }
            else if (op_sym.type == ident::LBRACE)
            {
                error_buffer.add(error("Missing close curly brace",
                                       *file,
                                       op_sym.line,
                                       op_sym.column));
            }
        }



        void scanner::behav_lbracket()
        {
            newIdent = ident::LBRACKET;

            open_symbol_indices.push(ident_t(newIdent,
                                                   line, column));
        }


        void scanner::behav_rbracket()
        {
            newIdent = ident::RBRACKET;

            ident_t op_sym;
            if (!open_symbol_indices.pop(op_sym))
            {
                error_buffer.add(error("Missing open square bracket",
                                        *file,
                                        line, column));
            }
            else if (op_sym.type == ident::LPARENTH)
            {
                error_buffer.add(error("Missing close parentheses",
                                       *file,
                                       op_sym.line,
                                       op_sym.column));
            }
            else if (op_sym.type == ident::LBRACE)
            {
                error_buffer.add(error("Missing close curly brace",
                                       *file,
                                       op_sym.line,
                                       op_sym.column));
            }
        }



        void scanner::behav_lbrace()
        {
            newIdent = ident::LBRACE;

            open_symbol_indices.push(ident_t(newIdent,
                                                   line, column));
        }


        void scanner::behav_rbrace()
        {
            newIdent = ident::RBRACE;

            ident_t op_sym;
            if (!open_symbol_indices.pop(op_sym))
            {
                error_buffer.add(error("Missing open curly brace",
                                       *file,
                                       line,
                                       column));
            }
            else if (op_sym.type == ident::LBRACKET)
            {
                error_buffer.add(error("Missing close square bracket",
                                 *file,
                                 op_sym.line,
                                 op_sym.column));
            }
            else if (op_sym.type == ident::LPARENTH)
            {
                error_buffer.add(error("Missing close parentheses",
                                 *file,
                                 op_sym.line,
                                 op_sym.column));
            }
        }



        void scanner::behav_in_string()
        {
            if (input->at(index) == open_string)
            {
                in_string = false;
                index++;
            }
            else
            {
                int esc_seq = (int)whatEscSequence(*input, index);

                if (esc_seq)
                {
                    core::string<CPL_CHAR> seq_name;
                    core::string<CPL_CHAR> seq_equiv;

                    escSequenceName(esc_seq, seq_name);
                    escSequenceEquiv(esc_seq, seq_equiv);

                    current_symbol += seq_equiv;

                    index += seq_name.length() - 1;
                    column += seq_name.length() - 1;
                }
                else
                {
                    current_symbol += input->at(index);

                    //we have some unknown escape sequence
                    if (input->at(index) == (CPL_CHAR)'\\')
                    {
                        error_buffer.add(
                                error("Unknown escape sequence",
                                      *file,
                                      current_ident.line,
                                      current_ident.column));
                    }
                }
            }
        }


        ///list all the possible operators in the string
        ///the string must contain ONLY operators and NO spaces
        //returns false if an error was found.

        bool scanner::list_opers(core::string<CPL_CHAR>& input)
        {
            bool oper_error = false;

            core::array< ident_t > temp_opers;

            int x_offset = 0;


            while ((x_offset < input.length()) && !oper_error)
            {
                int start = x_offset;
                int stop = start;

                oper find_oper (input.substr(start, stop), 0);

                int prev_index = -1;
                int oper_index = operators->find(find_oper);


                while((oper_index > -1) &&
                      (stop < input.length()))
                {
                    prev_index = oper_index;
                    find_oper.symbol = input.substr(start, ++stop);
                    oper_index = operators->find(find_oper);
                }


                if (prev_index > -1)
                {
                    ident_t this_oper (ident::OPERATOR,
                                             line,
                                             column+x_offset-input.length(),
                                             0,
                                             NULL,
                                             file);
                    this_oper.metaValue = (operators->at(prev_index)).value;


                    temp_opers.add(this_oper);

                    x_offset += (operators->at(prev_index)).symbol.length();
                }
                else
                {
                    if (x_offset > 0)
                        error_buffer.add(error("Ambiguous expression",
                                                *file,
                                                line, column));
                    else
                        error_buffer.add(error("Unknown operator",
                                                *file,
                                                line, column-input.length()));

                    oper_error = true;
                }
            }


            if (!oper_error)
            {
                identifiers->add(temp_opers);
            }
            else
            {
                ident this_type;

                this_type = ident::UNKNOWN;


                identifiers->add(ident_t(this_type,
                                               line, column-input.length()));
            }

            return oper_error;
        }


        ///If the current symbol matches a keyword, change the type to the appropriate keyword.

        void scanner::get_this_keyword()
        {
            if (current_ident.type == ident::IDENTIFIER)
            {
                int kwd_index = keywords->find(keyword(current_symbol,0));

                if (kwd_index > -1)//is a keyword
                {
                    current_ident.type = ident::KEYWORD;
                    current_ident.metaValue = (keywords->at(kwd_index)).value;
                }
            }
        }


        ///If the current identifier matches a number form, check what form the number is in
        ///(e.g. decimal, binary, octal or hexadecimal) and convert to a number.

        void scanner::check_this_number()
        {
            core::string<CPL_CHAR> symbol = current_symbol;

            bool return_good = true;
            bool is_complex = false;

            if (symbol.endsWith("i"))
            {
                is_complex = true;
                symbol.remove(symbol.length()-1, symbol.length());
            }


            int base;

            if (symbol.beginsWith("0b"))
            {
                base = 2;
                symbol.remove(0,1);
            }
            else if (current_symbol.beginsWith("0c") ||
                     current_symbol.beginsWith("0o"))
            {
                base = 8;
                symbol.remove(0,1);
            }
            else if (current_symbol.beginsWith("0h") ||
                     current_symbol.beginsWith("0x"))
            {
                base = 16;
                symbol.remove(0,1);
            }
            else
            {
                base = 10;
            }


            bool pastDecimal = false;

            for (int i=0; i<(symbol.length()); i++)
            {
                CPL_CHAR _char = symbol[i];


                if (_char == (CPL_CHAR)'.')
                {
                    if (pastDecimal)
                    {
                        error_buffer.add(
                                error("Number contains excess decimals",
                                      *file,
                                      current_ident.line,
                                      current_ident.column));

                        return_good = false;
                    }

                    pastDecimal = true;
                }
                else if (!core::isNumeric(_char, base))
                {
                    error_buffer.add(
                                error("Number contains illegal characters",
                                      *file,
                                      current_ident.line,
                                      current_ident.column));

                    return_good = false;
                }
            }


            if (return_good)
            {
                if (is_complex)
                    current_ident.value = std::complex<Float>(0,symbol.value(base));
                else
                    current_ident.value = symbol.value(base);
            }

            //std::cout << "[" << current_ident.value.complex() << "]\n";
        }

        ///Some operators have alphanumeric characters in them.
        ///if the current identifier matches an operator, change the type to OPERATOR.
        //does not produce any errors, so always returns true.

        void scanner::get_this_operator()
        {
            if (current_ident.type == ident::IDENTIFIER)
            {
                int oper_index = operators->find(oper(current_symbol,0));

                if (oper_index > -1)//is an alphanumeric operator
                {
                    current_ident.type = ident::OPERATOR;
                    current_ident.metaValue = (operators->at(oper_index)).value;
                }
            }
        }




        core::string<CPL_CHAR>* scanner::addToSymTable(const core::string<CPL_CHAR>& symbol) const
        {
            if (sym_table)
            {
                core::string<CPL_CHAR>* new_sym = new core::string<CPL_CHAR>(symbol);

                int existing = sym_table->find(new_sym);

                if (existing > -1)
                {
                    delete new_sym;

                    return sym_table->at(existing);
                }
                else
                {
                    sym_table->add(new_sym);
                    return new_sym;
                }
            }

            return NULL;
        }


    }
}
}


#endif // SCANNER_H_INCLUDED
