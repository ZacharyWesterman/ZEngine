/**
 * File:            statement.h
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
 * Last modified:   22 Aug. 2017
**/

#pragma once
#ifndef STATEMENT_H_INCLUDED
#define STATEMENT_H_INCLUDED

namespace z
{
namespace script
{
    namespace compiler
    {
        template <typename CHAR>
        bool lexer<CHAR>::statement()
        {
            if ((!phrase_nodes.is_valid(index-1) ||
                  ((phrase_nodes[index-1]->type >= phrase::STATEMENTLIST) &&
                   (phrase_nodes[index-1]->type <= phrase::FUNCTION_DECL)) ) &&
                (phrase_nodes[index]->type == ident::SEMICOLON))
            {
                delete phrase_nodes[index];
                phrase_nodes.remove(index);

                return true;
            }
            else if (phrase_nodes.is_valid(index-1) &&
                ((phrase_nodes[index-1]->type == ident::LBRACE) ||
                 (phrase_nodes[index-1]->type == ident::RPARENTH) ||
                 (phrase_nodes[index-1]->type == phrase::STATEMENT)))
            {
                if (phrase_nodes.is_valid(index+1) &&
                     ((((phrase_nodes[index]->type == phrase::BOOLEXPR) &&
                       !(((phrase_nodes[index-1]->type >= ident::OPER_ASSIGN) &&
                           (phrase_nodes[index-1]->type <= ident::OPER_MOD_ASSIGN)) ||
                          (phrase_nodes[index-1]->type == ident::KEYWORD_RUN) ||
                          (phrase_nodes[index-1]->type == ident::KEYWORD_STOP) ||
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
                      (phrase_nodes[index-1]->type == ident::SEMICOLON))) ||
                      ((phrase_nodes[index]->type == phrase::COMMAND) &&
                       (phrase_nodes[index+1]->type == ident::SEMICOLON))))
                {
                    if (phrase_nodes[index]->orig_type == ident::NONE)
                        phrase_nodes[index]->orig_type = phrase_nodes[index]->type;
                    phrase_nodes[index]->type = phrase::STATEMENT;

                    delete phrase_nodes[index+1];
                    phrase_nodes.remove(index+1);

                    return true;
                }
                else if (((phrase_nodes[index]->type == phrase::IF_STATEMENT) &&
                          !(phrase_nodes.is_valid(index+1) &&
                            phrase_nodes[index+1]->type == ident::KEYWORD_ELSE)) ||
                         (phrase_nodes[index]->type == phrase::FOR_STATEMENT) ||
                         (phrase_nodes[index]->type == phrase::FOREACH_STATEMENT) ||
                         (phrase_nodes[index]->type == phrase::LOOP_STATEMENT) ||
                         (phrase_nodes[index]->type == phrase::WHILE_PRE_STMT) ||
                         (phrase_nodes[index]->type == phrase::WHILE_POST_STMT) ||
                         (phrase_nodes[index]->type == phrase::RUN_STATEMENT) ||
                         (phrase_nodes[index]->type == phrase::STOP_STATEMENT) ||
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
    }
}
}

#endif // STATEMENT_H_INCLUDED
