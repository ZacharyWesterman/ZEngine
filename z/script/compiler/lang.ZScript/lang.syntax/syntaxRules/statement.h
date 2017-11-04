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

        bool lexer::statement()
        {
            if ((!phrase_nodes->is_valid(index-1) ||
                  ((phrase_nodes->at(index-1)->type >= STATEMENTLIST) &&
                   (phrase_nodes->at(index-1)->type <= FUNCTION_DECL)) ) &&
                (phrase_nodes->at(index)->type == ident::SEMICOLON))
            {
                delete phrase_nodes->at(index);
                phrase_nodes->remove(index);

                return true;
            }
            else if (phrase_nodes->is_valid(index-1) &&
                ((phrase_nodes->at(index-1)->type == ident::LBRACE) ||
                 (phrase_nodes->at(index-1)->type == ident::RPARENTH) ||
                 (phrase_nodes->at(index-1)->type == STATEMENT)))
            {
                if (phrase_nodes->is_valid(index+1) &&
                     ((((phrase_nodes->at(index)->type == BOOLEXPR) &&
                       !(((phrase_nodes->at(index-1)->type >= ident::OPER_ASSIGN) &&
                           (phrase_nodes->at(index-1)->type <= ident::OPER_MOD_ASSIGN)) ||
                          (phrase_nodes->at(index-1)->type == ident::KEYWORD_RUN) ||
                          (phrase_nodes->at(index-1)->type == ident::KEYWORD_STOP) ||
                          (phrase_nodes->at(index-1)->type == ident::KEYWORD_RETURN) ||
                          (phrase_nodes->at(index-1)->type == ident::KEYWORD_WAIT) ||
                          (phrase_nodes->at(index-1)->type == ident::KEYWORD_UNTIL) ||
                          (phrase_nodes->at(index-1)->type == ident::LBRACKET) ||
                          (phrase_nodes->at(index-1)->type == RANGELIST) ||
                          (phrase_nodes->at(index-1)->type == ident::OPER_R_ARROW) ||
                          (phrase_nodes->at(index-1)->type == ident::OPER_L_ARROW))) &&
                    (phrase_nodes->at(index+1)->type == ident::SEMICOLON) &&
                    !(phrase_nodes->is_valid(index-2) &&
                      (phrase_nodes->at(index-2)->type == ident::KEYWORD_FOR)) &&
                    !(phrase_nodes->is_valid(index-1) &&
                      (phrase_nodes->at(index-1)->type == ident::SEMICOLON))) ||
                      ((phrase_nodes->at(index)->type == COMMAND) &&
                       (phrase_nodes->at(index+1)->type == ident::SEMICOLON))))
                {
                    if (phrase_nodes->at(index)->orig_type == ident::NONE)
                        phrase_nodes->at(index)->orig_type = phrase_nodes->at(index)->type;
                    phrase_nodes->at(index)->type = STATEMENT;

                    delete phrase_nodes->at(index+1);
                    phrase_nodes->remove(index+1);

                    return true;
                }
                else if (((phrase_nodes->at(index)->type == IF_STATEMENT) &&
                          !(phrase_nodes->is_valid(index+1) &&
                            phrase_nodes->at(index+1)->type == ident::KEYWORD_ELSE)) ||
                         (phrase_nodes->at(index)->type == FOR_STATEMENT) ||
                         (phrase_nodes->at(index)->type == FOREACH_STATEMENT) ||
                         (phrase_nodes->at(index)->type == LOOP_STATEMENT) ||
                         (phrase_nodes->at(index)->type == WHILE_PRE_STMT) ||
                         (phrase_nodes->at(index)->type == WHILE_POST_STMT) ||
                         (phrase_nodes->at(index)->type == RUN_STATEMENT) ||
                         (phrase_nodes->at(index)->type == STOP_STATEMENT) ||
                         (phrase_nodes->at(index)->type == RETURN_STATEMENT) ||
                         (phrase_nodes->at(index)->type == WAIT_STATEMENT) ||
                         (phrase_nodes->at(index)->type == UNTIL_STATEMENT) ||
                         (((phrase_nodes->at(index)->type == VARIABLE_DECL) ||
                           (phrase_nodes->at(index)->type == TYPEVAR_DECL)) &&
                           !(phrase_nodes->is_valid(index-3) &&
                             (phrase_nodes->at(index-3)->type == ident::KEYWORD_TYPE))) ||
                         (phrase_nodes->at(index)->type == LABEL_STATEMENT) ||
                         (phrase_nodes->at(index)->type == GOTO_STATEMENT) ||
                         (phrase_nodes->at(index)->type == GOSUB_STATEMENT))
                {
                    if (phrase_nodes->at(index)->orig_type == ident::NONE)
                        phrase_nodes->at(index)->orig_type = phrase_nodes->at(index)->type;
                    phrase_nodes->at(index)->type = STATEMENT;

                    return true;
                }
            }

            return false;
        }
    }
}
}

#endif // STATEMENT_H_INCLUDED
