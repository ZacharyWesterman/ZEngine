/**
 * File:            if_statement.h
 * Namespace:       z::script
 * Description:     Implementation of the lexer's
 *                  syntax generating member functions.
 *                  This file should NOT be included
 *                  in a project. Include "lexer.h"
 *                  instead,and put this file in the
 *                  "syntaxRules" folder.
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   15 Jul. 2017
**/

#pragma once
#ifndef IF_STATEMENT_H_INCLUDED
#define IF_STATEMENT_H_INCLUDED

namespace z
{
namespace script
{
    namespace compiler
    {

        bool lexer::if_statement()
        {
            if (phrase_nodes->is_valid(index+3) &&
                (phrase_nodes->at(index)->type == ident::KEYWORD_IF) &&
                (phrase_nodes->at(index+1)->type == ident::LPARENTH) &&
                (phrase_nodes->at(index+2)->type == BOOLEXPR) &&
                (phrase_nodes->at(index+3)->type == ident::RPARENTH))
            {
                if (phrase_nodes->is_valid(index+6) &&
                    (phrase_nodes->at(index+4)->type == ident::LBRACE) &&
                    ((phrase_nodes->at(index+5)->type == STATEMENT) ||
                     (phrase_nodes->at(index+5)->type == STATEMENTLIST)) &&
                    (phrase_nodes->at(index+6)->type == ident::RBRACE))
                {
                    phrase_t* node = new phrase_t();

                    node->type = IF_STATEMENT;

                    node->line = phrase_nodes->at(index)->line;
                    node->column = phrase_nodes->at(index)->column;

                    phrase_nodes->at(index+2)->parent = node;
                    phrase_nodes->at(index+5)->parent = node;

                    node->children.add(phrase_nodes->at(index+2));
                    node->children.add(phrase_nodes->at(index+5));

                    node->file = phrase_nodes->at(index)->file;

                    delete phrase_nodes->at(index);
                    delete phrase_nodes->at(index+1);
                    delete phrase_nodes->at(index+3);
                    delete phrase_nodes->at(index+4);
                    delete phrase_nodes->at(index+6);
                    phrase_nodes->replace(index, index+6, node);

                    return true;
                }
                else if (phrase_nodes->is_valid(index+5) &&
                    (phrase_nodes->at(index+4)->type == ident::LBRACE) &&
                    (phrase_nodes->at(index+5)->type == ident::RBRACE))
                {
                    phrase_t* node = new phrase_t();

                    node->type = IF_STATEMENT;

                    node->line = phrase_nodes->at(index)->line;
                    node->column = phrase_nodes->at(index)->column;

                    phrase_nodes->at(index+2)->parent = node;

                    node->children.add(phrase_nodes->at(index+2));



                    delete phrase_nodes->at(index);
                    delete phrase_nodes->at(index+1);
                    delete phrase_nodes->at(index+3);
                    delete phrase_nodes->at(index+4);
                    delete phrase_nodes->at(index+5);
                    phrase_nodes->replace(index, index+5, node);

                    return true;
                }
                else if (phrase_nodes->is_valid(index+4))
                {
                    if (phrase_nodes->at(index+4)->type == STATEMENT)
                    {
                        phrase_t* node = new phrase_t();

                        node->type = IF_STATEMENT;

                        node->line = phrase_nodes->at(index)->line;
                        node->column = phrase_nodes->at(index)->column;

                        phrase_nodes->at(index+2)->parent = node;
                        phrase_nodes->at(index+4)->parent = node;

                        node->children.add(phrase_nodes->at(index+2));
                        node->children.add(phrase_nodes->at(index+4));



                        delete phrase_nodes->at(index);
                        delete phrase_nodes->at(index+1);
                        delete phrase_nodes->at(index+3);
                        phrase_nodes->replace(index, index+4, node);

                        return true;
                    }
                    else if (phrase_nodes->at(index+4)->type == ident::SEMICOLON)
                    {
                        phrase_t* node = new phrase_t();

                        node->type = IF_STATEMENT;

                        node->line = phrase_nodes->at(index)->line;
                        node->column = phrase_nodes->at(index)->column;

                        phrase_nodes->at(index+2)->parent = node;

                        node->children.add(phrase_nodes->at(index+2));



                        delete phrase_nodes->at(index);
                        delete phrase_nodes->at(index+1);
                        delete phrase_nodes->at(index+3);
                        delete phrase_nodes->at(index+4);
                        phrase_nodes->replace(index, index+4, node);

                        return true;
                    }
                    else
                        return false;
                }
                else
                    return false;
            }
            else if (phrase_nodes->is_valid(index+2) &&
                     (phrase_nodes->at(index)->type == IF_STATEMENT) &&
                     (phrase_nodes->at(index+1)->type == ident::KEYWORD_ELSE))
            {
                if (phrase_nodes->is_valid(index+4) &&
                    (phrase_nodes->at(index+2)->type == ident::LBRACE) &&
                    ((phrase_nodes->at(index+3)->type == STATEMENT) ||
                     (phrase_nodes->at(index+3)->type == STATEMENTLIST)) &&
                    (phrase_nodes->at(index+4)->type == ident::RBRACE))
                {
                    phrase_nodes->at(index+3)->parent = phrase_nodes->at(index);

                    phrase_nodes->at(index)->children.add(phrase_nodes->at(index+3));

                    delete phrase_nodes->at(index+1);
                    delete phrase_nodes->at(index+2);
                    delete phrase_nodes->at(index+4);
                    phrase_nodes->remove(index+4);
                    phrase_nodes->remove(index+3);
                    phrase_nodes->remove(index+2);
                    phrase_nodes->remove(index+1);

                    return true;
                }
                else if (phrase_nodes->is_valid(index+3) &&
                         (phrase_nodes->at(index+2)->type == ident::LBRACE) &&
                         (phrase_nodes->at(index+3)->type == ident::RBRACE))
                {
                    delete phrase_nodes->at(index+1);
                    delete phrase_nodes->at(index+2);
                    delete phrase_nodes->at(index+3);
                    phrase_nodes->remove(index+3);
                    phrase_nodes->remove(index+2);
                    phrase_nodes->remove(index+1);

                    return true;
                }
                else if (phrase_nodes->at(index+2)->type == STATEMENT)
                {
                    phrase_nodes->at(index+2)->parent = phrase_nodes->at(index);

                    phrase_nodes->at(index)->children.add(phrase_nodes->at(index+2));

                    delete phrase_nodes->at(index+1);
                    phrase_nodes->remove(index+2);
                    phrase_nodes->remove(index+1);

                    return true;
                }
                else if (phrase_nodes->at(index+2)->type == ident::SEMICOLON)
                {
                    delete phrase_nodes->at(index+1);
                    delete phrase_nodes->at(index+2);
                    phrase_nodes->remove(index+2);
                    phrase_nodes->remove(index+1);

                    return true;
                }
                else
                    return false;
            }
            else
                return false;
        }
    }
}
}

#endif // IF_STATEMENT_H_INCLUDED
