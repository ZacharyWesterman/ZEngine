/**
 * File:            lexerPhraseDetection.h
 * Namespace:       z::script
 * Description:     Implementation of the lexer's
 *                  syntax generating member functions.
 *                  This file should NOT be included
 *                  in a project. Include "lexer.h"
 *                  instead,and put this file in the
 *                  same folder.
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   12 Jul. 2017
**/

#pragma once
#ifndef LEXERPHRASEDETECTION_H_INCLUDED
#define LEXERPHRASEDETECTION_H_INCLUDED

namespace z
{
    namespace script
    {
        ///phrase detection

        template <typename CHAR>
        bool lexer<CHAR>::identifierlist()
        {
            if (!(phrase_nodes.is_valid(index-1) &&
                  ((phrase_nodes[index-1]->type == ident::KEYWORD_IN) ||
                   (phrase_nodes[index-1]->type == ident::KEYWORD_SUB) ||
                   (phrase_nodes[index-1]->type == ident::KEYWORD_TYPE) )))
            {
                if (phrase_nodes.is_valid(index+1) &&
                    ((phrase_nodes[index]->type == ident::IDENTIFIER) ||
                     (phrase_nodes[index]->type == phrase::IDENTIFIERLIST)) &&
                    (phrase_nodes[index+1]->type == ident::IDENTIFIER) &&
                    !(phrase_nodes.is_valid(index+2) &&
                      ((phrase_nodes[index+2]->type == ident::SEMICOLON) ||
                       (phrase_nodes[index+2]->type == ident::OPER_ASSIGN))))
                {
                    phrase_t<CHAR>* node = new phrase_t<CHAR>();

                    node->type = phrase::IDENTIFIERLIST;

                    node->line = phrase_nodes[index]->line;
                    node->column = phrase_nodes[index]->column;

                    phrase_nodes[index]->parent = node;
                    phrase_nodes[index+1]->parent = node;

                    node->children.add(phrase_nodes[index]);
                    node->children.add(phrase_nodes[index+1]);

                    node->file = phrase_nodes[index]->file;

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

                node->file = phrase_nodes[index]->file;

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
            if (phrase_nodes.is_valid(index-1) &&
                ((phrase_nodes[index-1]->type == ident::LBRACE) ||
                 (phrase_nodes[index-1]->type == phrase::STATEMENT)))
            {
                if (phrase_nodes.is_valid(index+1) &&
                     ((phrase_nodes[index]->type == phrase::BOOLEXPR) &&
                       !(((phrase_nodes[index-1]->type >= ident::OPER_ASSIGN) &&
                           (phrase_nodes[index-1]->type <= ident::OPER_MOD_ASSIGN)) ||
                          (phrase_nodes[index-1]->type == ident::KEYWORD_RUN) ||
                          (phrase_nodes[index-1]->type == ident::KEYWORD_RETURN) ||
                          (phrase_nodes[index-1]->type == ident::KEYWORD_WAIT) ||
                          (phrase_nodes[index-1]->type == ident::KEYWORD_UNTIL) ||
                          (phrase_nodes[index-1]->type == ident::LBRACKET) ||
                          (phrase_nodes[index-1]->type == phrase::RANGELIST) ||
                          (phrase_nodes[index-1]->type == ident::OPER_R_ARROW) ||
                          (phrase_nodes[index-1]->type == ident::OPER_L_ARROW))) &&
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
                         ((phrase_nodes[index]->type == phrase::IF_STATEMENT) &&
                          !(phrase_nodes.is_valid(index+1) &&
                            phrase_nodes[index+1]->type == ident::KEYWORD_ELSE)) ||
                         (phrase_nodes[index]->type == phrase::FOR_STATEMENT) ||
                         (phrase_nodes[index]->type == phrase::FOREACH_STATEMENT) ||
                         (phrase_nodes[index]->type == phrase::LOOP_STATEMENT) ||
                         (phrase_nodes[index]->type == phrase::WHILE_PRE_STMT) ||
                         (phrase_nodes[index]->type == phrase::WHILE_POST_STMT) ||
                         (phrase_nodes[index]->type == phrase::RUN_STATEMENT) ||
                         (phrase_nodes[index]->type == phrase::RETURN_STATEMENT) ||
                         (phrase_nodes[index]->type == phrase::WAIT_STATEMENT) ||
                         (phrase_nodes[index]->type == phrase::UNTIL_STATEMENT) ||
                         (((phrase_nodes[index]->type == phrase::VARIABLE_DECL) ||
                           (phrase_nodes[index]->type == phrase::TYPEVAR_DECL)) &&
                           !(phrase_nodes.is_valid(index-3) &&
                             (phrase_nodes[index-3]->type == ident::KEYWORD_TYPE))) ||
                         (phrase_nodes[index]->type == phrase::LABEL_STATEMENT) ||
                         (phrase_nodes[index]->type == phrase::GOTO_STATEMENT) ||
                         (phrase_nodes[index]->type == phrase::GOSUB_STATEMENT))
                {
                    if (phrase_nodes[index]->orig_type == ident::NONE)
                        phrase_nodes[index]->orig_type = phrase_nodes[index]->type;
                    phrase_nodes[index]->type = phrase::STATEMENT;

                    return true;
                }
            }

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

                node->file = phrase_nodes[index]->file;

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

                    node->file = phrase_nodes[index]->file;

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
                (phrase_nodes[index+2]->type == phrase::BOOLEXPR) &&
                (phrase_nodes[index+3]->type == ident::SEMICOLON) &&
                (phrase_nodes[index+4]->type == phrase::BOOLEXPR) &&
                (phrase_nodes[index+5]->type == ident::SEMICOLON) &&
                (phrase_nodes[index+6]->type == phrase::BOOLEXPR) &&
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

                    node->file = phrase_nodes[index]->file;

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

                    node->file = phrase_nodes[index]->file;

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
        bool lexer<CHAR>::loop_statement()
        {
            if (phrase_nodes.is_valid(index+3) &&
                (phrase_nodes[index]->type == ident::KEYWORD_LOOP) &&
                (phrase_nodes[index+1]->type == ident::LBRACE) &&
                ((phrase_nodes[index+2]->type == phrase::STATEMENT) ||
                 (phrase_nodes[index+2]->type == phrase::STATEMENTLIST)) &&
                (phrase_nodes[index+3]->type == ident::RBRACE) &&
                !(phrase_nodes.is_valid(index+4) &&
                  (phrase_nodes[index+4]->type == ident::KEYWORD_WHILE)))
            {
                phrase_t<CHAR>* node = new phrase_t<CHAR>();

                node->type = phrase::LOOP_STATEMENT;

                node->line = phrase_nodes[index]->line;
                node->column = phrase_nodes[index]->column;

                phrase_nodes[index+2]->parent = node;

                node->children.add(phrase_nodes[index+2]);

                node->file = phrase_nodes[index]->file;

                delete phrase_nodes[index];
                delete phrase_nodes[index+1];
                delete phrase_nodes[index+3];
                phrase_nodes.replace(index, index+3, node);

                return true;

            }
            else if (phrase_nodes.is_valid(index+2) &&
                (phrase_nodes[index]->type == ident::KEYWORD_LOOP) &&
                (phrase_nodes[index+1]->type == ident::LBRACE) &&
                (phrase_nodes[index+2]->type == ident::RBRACE) &&
                !(phrase_nodes.is_valid(index+3) &&
                  (phrase_nodes[index+3]->type == ident::KEYWORD_WHILE)))
            {
                phrase_t<CHAR>* node = new phrase_t<CHAR>();

                node->type = phrase::LOOP_STATEMENT;

                node->line = phrase_nodes[index]->line;
                node->column = phrase_nodes[index]->column;



                delete phrase_nodes[index];
                delete phrase_nodes[index+1];
                delete phrase_nodes[index+2];
                phrase_nodes.replace(index, index+2, node);

                return true;

            }
            else if (phrase_nodes.is_valid(index+1) &&
                (phrase_nodes[index]->type == ident::KEYWORD_LOOP) &&
                !(phrase_nodes.is_valid(index+2) &&
                  (phrase_nodes[index+2]->type == ident::KEYWORD_WHILE)))
            {
                if (phrase_nodes[index+1]->type == ident::SEMICOLON)
                {
                    phrase_t<CHAR>* node = new phrase_t<CHAR>();

                    node->type = phrase::LOOP_STATEMENT;

                    node->line = phrase_nodes[index]->line;
                    node->column = phrase_nodes[index]->column;



                    delete phrase_nodes[index];
                    delete phrase_nodes[index+1];
                    phrase_nodes.replace(index, index+1, node);

                    return true;
                }
                else if (phrase_nodes[index+1]->type == phrase::STATEMENT)
                {
                    phrase_t<CHAR>* node = new phrase_t<CHAR>();

                    node->type = phrase::LOOP_STATEMENT;

                    node->line = phrase_nodes[index]->line;
                    node->column = phrase_nodes[index]->column;

                    phrase_nodes[index+1]->parent = node;

                    node->children.add(phrase_nodes[index+1]);



                    delete phrase_nodes[index];
                    phrase_nodes.replace(index, index+1, node);

                    return true;
                }
            }

            return false;
        }

        template <typename CHAR>
        bool lexer<CHAR>::while_pre_stmt()
        {
            if (phrase_nodes.is_valid(index+5) &&
                (phrase_nodes[index]->type == ident::KEYWORD_LOOP) &&
                (phrase_nodes[index+1]->type == ident::KEYWORD_WHILE) &&
                (phrase_nodes[index+2]->type == ident::LPARENTH) &&
                (phrase_nodes[index+3]->type == phrase::BOOLEXPR) &&
                (phrase_nodes[index+4]->type == ident::RPARENTH))
            {
                if (phrase_nodes.is_valid(index+7) &&
                    (phrase_nodes[index+5]->type == ident::LBRACE) &&
                    ((phrase_nodes[index+6]->type == phrase::STATEMENT) ||
                     (phrase_nodes[index+6]->type == phrase::STATEMENTLIST)) &&
                    (phrase_nodes[index+7]->type == ident::RBRACE))
                {
                    phrase_t<CHAR>* node = new phrase_t<CHAR>();

                    node->type = phrase::WHILE_PRE_STMT;

                    node->line = phrase_nodes[index]->line;
                    node->column = phrase_nodes[index]->column;

                    phrase_nodes[index+3]->parent = node;
                    phrase_nodes[index+6]->parent = node;

                    node->children.add(phrase_nodes[index+3]);
                    node->children.add(phrase_nodes[index+6]);

                    node->file = phrase_nodes[index]->file;

                    delete phrase_nodes[index];
                    delete phrase_nodes[index+1];
                    delete phrase_nodes[index+2];
                    delete phrase_nodes[index+4];
                    delete phrase_nodes[index+5];
                    delete phrase_nodes[index+7];
                    phrase_nodes.replace(index, index+7, node);

                    return true;
                }
                else if (phrase_nodes.is_valid(index+6) &&
                    (phrase_nodes[index+5]->type == ident::LBRACE) &&
                    (phrase_nodes[index+6]->type == ident::RBRACE))
                {
                    phrase_t<CHAR>* node = new phrase_t<CHAR>();

                    node->type = phrase::WHILE_PRE_STMT;

                    node->line = phrase_nodes[index]->line;
                    node->column = phrase_nodes[index]->column;

                    phrase_nodes[index+3]->parent = node;

                    node->children.add(phrase_nodes[index+3]);

                    node->file = phrase_nodes[index]->file;

                    delete phrase_nodes[index];
                    delete phrase_nodes[index+1];
                    delete phrase_nodes[index+2];
                    delete phrase_nodes[index+4];
                    delete phrase_nodes[index+5];
                    delete phrase_nodes[index+6];
                    phrase_nodes.replace(index, index+6, node);

                    return true;
                }
                else if (phrase_nodes[index+5]->type == phrase::STATEMENT)
                {
                    phrase_t<CHAR>* node = new phrase_t<CHAR>();

                    node->type = phrase::WHILE_PRE_STMT;

                    node->line = phrase_nodes[index]->line;
                    node->column = phrase_nodes[index]->column;

                    phrase_nodes[index+3]->parent = node;
                    phrase_nodes[index+5]->parent = node;

                    node->children.add(phrase_nodes[index+3]);
                    node->children.add(phrase_nodes[index+5]);

                    node->file = phrase_nodes[index]->file;

                    delete phrase_nodes[index];
                    delete phrase_nodes[index+1];
                    delete phrase_nodes[index+2];
                    delete phrase_nodes[index+4];
                    phrase_nodes.replace(index, index+5, node);

                    return true;
                }
                else if (phrase_nodes[index+5]->type == ident::SEMICOLON)
                {
                    phrase_t<CHAR>* node = new phrase_t<CHAR>();

                    node->type = phrase::WHILE_PRE_STMT;

                    node->line = phrase_nodes[index]->line;
                    node->column = phrase_nodes[index]->column;

                    phrase_nodes[index+3]->parent = node;

                    node->children.add(phrase_nodes[index+3]);

                    node->file = phrase_nodes[index]->file;

                    delete phrase_nodes[index];
                    delete phrase_nodes[index+1];
                    delete phrase_nodes[index+2];
                    delete phrase_nodes[index+4];
                    delete phrase_nodes[index+5];
                    phrase_nodes.replace(index, index+5, node);

                    return true;
                }
            }

            return false;
        }

        template <typename CHAR>
        bool lexer<CHAR>::while_post_stmt()
        {
            if (phrase_nodes.is_valid(index+7) &&
                (phrase_nodes[index]->type == ident::KEYWORD_LOOP) &&
                (phrase_nodes[index+1]->type == ident::LBRACE) &&
                ((phrase_nodes[index+2]->type == phrase::STATEMENT) ||
                 (phrase_nodes[index+2]->type == phrase::STATEMENTLIST)) &&
                (phrase_nodes[index+3]->type == ident::RBRACE) &&
                (phrase_nodes[index+4]->type == ident::KEYWORD_WHILE) &&
                (phrase_nodes[index+5]->type == ident::LPARENTH) &&
                (phrase_nodes[index+6]->type == phrase::BOOLEXPR) &&
                (phrase_nodes[index+7]->type == ident::RPARENTH))
            {
                phrase_t<CHAR>* node = new phrase_t<CHAR>();

                node->type = phrase::WHILE_POST_STMT;

                node->line = phrase_nodes[index]->line;
                node->column = phrase_nodes[index]->column;

                phrase_nodes[index+2]->parent = node;
                phrase_nodes[index+6]->parent = node;

                node->children.add(phrase_nodes[index+2]);
                node->children.add(phrase_nodes[index+6]);

                node->file = phrase_nodes[index]->file;

                delete phrase_nodes[index];
                delete phrase_nodes[index+1];
                delete phrase_nodes[index+3];
                delete phrase_nodes[index+4];
                delete phrase_nodes[index+5];
                delete phrase_nodes[index+7];
                phrase_nodes.replace(index, index+7, node);

                return true;

            }
            else if (phrase_nodes.is_valid(index+6) &&
                (phrase_nodes[index]->type == ident::KEYWORD_LOOP) &&
                (phrase_nodes[index+1]->type == ident::LBRACE) &&
                (phrase_nodes[index+2]->type == ident::RBRACE) &&
                (phrase_nodes[index+3]->type == ident::KEYWORD_WHILE) &&
                (phrase_nodes[index+4]->type == ident::LPARENTH) &&
                (phrase_nodes[index+5]->type == phrase::BOOLEXPR) &&
                (phrase_nodes[index+6]->type == ident::RPARENTH))
            {
                phrase_t<CHAR>* node = new phrase_t<CHAR>();

                node->type = phrase::WHILE_POST_STMT;

                node->line = phrase_nodes[index]->line;
                node->column = phrase_nodes[index]->column;

                phrase_nodes[index+5]->parent = node;

                node->children.add(phrase_nodes[index+5]);

                node->file = phrase_nodes[index]->file;

                delete phrase_nodes[index];
                delete phrase_nodes[index+1];
                delete phrase_nodes[index+2];
                delete phrase_nodes[index+3];
                delete phrase_nodes[index+4];
                delete phrase_nodes[index+6];
                phrase_nodes.replace(index, index+6, node);

                return true;

            }
            else if (phrase_nodes.is_valid(index+5) &&
                (phrase_nodes[index]->type == ident::KEYWORD_LOOP) &&
                (phrase_nodes[index+2]->type == ident::KEYWORD_WHILE) &&
                (phrase_nodes[index+3]->type == ident::LPARENTH) &&
                (phrase_nodes[index+4]->type == phrase::BOOLEXPR) &&
                (phrase_nodes[index+5]->type == ident::RPARENTH))
            {
                if (phrase_nodes[index+1]->type == phrase::STATEMENT)
                {
                    phrase_t<CHAR>* node = new phrase_t<CHAR>();

                    node->type = phrase::WHILE_POST_STMT;

                    node->line = phrase_nodes[index]->line;
                    node->column = phrase_nodes[index]->column;

                    phrase_nodes[index+1]->parent = node;
                    phrase_nodes[index+4]->parent = node;

                    node->children.add(phrase_nodes[index+1]);
                    node->children.add(phrase_nodes[index+4]);

                    node->file = phrase_nodes[index]->file;

                    delete phrase_nodes[index];
                    delete phrase_nodes[index+2];
                    delete phrase_nodes[index+3];
                    delete phrase_nodes[index+5];
                    phrase_nodes.replace(index, index+5, node);

                    return true;
                }
                else if (phrase_nodes[index+1]->type == ident::SEMICOLON)
                {
                    phrase_t<CHAR>* node = new phrase_t<CHAR>();

                    node->type = phrase::WHILE_POST_STMT;

                    node->line = phrase_nodes[index]->line;
                    node->column = phrase_nodes[index]->column;

                    phrase_nodes[index+4]->parent = node;

                    node->children.add(phrase_nodes[index+4]);


                    delete phrase_nodes[index];
                    delete phrase_nodes[index+1];
                    delete phrase_nodes[index+2];
                    delete phrase_nodes[index+3];
                    delete phrase_nodes[index+5];
                    phrase_nodes.replace(index, index+5, node);

                    return true;
                }

            }

            return false;
        }

        template <typename CHAR>
        bool lexer<CHAR>::label_statement()
        {
            if (phrase_nodes.is_valid(index+2) &&
                (phrase_nodes[index]->type == ident::KEYWORD_LABEL) &&
                (phrase_nodes[index+1]->type == ident::IDENTIFIER) &&
                (phrase_nodes[index+2]->type == ident::SEMICOLON))
            {
                phrase_t<CHAR>* node = new phrase_t<CHAR>();

                node->type = phrase::LABEL_STATEMENT;

                node->line = phrase_nodes[index]->line;
                node->column = phrase_nodes[index]->column;

                phrase_nodes[index+1]->parent = node;

                node->children.add(phrase_nodes[index+1]);

                node->file = phrase_nodes[index]->file;

                delete phrase_nodes[index];
                delete phrase_nodes[index+2];
                phrase_nodes.replace(index, index+2, node);

                return true;
            }
            else
                return false;
        }

        template <typename CHAR>
        bool lexer<CHAR>::goto_statement()
        {
            if (phrase_nodes.is_valid(index+2) &&
                (phrase_nodes[index]->type == ident::KEYWORD_GOTO) &&
                (phrase_nodes[index+1]->type == ident::IDENTIFIER) &&
                (phrase_nodes[index+2]->type == ident::SEMICOLON))
            {
                phrase_t<CHAR>* node = new phrase_t<CHAR>();

                node->type = phrase::GOTO_STATEMENT;

                node->line = phrase_nodes[index]->line;
                node->column = phrase_nodes[index]->column;

                phrase_nodes[index+1]->parent = node;

                node->children.add(phrase_nodes[index+1]);

                node->file = phrase_nodes[index]->file;

                delete phrase_nodes[index];
                delete phrase_nodes[index+2];
                phrase_nodes.replace(index, index+2, node);

                return true;
            }
            else
                return false;
        }

        template <typename CHAR>
        bool lexer<CHAR>::gosub_statement()
        {
            if (phrase_nodes.is_valid(index+2) &&
                (phrase_nodes[index]->type == ident::KEYWORD_GOSUB) &&
                (phrase_nodes[index+1]->type == ident::IDENTIFIER) &&
                (phrase_nodes[index+2]->type == ident::SEMICOLON))
            {
                phrase_t<CHAR>* node = new phrase_t<CHAR>();

                node->type = phrase::GOSUB_STATEMENT;

                node->line = phrase_nodes[index]->line;
                node->column = phrase_nodes[index]->column;

                phrase_nodes[index+1]->parent = node;

                node->children.add(phrase_nodes[index+1]);

                node->file = phrase_nodes[index]->file;

                delete phrase_nodes[index];
                delete phrase_nodes[index+2];
                phrase_nodes.replace(index, index+2, node);

                return true;
            }
            else
                return false;
        }

        template <typename CHAR>
        bool lexer<CHAR>::run_statement()
        {
            if (phrase_nodes.is_valid(index+2) &&
                (phrase_nodes[index]->type == ident::KEYWORD_RUN) &&
                (phrase_nodes[index+1]->type == phrase::BOOLEXPR) &&
                (phrase_nodes[index+2]->type == ident::SEMICOLON))
            {
                phrase_t<CHAR>* node = new phrase_t<CHAR>();

                node->type = phrase::RUN_STATEMENT;

                node->line = phrase_nodes[index]->line;
                node->column = phrase_nodes[index]->column;

                phrase_nodes[index+1]->parent = node;

                node->children.add(phrase_nodes[index+1]);

                node->file = phrase_nodes[index]->file;

                delete phrase_nodes[index];
                delete phrase_nodes[index+2];
                phrase_nodes.replace(index, index+2, node);

                return true;
            }
            else
                return false;
        }

        template <typename CHAR>
        bool lexer<CHAR>::return_statement()
        {
            if (phrase_nodes.is_valid(index+2) &&
                (phrase_nodes[index]->type == ident::KEYWORD_RETURN) &&
                (phrase_nodes[index+1]->type == phrase::BOOLEXPR) &&
                (phrase_nodes[index+2]->type == ident::SEMICOLON))
            {
                phrase_t<CHAR>* node = new phrase_t<CHAR>();

                node->type = phrase::RETURN_STATEMENT;

                node->line = phrase_nodes[index]->line;
                node->column = phrase_nodes[index]->column;

                phrase_nodes[index+1]->parent = node;

                node->children.add(phrase_nodes[index+1]);

                node->file = phrase_nodes[index]->file;

                delete phrase_nodes[index];
                delete phrase_nodes[index+2];
                phrase_nodes.replace(index, index+2, node);

                return true;
            }
            else if (phrase_nodes.is_valid(index+1) &&
                (phrase_nodes[index]->type == ident::KEYWORD_RETURN) &&
                (phrase_nodes[index+1]->type == ident::SEMICOLON))
            {
                phrase_t<CHAR>* node = new phrase_t<CHAR>();

                node->type = phrase::RETURN_STATEMENT;

                node->line = phrase_nodes[index]->line;
                node->column = phrase_nodes[index]->column;

                node->file = phrase_nodes[index]->file;

                delete phrase_nodes[index];
                delete phrase_nodes[index+1];
                phrase_nodes.replace(index, index+1, node);

                return true;
            }
            else
                return false;
        }

        template <typename CHAR>
        bool lexer<CHAR>::wait_statement()
        {
            if (phrase_nodes.is_valid(index+2) &&
                (phrase_nodes[index]->type == ident::KEYWORD_WAIT) &&
                (phrase_nodes[index+1]->type == phrase::BOOLEXPR) &&
                (phrase_nodes[index+2]->type == ident::SEMICOLON))
            {
                phrase_t<CHAR>* node = new phrase_t<CHAR>();

                node->type = phrase::WAIT_STATEMENT;

                node->line = phrase_nodes[index]->line;
                node->column = phrase_nodes[index]->column;

                phrase_nodes[index+1]->parent = node;

                node->children.add(phrase_nodes[index+1]);

                node->file = phrase_nodes[index]->file;

                delete phrase_nodes[index];
                delete phrase_nodes[index+2];
                phrase_nodes.replace(index, index+2, node);

                return true;
            }
            else
                return false;
        }

        template <typename CHAR>
        bool lexer<CHAR>::until_statement()
        {
            if (phrase_nodes.is_valid(index+3) &&
                (phrase_nodes[index]->type == ident::KEYWORD_WAIT) &&
                (phrase_nodes[index+1]->type == ident::KEYWORD_UNTIL) &&
                (phrase_nodes[index+2]->type == phrase::BOOLEXPR) &&
                (phrase_nodes[index+3]->type == ident::SEMICOLON))
            {
                phrase_t<CHAR>* node = new phrase_t<CHAR>();

                node->type = phrase::UNTIL_STATEMENT;

                node->line = phrase_nodes[index]->line;
                node->column = phrase_nodes[index]->column;

                phrase_nodes[index+2]->parent = node;

                node->children.add(phrase_nodes[index+2]);

                node->file = phrase_nodes[index]->file;

                delete phrase_nodes[index];
                delete phrase_nodes[index+1];
                delete phrase_nodes[index+3];
                phrase_nodes.replace(index, index+3, node);

                return true;
            }
            else
                return false;
        }


        template <typename CHAR>
        bool lexer<CHAR>::subroutine_decl()
        {
            if (phrase_nodes.is_valid(index+4) &&
                (phrase_nodes[index]->type == ident::KEYWORD_SUB) &&
                (phrase_nodes[index+1]->type == ident::IDENTIFIER) &&
                (phrase_nodes[index+2]->type == ident::LBRACE) &&
                ((phrase_nodes[index+3]->type == phrase::STATEMENT) ||
                 (phrase_nodes[index+3]->type == phrase::STATEMENTLIST)) &&
                (phrase_nodes[index+4]->type == ident::RBRACE))
            {
                phrase_t<CHAR>* node = new phrase_t<CHAR>();

                node->type = phrase::SUBROUTINE_DECL;

                node->line = phrase_nodes[index]->line;
                node->column = phrase_nodes[index]->column;

                phrase_nodes[index+1]->parent = node;
                phrase_nodes[index+3]->parent = node;

                node->children.add(phrase_nodes[index+1]);
                node->children.add(phrase_nodes[index+3]);

                node->file = phrase_nodes[index]->file;

                delete phrase_nodes[index];
                delete phrase_nodes[index+2];
                delete phrase_nodes[index+4];
                phrase_nodes.replace(index, index+4, node);

                return true;
            }
            else if (phrase_nodes.is_valid(index+3) &&
                     (phrase_nodes[index]->type == ident::KEYWORD_SUB) &&
                     (phrase_nodes[index+1]->type == ident::IDENTIFIER) &&
                     (phrase_nodes[index+2]->type == ident::LBRACE) &&
                     (phrase_nodes[index+3]->type == ident::RBRACE))
            {
                phrase_t<CHAR>* node = new phrase_t<CHAR>();

                node->type = phrase::SUBROUTINE_DECL;

                node->line = phrase_nodes[index]->line;
                node->column = phrase_nodes[index]->column;

                phrase_nodes[index+1]->parent = node;

                node->children.add(phrase_nodes[index+1]);

                node->file = phrase_nodes[index]->file;

                delete phrase_nodes[index];
                delete phrase_nodes[index+2];
                delete phrase_nodes[index+3];
                phrase_nodes.replace(index, index+3, node);

                return true;
            }
            else
                return false;
        }


        template <typename CHAR>
        bool lexer<CHAR>::variable_decl()
        {
            if (phrase_nodes.is_valid(index+2) &&
                (phrase_nodes[index]->type == ident::KEYWORD_VAR) &&
                (phrase_nodes[index+1]->type == ident::IDENTIFIER))
            {
                if (phrase_nodes[index+2]->type == ident::SEMICOLON)
                {
                    phrase_t<CHAR>* node = new phrase_t<CHAR>();

                    node->type = phrase::VARIABLE_DECL;

                    node->line = phrase_nodes[index]->line;
                    node->column = phrase_nodes[index]->column;

                    phrase_nodes[index+1]->parent = node;

                    node->children.add(phrase_nodes[index+1]);

                    node->file = phrase_nodes[index]->file;

                    delete phrase_nodes[index];
                    delete phrase_nodes[index+2];
                    phrase_nodes.replace(index, index+2, node);

                    return true;
                }
                else if (phrase_nodes.is_valid(index+4) &&
                         (phrase_nodes[index+2]->type == ident::OPER_ASSIGN) &&
                         (phrase_nodes[index+3]->type == phrase::BOOLEXPR) &&
                         (phrase_nodes[index+4]->type == ident::SEMICOLON))
                {
                    phrase_t<CHAR>* node = new phrase_t<CHAR>();

                    node->type = phrase::VARIABLE_DECL;

                    node->line = phrase_nodes[index]->line;
                    node->column = phrase_nodes[index]->column;

                    phrase_nodes[index+1]->parent = node;
                    phrase_nodes[index+3]->parent = node;

                    node->children.add(phrase_nodes[index+1]);
                    node->children.add(phrase_nodes[index+3]);

                    node->file = phrase_nodes[index]->file;

                    delete phrase_nodes[index];
                    delete phrase_nodes[index+2];
                    delete phrase_nodes[index+4];
                    phrase_nodes.replace(index, index+4, node);

                    return true;
                }
            }

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

                    node->file = phrase_nodes[index]->file;

                    delete phrase_nodes[index+2];
                    phrase_nodes.replace(index, index+2, node);

                    return true;
            }
            else if (phrase_nodes.is_valid(index+1) &&
                     phrase_nodes.is_valid(index-3) &&
                     (phrase_nodes[index-3]->type == ident::IDENTIFIER) &&
                     (phrase_nodes[index-2]->type == ident::IDENTIFIER) &&
                     (phrase_nodes[index-1]->type == ident::OPER_ASSIGN) &&
                     (phrase_nodes[index]->type == phrase::LIST) &&
                     (phrase_nodes[index+1]->type == ident::SEMICOLON))
                {
                    phrase_t<CHAR>* node = new phrase_t<CHAR>();

                    node->type = phrase::TYPEVAR_DECL;

                    node->line = phrase_nodes[index]->line;
                    node->column = phrase_nodes[index]->column;

                    phrase_nodes[index-3]->parent = node;
                    phrase_nodes[index-2]->parent = node;
                    phrase_nodes[index]->parent = node;

                    node->children.add(phrase_nodes[index-3]);
                    node->children.add(phrase_nodes[index-2]);
                    node->children.add(phrase_nodes[index]);

                    node->file = phrase_nodes[index]->file;

                    delete phrase_nodes[index-1];
                    delete phrase_nodes[index+1];
                    phrase_nodes.replace(index-3, index+1, node);

                    return true;
                }

            return false;
        }

        template <typename CHAR>
        bool lexer<CHAR>::_range()
        {
            if (phrase_nodes.is_valid(index+2) &&
                (phrase_nodes[index]->type == phrase::BOOLEXPR) &&
                ((phrase_nodes[index+1]->type == ident::OPER_R_ARROW) ||
                 (phrase_nodes[index+1]->type == ident::OPER_L_ARROW)) &&
                (phrase_nodes[index+2]->type == phrase::BOOLEXPR))
            {
                phrase_t<CHAR>* node = new phrase_t<CHAR>();

                node->type = phrase::RANGE;

                node->line = phrase_nodes[index]->line;
                node->column = phrase_nodes[index]->column;

                phrase_nodes[index]->parent = node;
                phrase_nodes[index+2]->parent = node;

                if (phrase_nodes[index+1]->type == ident::OPER_R_ARROW)
                {
                    node->children.add(phrase_nodes[index]);
                    node->children.add(phrase_nodes[index+2]);
                }
                else
                {
                    node->children.add(phrase_nodes[index+2]);
                    node->children.add(phrase_nodes[index]);
                }

                node->file = phrase_nodes[index]->file;

                delete phrase_nodes[index+1];
                phrase_nodes.replace(index, index+2, node);

                return true;
            }
            else
                return false;
        }

        template <typename CHAR>
        bool lexer<CHAR>::rangelist()
        {
            if (phrase_nodes.is_valid(index+2) &&
                phrase_nodes.is_valid(index-1) &&
                (phrase_nodes[index-1]->type == ident::LBRACKET) &&
                (phrase_nodes[index]->type == phrase::RANGELIST) &&
                ((phrase_nodes[index+1]->type == phrase::RANGE) ||
                 (phrase_nodes[index+1]->type == phrase::BOOLEXPR)) &&
                ((phrase_nodes[index+2]->type == ident::SEMICOLON) ||
                 (phrase_nodes[index+2]->type == ident::RBRACKET)))
            {
                phrase_t<CHAR>* node = new phrase_t<CHAR>();

                node->type = phrase::RANGELIST;

                node->line = phrase_nodes[index]->line;
                node->column = phrase_nodes[index]->column;

                phrase_nodes[index]->parent = node;
                phrase_nodes[index+1]->parent = node;

                node->children.add(phrase_nodes[index]);
                node->children.add(phrase_nodes[index+1]);

                node->file = phrase_nodes[index]->file;

                if (phrase_nodes[index+2]->type == ident::SEMICOLON)
                {
                    delete phrase_nodes[index+2];
                    phrase_nodes.replace(index, index+2, node);
                }
                else
                {
                    phrase_nodes.replace(index, index+1, node);
                }

                return true;
            }
            else if (phrase_nodes.is_valid(index+1) &&
                phrase_nodes.is_valid(index-1) &&
                (phrase_nodes[index-1]->type == ident::LBRACKET) &&
                ((phrase_nodes[index]->type == phrase::RANGE) ||
                 (phrase_nodes[index]->type == phrase::BOOLEXPR)) &&
                (phrase_nodes[index+1]->type == ident::SEMICOLON))
            {
                phrase_t<CHAR>* node = new phrase_t<CHAR>();

                node->type = phrase::RANGELIST;

                node->line = phrase_nodes[index]->line;
                node->column = phrase_nodes[index]->column;

                phrase_nodes[index]->parent = node;

                node->children.add(phrase_nodes[index]);

                node->file = phrase_nodes[index]->file;

                delete phrase_nodes[index+1];
                phrase_nodes.replace(index, index+1, node);

                return true;
            }
            else
                return false;
        }

        template <typename CHAR>
        bool lexer<CHAR>::_index()
        {
            if (phrase_nodes.is_valid(index+2) &&
                (phrase_nodes[index]->type == ident::LBRACKET) &&
                ((phrase_nodes[index+1]->type == phrase::BOOLEXPR) ||
                 (phrase_nodes[index+1]->type == phrase::RANGE) ||
                 (phrase_nodes[index+1]->type == phrase::RANGELIST)) &&
                (phrase_nodes[index+2]->type == ident::RBRACKET))
            {
                phrase_t<CHAR>* node = new phrase_t<CHAR>();

                node->type = phrase::INDEX;

                node->line = phrase_nodes[index]->line;
                node->column = phrase_nodes[index]->column;

                phrase_nodes[index+1]->parent = node;

                node->children.add(phrase_nodes[index+1]);

                node->file = phrase_nodes[index]->file;

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

                node->file = phrase_nodes[index-1]->file;

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
                 (phrase_nodes[index]->type == phrase::EXPRLIST) ||
                 (phrase_nodes[index]->type == phrase::LIST)) &&
                (phrase_nodes[index+1]->type == ident::COMMA) &&
                ((phrase_nodes[index+2]->type == phrase::BOOLEXPR) ||
                 (phrase_nodes[index+2]->type == phrase::EXPRLIST) ||
                 (phrase_nodes[index+2]->type == phrase::LIST)))
            {
                phrase_t<CHAR>* node = new phrase_t<CHAR>();

                node->type = phrase::EXPRLIST;

                node->line = phrase_nodes[index]->line;
                node->column = phrase_nodes[index]->column;

                phrase_nodes[index]->parent = node;
                phrase_nodes[index+2]->parent = node;

                node->children.add(phrase_nodes[index]);
                node->children.add(phrase_nodes[index+2]);

                node->file = phrase_nodes[index]->file;

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

                    node->file = phrase_nodes[index]->file;

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

                    node->file = phrase_nodes[index]->file;

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
            if (!(phrase_nodes.is_valid(index-1) &&
                  phrase_nodes[index-1]->type == ident::KEYWORD_FUNCTION))
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

                    node->file = phrase_nodes[index]->file;

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

                    node->file = phrase_nodes[index]->file;

                    delete phrase_nodes[index+1];
                    delete phrase_nodes[index+2];
                    phrase_nodes.replace(index, index+2, node);

                    return true;
                }
            }

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

                node->file = phrase_nodes[index]->file;

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

                node->file = phrase_nodes[index]->file;

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

                node->file = phrase_nodes[index]->file;

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
            if (((phrase_nodes[index]->type == ident::IDENTIFIER) &&
                     !(phrase_nodes.is_valid(index-1) &&
                       ((phrase_nodes[index-1]->type == ident::KEYWORD_SUB) ||
                        (phrase_nodes[index-1]->type == ident::KEYWORD_TYPE) ||
                        (phrase_nodes[index-1]->type == ident::KEYWORD_VAR) ||
                        (phrase_nodes[index-1]->type == ident::IDENTIFIER))) &&
                     !(phrase_nodes.is_valid(index+1) &&
                       ((phrase_nodes[index+1]->type == ident::PERIOD) ||
                       (phrase_nodes[index+1]->type == ident::LBRACKET) ||
                       (phrase_nodes[index+1]->type == ident::LPARENTH) ||
                       (phrase_nodes[index+1]->type == ident::IDENTIFIER)))))
            {
                phrase_t<CHAR>* node = new phrase_t<CHAR>();

                node->type = phrase::VARIABLE;

                node->line = phrase_nodes[index]->line;
                node->column = phrase_nodes[index]->column;

                phrase_nodes[index]->parent = node;

                node->children.add(phrase_nodes[index]);

                node->file = phrase_nodes[index]->file;

                phrase_nodes[index] = node;

                return true;
            }
            else if ((phrase_nodes[index]->type == phrase::TYPEVAR) ||
                     (phrase_nodes[index]->type == phrase::VARINDEX))
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
                     (!(phrase_nodes.is_valid(index+1) &&
                       (phrase_nodes[index+1]->type >= ident::OPER_ASSIGN) &&
                       (phrase_nodes[index+1]->type <= ident::OPER_MOD_ASSIGN)) ||
                      (phrase_nodes.is_valid(index-1) &&
                       (phrase_nodes[index-1]->type >= ident::OPER_ASSIGN) &&
                       (phrase_nodes[index-1]->type <= ident::OPER_LT_EQ)))) ||
                    (phrase_nodes[index]->type == ident::NUMERIC_LITERAL) ||
                    (phrase_nodes[index]->type == ident::COMPLEX_LITERAL) ||
                    (phrase_nodes[index]->type == ident::STRING_LITERAL) ||
                    (phrase_nodes[index]->type == phrase::DIMENSIONEXPR) ||
                    (phrase_nodes[index]->type == phrase::SIZEOFEXPR) ||
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

                phrase_nodes[index]->parent = node;

                node->file = phrase_nodes[index]->file;

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
        bool lexer<CHAR>::add1expr()
        {
            if (phrase_nodes[index]->type == phrase::FACTORIALEXPR)
            {
                if (phrase_nodes.is_valid(index+1) &&
                    ((phrase_nodes[index+1]->type == ident::OPER_ADD1) ||
                     (phrase_nodes[index+1]->type == ident::OPER_SUB1)))
                {
                    phrase_t<CHAR>* node = new phrase_t<CHAR>();

                    node->type = phrase::ADD1EXPR;

                    node->line = phrase_nodes[index]->line;
                    node->column = phrase_nodes[index]->column;

                    node->children.add(phrase_nodes[index]);
                    node->children.add(phrase_nodes[index+1]);

                    phrase_nodes[index]->parent = node;
                    phrase_nodes[index+1]->parent = node;

                    node->file = phrase_nodes[index]->file;

                    phrase_nodes.replace(index, index+1, node);

                    return true;
                }
                else if (phrase_nodes.is_valid(index-1) &&
                         ((phrase_nodes[index-1]->type == ident::OPER_ADD1) ||
                          (phrase_nodes[index-1]->type == ident::OPER_SUB1)))
                {
                    phrase_t<CHAR>* node = new phrase_t<CHAR>();

                    node->type = phrase::ADD1EXPR;

                    node->line = phrase_nodes[index]->line;
                    node->column = phrase_nodes[index]->column;

                    node->children.add(phrase_nodes[index-1]);
                    node->children.add(phrase_nodes[index]);

                    phrase_nodes[index-1]->parent = node;
                    phrase_nodes[index]->parent = node;

                    phrase_nodes.replace(index-1, index, node);

                    return true;
                }
                else
                {
                    if (phrase_nodes[index]->orig_type == ident::NONE)
                        phrase_nodes[index]->orig_type = phrase_nodes[index]->type;
                    phrase_nodes[index]->type = phrase::ADD1EXPR;

                    return true;
                }
            }

            return false;
        }

        template <typename CHAR>
        bool lexer<CHAR>::negatexpr()
        {
            if ((phrase_nodes[index]->type == phrase::ADD1EXPR) &&
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

                phrase_nodes[index]->parent = node;

                node->file = phrase_nodes[index]->file;

                delete phrase_nodes[index-1];
                phrase_nodes.replace(index-1, index, node);

                return true;
            }
            else if (phrase_nodes[index]->type == phrase::ADD1EXPR)
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

                phrase_nodes[index]->parent = node;
                phrase_nodes[index+2]->parent = node;

                node->children.add(phrase_nodes[index]);
                node->children.add(phrase_nodes[index+2]);

                node->file = phrase_nodes[index]->file;

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

                phrase_nodes[index]->parent = node;
                phrase_nodes[index+1]->parent = node;
                phrase_nodes[index+2]->parent = node;

                node->children.add(phrase_nodes[index]);
                node->children.add(phrase_nodes[index+1]);
                node->children.add(phrase_nodes[index+2]);

                node->file = phrase_nodes[index]->file;

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

                phrase_nodes[index]->parent = node;
                phrase_nodes[index+1]->parent = node;
                phrase_nodes[index+2]->parent = node;

                node->children.add(phrase_nodes[index]);
                node->children.add(phrase_nodes[index+1]);
                node->children.add(phrase_nodes[index+2]);

                node->file = phrase_nodes[index]->file;

                phrase_nodes.replace(index, index+2, node);

                return true;
            }
            else
                return false;
        }

        template <typename CHAR>
        bool lexer<CHAR>::boolexpr()
        {
            //if no detected boolean operators, continue to the next phase
            if ((phrase_nodes[index]->type == phrase::ASSIGNEXPR) ||
                ((phrase_nodes[index]->type == phrase::ADDEXPR) &&
                !(phrase_nodes.is_valid(index+1) &&
                  (phrase_nodes[index+1]->type >= ident::OPER_AND_LGCL) &&
                  (phrase_nodes[index+1]->type <= ident::OPER_LT_EQ)) &&
                !(phrase_nodes.is_valid(index-1) &&
                  (phrase_nodes[index-1]->type >= ident::OPER_AND_LGCL) &&
                  (phrase_nodes[index-1]->type <= ident::OPER_LT_EQ))))
            {
                if (phrase_nodes[index]->orig_type == ident::NONE)
                    phrase_nodes[index]->orig_type = phrase_nodes[index]->type;
                phrase_nodes[index]->type = phrase::BOOLEXPR;

                return true;
            }
            //otherwise, if a boolean operator is detected
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

                phrase_nodes[index]->parent = node;
                phrase_nodes[index+1]->parent = node;
                phrase_nodes[index+2]->parent = node;

                node->children.add(phrase_nodes[index]);
                node->children.add(phrase_nodes[index+1]);
                node->children.add(phrase_nodes[index+2]);

                node->file = phrase_nodes[index]->file;

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
                (phrase_nodes[index+1]->type >= ident::OPER_ASSIGN) &&
                (phrase_nodes[index+1]->type <= ident::OPER_MOD_ASSIGN) &&
                (phrase_nodes[index+2]->type == phrase::BOOLEXPR))
            {
                phrase_t<CHAR>* node = new phrase_t<CHAR>();

                node->type = phrase::ASSIGNEXPR;

                node->line = phrase_nodes[index]->line;
                node->column = phrase_nodes[index]->column;

                phrase_nodes[index]->parent = node;
                phrase_nodes[index+1]->parent = node;
                phrase_nodes[index+2]->parent = node;

                node->children.add(phrase_nodes[index]);
                node->children.add(phrase_nodes[index+1]);
                node->children.add(phrase_nodes[index+2]);

                node->file = phrase_nodes[index]->file;

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

                phrase_nodes[index]->parent = node;

                node->children.add(phrase_nodes[index]);

                node->file = phrase_nodes[index]->file;

                delete phrase_nodes[index-1];
                phrase_nodes.replace(index-1, index, node);

                return true;
            }
            else
                return false;
        }

        template <typename CHAR>
        bool lexer<CHAR>::sizeofexpr()
        {
            if (phrase_nodes.is_valid(index+3) &&
                (phrase_nodes[index]->type == ident::OPER_SIZEOF) &&
                (phrase_nodes[index+1]->type == ident::LPARENTH) &&
                (phrase_nodes[index+2]->type == phrase::BOOLEXPR) &&
                (phrase_nodes[index+3]->type == ident::RPARENTH))
            {
                phrase_t<CHAR>* node = new phrase_t<CHAR>();

                node->type = phrase::SIZEOFEXPR;

                node->line = phrase_nodes[index]->line;
                node->column = phrase_nodes[index]->column;

                phrase_nodes[index+2]->parent = node;

                node->children.add(phrase_nodes[index+2]);

                node->file = phrase_nodes[index]->file;

                delete phrase_nodes[index];
                delete phrase_nodes[index+1];
                delete phrase_nodes[index+3];
                phrase_nodes.replace(index, index+3, node);

                return true;
            }
            else
                return false;
        }


        template <typename CHAR>
        bool lexer<CHAR>::externaldecl()
        {
            if (phrase_nodes.is_valid(index+1) &&
                phrase_nodes.is_valid(index-1) &&
                (phrase_nodes[index-1]->type == ident::KEYWORD_EXTERNAL) &&
                (phrase_nodes[index]->type == phrase::VARIABLE) &&
                (phrase_nodes[index+1]->type == ident::SEMICOLON))
            {
                phrase_t<CHAR>* node = new phrase_t<CHAR>();

                node->type = phrase::EXTERNALDECL;

                node->line = phrase_nodes[index]->line;
                node->column = phrase_nodes[index]->column;

                phrase_nodes[index]->parent = node;

                node->children.add(phrase_nodes[index]);

                node->file = phrase_nodes[index-1]->file;

                delete phrase_nodes[index-1];
                delete phrase_nodes[index+1];
                phrase_nodes.replace(index-1, index+1, node);

                return true;
            }
            else
                return false;
        }


        template <typename CHAR>
        bool lexer<CHAR>::shareddecl()
        {
            if (phrase_nodes.is_valid(index+1) &&
                phrase_nodes.is_valid(index-1) &&
                (phrase_nodes[index-1]->type == ident::KEYWORD_SHARED) &&
                (phrase_nodes[index]->type == phrase::VARIABLE) &&
                (phrase_nodes[index+1]->type == ident::SEMICOLON))
            {
                phrase_t<CHAR>* node = new phrase_t<CHAR>();

                node->type = phrase::SHAREDDECL;

                node->line = phrase_nodes[index]->line;
                node->column = phrase_nodes[index]->column;

                phrase_nodes[index]->parent = node;

                node->children.add(phrase_nodes[index]);

                node->file = phrase_nodes[index-1]->file;

                delete phrase_nodes[index-1];
                delete phrase_nodes[index+1];
                phrase_nodes.replace(index-1, index+1, node);

                return true;
            }
            else
                return false;
        }


        template <typename CHAR>
        bool lexer<CHAR>::formalvardecl()
        {
            if ((phrase_nodes.is_valid(index-3) &&
                 (phrase_nodes[index-3]->type == ident::KEYWORD_FUNCTION) &&
                 (phrase_nodes[index-2]->type == ident::IDENTIFIER) &&
                 (phrase_nodes[index-1]->type == ident::LPARENTH)) ||
                (phrase_nodes.is_valid(index-2) &&
                 ((phrase_nodes[index-2]->type == phrase::FORMALVARDECL) ||
                  (phrase_nodes[index-2]->type == phrase::FORMALTYPEDECL)) &&
                 (phrase_nodes[index-1]->type == ident::COMMA)))
            {

                if (phrase_nodes.is_valid(index+1) &&
                    (phrase_nodes[index]->type == ident::KEYWORD_VAR) &&
                    (phrase_nodes[index+1]->type == ident::IDENTIFIER))
                {
                    phrase_t<CHAR>* node = new phrase_t<CHAR>();

                    node->type = phrase::FORMALVARDECL;

                    node->line = phrase_nodes[index]->line;
                    node->column = phrase_nodes[index]->column;

                    phrase_nodes[index+1]->parent = node;

                    node->children.add(phrase_nodes[index+1]);

                    node->file = phrase_nodes[index]->file;

                    delete phrase_nodes[index];
                    phrase_nodes.replace(index, index+1, node);

                    return true;
                }
                else if ((phrase_nodes[index]->type == ident::IDENTIFIER) &&
                         !(phrase_nodes.is_valid(index+1) &&
                          (phrase_nodes[index+1]->type == ident::IDENTIFIER)))
                {
                    phrase_t<CHAR>* node = new phrase_t<CHAR>();

                    node->type = phrase::FORMALVARDECL;

                    node->line = phrase_nodes[index]->line;
                    node->column = phrase_nodes[index]->column;

                    phrase_nodes[index]->parent = node;

                    node->children.add(phrase_nodes[index]);

                    node->file = phrase_nodes[index]->file;

                    phrase_nodes[index] = node;

                    return true;
                }
            }

            return false;
        }

        template <typename CHAR>
        bool lexer<CHAR>::formaltypedecl()
        {
            if ((phrase_nodes.is_valid(index-3) &&
                 (phrase_nodes[index-3]->type == ident::KEYWORD_FUNCTION) &&
                 (phrase_nodes[index-2]->type == ident::IDENTIFIER) &&
                 (phrase_nodes[index-1]->type == ident::LPARENTH)) ||
                (phrase_nodes.is_valid(index-2) &&
                 ((phrase_nodes[index-2]->type == phrase::FORMALVARDECL) ||
                  (phrase_nodes[index-2]->type == phrase::FORMALTYPEDECL)) &&
                 (phrase_nodes[index-1]->type == ident::COMMA)))
            {

                if (phrase_nodes.is_valid(index+1) &&
                    (phrase_nodes[index]->type == ident::IDENTIFIER) &&
                    (phrase_nodes[index+1]->type == ident::IDENTIFIER))
                {
                    phrase_t<CHAR>* node = new phrase_t<CHAR>();

                    node->type = phrase::FORMALTYPEDECL;

                    node->line = phrase_nodes[index]->line;
                    node->column = phrase_nodes[index]->column;

                    phrase_nodes[index]->parent = node;
                    phrase_nodes[index+1]->parent = node;

                    node->children.add(phrase_nodes[index]);
                    node->children.add(phrase_nodes[index+1]);

                    node->file = phrase_nodes[index]->file;

                    phrase_nodes.replace(index, index+1, node);

                    return true;
                }
            }

            return false;
        }

        template <typename CHAR>
        bool lexer<CHAR>::formaldecllist()
        {
            if (phrase_nodes.is_valid(index+2) &&
                 ((phrase_nodes[index]->type == phrase::FORMALVARDECL) ||
                  (phrase_nodes[index]->type == phrase::FORMALTYPEDECL) ||
                  (phrase_nodes[index]->type == phrase::FORMALDECLLIST)) &&
                 (phrase_nodes[index+1]->type == ident::COMMA) &&
                ((phrase_nodes[index+2]->type == phrase::FORMALVARDECL) ||
                  (phrase_nodes[index+2]->type == phrase::FORMALTYPEDECL)))
            {
                phrase_t<CHAR>* node = new phrase_t<CHAR>();

                node->type = phrase::FORMALDECLLIST;

                node->line = phrase_nodes[index]->line;
                node->column = phrase_nodes[index]->column;

                phrase_nodes[index]->parent = node;
                phrase_nodes[index+2]->parent = node;

                node->children.add(phrase_nodes[index]);
                node->children.add(phrase_nodes[index+2]);

                node->file = phrase_nodes[index]->file;

                delete phrase_nodes[index+1];
                phrase_nodes.replace(index, index+2, node);

                return true;
            }
            else
                return false;
        }

        template <typename CHAR>
        bool lexer<CHAR>::function_decl()
        {
            if (phrase_nodes.is_valid(index+5) &&
                (phrase_nodes[index]->type == ident::KEYWORD_FUNCTION) &&
                (phrase_nodes[index+1]->type == ident::IDENTIFIER) &&
                (phrase_nodes[index+2]->type == ident::LPARENTH))
            {
                if ((phrase_nodes[index+3]->type == ident::RPARENTH) &&
                         (phrase_nodes[index+4]->type == ident::LBRACE) &&
                         (phrase_nodes[index+5]->type == ident::RBRACE))
                {
                    phrase_t<CHAR>* node = new phrase_t<CHAR>();

                    node->type = phrase::FUNCTION_DECL;

                    node->line = phrase_nodes[index]->line;
                    node->column = phrase_nodes[index]->column;

                    phrase_nodes[index+1]->parent = node;

                    node->children.add(phrase_nodes[index+1]);

                    node->file = phrase_nodes[index]->file;

                    delete phrase_nodes[index];
                    delete phrase_nodes[index+2];
                    delete phrase_nodes[index+3];
                    delete phrase_nodes[index+4];
                    delete phrase_nodes[index+5];
                    phrase_nodes.replace(index, index+5, node);

                    return true;
                }
                else if (phrase_nodes.is_valid(index+7) &&
                    ((phrase_nodes[index+3]->type == phrase::FORMALVARDECL) ||
                     (phrase_nodes[index+3]->type == phrase::FORMALTYPEDECL) ||
                     (phrase_nodes[index+3]->type == phrase::FORMALDECLLIST)) &&
                    (phrase_nodes[index+4]->type == ident::RPARENTH) &&
                    (phrase_nodes[index+5]->type == ident::LBRACE) &&
                    ((phrase_nodes[index+6]->type == phrase::STATEMENTLIST) ||
                     (phrase_nodes[index+6]->type == phrase::STATEMENT)) &&
                    (phrase_nodes[index+7]->type == ident::RBRACE))
                {
                    phrase_t<CHAR>* node = new phrase_t<CHAR>();

                    node->type = phrase::FUNCTION_DECL;

                    node->line = phrase_nodes[index]->line;
                    node->column = phrase_nodes[index]->column;

                    phrase_nodes[index+1]->parent = node;
                    phrase_nodes[index+3]->parent = node;
                    phrase_nodes[index+6]->parent = node;

                    node->children.add(phrase_nodes[index+1]);
                    node->children.add(phrase_nodes[index+3]);
                    node->children.add(phrase_nodes[index+6]);

                    node->file = phrase_nodes[index]->file;

                    delete phrase_nodes[index];
                    delete phrase_nodes[index+2];
                    delete phrase_nodes[index+4];
                    delete phrase_nodes[index+5];
                    delete phrase_nodes[index+7];
                    phrase_nodes.replace(index, index+7, node);

                    return true;
                }
                else if (phrase_nodes.is_valid(index+6))
                {
                    if (((phrase_nodes[index+3]->type == phrase::FORMALVARDECL) ||
                         (phrase_nodes[index+3]->type == phrase::FORMALTYPEDECL) ||
                         (phrase_nodes[index+3]->type == phrase::FORMALDECLLIST)) &&
                        (phrase_nodes[index+4]->type == ident::RPARENTH) &&
                        (phrase_nodes[index+5]->type == ident::LBRACE) &&
                        (phrase_nodes[index+6]->type == ident::RBRACE))
                    {
                        phrase_t<CHAR>* node = new phrase_t<CHAR>();

                        node->type = phrase::FUNCTION_DECL;

                        node->line = phrase_nodes[index]->line;
                        node->column = phrase_nodes[index]->column;

                        phrase_nodes[index+1]->parent = node;
                        phrase_nodes[index+3]->parent = node;

                        node->children.add(phrase_nodes[index+1]);
                        node->children.add(phrase_nodes[index+3]);

                        node->file = phrase_nodes[index]->file;

                        delete phrase_nodes[index];
                        delete phrase_nodes[index+2];
                        delete phrase_nodes[index+4];
                        delete phrase_nodes[index+5];
                        delete phrase_nodes[index+6];
                        phrase_nodes.replace(index, index+6, node);

                        return true;
                    }
                    else if ((phrase_nodes[index+3]->type == ident::RPARENTH) &&
                             (phrase_nodes[index+4]->type == ident::LBRACE) &&
                             ((phrase_nodes[index+5]->type == phrase::STATEMENTLIST) ||
                              (phrase_nodes[index+5]->type == phrase::STATEMENT)) &&
                             (phrase_nodes[index+6]->type == ident::RBRACE))
                    {
                        phrase_t<CHAR>* node = new phrase_t<CHAR>();

                        node->type = phrase::FUNCTION_DECL;

                        node->line = phrase_nodes[index]->line;
                        node->column = phrase_nodes[index]->column;

                        phrase_nodes[index+1]->parent = node;
                        phrase_nodes[index+5]->parent = node;

                        node->children.add(phrase_nodes[index+1]);
                        node->children.add(phrase_nodes[index+5]);

                        node->file = phrase_nodes[index]->file;

                        delete phrase_nodes[index];
                        delete phrase_nodes[index+2];
                        delete phrase_nodes[index+3];
                        delete phrase_nodes[index+4];
                        delete phrase_nodes[index+6];
                        phrase_nodes.replace(index, index+6, node);

                        return true;
                    }
                }
            }

            return false;
        }

        template <typename CHAR>
        bool lexer<CHAR>::int_decllist()
        {
            if (((phrase_nodes.is_valid(index-4) &&
                 (phrase_nodes[index-4]->type == ident::KEYWORD_TYPE) &&
                 (phrase_nodes[index-3]->type == ident::IDENTIFIER) &&
                 (phrase_nodes[index-2]->type == ident::LBRACE) &&
                 ((phrase_nodes[index-1]->type == phrase::VARIABLE_DECL) ||
                  (phrase_nodes[index-1]->type == phrase::TYPEVAR_DECL) ||
                  (phrase_nodes[index-1]->type == phrase::FUNCTION_DECL))) ||

                 (phrase_nodes.is_valid(index-1) &&
                  (phrase_nodes[index-1]->type == phrase::INT_DECLLIST))) &&

                ((phrase_nodes[index]->type == phrase::VARIABLE_DECL) ||
                 (phrase_nodes[index]->type == phrase::TYPEVAR_DECL) ||
                 (phrase_nodes[index]->type == phrase::FUNCTION_DECL)))
            {
                phrase_t<CHAR>* node = new phrase_t<CHAR>();

                node->type = phrase::INT_DECLLIST;

                node->line = phrase_nodes[index]->line;
                node->column = phrase_nodes[index]->column;

                phrase_nodes[index-1]->parent = node;
                phrase_nodes[index]->parent = node;

                node->children.add(phrase_nodes[index-1]);
                node->children.add(phrase_nodes[index]);

                node->file = phrase_nodes[index-1]->file;

                phrase_nodes.replace(index-1, index, node);

                return true;
            }
            else
                return false;
        }

        template <typename CHAR>
        bool lexer<CHAR>::typedecl()
        {
            if ((phrase_nodes.is_valid(index+4) &&
                 (phrase_nodes[index]->type == ident::KEYWORD_TYPE) &&
                 (phrase_nodes[index+1]->type == ident::IDENTIFIER) &&
                 (phrase_nodes[index+2]->type == ident::LBRACE)) &&
                 ((phrase_nodes[index+3]->type == phrase::VARIABLE_DECL) ||
                  (phrase_nodes[index+3]->type == phrase::TYPEVAR_DECL) ||
                  (phrase_nodes[index+3]->type == phrase::FUNCTION_DECL) ||
                  (phrase_nodes[index+3]->type == phrase::INT_DECLLIST)) &&
                (phrase_nodes[index+4]->type == ident::RBRACE))
            {
                phrase_t<CHAR>* node = new phrase_t<CHAR>();

                node->type = phrase::TYPEDECL;

                node->line = phrase_nodes[index]->line;
                node->column = phrase_nodes[index]->column;

                phrase_nodes[index+1]->parent = node;
                phrase_nodes[index+3]->parent = node;

                node->children.add(phrase_nodes[index+1]);
                node->children.add(phrase_nodes[index+3]);

                node->file = phrase_nodes[index]->file;

                delete phrase_nodes[index];
                delete phrase_nodes[index+2];
                delete phrase_nodes[index+4];
                phrase_nodes.replace(index, index+4, node);

                return true;
            }
            else
                return false;
        }

        template <typename CHAR>
        bool lexer<CHAR>::program()
        {
            if ((phrase_nodes[index]->type == phrase::VARIABLE_DECL) ||
                (phrase_nodes[index]->type == phrase::TYPEVAR_DECL) ||
                (phrase_nodes[index]->type == phrase::FUNCTION_DECL) ||
                (phrase_nodes[index]->type == phrase::TYPEDECL) ||
                (phrase_nodes[index]->type == phrase::EXTERNALDECL) ||
                (phrase_nodes[index]->type == phrase::SHAREDDECL))
            {
                phrase_t<CHAR>* node = new phrase_t<CHAR>();

                node->type = phrase::PROGRAM;

                node->line = phrase_nodes[index]->line;
                node->column = phrase_nodes[index]->column;

                phrase_nodes[index]->parent = node;

                node->children.add(phrase_nodes[index]);

                node->file = phrase_nodes[index]->file;

                phrase_nodes[index] = node;

                return true;
            }
            else if (phrase_nodes.is_valid(index+1) &&
                     (phrase_nodes[index]->type == phrase::PROGRAM) &&

                     ((phrase_nodes[index+1]->type == phrase::VARIABLE_DECL) ||
                      (phrase_nodes[index+1]->type == phrase::TYPEVAR_DECL) ||
                      (phrase_nodes[index+1]->type == phrase::FUNCTION_DECL) ||
                      (phrase_nodes[index+1]->type == phrase::TYPEDECL) ||
                      (phrase_nodes[index+1]->type == phrase::EXTERNALDECL) ||
                      (phrase_nodes[index+1]->type == phrase::SHAREDDECL)))
            {
                phrase_nodes[index+1]->parent = phrase_nodes[index];

                phrase_nodes[index]->children.add(phrase_nodes[index+1]);

                phrase_nodes.remove(index+1);

                return true;
            }
            else
                return false;
        }


        ///end of phrase detection
    }
}

#endif //LEXERPHRASEDETECTION_H_INCLUDED
