/**
 * File:            lexer.h
 * Namespace:       z::script
 * Description:     Script lexer template. The lexer
 *                  takes the list of tokens generated
 *                  by the scanner and groups them into
 *                  functional phrases (e.g. expression,
 *                  scope, etc.)
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   6 Mar. 2017
**/


#pragma once
#ifndef LEXER_H_INCLUDED
#define LEXER_H_INCLUDED

#include <z/core/timeout.h>
#include <z/core/array.h>
#include <z/core/dynamicStack.h>

#include "phrase.h"

#include <iostream>

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
            "#str","#num", "#compl",
            "id",
            "if","else",
            "for","each","in","loop","while",
            "goto","gosub",
            "run","include",
            "break","return",
            "dim","exit",
            "wait","until",
            "var","type","function",
            "global","external","shared",
            "=",
            "+","-",
            "++","--",
            "sizeof",
            "*","/","//","%",
            "^","!",
            "and","&",
            "or","|",
            "xor",":",
            "not","~",
            "nand","~&",
            "nor","~|",
            "nxor", "~:",
            "==","<>",">",">=","<","<=",
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
            "return_statement",
            "variable_decl",
            "typevar_decl",
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
            "negatexpr",
            "powerexpr",
            "multiplyexpr",
            "addexpr",
            "boolexpr",
            "assignexpr",
            "dimensionexpr",
            "sizeofexpr"
        };

        namespace lex
        {
            enum progress
            {
                NONE = 0,
                LEXICALS,
                TREE_CLEANUP,
                DONE
            };
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

            bool found_error;

            int progress;
            bool input_in_use;

            core::dynamic_stack<int> cleanup_stack;

            ///debug
            void print_lex_ast();
            void print_lx_ch(int, phrase_t<CHAR>*);

            void del_nodes(phrase_t<CHAR>* root)
            {
                for (int i=0; i<(root->children.size()); i++)
                {
                    del_nodes(root->children[i]);
                }

                delete root;
            }

            ///phrase detection
            bool identifierlist();
            bool _command();
            bool statementlist();
            bool statement();
            bool if_statement();
            bool for_statement();
            bool foreach_statement();
            bool variable_decl();
            bool typevar_decl();
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
            bool negatexpr();
            bool powerexpr();
            bool multiplyexpr();
            bool addexpr();
            bool boolexpr();
            bool assignexpr();
            bool dimensionexpr();

        public:
            lexer()
            {
                input_ident = NULL;
                progress = lex::NONE;

                index = 0;
                found_error = false;

                input_in_use = true;

                did_concat = false;
            }

            ~lexer()
            {
                for (int i=0; i<phrase_nodes.size(); i++)
                    del_nodes(phrase_nodes[i]);
            }

            void setInput(core::array< ident_t<CHAR> >& identifiers)
            {
                input_ident = &identifiers;

                for (int i=0; i<phrase_nodes.size(); i++)
                    del_nodes(phrase_nodes[i]);
                phrase_nodes.clear();

                progress = lex::NONE;

                index = 0;
                found_error = false;

                did_concat = false;
            }


            bool lex(const core::timeout&);

            inline bool error() {return found_error;}

            inline bool usingInput() {return input_in_use;}
        };


        template <typename CHAR>
        bool lexer<CHAR>::lex(const core::timeout& time)
        {
            while (!time.timedOut() && (progress != lex::DONE))
            {
                if (progress == lex::NONE)
                {
                    if (index >= input_ident->size())
                    {
                        progress = lex::LEXICALS;
                        index = 0;

                        input_in_use = false;
                    }
                    else
                    {
                        phrase_nodes.add(new phrase_t<CHAR>(input_ident->at(index)));

                        index++;
                    }
                }
                else if (progress == lex::LEXICALS)
                {
                    if (index >= phrase_nodes.size())
                    {
                        if (!did_concat)
                        {
                            progress = lex::TREE_CLEANUP;
                            //progress = lex::DONE;
                            current_node = 0;
                        }

                        index = 0;
                        did_concat = false;
                    }

                    else if (identifierlist()||
                             _command()     ||
                             if_statement() ||
                             statementlist()||
                             for_statement()||
                             foreach_statement()||
                             variable_decl()||
                             typevar_decl() ||
                             _index()       ||
                             indexlist()    ||
                             exprlist()     ||
                             _list()        ||
                             funccall()     ||
                             type_funccall()||
                             varindex()     ||
                             typevar()      ||
                             variable()     ||
                             operand()      ||
                             parenthexpr()  ||
                             factorialexpr()||
                             negatexpr()    ||
                             powerexpr()    ||
                             multiplyexpr() ||
                             addexpr()      ||
                             boolexpr()     ||
                             assignexpr()   ||
                             dimensionexpr()||
                             statement()
                             )
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
                                    progress = lex::DONE;
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


            }

            ///debug
            if (progress == lex::DONE)
                print_lex_ast();

            return !time.timedOut();
        }


        ///phrase detection

        template <typename CHAR>
        bool lexer<CHAR>::identifierlist()
        {
            if (!(phrase_nodes.is_valid(index-1) &&
                  (phrase_nodes[index-1]->type == ident::KEYWORD_IN)))
            {
                if (phrase_nodes.is_valid(index+1) &&
                    ((phrase_nodes[index]->type == ident::IDENTIFIER) ||
                     (phrase_nodes[index]->type == phrase::IDENTIFIERLIST)) &&
                    (phrase_nodes[index+1]->type == ident::IDENTIFIER) &&
                    !(phrase_nodes.is_valid(index+2) &&
                      (phrase_nodes[index+2]->type == ident::SEMICOLON)))
                {
                    phrase_t<CHAR>* node = new phrase_t<CHAR>();

                    node->type = phrase::IDENTIFIERLIST;

                    node->line = phrase_nodes[index]->line;
                    node->column = phrase_nodes[index]->column;

                    phrase_nodes[index]->parent = node;
                    phrase_nodes[index+1]->parent = node;

                    node->children.add(phrase_nodes[index]);
                    node->children.add(phrase_nodes[index+1]);

                    node->err = error::NONE;

                    phrase_nodes.replace(index, index+1, node);

                    return true;
                }
                else if (phrase_nodes.is_valid(index+1) &&
                         (phrase_nodes[index]->type == ident::IDENTIFIER) &&
                         (phrase_nodes[index+1]->type == ident::LBRACE))
                {
                    if (phrase_nodes[index]->orig_type == ident::NONE)
                        phrase_nodes[index]->orig_type = phrase_nodes[index]->type;
                    phrase_nodes[index]->type = phrase::IDENTIFIERLIST;

                    return true;
                }
            }

            return false;
        }

        template <typename CHAR>
        bool lexer<CHAR>::_command()
        {
            if (phrase_nodes.is_valid(index+2) &&
                (phrase_nodes[index]->type == phrase::IDENTIFIERLIST) &&
                (phrase_nodes[index+1]->type == phrase::LIST) &&
                (phrase_nodes[index+2]->type == ident::SEMICOLON))
            {
                phrase_t<CHAR>* node = new phrase_t<CHAR>();

                node->type = phrase::COMMAND;

                node->line = phrase_nodes[index]->line;
                node->column = phrase_nodes[index]->column;

                phrase_nodes[index]->parent = node;
                phrase_nodes[index+1]->parent = node;

                node->children.add(phrase_nodes[index]);
                node->children.add(phrase_nodes[index+1]);

                node->err = error::NONE;

                delete phrase_nodes[index+2];
                phrase_nodes.replace(index, index+2, node);

                return true;
            }
            else
                return false;
        }

        template <typename CHAR>
        bool lexer<CHAR>::statement()
        {
            if (phrase_nodes.is_valid(index+1) &&
                 (((phrase_nodes[index]->type == phrase::BOOLEXPR) &&
                   !(phrase_nodes.is_valid(index-1) &&
                     (phrase_nodes[index-1]->type == ident::OPER_ASSIGN))) ||
                  (phrase_nodes[index]->type == phrase::ASSIGNEXPR)) &&
                (phrase_nodes[index+1]->type == ident::SEMICOLON) &&
                !(phrase_nodes.is_valid(index-2) &&
                  (phrase_nodes[index-2]->type == ident::KEYWORD_FOR)) &&
                !(phrase_nodes.is_valid(index-1) &&
                  (phrase_nodes[index-1]->type == ident::SEMICOLON)))
            {
                if (phrase_nodes[index]->orig_type == ident::NONE)
                    phrase_nodes[index]->orig_type = phrase_nodes[index]->type;
                phrase_nodes[index]->type = phrase::STATEMENT;

                delete phrase_nodes[index+1];
                phrase_nodes.remove(index+1);

                return true;
            }
            else if ((phrase_nodes[index]->type == phrase::COMMAND) ||
                     (phrase_nodes[index]->type == phrase::IF_STATEMENT) ||
                     (phrase_nodes[index]->type == phrase::FOR_STATEMENT) ||
                     (phrase_nodes[index]->type == phrase::FOREACH_STATEMENT) ||
                     (phrase_nodes[index]->type == phrase::LOOP_STATEMENT) ||
                     (phrase_nodes[index]->type == phrase::WHILE_PRE_STMT) ||
                     (phrase_nodes[index]->type == phrase::WHILE_POST_STMT) ||
                     (phrase_nodes[index]->type == phrase::RUN_STATEMENT) ||
                     (phrase_nodes[index]->type == phrase::RETURN_STATEMENT) ||
                     (phrase_nodes[index]->type == phrase::WAIT_STATEMENT) ||
                     (phrase_nodes[index]->type == phrase::UNTIL_STATEMENT) ||
                     (phrase_nodes[index]->type == phrase::VARIABLE_DECL) ||
                     (phrase_nodes[index]->type == phrase::TYPEVAR_DECL))
            {
                if (phrase_nodes[index]->orig_type == ident::NONE)
                    phrase_nodes[index]->orig_type = phrase_nodes[index]->type;
                phrase_nodes[index]->type = phrase::STATEMENT;

                return true;
            }
            else
                return false;
        }

        template <typename CHAR>
        bool lexer<CHAR>::statementlist()
        {
            if (phrase_nodes.is_valid(index+1) &&
                ((phrase_nodes[index]->type == phrase::STATEMENT) ||
                 (phrase_nodes[index]->type == phrase::STATEMENTLIST)) &&
                (phrase_nodes[index+1]->type == phrase::STATEMENT))
            {
                phrase_t<CHAR>* node = new phrase_t<CHAR>();

                node->type = phrase::STATEMENTLIST;

                node->line = phrase_nodes[index]->line;
                node->column = phrase_nodes[index]->column;

                phrase_nodes[index]->parent = node;
                phrase_nodes[index+1]->parent = node;

                node->children.add(phrase_nodes[index]);
                node->children.add(phrase_nodes[index+1]);

                node->err = error::NONE;

                phrase_nodes.replace(index, index+1, node);

                return true;
            }
            else
                return false;
        }

        template <typename CHAR>
        bool lexer<CHAR>::if_statement()
        {
            if (phrase_nodes.is_valid(index+3) &&
                (phrase_nodes[index]->type == ident::KEYWORD_IF) &&
                (phrase_nodes[index+1]->type == ident::LPARENTH) &&
                (phrase_nodes[index+2]->type == phrase::BOOLEXPR) &&
                (phrase_nodes[index+3]->type == ident::RPARENTH))
            {
                if (phrase_nodes.is_valid(index+6) &&
                    (phrase_nodes[index+4]->type == ident::LBRACE) &&
                    ((phrase_nodes[index+5]->type == phrase::STATEMENT) ||
                     (phrase_nodes[index+5]->type == phrase::STATEMENTLIST)) &&
                    (phrase_nodes[index+6]->type == ident::RBRACE))
                {
                    phrase_t<CHAR>* node = new phrase_t<CHAR>();

                    node->type = phrase::IF_STATEMENT;

                    node->line = phrase_nodes[index]->line;
                    node->column = phrase_nodes[index]->column;

                    phrase_nodes[index+2]->parent = node;
                    phrase_nodes[index+5]->parent = node;

                    node->children.add(phrase_nodes[index+2]);
                    node->children.add(phrase_nodes[index+5]);

                    node->err = error::NONE;

                    delete phrase_nodes[index];
                    delete phrase_nodes[index+1];
                    delete phrase_nodes[index+3];
                    delete phrase_nodes[index+4];
                    delete phrase_nodes[index+6];
                    phrase_nodes.replace(index, index+6, node);

                    return true;
                }
                else if (phrase_nodes.is_valid(index+5) &&
                    (phrase_nodes[index+4]->type == ident::LBRACE) &&
                    (phrase_nodes[index+5]->type == ident::RBRACE))
                {
                    phrase_t<CHAR>* node = new phrase_t<CHAR>();

                    node->type = phrase::IF_STATEMENT;

                    node->line = phrase_nodes[index]->line;
                    node->column = phrase_nodes[index]->column;

                    phrase_nodes[index+2]->parent = node;

                    node->children.add(phrase_nodes[index+2]);

                    node->err = error::NONE;

                    delete phrase_nodes[index];
                    delete phrase_nodes[index+1];
                    delete phrase_nodes[index+3];
                    delete phrase_nodes[index+4];
                    delete phrase_nodes[index+5];
                    phrase_nodes.replace(index, index+5, node);

                    return true;
                }
                else if (phrase_nodes.is_valid(index+4))
                {
                    if (phrase_nodes[index+4]->type == phrase::STATEMENT)
                    {
                        phrase_t<CHAR>* node = new phrase_t<CHAR>();

                        node->type = phrase::IF_STATEMENT;

                        node->line = phrase_nodes[index]->line;
                        node->column = phrase_nodes[index]->column;

                        phrase_nodes[index+2]->parent = node;
                        phrase_nodes[index+4]->parent = node;

                        node->children.add(phrase_nodes[index+2]);
                        node->children.add(phrase_nodes[index+4]);

                        node->err = error::NONE;

                        delete phrase_nodes[index];
                        delete phrase_nodes[index+1];
                        delete phrase_nodes[index+3];
                        phrase_nodes.replace(index, index+4, node);

                        return true;
                    }
                    else if (phrase_nodes[index+4]->type == ident::SEMICOLON)
                    {
                        phrase_t<CHAR>* node = new phrase_t<CHAR>();

                        node->type = phrase::IF_STATEMENT;

                        node->line = phrase_nodes[index]->line;
                        node->column = phrase_nodes[index]->column;

                        phrase_nodes[index+2]->parent = node;

                        node->children.add(phrase_nodes[index+2]);

                        node->err = error::NONE;

                        delete phrase_nodes[index];
                        delete phrase_nodes[index+1];
                        delete phrase_nodes[index+3];
                        delete phrase_nodes[index+4];
                        phrase_nodes.replace(index, index+4, node);

                        return true;
                    }
                    else
                        return false;
                }
                else
                    return false;
            }
            else if (phrase_nodes.is_valid(index+2) &&
                     (phrase_nodes[index]->type == phrase::IF_STATEMENT) &&
                     (phrase_nodes[index+1]->type == ident::KEYWORD_ELSE))
            {
                if (phrase_nodes.is_valid(index+4) &&
                    (phrase_nodes[index+2]->type == ident::LBRACE) &&
                    ((phrase_nodes[index+3]->type == phrase::STATEMENT) ||
                     (phrase_nodes[index+3]->type == phrase::STATEMENTLIST)) &&
                    (phrase_nodes[index+4]->type == ident::RBRACE))
                {
                    phrase_nodes[index+3]->parent = phrase_nodes[index];

                    phrase_nodes[index]->children.add(phrase_nodes[index+3]);

                    delete phrase_nodes[index+1];
                    delete phrase_nodes[index+2];
                    delete phrase_nodes[index+4];
                    phrase_nodes.remove(index+4);
                    phrase_nodes.remove(index+3);
                    phrase_nodes.remove(index+2);
                    phrase_nodes.remove(index+1);

                    return true;
                }
                else if (phrase_nodes.is_valid(index+3) &&
                         (phrase_nodes[index+2]->type == ident::LBRACE) &&
                         (phrase_nodes[index+3]->type == ident::RBRACE))
                {
                    delete phrase_nodes[index+1];
                    delete phrase_nodes[index+2];
                    delete phrase_nodes[index+3];
                    phrase_nodes.remove(index+3);
                    phrase_nodes.remove(index+2);
                    phrase_nodes.remove(index+1);

                    return true;
                }
                else if (phrase_nodes[index+2]->type == phrase::STATEMENT)
                {
                    phrase_nodes[index+2]->parent = phrase_nodes[index];

                    phrase_nodes[index]->children.add(phrase_nodes[index+2]);

                    delete phrase_nodes[index+1];
                    phrase_nodes.remove(index+2);
                    phrase_nodes.remove(index+1);

                    return true;
                }
                else if (phrase_nodes[index+2]->type == ident::SEMICOLON)
                {
                    delete phrase_nodes[index+1];
                    delete phrase_nodes[index+2];
                    phrase_nodes.remove(index+2);
                    phrase_nodes.remove(index+1);

                    return true;
                }
                else
                    return false;
            }
            else
                return false;
        }

        template <typename CHAR>
        bool lexer<CHAR>::for_statement()
        {
            if (phrase_nodes.is_valid(index+7) &&
                (phrase_nodes[index]->type == ident::KEYWORD_FOR) &&
                (phrase_nodes[index+1]->type == ident::LPARENTH) &&
                (phrase_nodes[index+2]->type == phrase::ASSIGNEXPR) &&
                (phrase_nodes[index+3]->type == ident::SEMICOLON) &&
                (phrase_nodes[index+4]->type == phrase::BOOLEXPR) &&
                (phrase_nodes[index+5]->type == ident::SEMICOLON) &&
                (phrase_nodes[index+6]->type == phrase::ASSIGNEXPR) &&
                ((phrase_nodes[index+7]->type == ident::RPARENTH) ||
                 (phrase_nodes.is_valid(index+8) &&
                  (phrase_nodes[index+7]->type == ident::SEMICOLON) &&
                  (phrase_nodes[index+8]->type == ident::RPARENTH))))
            {
                if (phrase_nodes[index+7]->type == ident::SEMICOLON)
                {
                    delete phrase_nodes[index+7];
                    phrase_nodes.remove(index+7);
                }

                if (phrase_nodes.is_valid(index+10) &&
                    (phrase_nodes[index+8]->type == ident::LBRACE) &&
                    ((phrase_nodes[index+9]->type == phrase::STATEMENT) ||
                     (phrase_nodes[index+9]->type == phrase::STATEMENTLIST)) &&
                    (phrase_nodes[index+10]->type == ident::RBRACE))
                {
                    phrase_t<CHAR>* node = new phrase_t<CHAR>();

                    node->type = phrase::FOR_STATEMENT;

                    node->line = phrase_nodes[index]->line;
                    node->column = phrase_nodes[index]->column;

                    phrase_nodes[index+2]->parent = node;
                    phrase_nodes[index+4]->parent = node;
                    phrase_nodes[index+6]->parent = node;
                    phrase_nodes[index+9]->parent = node;

                    node->children.add(phrase_nodes[index+2]);
                    node->children.add(phrase_nodes[index+4]);
                    node->children.add(phrase_nodes[index+6]);
                    node->children.add(phrase_nodes[index+9]);

                    node->err = error::NONE;

                    delete phrase_nodes[index];
                    delete phrase_nodes[index+1];
                    delete phrase_nodes[index+3];
                    delete phrase_nodes[index+5];
                    delete phrase_nodes[index+7];
                    delete phrase_nodes[index+8];
                    delete phrase_nodes[index+10];
                    phrase_nodes.replace(index, index+10, node);

                    return true;
                }
                else if (phrase_nodes.is_valid(index+9) &&
                    (phrase_nodes[index+8]->type == ident::LBRACE) &&
                    (phrase_nodes[index+9]->type == ident::RBRACE))
                {
                    phrase_t<CHAR>* node = new phrase_t<CHAR>();

                    node->type = phrase::FOR_STATEMENT;

                    node->line = phrase_nodes[index]->line;
                    node->column = phrase_nodes[index]->column;

                    phrase_nodes[index+2]->parent = node;
                    phrase_nodes[index+4]->parent = node;
                    phrase_nodes[index+6]->parent = node;

                    node->children.add(phrase_nodes[index+2]);
                    node->children.add(phrase_nodes[index+4]);
                    node->children.add(phrase_nodes[index+6]);

                    node->err = error::NONE;

                    delete phrase_nodes[index];
                    delete phrase_nodes[index+1];
                    delete phrase_nodes[index+3];
                    delete phrase_nodes[index+5];
                    delete phrase_nodes[index+7];
                    delete phrase_nodes[index+8];
                    delete phrase_nodes[index+9];
                    phrase_nodes.replace(index, index+9, node);

                    return true;
                }
                if (phrase_nodes.is_valid(index+8))
                {
                    if (phrase_nodes[index+8]->type == phrase::STATEMENT)
                    {
                        phrase_t<CHAR>* node = new phrase_t<CHAR>();

                        node->type = phrase::FOR_STATEMENT;

                        node->line = phrase_nodes[index]->line;
                        node->column = phrase_nodes[index]->column;

                        phrase_nodes[index+2]->parent = node;
                        phrase_nodes[index+4]->parent = node;
                        phrase_nodes[index+6]->parent = node;
                        phrase_nodes[index+8]->parent = node;

                        node->children.add(phrase_nodes[index+2]);
                        node->children.add(phrase_nodes[index+4]);
                        node->children.add(phrase_nodes[index+6]);
                        node->children.add(phrase_nodes[index+8]);

                        node->err = error::NONE;

                        delete phrase_nodes[index];
                        delete phrase_nodes[index+1];
                        delete phrase_nodes[index+3];
                        delete phrase_nodes[index+5];
                        delete phrase_nodes[index+7];
                        phrase_nodes.replace(index, index+8, node);

                        return true;
                    }
                    else if (phrase_nodes[index+8]->type == ident::SEMICOLON)
                    {
                        phrase_t<CHAR>* node = new phrase_t<CHAR>();

                        node->type = phrase::FOR_STATEMENT;

                        node->line = phrase_nodes[index]->line;
                        node->column = phrase_nodes[index]->column;

                        phrase_nodes[index+2]->parent = node;
                        phrase_nodes[index+4]->parent = node;
                        phrase_nodes[index+6]->parent = node;

                        node->children.add(phrase_nodes[index+2]);
                        node->children.add(phrase_nodes[index+4]);
                        node->children.add(phrase_nodes[index+6]);

                        node->err = error::NONE;

                        delete phrase_nodes[index];
                        delete phrase_nodes[index+1];
                        delete phrase_nodes[index+3];
                        delete phrase_nodes[index+5];
                        delete phrase_nodes[index+7];
                        delete phrase_nodes[index+8];
                        phrase_nodes.replace(index, index+8, node);

                        return true;
                    }
                    else
                        return false;
                }
                else
                    return false;
            }
            else
                return false;
        }

        template <typename CHAR>
        bool lexer<CHAR>::foreach_statement()
        {
            if (phrase_nodes.is_valid(index+5) &&
                (phrase_nodes[index]->type == ident::KEYWORD_FOR) &&
                (phrase_nodes[index+1]->type == ident::KEYWORD_EACH) &&
                (phrase_nodes[index+2]->type == phrase::VARIABLE) &&
                (phrase_nodes[index+3]->type == ident::KEYWORD_IN) &&
                ((phrase_nodes[index+4]->type == phrase::VARIABLE) ||
                 (phrase_nodes[index+4]->type == phrase::LIST)))
            {
                if (phrase_nodes[index+5]->type == ident::SEMICOLON)
                {
                    phrase_t<CHAR>* node = new phrase_t<CHAR>();

                    node->type = phrase::FOREACH_STATEMENT;

                    node->line = phrase_nodes[index]->line;
                    node->column = phrase_nodes[index]->column;

                    phrase_nodes[index+2]->parent = node;
                    phrase_nodes[index+4]->parent = node;

                    node->children.add(phrase_nodes[index+2]);
                    node->children.add(phrase_nodes[index+4]);

                    node->err = error::NONE;

                    delete phrase_nodes[index];
                    delete phrase_nodes[index+1];
                    delete phrase_nodes[index+3];
                    delete phrase_nodes[index+5];
                    phrase_nodes.replace(index, index+5, node);

                    return true;
                }
                else if (phrase_nodes[index+5]->type == phrase::STATEMENT)
                {
                    phrase_t<CHAR>* node = new phrase_t<CHAR>();

                    node->type = phrase::FOREACH_STATEMENT;

                    node->line = phrase_nodes[index]->line;
                    node->column = phrase_nodes[index]->column;

                    phrase_nodes[index+2]->parent = node;
                    phrase_nodes[index+4]->parent = node;
                    phrase_nodes[index+5]->parent = node;

                    node->children.add(phrase_nodes[index+2]);
                    node->children.add(phrase_nodes[index+4]);
                    node->children.add(phrase_nodes[index+5]);

                    node->err = error::NONE;

                    delete phrase_nodes[index];
                    delete phrase_nodes[index+1];
                    delete phrase_nodes[index+3];
                    phrase_nodes.replace(index, index+5, node);

                    return true;
                }
                else if (phrase_nodes.is_valid(index+6) &&
                         (phrase_nodes[index+5]->type == ident::LBRACE) &&
                         (phrase_nodes[index+6]->type == ident::RBRACE))
                {
                    phrase_t<CHAR>* node = new phrase_t<CHAR>();

                    node->type = phrase::FOREACH_STATEMENT;

                    node->line = phrase_nodes[index]->line;
                    node->column = phrase_nodes[index]->column;

                    phrase_nodes[index+2]->parent = node;
                    phrase_nodes[index+4]->parent = node;

                    node->children.add(phrase_nodes[index+2]);
                    node->children.add(phrase_nodes[index+4]);

                    node->err = error::NONE;

                    delete phrase_nodes[index];
                    delete phrase_nodes[index+1];
                    delete phrase_nodes[index+3];
                    delete phrase_nodes[index+5];
                    delete phrase_nodes[index+6];
                    phrase_nodes.replace(index, index+6, node);

                    return true;
                }
                else if (phrase_nodes.is_valid(index+7) &&
                         (phrase_nodes[index+5]->type == ident::LBRACE) &&
                         ((phrase_nodes[index+6]->type == phrase::STATEMENT) ||
                          (phrase_nodes[index+6]->type == phrase::STATEMENTLIST)) &&
                         (phrase_nodes[index+7]->type == ident::RBRACE))
                {
                    phrase_t<CHAR>* node = new phrase_t<CHAR>();

                    node->type = phrase::FOREACH_STATEMENT;

                    node->line = phrase_nodes[index]->line;
                    node->column = phrase_nodes[index]->column;

                    phrase_nodes[index+2]->parent = node;
                    phrase_nodes[index+4]->parent = node;
                    phrase_nodes[index+6]->parent = node;

                    node->children.add(phrase_nodes[index+2]);
                    node->children.add(phrase_nodes[index+4]);
                    node->children.add(phrase_nodes[index+6]);

                    node->err = error::NONE;

                    delete phrase_nodes[index];
                    delete phrase_nodes[index+1];
                    delete phrase_nodes[index+3];
                    delete phrase_nodes[index+5];
                    delete phrase_nodes[index+7];
                    phrase_nodes.replace(index, index+7, node);

                    return true;
                }
            }

            return false;
        }

        template <typename CHAR>
        bool lexer<CHAR>::variable_decl()
        {
            if (phrase_nodes.is_valid(index+2) &&
                (phrase_nodes[index]->type == ident::KEYWORD_VAR) &&
                (phrase_nodes[index+1]->type == ident::IDENTIFIER) &&
                (phrase_nodes[index+2]->type == ident::SEMICOLON))
            {
                phrase_t<CHAR>* node = new phrase_t<CHAR>();

                node->type = phrase::VARIABLE_DECL;

                node->line = phrase_nodes[index]->line;
                node->column = phrase_nodes[index]->column;

                phrase_nodes[index+1]->parent = node;

                node->children.add(phrase_nodes[index+1]);

                node->err = error::NONE;

                delete phrase_nodes[index];
                delete phrase_nodes[index+2];
                phrase_nodes.replace(index, index+2, node);

                return true;
            }
            else
                return false;
        }

        template <typename CHAR>
        bool lexer<CHAR>::typevar_decl()
        {
            if (phrase_nodes.is_valid(index+2) &&
                (phrase_nodes[index]->type == ident::IDENTIFIER) &&
                (phrase_nodes[index+1]->type == ident::IDENTIFIER) &&
                (phrase_nodes[index+2]->type == ident::SEMICOLON))
            {
                phrase_t<CHAR>* node = new phrase_t<CHAR>();

                node->type = phrase::TYPEVAR_DECL;

                node->line = phrase_nodes[index]->line;
                node->column = phrase_nodes[index]->column;

                phrase_nodes[index]->parent = node;
                phrase_nodes[index+1]->parent = node;

                node->children.add(phrase_nodes[index]);
                node->children.add(phrase_nodes[index+1]);

                node->err = error::NONE;

                delete phrase_nodes[index+2];
                phrase_nodes.replace(index, index+2, node);

                return true;
            }
            else
                return false;
        }

        template <typename CHAR>
        bool lexer<CHAR>::_index()
        {
            if (phrase_nodes.is_valid(index+2) &&
                (phrase_nodes[index+1]->type == phrase::BOOLEXPR) &&
                (phrase_nodes[index]->type == ident::LBRACKET) &&
                (phrase_nodes[index+2]->type == ident::RBRACKET))
            {
                phrase_t<CHAR>* node = new phrase_t<CHAR>();

                node->type = phrase::INDEX;

                node->line = phrase_nodes[index]->line;
                node->column = phrase_nodes[index]->column;

                phrase_nodes[index+1]->parent = node;

                node->children.add(phrase_nodes[index+1]);

                node->err = error::NONE;

                delete phrase_nodes[index];
                delete phrase_nodes[index+2];
                phrase_nodes.replace(index, index+2, node);

                return true;
            }
            else
                return false;
        }

        template <typename CHAR>
        bool lexer<CHAR>::indexlist()
        {
            if (phrase_nodes.is_valid(index-1) &&
                ((phrase_nodes[index-1]->type == phrase::INDEX) ||
                 (phrase_nodes[index-1]->type == phrase::INDEXLIST)) &&
                (phrase_nodes[index]->type == phrase::INDEX))
            {
                phrase_t<CHAR>* node = new phrase_t<CHAR>();

                node->type = phrase::INDEXLIST;

                node->line = phrase_nodes[index]->line;
                node->column = phrase_nodes[index]->column;

                phrase_nodes[index-1]->parent = node;
                phrase_nodes[index]->parent = node;

                node->children.add(phrase_nodes[index-1]);
                node->children.add(phrase_nodes[index]);

                node->err = error::NONE;

                phrase_nodes.replace(index-1, index, node);

                return true;
            }
            else
                return false;
        }

        template <typename CHAR>
        bool lexer<CHAR>::exprlist()
        {
            if (phrase_nodes.is_valid(index+2) &&
                ((phrase_nodes[index]->type == phrase::BOOLEXPR) ||
                 (phrase_nodes[index]->type == phrase::EXPRLIST)) &&
                (phrase_nodes[index+1]->type == ident::COMMA) &&
                (phrase_nodes[index+2]->type == phrase::BOOLEXPR))
            {
                phrase_t<CHAR>* node = new phrase_t<CHAR>();

                node->type = phrase::EXPRLIST;

                node->line = phrase_nodes[index]->line;
                node->column = phrase_nodes[index]->column;

                phrase_nodes[index]->parent = node;
                phrase_nodes[index+2]->parent = node;

                node->children.add(phrase_nodes[index]);
                node->children.add(phrase_nodes[index+2]);

                node->err = error::NONE;

                delete phrase_nodes[index+1];
                phrase_nodes.replace(index, index+2, node);

                return true;
            }
            else
                return false;
        }

        template <typename CHAR>
        bool lexer<CHAR>::_list()
        {
            if (!(phrase_nodes.is_valid(index-2) &&
                  (phrase_nodes[index-2]->type == ident::KEYWORD_IN)))
            {
                if (phrase_nodes.is_valid(index+2) &&
                    ((phrase_nodes[index+1]->type == phrase::BOOLEXPR) ||
                     (phrase_nodes[index+1]->type == phrase::EXPRLIST)) &&
                    (phrase_nodes[index]->type == ident::LBRACE) &&
                    (phrase_nodes[index+2]->type == ident::RBRACE))
                {
                    phrase_t<CHAR>* node = new phrase_t<CHAR>();

                    node->type = phrase::LIST;

                    node->line = phrase_nodes[index]->line;
                    node->column = phrase_nodes[index]->column;

                    phrase_nodes[index+1]->parent = node;

                    node->children.add(phrase_nodes[index+1]);

                    node->err = error::NONE;

                    delete phrase_nodes[index];
                    delete phrase_nodes[index+2];
                    phrase_nodes.replace(index, index+2, node);

                    return true;
                }
                else if (phrase_nodes.is_valid(index+1) &&
                    (phrase_nodes[index]->type == ident::LBRACE) &&
                    (phrase_nodes[index+1]->type == ident::RBRACE) &&
                    !(phrase_nodes.is_valid(index-1) &&
                      ((phrase_nodes[index-1]->type == ident::RPARENTH) ||
                       (phrase_nodes[index-1]->type == ident::KEYWORD_ELSE) ||
                       (phrase_nodes[index-1]->type == ident::KEYWORD_LOOP))))
                {
                    phrase_t<CHAR>* node = new phrase_t<CHAR>();

                    node->type = phrase::LIST;

                    node->line = phrase_nodes[index]->line;
                    node->column = phrase_nodes[index]->column;

                    node->err = error::NONE;

                    delete phrase_nodes[index];
                    delete phrase_nodes[index+1];
                    phrase_nodes.replace(index, index+1, node);

                    return true;
                }
            }

            return false;
        }

        template <typename CHAR>
        bool lexer<CHAR>::funccall()
        {
            if (phrase_nodes.is_valid(index+3) &&
                (phrase_nodes[index]->type == ident::IDENTIFIER) &&
                ((phrase_nodes[index+2]->type == phrase::BOOLEXPR) ||
                 (phrase_nodes[index+2]->type == phrase::EXPRLIST)) &&
                (phrase_nodes[index+1]->type == ident::LPARENTH) &&
                (phrase_nodes[index+3]->type == ident::RPARENTH))
            {
                phrase_t<CHAR>* node = new phrase_t<CHAR>();

                node->type = phrase::FUNCCALL;

                node->line = phrase_nodes[index]->line;
                node->column = phrase_nodes[index]->column;

                phrase_nodes[index]->parent = node;
                phrase_nodes[index+2]->parent = node;

                node->children.add(phrase_nodes[index]);
                node->children.add(phrase_nodes[index+2]);

                node->err = error::NONE;

                delete phrase_nodes[index+1];
                delete phrase_nodes[index+3];
                phrase_nodes.replace(index, index+3, node);

                return true;
            }
            else if (phrase_nodes.is_valid(index+2) &&
                (phrase_nodes[index]->type == ident::IDENTIFIER) &&
                (phrase_nodes[index+1]->type == ident::LPARENTH) &&
                (phrase_nodes[index+2]->type == ident::RPARENTH))
            {
                phrase_t<CHAR>* node = new phrase_t<CHAR>();

                node->type = phrase::FUNCCALL;

                node->line = phrase_nodes[index]->line;
                node->column = phrase_nodes[index]->column;

                phrase_nodes[index]->parent = node;

                node->children.add(phrase_nodes[index]);

                node->err = error::NONE;

                delete phrase_nodes[index+1];
                delete phrase_nodes[index+2];
                phrase_nodes.replace(index, index+2, node);

                return true;
            }
            else
                return false;
        }

        template <typename CHAR>
        bool lexer<CHAR>::type_funccall()
        {
            if (phrase_nodes.is_valid(index+2) &&
                (phrase_nodes[index]->type == ident::IDENTIFIER) &&
                (phrase_nodes[index+1]->type == ident::PERIOD) &&
                (phrase_nodes[index+2]->type == phrase::FUNCCALL))
            {
                phrase_t<CHAR>* node = new phrase_t<CHAR>();

                node->type = phrase::TYPE_FUNCCALL;

                node->line = phrase_nodes[index]->line;
                node->column = phrase_nodes[index]->column;

                node->err = error::NONE;

                phrase_nodes[index]->parent = node;
                phrase_nodes[index+2]->parent = node;

                node->children.add(phrase_nodes[index]);
                node->children.add(phrase_nodes[index+2]);

                delete phrase_nodes[index+1];
                phrase_nodes.replace(index, index+2, node);

                return true;
            }
            else
                return false;
        }

        template <typename CHAR>
        bool lexer<CHAR>::varindex()
        {
            if (phrase_nodes.is_valid(index+1) &&
                (phrase_nodes[index]->type == ident::IDENTIFIER) &&
                ((phrase_nodes[index+1]->type == phrase::INDEX) ||
                 (phrase_nodes[index+1]->type == phrase::INDEXLIST))
                )
            {
                phrase_t<CHAR>* node = new phrase_t<CHAR>();

                node->type = phrase::VARINDEX;

                node->line = phrase_nodes[index]->line;
                node->column = phrase_nodes[index]->column;

                phrase_nodes[index]->parent = node;
                phrase_nodes[index+1]->parent = node;

                node->children.add(phrase_nodes[index]);
                node->children.add(phrase_nodes[index+1]);

                node->err = error::NONE;

                phrase_nodes.replace(index, index+1, node);

                return true;
            }
            else
                return false;
        }

        template <typename CHAR>
        bool lexer<CHAR>::typevar()
        {
            if (phrase_nodes.is_valid(index+2) &&
                (phrase_nodes[index]->type == ident::IDENTIFIER) &&
                (phrase_nodes[index+1]->type == ident::PERIOD) &&
                ((phrase_nodes[index+2]->type == phrase::VARINDEX) ||
                 ((phrase_nodes[index+2]->type == ident::IDENTIFIER) &&
                  !(phrase_nodes.is_valid(index+3) &&
                    ((phrase_nodes[index+3]->type == ident::LBRACKET) ||
                     (phrase_nodes[index+3]->type == ident::LPARENTH) ||
                     (phrase_nodes[index+3]->type == phrase::INDEXLIST) ||
                     (phrase_nodes[index+3]->type == phrase::INDEX) )))))
            {
                phrase_t<CHAR>* node = new phrase_t<CHAR>();

                node->type = phrase::TYPEVAR;

                node->line = phrase_nodes[index]->line;
                node->column = phrase_nodes[index]->column;

                phrase_nodes[index]->parent = node;
                phrase_nodes[index+2]->parent = node;

                node->children.add(phrase_nodes[index]);
                node->children.add(phrase_nodes[index+2]);

                node->err = error::NONE;

                delete phrase_nodes[index+1];
                phrase_nodes.replace(index, index+2, node);

                return true;
            }
            else
                return false;
        }

        template <typename CHAR>
        bool lexer<CHAR>::variable()
        {
            if ((phrase_nodes[index]->type == phrase::TYPEVAR) ||
                (phrase_nodes[index]->type == phrase::VARINDEX) ||
                ((phrase_nodes[index]->type == ident::IDENTIFIER) &&
                     !(phrase_nodes.is_valid(index+1) &&
                       ((phrase_nodes[index+1]->type == ident::PERIOD) ||
                       (phrase_nodes[index+1]->type == ident::LBRACKET) ||
                       (phrase_nodes[index+1]->type == ident::LPARENTH)))))
            {
                if (phrase_nodes[index]->orig_type == ident::NONE)
                    phrase_nodes[index]->orig_type = phrase_nodes[index]->type;
                phrase_nodes[index]->type = phrase::VARIABLE;

                return true;
            }
            else
                return false;
        }

        template <typename CHAR>
        bool lexer<CHAR>::operand()
        {
            if (!(phrase_nodes.is_valid(index-1) &&
                   ((phrase_nodes[index-1]->type == ident::KEYWORD_EACH) ||
                    (phrase_nodes[index-1]->type == ident::KEYWORD_IN))))
            {
                if (((phrase_nodes[index]->type == phrase::VARIABLE) &&
                     !(phrase_nodes.is_valid(index+1) &&
                       (phrase_nodes[index+1]->type == ident::OPER_ASSIGN))) ||
                    (phrase_nodes[index]->type == ident::NUMERIC_LITERAL) ||
                    (phrase_nodes[index]->type == ident::COMPLEX_LITERAL) ||
                    (phrase_nodes[index]->type == ident::STRING_LITERAL) ||
                    ((phrase_nodes[index]->type == phrase::LIST) &&
                      !(phrase_nodes.is_valid(index-1) &&
                       ((phrase_nodes[index-1]->type == phrase::IDENTIFIERLIST) ||
                        (phrase_nodes[index-1]->type == ident::IDENTIFIER)))) ||
                    (phrase_nodes[index]->type == phrase::TYPE_FUNCCALL) ||
                    ((phrase_nodes[index]->type == phrase::FUNCCALL) &&
                     !(phrase_nodes.is_valid(index-1) &&
                       (phrase_nodes[index-1]->type == ident::PERIOD))))
                {
                    if (phrase_nodes[index]->orig_type == ident::NONE)
                        phrase_nodes[index]->orig_type = phrase_nodes[index]->type;
                    phrase_nodes[index]->type = phrase::OPERAND;

                    return true;
                }
            }

            return false;
        }

        template <typename CHAR>
        bool lexer<CHAR>::parenthexpr()
        {

            if (phrase_nodes.is_valid(index+2) &&
                (phrase_nodes[index]->type == ident::LPARENTH) &&
                (phrase_nodes[index+2]->type == ident::RPARENTH) &&
                (phrase_nodes[index+1]->type == phrase::BOOLEXPR) &&
                     !(phrase_nodes.is_valid(index-1) &&
                       ((phrase_nodes[index-1]->type == ident::IDENTIFIER) ||
                        (phrase_nodes[index-1]->type == ident::KEYWORD_IF) ||
                        (phrase_nodes[index-1]->type == ident::KEYWORD_WHILE))))
            {
                if (phrase_nodes[index+1]->orig_type == ident::NONE)
                    phrase_nodes[index+1]->orig_type = phrase_nodes[index]->type;
                phrase_nodes[index+1]->type = phrase::PARENTHEXPR;

                delete phrase_nodes[index];
                delete phrase_nodes[index+2];
                phrase_nodes.remove(index+2);
                phrase_nodes.remove(index);

                return true;
            }
            else if ((phrase_nodes[index]->type == phrase::OPERAND) ||
                     (phrase_nodes[index]->type == phrase::SIZEOFEXPR))
            {
                if (phrase_nodes[index]->orig_type == ident::NONE)
                    phrase_nodes[index]->orig_type = phrase_nodes[index]->type;
                phrase_nodes[index]->type = phrase::PARENTHEXPR;

                return true;
            }
            else
                return false;
        }

        template <typename CHAR>
        bool lexer<CHAR>::factorialexpr()
        {
            if ((phrase_nodes[index]->type == phrase::PARENTHEXPR) &&
                (phrase_nodes.is_valid(index+1) &&
                      (phrase_nodes[index+1]->type == ident::OPER_FAC)))
            {
                phrase_t<CHAR>* node = new phrase_t<CHAR>();

                node->type = phrase::FACTORIALEXPR;

                node->line = phrase_nodes[index]->line;
                node->column = phrase_nodes[index]->column;

                node->children.add(phrase_nodes[index]);

                node->err = error::NONE;

                phrase_nodes[index]->parent = node;

                delete phrase_nodes[index+1];
                phrase_nodes.replace(index, index+1, node);

                return true;
            }
            else if (phrase_nodes[index]->type == phrase::PARENTHEXPR)
            {
                if (phrase_nodes[index]->orig_type == ident::NONE)
                    phrase_nodes[index]->orig_type = phrase_nodes[index]->type;
                phrase_nodes[index]->type = phrase::FACTORIALEXPR;

                return true;
            }
            else
                return false;
        }

        template <typename CHAR>
        bool lexer<CHAR>::negatexpr()
        {
            if ((phrase_nodes[index]->type == phrase::FACTORIALEXPR) &&
                (phrase_nodes.is_valid(index-1) &&
                      (phrase_nodes[index-1]->type == ident::OPER_SUB)) &&
                    !(phrase_nodes.is_valid(index-2) &&
                      (phrase_nodes[index-2]->type >= phrase::NEGATEXPR)))
            {

                phrase_t<CHAR>* node = new phrase_t<CHAR>();

                node->type = phrase::NEGATEXPR;

                node->line = phrase_nodes[index]->line;
                node->column = phrase_nodes[index]->column;

                node->children.add(phrase_nodes[index]);

                node->err = error::NONE;

                phrase_nodes[index]->parent = node;

                delete phrase_nodes[index-1];
                phrase_nodes.replace(index-1, index, node);

                return true;
            }
            else if (phrase_nodes[index]->type == phrase::FACTORIALEXPR)
            {
                if (phrase_nodes[index]->orig_type == ident::NONE)
                    phrase_nodes[index]->orig_type = phrase_nodes[index]->type;
                phrase_nodes[index]->type = phrase::NEGATEXPR;

                return true;
            }
            else
                return false;
        }

        template <typename CHAR>
        bool lexer<CHAR>::powerexpr()
        {
            //if no detected power operators, continue to the next phase
            if ((phrase_nodes[index]->type == phrase::NEGATEXPR) &&
                !(phrase_nodes.is_valid(index+1) &&
                  (phrase_nodes[index+1]->type == ident::OPER_POW)) &&
                !(phrase_nodes.is_valid(index-1) &&
                  (phrase_nodes[index-1]->type == ident::OPER_POW)))
            {
                if (phrase_nodes[index]->orig_type == ident::NONE)
                    phrase_nodes[index]->orig_type = phrase_nodes[index]->type;
                phrase_nodes[index]->type = phrase::POWEREXPR;

                return true;
            }
            //otherwise, if a power operator is detected
            else if (phrase_nodes.is_valid(index+2) &&
                     ((phrase_nodes[index]->type == phrase::NEGATEXPR) ||
                      (phrase_nodes[index]->type == phrase::POWEREXPR)) &&
                     (phrase_nodes[index+1]->type == ident::OPER_POW) &&
                     (phrase_nodes[index+2]->type == phrase::NEGATEXPR))
            {
                phrase_t<CHAR>* node = new phrase_t<CHAR>();

                node->type = phrase::POWEREXPR;

                node->line = phrase_nodes[index]->line;
                node->column = phrase_nodes[index]->column;

                node->err = error::NONE;

                phrase_nodes[index]->parent = node;
                phrase_nodes[index+2]->parent = node;

                node->children.add(phrase_nodes[index]);
                node->children.add(phrase_nodes[index+2]);

                delete phrase_nodes[index+1];
                phrase_nodes.replace(index, index+2, node);

                return true;
            }
            else
                return false;
        }

        template <typename CHAR>
        bool lexer<CHAR>::multiplyexpr()
        {
            //if no detected multiplication operators, continue to the next phase
            if ((phrase_nodes[index]->type == phrase::POWEREXPR) &&
                !(phrase_nodes.is_valid(index+1) &&
                  (phrase_nodes[index+1]->type >= ident::OPER_MUL) &&
                  (phrase_nodes[index+1]->type <= ident::OPER_MOD)) &&
                !(phrase_nodes.is_valid(index-1) &&
                  (phrase_nodes[index-1]->type >= ident::OPER_MUL) &&
                  (phrase_nodes[index-1]->type <= ident::OPER_MOD)))
            {
                if (phrase_nodes[index]->orig_type == ident::NONE)
                    phrase_nodes[index]->orig_type = phrase_nodes[index]->type;
                phrase_nodes[index]->type = phrase::MULTIPLYEXPR;

                return true;
            }
            //otherwise, if a multiplication operator is detected
            else if (phrase_nodes.is_valid(index+2) &&
                     ((phrase_nodes[index]->type == phrase::POWEREXPR) ||
                      (phrase_nodes[index]->type == phrase::MULTIPLYEXPR)) &&
                     (phrase_nodes[index+1]->type >= ident::OPER_MUL) &&
                     (phrase_nodes[index+1]->type <= ident::OPER_MOD) &&
                     (phrase_nodes[index+2]->type == phrase::POWEREXPR))
            {
                phrase_t<CHAR>* node = new phrase_t<CHAR>();

                node->type = phrase::MULTIPLYEXPR;

                node->line = phrase_nodes[index]->line;
                node->column = phrase_nodes[index]->column;

                node->err = error::NONE;

                phrase_nodes[index]->parent = node;
                phrase_nodes[index+1]->parent = node;
                phrase_nodes[index+2]->parent = node;

                node->children.add(phrase_nodes[index]);
                node->children.add(phrase_nodes[index+1]);
                node->children.add(phrase_nodes[index+2]);

                phrase_nodes.replace(index, index+2, node);

                return true;
            }
            else
                return false;
        }

        template <typename CHAR>
        bool lexer<CHAR>::addexpr()
        {
            //if no detected addition operators, continue to the next phase
            if ((phrase_nodes[index]->type == phrase::MULTIPLYEXPR) &&
                !(phrase_nodes.is_valid(index+1) &&
                  (phrase_nodes[index+1]->type >= ident::OPER_ADD) &&
                  (phrase_nodes[index+1]->type <= ident::OPER_SUB)) &&
                !(phrase_nodes.is_valid(index-1) &&
                  (phrase_nodes[index-1]->type >= ident::OPER_ADD) &&
                  (phrase_nodes[index-1]->type <= ident::OPER_SUB)))
            {
                if (phrase_nodes[index]->orig_type == ident::NONE)
                    phrase_nodes[index]->orig_type = phrase_nodes[index]->type;
                phrase_nodes[index]->type = phrase::ADDEXPR;

                return true;
            }
            //otherwise, if an addition operator is detected
            else if (phrase_nodes.is_valid(index+2) &&
                     ((phrase_nodes[index]->type == phrase::MULTIPLYEXPR) ||
                      (phrase_nodes[index]->type == phrase::ADDEXPR)) &&
                     (phrase_nodes[index+1]->type >= ident::OPER_ADD) &&
                     (phrase_nodes[index+1]->type <= ident::OPER_SUB) &&
                     (phrase_nodes[index+2]->type == phrase::MULTIPLYEXPR))
            {
                phrase_t<CHAR>* node = new phrase_t<CHAR>();

                node->type = phrase::ADDEXPR;

                node->line = phrase_nodes[index]->line;
                node->column = phrase_nodes[index]->column;

                node->err = error::NONE;

                phrase_nodes[index]->parent = node;
                phrase_nodes[index+1]->parent = node;
                phrase_nodes[index+2]->parent = node;

                node->children.add(phrase_nodes[index]);
                node->children.add(phrase_nodes[index+1]);
                node->children.add(phrase_nodes[index+2]);

                phrase_nodes.replace(index, index+2, node);

                return true;
            }
            else
                return false;
        }

        template <typename CHAR>
        bool lexer<CHAR>::boolexpr()
        {
            //if no detected addition operators, continue to the next phase
            if ((phrase_nodes[index]->type == phrase::ADDEXPR) &&
                !(phrase_nodes.is_valid(index+1) &&
                  (phrase_nodes[index+1]->type >= ident::OPER_AND_LGCL) &&
                  (phrase_nodes[index+1]->type <= ident::OPER_LT_EQ)) &&
                !(phrase_nodes.is_valid(index-1) &&
                  (phrase_nodes[index-1]->type >= ident::OPER_AND_LGCL) &&
                  (phrase_nodes[index-1]->type <= ident::OPER_LT_EQ)))
            {
                if (phrase_nodes[index]->orig_type == ident::NONE)
                    phrase_nodes[index]->orig_type = phrase_nodes[index]->type;
                phrase_nodes[index]->type = phrase::BOOLEXPR;

                return true;
            }
            //otherwise, if an addition operator is detected
            else if (phrase_nodes.is_valid(index+2) &&
                     ((phrase_nodes[index]->type == phrase::ADDEXPR) ||
                      (phrase_nodes[index]->type == phrase::BOOLEXPR)) &&
                     (phrase_nodes[index+1]->type >= ident::OPER_AND_LGCL) &&
                     (phrase_nodes[index+1]->type <= ident::OPER_LT_EQ) &&
                     (phrase_nodes[index+2]->type == phrase::ADDEXPR))
            {
                phrase_t<CHAR>* node = new phrase_t<CHAR>();

                node->type = phrase::BOOLEXPR;

                node->line = phrase_nodes[index]->line;
                node->column = phrase_nodes[index]->column;

                node->err = error::NONE;

                phrase_nodes[index]->parent = node;
                phrase_nodes[index+1]->parent = node;
                phrase_nodes[index+2]->parent = node;

                node->children.add(phrase_nodes[index]);
                node->children.add(phrase_nodes[index+1]);
                node->children.add(phrase_nodes[index+2]);

                phrase_nodes.replace(index, index+2, node);

                return true;
            }
            else
                return false;
        }

        template <typename CHAR>
        bool lexer<CHAR>::assignexpr()
        {
            if (phrase_nodes.is_valid(index+2) &&
                (phrase_nodes[index]->type == phrase::VARIABLE) &&
                (phrase_nodes[index+1]->type == ident::OPER_ASSIGN) &&
                ((phrase_nodes[index+2]->type == phrase::BOOLEXPR) ||
                 (phrase_nodes[index+2]->type == phrase::DIMENSIONEXPR)))
            {
                phrase_t<CHAR>* node = new phrase_t<CHAR>();

                node->type = phrase::ASSIGNEXPR;

                node->line = phrase_nodes[index]->line;
                node->column = phrase_nodes[index]->column;

                node->err = error::NONE;

                phrase_nodes[index]->parent = node;
                phrase_nodes[index+2]->parent = node;

                node->children.add(phrase_nodes[index]);
                node->children.add(phrase_nodes[index+2]);

                delete phrase_nodes[index+1];
                phrase_nodes.replace(index, index+2, node);

                return true;
            }
            else
                return false;
        }

        template <typename CHAR>
        bool lexer<CHAR>::dimensionexpr()
        {
            if (phrase_nodes.is_valid(index-1) &&
                (phrase_nodes[index-1]->type == ident::KEYWORD_DIM) &&
                (phrase_nodes[index]->type == phrase::BOOLEXPR))
            {
                phrase_t<CHAR>* node = new phrase_t<CHAR>();

                node->type = phrase::DIMENSIONEXPR;

                node->line = phrase_nodes[index]->line;
                node->column = phrase_nodes[index]->column;

                node->err = error::NONE;

                phrase_nodes[index]->parent = node;

                node->children.add(phrase_nodes[index]);

                delete phrase_nodes[index-1];
                phrase_nodes.replace(index-1, index, node);

                return true;
            }
            else
                return false;
        }

        ///debug
        template <typename CHAR>
        void lexer<CHAR>::print_lex_ast()
        {
            for (int n=0; n<phrase_nodes.size(); n++)
            {
                print_lx_ch(0, phrase_nodes[n]);
            }
        }

        ///debug
        template <typename CHAR>
        void lexer<CHAR>::print_lx_ch(int level, phrase_t<CHAR>* node)
        {
            for (int i=0; i<level; i++)
                std::cout << "    ";



            if (node->type == ident::STRING_LITERAL)
                std::cout << "<" << ((core::string<CHAR>*)(node->meta))->str() << ">";
            else if (node->type == ident::NUMERIC_LITERAL)
                std::cout << "<" << node->value << ">";
            else if (node->type == ident::COMPLEX_LITERAL)
                std::cout << "<" << node->value << "i>";
            else if (node->type == ident::IDENTIFIER)
                std::cout << symTypeStr[node->type] << " : {"\
                 << ((core::string<CHAR>*)(node->meta))->str() << "}";
            else
                std::cout << symTypeStr[node->type];

            std::cout << std::endl;

            for (int i=0; i<(node->children.size()); i++)
                print_lx_ch(level+1, node->children[i]);
        }
    }
}

#endif // LEXER_H_INCLUDED
