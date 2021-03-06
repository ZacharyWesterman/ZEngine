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

#include "syntaxRule.h"

#include <z/script/error.h>

#include "phrase.h"

#include <iostream>

#ifndef NULL
    #define NULL 0
#endif // NULL

namespace z
{
namespace script
{
    namespace compiler
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
            "keyword",
            "operator",
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
            "int_decllist",
            "typedecl",
            "externaldecl",
            "shareddecl",
            "func_prototype",
            "function_decl",
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
            "formalvardecl",
            "formaltypedecl",
            "formaldecllist",
            "program",
            "funccall_builtin"//72
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


        ///debug
        void print_lex_ast(int, phrase_t*);



        class lexer
        {
        private:
            core::array< ident_t >* input_ident;

            core::array< phrase_t* > phrase_nodes;

            core::array< syntaxRule* >* rules;
            syntaxRule* program_rule;

            phrase_t* current_node;

            int index;
            bool did_concat;

            int scope;

            int progress;
            bool input_in_use;

            core::dynamicStack<int> cleanup_stack;


            void prg_none();
            void prg_general();
            void prg_program();
            void prg_cleanup();


        public:
            core::array< error > error_buffer;

            lexer(core::array< syntaxRule* >* Rules,
                  syntaxRule* programRule)
            {
                rules = Rules;
                program_rule = programRule;

                input_ident = NULL;
                progress = lex::NONE;

                index = 0;
                scope = 0;

                input_in_use = true;

                did_concat = false;
            }

            ~lexer()
            {
                for (int i=0; i<phrase_nodes.size(); i++)
                    deleteNode(phrase_nodes[i]);
            }

            void linkInput(core::array< ident_t >* identifiers)
            {
                input_ident = identifiers;

                for (int i=0; i<phrase_nodes.size(); i++)
                    deleteNode(phrase_nodes[i]);
                phrase_nodes.clear();

                progress = lex::NONE;

                index = 0;
                scope = 0;

                error_buffer.clear();

                did_concat = false;
            }


            bool lex(const core::timeout& time = -1);

            inline bool good() {return error_buffer.size() == 0;}
            inline bool bad() {return error_buffer.size() != 0;}

            inline bool usingInput() {return input_in_use;}

            inline bool done() {return (progress == lex::DONE);}


            phrase_t* moveResultAST()
            {
                if ((progress == lex::DONE) &&
                    !error_buffer.size() &&
                    (phrase_nodes.size() == 1))
                {
                    phrase_t* result = phrase_nodes[0];
                    phrase_nodes.clear();

                    return result;
                }
                else
                    return NULL;
            }
        };


        ///template for lexing function. Attempts to parse identifiers into
        ///valid syntax. Returns true if done, false otherwise.

        bool lexer::lex(const core::timeout& time)
        {
            while (!time.timedOut() && (progress != lex::DONE))
            {
                if (progress == lex::NONE)
                {
                    prg_none();
                }
                else if (progress == lex::GENERAL)
                {
                    prg_general();
                }
                else if (progress == lex::PROGRAM)
                {
                    prg_program();
                }
                else if (progress == lex::TREE_CLEANUP)
                {
                    prg_cleanup();
                }
                /*else if (progress == lex::ERROR_CHECK)
                {
                    if (index >= phrase_nodes.size())
                    {
                        progress = lex::DONE;

                        current_node = NULL;

                        index = 0;
                    }
                    else
                    {
                        if (false error_oper()        ||
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
                }*/

            }


            ///debug, REMEMBER TO REMOVE
            if (progress == lex::DONE)
            {
                if (phrase_nodes.size() > 1)
                {
                    //error_buffer.add(error("Syntax error",-1,-1));

                    for (int n=0; n<phrase_nodes.size(); n++)
                    {
                        print_lex_ast(0, phrase_nodes[n]);
                    }
                }
                else if (phrase_nodes.size())
                    print_lex_ast(0, phrase_nodes[0]);
                else
                    std::cout << "<<<NULL>>>\n";
            }

            return (progress == lex::DONE);
        }



        void lexer::prg_none()
        {
            if (index >= input_ident->size())
            {
                progress = lex::GENERAL;
                index = 0;

                input_in_use = false;
            }
            else
            {
                phrase_nodes.add(new phrase_t(input_ident->at(index)));

                index++;
            }
        }


        void lexer::prg_general()
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
            else
            {
                int r=0;

                while ((r < (rules->size())) &&
                       !(rules->at(r)->apply(&phrase_nodes, index, &error_buffer))
                       )
                {
                    r++;
                }

                if (r < (rules->size()))
                {
                    did_concat = true;
                }
                else
                    index++;

            }
        }


        void lexer::prg_program()
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
            else if (program_rule->apply(&phrase_nodes, index, &error_buffer))
                did_concat = true;
            else
                index++;
        }


        void lexer::prg_cleanup()
        {
            //print_lex_ast(0, phrase_nodes[0]);

            if (phrase_nodes.size() == 0)
                progress = lex::DONE;
            else
            {
                if (!current_node)
                {
                    cleanup_stack.push(1);
                    current_node = phrase_nodes.at(0);
                    index = 0;
                }

                //int ind_ceil = ;

                if (index >= (current_node->children.size()))
                {
                    if (current_node->parent)
                    {
                        if (current_node->orig_type)
                        {
                            current_node->type = current_node->orig_type;
                            current_node->orig_type = ident::NONE;
                        }

                        /*std::cout << (current_node->type) << " @ "
                                      << (current_node->line) <<","
                                      << (current_node->column) <<"::";
                            if (((current_node->type) < 73) &&
                                ((current_node->type) >= 0))
                                 std::cout << symTypeStr[current_node->type];
                            std::cout << std::endl;*/

                        cleanup_stack.pop(index);
                        current_node = current_node->parent;
                    }
                    else
                    {
                        /*if (((current_node->type) < 73) &&
                                ((current_node->type) >= 0))
                                 std::cout << symTypeStr[current_node->type];
                        else
                            std::cout << (current_node->type);
                        std::cout << ":" << current_node->children.size() << "]]\n";*/

                        progress = lex::DONE;
                    }


                    /*if (current_node->parent)
                    {
                        cleanup_stack.pop(index);
                        current_node = current_node->parent;
                    }*/
                    /*else
                    {
                        if (cleanup_stack.pop(index) &&
                            (index < phrase_nodes.size()))
                        {
                            current_node = phrase_nodes[index];
                            cleanup_stack.push(index+1);

                            index = 0;
                        }
                        else
                        {
                            index = 0;
                            progress = lex::DONE;//ERROR_CHECK;
                        }
                    }*/
                }
                else
                {
                    cleanup_stack.push(index+1);

                    current_node = (current_node->children).at(index);

                    index = 0;
                }
            }
        }

        ///debug
        void print_lex_ast(int level, phrase_t* node)
        {
            if (node)
            {
                std::cout << ":";

                for (int i=0; i<level; i++)
                    std::cout << "    ";



                if (node->type == ident::STRING_LITERAL)
                    std::cout << "<" << node->value.string().str() << ">";
                else if (node->type == ident::NUMERIC_LITERAL)
                    std::cout << "<" << node->value.string().str() << ">";
                else if (node->type == ident::COMPLEX_LITERAL)
                    std::cout << "<" << node->value.string().str() << "i>";
                else if (node->type == ident::LITERAL)
                    std::cout << "#const<" << node->value.string().narrow().str() << ">";
                else if (node->type == ident::UNKNOWN)
                    std::cout << "#unkn<" << ((core::string<CPL_CHAR>*)(node->meta))->narrow().str() << ">";
                else if (node->type == ident::IDENTIFIER)
                    std::cout << symTypeStr[node->type] << " : {"\
                     << ((core::string<CPL_CHAR>*)(node->meta))->narrow().str() << "}";
                else
                    std::cout << symTypeStr[node->type];

                if (node->type >= ident::ID_COUNT)
                    std::cout << " ID=" << node->metaValue;

                std::cout << " [" << symTypeStr[node->orig_type] << "]";

                std::cout << std::endl;

                for (int i=0; i<(node->children.size()); i++)
                    print_lex_ast(level+1, node->children[i]);
            }
        }
    }
}
}

/*
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
#include "syntaxRules/func_prototype.h"
#include "syntaxRules/function_decl.h"
#include "syntaxRules/int_decllist.h"
#include "syntaxRules/typedecl.h"
#include "syntaxRules/program.h"
#include "syntaxRules/stop_statement.h"*/


//#include "syntaxErrors/error_oper.h"
//#include "syntaxErrors/error_semicolon.h"
//#include "syntaxErrors/error_for.h"


#endif // LEXER_H_INCLUDED
