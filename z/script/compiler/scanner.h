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

        template <typename CHAR>
        class scanner
        {
        private:
            core::string<CHAR>* input;
            core::array< ident_t<CHAR> >* identifiers;

            core::dynamicStack< ident_t<CHAR> > open_symbol_indices;

            int index;

            bool in_string;

            bool in_comment;
            bool multiline_comment;

            int line;
            int column;
            int indent;

            ident_t<CHAR> current_ident;
            ident newIdent;
            core::string<CHAR> current_symbol;

            bool done;

            core::sortedRefArray< core::string<CHAR>* >* sym_table;

            core::array<keyword>* keywords;
            core::array<oper>* operators;

            //keep track of the current file
            core::string<CHAR>* file;

        public:
            core::array< error > error_buffer;


            scanner(core::sortedRefArray< core::string<CHAR>* >*,
                     core::array<keyword>*,
                     core::array<oper>*,
                     core::string<CHAR>*,
                     core::array< ident_t<CHAR> >*);

            void set(core::sortedRefArray< core::string<CHAR>* >*,
                     core::array<keyword>*,
                     core::array<oper>*,
                     core::string<CHAR>*,
                     core::array< ident_t<CHAR> >*);

            inline void linkInput(core::string<CHAR>*);

            bool scan(const core::timeout& time = -1);

            void clear();
            inline bool good();
            inline bool bad();

        private:
            bool list_opers(core::string<CHAR>&);

            void get_this_keyword();
            void get_this_operator();

            void check_this_number();

            core::string<CHAR>* addToSymTable(core::string<CHAR>*) const;


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



        template <typename CHAR>
        //constructor allows operators, commands, and functions be set
        scanner<CHAR>::scanner(core::sortedRefArray<core::string<CHAR>* >*
                                                            symbol_table,
                               core::array<keyword>* Keywords,
                               core::array<oper>* Operators,
                               core::string<CHAR>* File,
                               core::array< ident_t<CHAR> >* Output)
        {
            clear();

            set(symbol_table,
                Keywords,
                Operators,
                File,
                Output);
        }

        template <typename CHAR>
        void scanner<CHAR>::set(core::sortedRefArray<core::string<CHAR>* >*
                                                            symbol_table,
                                core::array<keyword>* Keywords,
                                core::array<oper>* Operators,
                                core::string<CHAR>* File,
                                core::array< ident_t<CHAR> >* Output)
        {
            sym_table = symbol_table;
            keywords = Keywords;
            operators = Operators;
            file = File;
            identifiers = Output;

            done = true;
        }

        template <typename CHAR>
        inline void scanner<CHAR>::linkInput(core::string<CHAR>* string_input)
        {
            input = string_input;

            clear();
        }

        template <typename CHAR>
        void scanner<CHAR>::clear()
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

            current_ident = ident_t<CHAR>(ident::NONE, 0, 0);
            newIdent = ident::NONE;
            current_symbol.clear();

            error_buffer.clear();
            done = false;
        }

        template <typename CHAR>
        inline bool scanner<CHAR>::good()
        {
            return error_buffer.size() == 0;
        }

        template <typename CHAR>
        inline bool scanner<CHAR>::bad()
        {
            return error_buffer.size() != 0;
        }


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
                    behav_in_string();


                if (in_comment &&
                    input->foundAt("*/", index))
                {
                    //end of multiple-line comment
                    in_comment = false;
                    multiline_comment = false;
                    index+=2;
                }

                if (!in_string && !in_comment)
                {
                    //now in a string
                    if (input->at(index) == (CHAR)'\"')
                    {
                        newIdent = ident::STRING_LITERAL;
                        in_string = true;
                        in_comment = false;
                        multiline_comment = false;

                        symbol_type_change();
                    }
                    //now in a multiple-line comment
                    else if (input->foundAt("/*", index))
                    {
                        newIdent = ident::NONE;
                        in_comment = true;
                        in_string = false;

                        multiline_comment = true;

                        symbol_type_change();
                    }
                    //now in a single-line comment
                    else if (input->foundAt("//", index))
                    {
                        newIdent = ident::NONE;
                        in_comment = true;
                        in_string = false;

                        multiline_comment = false;

                        symbol_type_change();
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
                             (input->at(index) == (CHAR)'_'))
                        behav_identifier();
                    //period
                    else if (input->at(index) == (CHAR)'.')
                        behav_period();
                    //parentheses
                    else if (input->at(index) == (CHAR)'(')
                        behav_lparenth();
                    else if (input->at(index) == (CHAR)')')
                        behav_rparenth();
                    //brackets
                    else if (input->at(index) == (CHAR)'[')
                        behav_lbracket();
                    else if (input->at(index) == (CHAR)']')
                        behav_rbracket();
                    //curly braces
                    else if (input->at(index) == (CHAR)'{')
                        behav_lbrace();
                    else if (input->at(index) == (CHAR)'}')
                        behav_rbrace();
                    //comma
                    else if (input->at(index) == (CHAR)',')
                        newIdent = ident::COMMA;
                    //semicolon
                    else if (input->at(index) == (CHAR)';')
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


        template <typename CHAR>
        void scanner<CHAR>::check_open_symbols()
        {
            ident_t<CHAR> op_sym;
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

        template <typename CHAR>
        void scanner<CHAR>::symbol_type_change()
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
                    current_ident.meta = addToSymTable(&current_symbol);
                identifiers->add(current_ident);
            }

            current_symbol.clear();
            current_ident.type = newIdent;

            current_ident.line = line;
            current_ident.column = column;
            current_ident.indent = indent;

            current_ident.meta = NULL;
            current_ident.file = file;

            current_ident.value = generic<CHAR>();
        }

        template <typename CHAR>
        void scanner<CHAR>::update_line_column()
        {
            if (input->at(index) == (CHAR)'\n')
            {
                if (input->at(index+1) == (CHAR)'\r')
                    index++;

                line++;
                column = 0;
                indent = 0;

                in_comment = multiline_comment;
            }
            else if (input->at(index) == (CHAR)'\r')
            {
                if (input->at(index+1) == (CHAR)'\n')
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


        template <typename CHAR>
        void scanner<CHAR>::behav_identifier()
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

        template <typename CHAR>
        void scanner<CHAR>::behav_period()
        {
            //if a decimal point precedes a number
            //and no alphanumeric character directly precedes it,
            //we can assume we have a number (e.g. ".10")
            if (!newIdent && core::isNumeric(input->at(index+1)))
                newIdent = ident::NUMERIC_LITERAL;
            else if (newIdent != ident::NUMERIC_LITERAL)
                newIdent = ident::PERIOD;
        }

        template <typename CHAR>
        void scanner<CHAR>::behav_lparenth()
        {
            newIdent = ident::LPARENTH;

            open_symbol_indices.push(ident_t<CHAR>(newIdent,
                                                   line, column));
        }

        template <typename CHAR>
        void scanner<CHAR>::behav_rparenth()
        {
            newIdent = ident::RPARENTH;

            ident_t<CHAR> op_sym;
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


        template <typename CHAR>
        void scanner<CHAR>::behav_lbracket()
        {
            newIdent = ident::LBRACKET;

            open_symbol_indices.push(ident_t<CHAR>(newIdent,
                                                   line, column));
        }

        template <typename CHAR>
        void scanner<CHAR>::behav_rbracket()
        {
            newIdent = ident::RBRACKET;

            ident_t<CHAR> op_sym;
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


        template <typename CHAR>
        void scanner<CHAR>::behav_lbrace()
        {
            newIdent = ident::LBRACE;

            open_symbol_indices.push(ident_t<CHAR>(newIdent,
                                                   line, column));
        }

        template <typename CHAR>
        void scanner<CHAR>::behav_rbrace()
        {
            newIdent = ident::RBRACE;

            ident_t<CHAR> op_sym;
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


        template <typename CHAR>
        void scanner<CHAR>::behav_in_string()
        {
            if (input->at(index) == (CHAR)34)
            {
                in_string = false;
                index++;
            }
            else
            {
                int esc_seq = (int)whatEscSequence(*input, index);

                if (esc_seq)
                {
                    core::string<CHAR> seq_name;
                    core::string<CHAR> seq_equiv;

                    escSequenceName(esc_seq, seq_name);
                    escSequenceEquiv(esc_seq, seq_equiv);

                    current_symbol += seq_equiv;

                    index += seq_name.length() - 1;
                    column += seq_name.length() - 1;
                }
                else
                {
                    current_symbol += input->at(index);

                    if (input->at(index) == (CHAR)92) //we have some unknown escape sequence
                    {
                        core::string<char> bad_esc_str = "\\";
                        bad_esc_str += core::string<char>(input->at(index+1));

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
        template <typename CHAR>
        bool scanner<CHAR>::list_opers(core::string<CHAR>& input)
        {
            bool oper_error = false;

            core::array< ident_t<CHAR> > temp_opers;

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
                    ident_t<CHAR> this_oper (ident::OPERATOR,
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


                identifiers->add(ident_t<CHAR>(this_type,
                                               line, column-input.length()));
            }

            return oper_error;
        }


        ///If the current symbol matches a keyword, change the type to the appropriate keyword.
        template <typename CHAR>
        void scanner<CHAR>::get_this_keyword()
        {
            if (current_ident.type == ident::IDENTIFIER)
            {
                int kwd_index = keywords->find(keyword(current_symbol,0));

                if (kwd_index > -1)//is a keyword
                {
                    current_ident.type = ident::KEYWORD;
                    current_ident.value = (keywords->at(kwd_index)).value;
                }
            }
        }


        ///If the current identifier matches a number form, check what form the number is in
        ///(e.g. decimal, binary, octal or hexadecimal) and convert to a number.
        template <typename CHAR>
        void scanner<CHAR>::check_this_number()
        {
            core::string<CHAR> symbol = current_symbol;

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
                CHAR _char = symbol[i];


                if (_char == (CHAR)'.')
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
        }

        ///Some operators have alphanumeric characters in them.
        ///if the current identifier matches an operator, change the type to OPERATOR.
        //does not produce any errors, so always returns true.
        template <typename CHAR>
        void scanner<CHAR>::get_this_operator()
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
}


#endif // SCANNER_H_INCLUDED
