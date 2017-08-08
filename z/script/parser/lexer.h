/**
 * File:            lexer.h
 * Namespace:       z::script
 * Description:     Script lexer template. The lexer
 *                  takes the list of tokens generated
 *                  by the scanner and groups them into
 *                  functional phrases (e.g. expression,
 *                  if statement, etc.)
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   8 Aug. 2017
**/


#pragma once
#ifndef LEXER_H_INCLUDED
#define LEXER_H_INCLUDED

#include <z/core/timeout.h>
#include <z/core/array.h>
#include <z/core/dynamicStack.h>

#include "../errors.h"

#include "phrase.h"

#include <iostream>

#ifndef NULL
    #define NULL 0
#endif // NULL

namespace z
{
    namespace script
    {
        ///debug
        const char* symTypeStr[] =
        {
            "none",
            "(",")",
            "{","}",
            "[","]",
            ",",";",".",
            "#str","#num", "#compl", "#const",
            "id",
            "if","else",
            "for","each","in","loop","while",
            "goto","gosub","label","sub",
            "run","stop","include",
            "break","return",
            "dim",
            "wait","until",
            "var","type","function",
            "global","external","shared",
            "=",
            "+=","-=",
            "*=","/=","//=","%=",
            "++","--",
            "sizeof",
            "^","!",
            "+","-",
            "*","/","//","%",
            "and","&",
            "or","|",
            "xor",":",
            "nand","~&",
            "nor","~|",
            "nxor", "~:",
            "==","<>",">",">=","<","<=",
            "not","~",
            "->", "<-",
            "unknown",

            "identifierlist",
            "command",
            "statementlist",
            "statement",
            "if_statement",
            "for_statement",
            "foreach_statement",
            "loop_statement",
            "while_pre_stmt",
            "while_post_stmt",
            "run_statement",
            "stop_statement",
            "return_statement",
            "wait_statement",
            "until_statement",
            "label_statement",
            "goto_statement",
            "gosub_statement",
            "subroutine_decl",
            "variable_decl",
            "typevar_decl",
            "range",
            "rangelist",
            "index",
            "indexlist",
            "exprlist",
            "list",
            "funccall",
            "type_funccall",
            "varindex",
            "typevar",
            "variable",
            "operand",
            "parenthexpr",
            "factorialexpr",
            "add1expr",
            "negatexpr",
            "powerexpr",
            "multiplyexpr",
            "addexpr",
            "boolexpr",
            "assignexpr",
            "dimensionexpr",
            "sizeofexpr",
            "externaldecl",
            "shareddecl",
            "formalvardecl",
            "formaltypedecl",
            "formaldecllist",
            "function_decl",
            "int_decllist",
            "typedecl",
            "program"
        };

        namespace lex
        {
            enum progress
            {
                NONE = 0,
                GENERAL,
                PROGRAM,
                TREE_CLEANUP,
                ERROR_CHECK,
                DONE
            };
        }


        template <typename CHAR>
        void delete_ast(phrase_t<CHAR>* root)
        {
            for (int i=0; i<(root->children.size()); i++)
            {
                delete_ast(root->children[i]);
            }

            delete root;
        }


        template <typename CHAR>
        class lexer
        {
        private:
            core::array< ident_t<CHAR> >* input_ident;

            core::array< phrase_t<CHAR>* > phrase_nodes;

            phrase_t<CHAR>* current_node;

            int index;
            bool did_concat;

            int progress;
            bool input_in_use;

            core::dynamicStack<int> cleanup_stack;

            ///error checking
            bool error_check();

            bool error_oper();
            bool error_semicolon();
            bool error_for();


            ///phrase detection
            bool identifierlist();
            bool _command();

            bool statementlist();
            bool statement();
            bool if_statement();
            bool for_statement();
            bool foreach_statement();
            bool loop_statement();
            bool while_pre_stmt();
            bool while_post_stmt();
            bool run_statement();
            bool stop_statement();
            bool return_statement();
            bool wait_statement();
            bool until_statement();
            bool label_statement();
            bool goto_statement();
            bool gosub_statement();

            bool subroutine_decl();

            bool variable_decl();
            bool typevar_decl();

            bool _range();
            bool rangelist();
            bool _index();
            bool indexlist();

            bool exprlist();
            bool _list();

            bool funccall();
            bool type_funccall();

            bool varindex();
            bool typevar();
            bool variable();
            bool operand();

            bool parenthexpr();
            bool factorialexpr();
            bool add1expr();
            bool negatexpr();
            bool powerexpr();
            bool multiplyexpr();
            bool addexpr();
            bool boolexpr();
            bool assignexpr();
            bool dimensionexpr();
            bool sizeofexpr();
            bool expression();

            bool externaldecl();
            bool shareddecl();

            bool formalvardecl();
            bool formaltypedecl();
            bool formaldecllist();
            bool function_decl();

            bool int_decllist();
            bool typedecl();

            bool program();



        public:
            core::array< parser_error<CHAR> > error_buffer;

            lexer()
            {
                input_ident = NULL;
                progress = lex::NONE;

                index = 0;

                input_in_use = true;

                did_concat = false;
            }

            ~lexer()
            {
                for (int i=0; i<phrase_nodes.size(); i++)
                    delete_ast(phrase_nodes[i]);
            }

            void setInput(core::array< ident_t<CHAR> >& identifiers)
            {
                input_ident = &identifiers;

                for (int i=0; i<phrase_nodes.size(); i++)
                    delete_ast(phrase_nodes[i]);
                phrase_nodes.clear();

                progress = lex::NONE;

                index = 0;

                error_buffer.clear();

                did_concat = false;
            }


            bool lex(const core::timeout&);

            inline bool error() {return error_buffer.size() > 0;}

            inline bool usingInput() {return input_in_use;}

            inline bool done() {return (progress == lex::DONE);}


            phrase_t<CHAR>* moveResultAST()
            {
                if ((progress == lex::DONE) &&
                    !error_buffer.size() &&
                    (phrase_nodes.size() == 1))
                {
                    phrase_t<CHAR>* result = phrase_nodes[0];
                    phrase_nodes.remove(0);

                    return result;
                }
                else
                    return NULL;
            }
        };


        ///template for lexing function. Attempts to parse identifiers into
        ///valid syntax. Returns true if done, false otherwise.
        template <typename CHAR>
        bool lexer<CHAR>::lex(const core::timeout& time)
        {
            while (!time.timedOut() && (progress != lex::DONE))
            {
                if (progress == lex::NONE)
                {
                    if (index >= input_ident->size())
                    {
                        progress = lex::GENERAL;
                        index = 0;

                        input_in_use = false;
                    }
                    else
                    {
                        phrase_nodes.add(new phrase_t<CHAR>(input_ident->at(index)));

                        index++;
                    }
                }
                else if (progress == lex::GENERAL)
                {
                    if (index >= phrase_nodes.size())
                    {
                        if (!did_concat)
                        {
                            progress = lex::PROGRAM;
                            current_node = NULL;
                        }

                        index = 0;
                        did_concat = false;
                    }
                    else if (formalvardecl()    ||
                             formaltypedecl()   ||
                             formaldecllist()   ||
                             function_decl()    ||
                             int_decllist()     ||
                             typedecl()         ||

                             externaldecl()     ||
                             shareddecl()       ||

                             identifierlist()   ||
                             _command()         ||

                             if_statement()     ||
                             statementlist()    ||
                             for_statement()    ||
                             foreach_statement()||
                             loop_statement()   ||
                             while_pre_stmt()   ||
                             while_post_stmt()  ||
                             label_statement()  ||
                             goto_statement()   ||
                             gosub_statement()  ||
                             run_statement()    ||
                             stop_statement()   ||
                             return_statement() ||
                             wait_statement()   ||
                             until_statement()  ||

                             subroutine_decl()  ||
                             variable_decl()    ||
                             typevar_decl()     ||
                             _range()           ||
                             rangelist()        ||
                             _index()           ||
                             indexlist()        ||
                             exprlist()         ||
                             _list()            ||
                             funccall()         ||
                             type_funccall()    ||

                             varindex()         ||
                             typevar()          ||
                             variable()         ||
                             operand()          ||

                             parenthexpr()      ||
                             factorialexpr()    ||
                             add1expr()         ||
                             negatexpr()        ||
                             powerexpr()        ||
                             multiplyexpr()     ||
                             addexpr()          ||
                             boolexpr()         ||
                             assignexpr()       ||
                             dimensionexpr()    ||
                             sizeofexpr()       ||

                             statement()
                             )
                        did_concat = true;
                    else
                        index++;

                }
                else if (progress == lex::PROGRAM)
                {
                    if (index >= phrase_nodes.size())
                    {
                        if (!did_concat)
                        {
                            progress = lex::TREE_CLEANUP;
                            current_node = NULL;
                        }

                        index = 0;
                        did_concat = false;
                    }
                    else if (program())
                        did_concat = true;
                    else
                        index++;

                }
                else if (progress == lex::TREE_CLEANUP)
                {
                    if (phrase_nodes.size() == 0)
                        progress = lex::DONE;
                    else
                    {
                        if (!current_node)
                        {
                            cleanup_stack.push(1);
                            current_node = phrase_nodes[0];
                            index = 0;
                        }
                        else if (index >= current_node->children.size())
                        {
                            if (current_node->orig_type != ident::NONE)
                            {
                                current_node->type = current_node->orig_type;
                                current_node->orig_type = ident::NONE;
                            }

                            if (current_node->parent)
                            {
                                cleanup_stack.pop(index);
                                current_node = current_node->parent;

                            }
                            else
                            {
                                cleanup_stack.pop(index);

                                if (index < phrase_nodes.size())
                                {
                                    current_node = phrase_nodes[index];
                                    cleanup_stack.push(index+1);

                                    index = 0;
                                }
                                else
                                {
                                    index = 0;
                                    progress = lex::ERROR_CHECK;
                                }
                            }
                        }
                        else
                        {
                            cleanup_stack.push(index+1);

                            current_node = current_node->children[index];

                            index = 0;
                        }
                    }
                }
                else if (progress == lex::ERROR_CHECK)
                {
                    if (index >= phrase_nodes.size())
                    {
                        progress = lex::DONE;

                        current_node = NULL;

                        index = 0;
                    }
                    else
                    {
                        if (error_oper()        ||
                            error_semicolon()   ||
                            error_for()
                            )
                        {
                            index = 0;
                            current_node = NULL;

                            progress = lex::GENERAL;
                        }
                        else
                            index++;
                    }
                }

            }


            ///debug, REMEMBER TO REMOVE
            if (progress == lex::DONE)
            {
                if (phrase_nodes.size() > 1)
                {
                    error_buffer.add(parser_error<CHAR>(-1, -1, error::SYNTAX_ERROR, -1));

                    for (int n=0; n<phrase_nodes.size(); n++)
                    {
                        print_lex_ast(0, phrase_nodes[n]);
                    }
                }
            }

            return (progress == lex::DONE);
        }



        template <typename CHAR>
        bool lexer<CHAR>::error_oper()
        {
            //unexpected operator
            if ((phrase_nodes[index]->type >= ident::OPER_ASSIGN) &&
                (phrase_nodes[index]->type <= ident::OPER_L_ARROW))
            {
                error_buffer.add(parser_error<CHAR>(phrase_nodes[index]->line,
                                                    phrase_nodes[index]->column,
                                                    error::UNEXPECTED_OPERATOR,
                                                    phrase_nodes[index]->file
                                                    ));

                delete phrase_nodes[index];
                phrase_nodes.remove(index);

                return true;
            }

            return false;
        }


        template <typename CHAR>
        bool lexer<CHAR>::error_semicolon()
        {
            if (phrase_nodes[index]->type == ident::SEMICOLON)
            {
                //statements outside of function declaration
                if (phrase_nodes.is_valid(index-1) &&
                    (phrase_nodes[index-1]->type == phrase::BOOLEXPR))
                {
                    error_buffer.add(parser_error<CHAR>(phrase_nodes[index]->line,
                                                    phrase_nodes[index]->column,
                                                    error::STMT_OUTSIDE_FUNCTION,
                                                    phrase_nodes[index]->file
                                                    ));

                    delete phrase_nodes[index];
                    delete_ast(phrase_nodes[index-1]);

                    phrase_nodes.remove(index);
                    phrase_nodes.remove(index-1);
                }
                //misplaced semicolons
                else
                {
                    error_buffer.add(parser_error<CHAR>(phrase_nodes[index]->line,
                                                    phrase_nodes[index]->column,
                                                    error::UNEXPECTED_SEMICOLON,
                                                    phrase_nodes[index]->file
                                                    ));

                    delete phrase_nodes[index];
                    phrase_nodes.remove(index);
                }

                return true;
            }


            return false;
        }


        template <typename CHAR>
        bool lexer<CHAR>::error_for()
        {
            //incomplete FOR statements
            if (phrase_nodes[index]->type == ident::KEYWORD_FOR)
            {
                if (phrase_nodes.is_valid(index+1) &&
                    (phrase_nodes[index+1]->type == ident::LPARENTH))
                {
                    int param_ct = 1;

                    int i=index+1;
                    while (phrase_nodes[i]->type != ident::RPARENTH)
                    {
                        if (phrase_nodes[i]->type == ident::SEMICOLON)
                            param_ct++;

                        i++;
                    }

                    if (phrase_nodes[i-1]->type == ident::SEMICOLON)
                        param_ct--;


                    if (param_ct > 3)
                        error_buffer.add(parser_error<CHAR>(phrase_nodes[index]->line,
                                                    phrase_nodes[index]->column,
                                                    error::TOO_MANY_PARAMS,
                                                    phrase_nodes[index]->file
                                                    ));
                    else if (param_ct < 3)
                        error_buffer.add(parser_error<CHAR>(phrase_nodes[index]->line,
                                                    phrase_nodes[index]->column,
                                                    error::TOO_FEW_PARAMS,
                                                    phrase_nodes[index]->file
                                                    ));
                    else
                        return false;

                    for (int j=i; j>=index; j--)
                    {
                        delete_ast(phrase_nodes[j]);
                        phrase_nodes.remove(j);
                    }
                }
                else
                {
                    error_buffer.add(parser_error<CHAR>(phrase_nodes[index]->line,
                                                    phrase_nodes[index]->column,
                                                    error::EXPECTED_PARENTHS,
                                                    phrase_nodes[index]->file
                                                    ));

                    delete phrase_nodes[index];
                    phrase_nodes.remove(index);

                }

                return true;
            }


            return false;
        }


        ///debug
        template <typename CHAR>
        void print_lex_ast(int level, phrase_t<CHAR>* node)
        {
            if (node)
            {
                for (int i=0; i<level; i++)
                    std::cout << "    ";



                if (node->type == ident::STRING_LITERAL)
                    std::cout << "<" << node->value.string().str() << ">";
                else if (node->type == ident::NUMERIC_LITERAL)
                    std::cout << "<" << node->value.string().str() << ">";
                else if (node->type == ident::COMPLEX_LITERAL)
                    std::cout << "<" << node->value.string().str() << "i>";
                else if (node->type == ident::LITERAL)
                    std::cout << "#const<" << node->value.string().str() << ">";
                else if (node->type == ident::UNKNOWN)
                    std::cout << "#unkn<" << ((core::string<CHAR>*)(node->meta))->str() << ">";
                else if (node->type == ident::IDENTIFIER)
                    std::cout << symTypeStr[node->type] << " : {"\
                     << ((core::string<CHAR>*)(node->meta))->str() << "}";
                else
                    std::cout << symTypeStr[node->type];

                std::cout << std::endl;

                for (int i=0; i<(node->children.size()); i++)
                    print_lex_ast(level+1, node->children[i]);
            }
        }
    }
}


#include "syntaxRules/identifierlist.h"
#include "syntaxRules/command.h"
#include "syntaxRules/statement.h"
#include "syntaxRules/statementlist.h"
#include "syntaxRules/if_statement.h"
#include "syntaxRules/for_statement.h"
#include "syntaxRules/foreach_statement.h"
#include "syntaxRules/loop_statement.h"
#include "syntaxRules/while_pre_stmt.h"
#include "syntaxRules/while_post_stmt.h"
#include "syntaxRules/label_statement.h"
#include "syntaxRules/goto_statement.h"
#include "syntaxRules/gosub_statement.h"
#include "syntaxRules/run_statement.h"
#include "syntaxRules/return_statement.h"
#include "syntaxRules/wait_statement.h"
#include "syntaxRules/until_statement.h"
#include "syntaxRules/subroutine_decl.h"
#include "syntaxRules/variable_decl.h"
#include "syntaxRules/typevar_decl.h"
#include "syntaxRules/range.h"
#include "syntaxRules/rangelist.h"
#include "syntaxRules/index.h"
#include "syntaxRules/indexlist.h"
#include "syntaxRules/exprlist.h"
#include "syntaxRules/list.h"
#include "syntaxRules/funccall.h"
#include "syntaxRules/type_funccall.h"
#include "syntaxRules/varindex.h"
#include "syntaxRules/typevar.h"
#include "syntaxRules/variable.h"
#include "syntaxRules/operand.h"
#include "syntaxRules/parenthexpr.h"
#include "syntaxRules/factorialexpr.h"
#include "syntaxRules/add1expr.h"
#include "syntaxRules/negatexpr.h"
#include "syntaxRules/powerexpr.h"
#include "syntaxRules/multiplyexpr.h"
#include "syntaxRules/addexpr.h"
#include "syntaxRules/boolexpr.h"
#include "syntaxRules/assignexpr.h"
#include "syntaxRules/dimensionexpr.h"
#include "syntaxRules/sizeofexpr.h"
#include "syntaxRules/externaldecl.h"
#include "syntaxRules/shareddecl.h"
#include "syntaxRules/formalvardecl.h"
#include "syntaxRules/formaltypedecl.h"
#include "syntaxRules/formaldecllist.h"
#include "syntaxRules/function_decl.h"
#include "syntaxRules/int_decllist.h"
#include "syntaxRules/typedecl.h"
#include "syntaxRules/program.h"
#include "syntaxRules/stop_statement.h"


#endif // LEXER_H_INCLUDED
