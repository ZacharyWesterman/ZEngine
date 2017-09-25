/**
 * File:            function_decl.h
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
 * Last modified:   23 Aug. 2017
**/

#pragma once
#ifndef FUNCTION_DECL_H_INCLUDED
#define FUNCTION_DECL_H_INCLUDED

namespace z
{
namespace script
{
    namespace compiler
    {
        template <typename CHAR>
        bool lexer<CHAR>::function_decl()
        {
            bool result = false;

            //IDENTIFIER ( ... ) { ... }
            if (phrase_nodes.is_valid(index+6) &&
                (phrase_nodes[index]->type == ident::IDENTIFIER) &&
                (phrase_nodes[index+1]->type == ident::LPARENTH) &&
                ((phrase_nodes[index+2]->type == phrase::FORMALVARDECL) ||
                 (phrase_nodes[index+2]->type == phrase::FORMALTYPEDECL) ||
                 (phrase_nodes[index+2]->type == phrase::FORMALDECLLIST)) &&
                (phrase_nodes[index+3]->type == ident::RPARENTH) &&
                (phrase_nodes[index+4]->type == ident::LBRACE) &&
                ((phrase_nodes[index+5]->type == phrase::STATEMENTLIST) ||
                 (phrase_nodes[index+5]->type == phrase::STATEMENT)) &&
                (phrase_nodes[index+6]->type == ident::RBRACE))
            {
                phrase_t<CHAR>* node = new phrase_t<CHAR>();

                node->type = phrase::FUNCTION_DECL;

                node->line = phrase_nodes[index]->line;
                node->column = phrase_nodes[index]->column;

                phrase_nodes[index]->parent = node;
                phrase_nodes[index+2]->parent = node;
                phrase_nodes[index+5]->parent = node;

                node->children.add(phrase_nodes[index]);
                node->children.add(phrase_nodes[index+2]);
                node->children.add(phrase_nodes[index+5]);

                node->file = phrase_nodes[index]->file;

                delete phrase_nodes[index+1];
                delete phrase_nodes[index+3];
                delete phrase_nodes[index+4];
                delete phrase_nodes[index+6];
                phrase_nodes.replace(index, index+6, node);

                result = true;
            }
            //IDENTIFIER ( ) { ... }
            else if (phrase_nodes.is_valid(index+5) &&
                (phrase_nodes[index]->type == ident::IDENTIFIER) &&
                (phrase_nodes[index+1]->type == ident::LPARENTH) &&
                (phrase_nodes[index+2]->type == ident::RPARENTH) &&
                (phrase_nodes[index+3]->type == ident::LBRACE) &&
                ((phrase_nodes[index+4]->type == phrase::STATEMENTLIST) ||
                 (phrase_nodes[index+4]->type == phrase::STATEMENT)) &&
                (phrase_nodes[index+5]->type == ident::RBRACE))
            {
                phrase_t<CHAR>* node = new phrase_t<CHAR>();

                node->type = phrase::FUNCTION_DECL;

                node->line = phrase_nodes[index]->line;
                node->column = phrase_nodes[index]->column;

                phrase_nodes[index]->parent = node;
                phrase_nodes[index+4]->parent = node;

                node->children.add(phrase_nodes[index]);
                node->children.add(phrase_nodes[index+4]);

                node->file = phrase_nodes[index]->file;

                delete phrase_nodes[index+1];
                delete phrase_nodes[index+2];
                delete phrase_nodes[index+3];
                delete phrase_nodes[index+5];
                phrase_nodes.replace(index, index+5, node);

                result = true;
            }
            //IDENTIFIER ( ... ) { }
            else if (phrase_nodes.is_valid(index+5) &&
                (phrase_nodes[index]->type == ident::IDENTIFIER) &&
                (phrase_nodes[index+1]->type == ident::LPARENTH) &&
                ((phrase_nodes[index+2]->type == phrase::FORMALVARDECL) ||
                 (phrase_nodes[index+2]->type == phrase::FORMALTYPEDECL) ||
                 (phrase_nodes[index+2]->type == phrase::FORMALDECLLIST)) &&
                (phrase_nodes[index+3]->type == ident::RPARENTH) &&
                (phrase_nodes[index+4]->type == ident::LBRACE) &&
                (phrase_nodes[index+5]->type == ident::RBRACE))
            {
                phrase_t<CHAR>* node = new phrase_t<CHAR>();

                node->type = phrase::FUNCTION_DECL;

                node->line = phrase_nodes[index]->line;
                node->column = phrase_nodes[index]->column;

                phrase_nodes[index]->parent = node;
                phrase_nodes[index+2]->parent = node;

                node->children.add(phrase_nodes[index]);
                node->children.add(phrase_nodes[index+2]);

                node->file = phrase_nodes[index]->file;

                delete phrase_nodes[index+1];
                delete phrase_nodes[index+3];
                delete phrase_nodes[index+4];
                delete phrase_nodes[index+5];
                phrase_nodes.replace(index, index+5, node);

                result = true;
            }
            //IDENTIFIER ( ) { }
            else if (phrase_nodes.is_valid(index+4) &&
                (phrase_nodes[index]->type == ident::IDENTIFIER) &&
                (phrase_nodes[index+1]->type == ident::LPARENTH) &&
                (phrase_nodes[index+2]->type == ident::RPARENTH) &&
                (phrase_nodes[index+3]->type == ident::LBRACE) &&
                (phrase_nodes[index+4]->type == ident::RBRACE))
            {
                phrase_t<CHAR>* node = new phrase_t<CHAR>();

                node->type = phrase::FUNCTION_DECL;

                node->line = phrase_nodes[index]->line;
                node->column = phrase_nodes[index]->column;

                phrase_nodes[index]->parent = node;

                node->children.add(phrase_nodes[index]);

                node->file = phrase_nodes[index]->file;

                delete phrase_nodes[index+1];
                delete phrase_nodes[index+2];
                delete phrase_nodes[index+3];
                delete phrase_nodes[index+4];
                phrase_nodes.replace(index, index+4, node);

                result = true;
            }

            /*if (phrase_nodes.is_valid(index+5) &&
                (phrase_nodes[index]->type == ident::KEYWORD_FUNCTION) &&
                (phrase_nodes[index+1]->type == ident::IDENTIFIER) &&
                (phrase_nodes[index+2]->type == ident::LPARENTH))
            {
                //"function" IDENTIFIER ( ) ...
                if (phrase_nodes[index+3]->type == ident::RPARENTH)
                {
                    //"function" IDENTIFIER ( ) { }
                    if ((phrase_nodes[index+4]->type == ident::LBRACE) &&
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
                    //"function" IDENTIFIER ( ) IDENTIFIER { }
                    else if (phrase_nodes.is_valid(index+6) &&
                             (phrase_nodes[index+4]->type == ident::IDENTIFIER) &&
                             (phrase_nodes[index+5]->type == ident::LBRACE) &&
                             (phrase_nodes[index+6]->type == ident::RBRACE))
                    {
                        phrase_t<CHAR>* node = new phrase_t<CHAR>();

                        node->type = phrase::FUNCTION_DECL;
                        node->meta = phrase_nodes[index+4]->meta;

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
                        delete phrase_nodes[index+6];
                        phrase_nodes.replace(index, index+6, node);

                        return true;
                    }
                    //"function" IDENTIFIER ( ) "var" { }
                    else if (phrase_nodes.is_valid(index+6) &&
                             (phrase_nodes[index+4]->type == ident::KEYWORD_VAR) &&
                             (phrase_nodes[index+5]->type == ident::LBRACE) &&
                             (phrase_nodes[index+6]->type == ident::RBRACE))
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
                        delete phrase_nodes[index+6];
                        phrase_nodes.replace(index, index+6, node);

                        return true;
                    }
                    //"function" IDENTIFIER ( ) { ... }
                    else if (phrase_nodes.is_valid(index+6) &&
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
                    //"function" IDENTIFIER ( ) IDENTIFIER { ... }
                    else if (phrase_nodes.is_valid(index+7) &&
                             (phrase_nodes[index+4]->type == ident::IDENTIFIER) &&
                             (phrase_nodes[index+5]->type == ident::LBRACE) &&
                             ((phrase_nodes[index+6]->type == phrase::STATEMENTLIST) ||
                              (phrase_nodes[index+6]->type == phrase::STATEMENT)) &&
                             (phrase_nodes[index+7]->type == ident::RBRACE))
                    {
                        phrase_t<CHAR>* node = new phrase_t<CHAR>();

                        node->type = phrase::FUNCTION_DECL;
                        node->meta = phrase_nodes[index+4]->meta;

                        node->line = phrase_nodes[index]->line;
                        node->column = phrase_nodes[index]->column;

                        phrase_nodes[index+1]->parent = node;
                        phrase_nodes[index+6]->parent = node;

                        node->children.add(phrase_nodes[index+1]);
                        node->children.add(phrase_nodes[index+6]);

                        node->file = phrase_nodes[index]->file;

                        delete phrase_nodes[index];
                        delete phrase_nodes[index+2];
                        delete phrase_nodes[index+3];
                        delete phrase_nodes[index+4];
                        delete phrase_nodes[index+5];
                        delete phrase_nodes[index+7];
                        phrase_nodes.replace(index, index+7, node);

                        return true;
                    }
                    //"function" IDENTIFIER ( ) "var" { ... }
                    else if (phrase_nodes.is_valid(index+7) &&
                             (phrase_nodes[index+4]->type == ident::KEYWORD_VAR) &&
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
                        phrase_nodes[index+6]->parent = node;

                        node->children.add(phrase_nodes[index+1]);
                        node->children.add(phrase_nodes[index+6]);

                        node->file = phrase_nodes[index]->file;

                        delete phrase_nodes[index];
                        delete phrase_nodes[index+2];
                        delete phrase_nodes[index+3];
                        delete phrase_nodes[index+4];
                        delete phrase_nodes[index+5];
                        delete phrase_nodes[index+7];
                        phrase_nodes.replace(index, index+7, node);

                        return true;
                    }
                }
                //"function" IDENTIFIER ( ... ) ...
                else if (phrase_nodes.is_valid(index+6) &&
                         ((phrase_nodes[index+3]->type == phrase::FORMALVARDECL) ||
                          (phrase_nodes[index+3]->type == phrase::FORMALTYPEDECL) ||
                          (phrase_nodes[index+3]->type == phrase::FORMALDECLLIST)) &&
                         (phrase_nodes[index+4]->type == ident::RPARENTH))
                {
                    //"function" IDENTIFIER ( ... ) IDENTIFIER { ... }
                    if (phrase_nodes.is_valid(index+8) &&
                        (phrase_nodes[index+5]->type == ident::IDENTIFIER) &&
                        (phrase_nodes[index+6]->type == ident::LBRACE) &&
                        ((phrase_nodes[index+7]->type == phrase::STATEMENTLIST) ||
                         (phrase_nodes[index+7]->type == phrase::STATEMENT)) &&
                        (phrase_nodes[index+8]->type == ident::RBRACE))
                    {
                        phrase_t<CHAR>* node = new phrase_t<CHAR>();

                        node->type = phrase::FUNCTION_DECL;
                        node->meta = phrase_nodes[index+5]->meta;

                        node->line = phrase_nodes[index]->line;
                        node->column = phrase_nodes[index]->column;

                        phrase_nodes[index+1]->parent = node;
                        phrase_nodes[index+3]->parent = node;
                        phrase_nodes[index+7]->parent = node;

                        node->children.add(phrase_nodes[index+1]);
                        node->children.add(phrase_nodes[index+3]);
                        node->children.add(phrase_nodes[index+7]);

                        node->file = phrase_nodes[index]->file;

                        delete phrase_nodes[index];
                        delete phrase_nodes[index+2];
                        delete phrase_nodes[index+4];
                        delete phrase_nodes[index+5];
                        delete phrase_nodes[index+6];
                        delete phrase_nodes[index+8];
                        phrase_nodes.replace(index, index+8, node);

                        return true;
                    }
                    //"function" IDENTIFIER ( ... ) "var" { ... }
                    else if (phrase_nodes.is_valid(index+8) &&
                             (phrase_nodes[index+5]->type == ident::KEYWORD_VAR) &&
                             (phrase_nodes[index+6]->type == ident::LBRACE) &&
                             ((phrase_nodes[index+7]->type == phrase::STATEMENTLIST) ||
                              (phrase_nodes[index+7]->type == phrase::STATEMENT)) &&
                             (phrase_nodes[index+8]->type == ident::RBRACE))
                    {
                        phrase_t<CHAR>* node = new phrase_t<CHAR>();

                        node->type = phrase::FUNCTION_DECL;

                        node->line = phrase_nodes[index]->line;
                        node->column = phrase_nodes[index]->column;

                        phrase_nodes[index+1]->parent = node;
                        phrase_nodes[index+3]->parent = node;
                        phrase_nodes[index+7]->parent = node;

                        node->children.add(phrase_nodes[index+1]);
                        node->children.add(phrase_nodes[index+3]);
                        node->children.add(phrase_nodes[index+7]);

                        node->file = phrase_nodes[index]->file;

                        delete phrase_nodes[index];
                        delete phrase_nodes[index+2];
                        delete phrase_nodes[index+4];
                        delete phrase_nodes[index+5];
                        delete phrase_nodes[index+6];
                        delete phrase_nodes[index+8];
                        phrase_nodes.replace(index, index+8, node);

                        return true;
                    }
                    //"function" IDENTIFIER ( ... ) { ... }
                    else if (phrase_nodes.is_valid(index+7) &&
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
                    //"function" IDENTIFIER ( ... ) IDENTIFIER { }
                    else if (phrase_nodes.is_valid(index+7) &&
                             (phrase_nodes[index+5]->type == ident::IDENTIFIER) &&
                             (phrase_nodes[index+6]->type == ident::LBRACE) &&
                             (phrase_nodes[index+7]->type == ident::RBRACE))
                    {
                        phrase_t<CHAR>* node = new phrase_t<CHAR>();

                        node->type = phrase::FUNCTION_DECL;
                        node->meta = phrase_nodes[index+5]->meta;

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
                        delete phrase_nodes[index+7];
                        phrase_nodes.replace(index, index+7, node);

                        return true;
                    }
                    //"function" IDENTIFIER ( ... ) "var" { }
                    else if (phrase_nodes.is_valid(index+7) &&
                             (phrase_nodes[index+5]->type == ident::KEYWORD_VAR) &&
                             (phrase_nodes[index+6]->type == ident::LBRACE) &&
                             (phrase_nodes[index+7]->type == ident::RBRACE))
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
                        delete phrase_nodes[index+7];
                        phrase_nodes.replace(index, index+7, node);

                        return true;
                    }
                    //"function" IDENTIFIER ( ... ) { }
                    else if ((phrase_nodes[index+5]->type == ident::LBRACE) &&
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


                }
            }*/

            if (result &&
                phrase_nodes.is_valid(index-1))
            {
                //explicit function return type "var"
                if (phrase_nodes[index-1]->type == ident::KEYWORD_VAR)
                {
                    delete phrase_nodes[index-1];
                    phrase_nodes.remove(index-1);
                }
                //specific function return type
                else if (phrase_nodes[index-1]->type == ident::IDENTIFIER)
                {
                    phrase_nodes[index]->meta = phrase_nodes[index-1]->meta;

                    delete phrase_nodes[index-1];
                    phrase_nodes.remove(index-1);
                }
            }

            return result;
        }
    }
}
}

#endif // FUNCTION_DECL_H_INCLUDED
