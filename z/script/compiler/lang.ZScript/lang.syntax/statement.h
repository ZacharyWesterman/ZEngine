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

        class statement : public syntaxRule
        {
        public:
            ~statement() {}

            bool apply(core::array< phrase_t* >*,
                       int,
                       core::array<error>*);
        };


        bool statement::apply(core::array< phrase_t* >* phrase_nodes,
                                  int index,
                                  core::array<error>* error_buffer)
        {
            //remove excess
            if ((!phrase_nodes->is_valid(index-1) ||
                  ((phrase_nodes->at(index-1)->type >= STATEMENTLIST) &&
                   (phrase_nodes->at(index-1)->type <= FUNCTION_DECL)
                   ) ||
                 (phrase_nodes->at(index-1)->type == ident::SEMICOLON) ||
                 (phrase_nodes->at(index-1)->type == ident::LBRACE)
                 ) &&
                (phrase_nodes->at(index)->type == ident::SEMICOLON)
                )
            {
                delete phrase_nodes->at(index);
                phrase_nodes->remove(index);

                return true;
            }
            else if (phrase_nodes->is_valid(index-1) &&
                     ((phrase_nodes->at(index-1)->type == ident::LBRACE) /*||
                      ((phrase_nodes->at(index-1)->type == ident::RPARENTH) &&
                       !(phrase_nodes->is_valid(index-3) &&
                         (phrase_nodes->at(index-2)->type == ident::IDENTIFIER) &&
                         (phrase_nodes->at(index-3)->type == ident::KEYWORD) &&
                         (phrase_nodes->at(index-3)->metaValue == TYPE)
                         )
                       )*/ ||
                      (phrase_nodes->at(index-1)->type == STATEMENT) ||
                      (phrase_nodes->at(index-1)->type == STATEMENTLIST)
                      )
                     )
            {
                if (phrase_nodes->is_valid(index+1) &&
                    (((phrase_nodes->at(index)->type == BOOLEXPR) &&
                      (phrase_nodes->at(index+1)->type == ident::SEMICOLON)
                      ) ||
                     ((phrase_nodes->at(index)->type == COMMAND) &&
                      (phrase_nodes->at(index+1)->type == ident::SEMICOLON)
                      )
                     )
                    )
                {
                    setSuperType(phrase_nodes->at(index), STATEMENT);

                    delete phrase_nodes->at(index+1);
                    phrase_nodes->remove(index+1);

                    return true;
                }
                else if (((phrase_nodes->at(index)->type == IF_STATEMENT) &&
                          !(phrase_nodes->is_valid(index+1) &&
                            (phrase_nodes->at(index+1)->type == ident::KEYWORD) &&
                            (phrase_nodes->at(index+1)->metaValue == ELSE)
                            )
                          ) ||
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
                           (phrase_nodes->at(index)->type == TYPEVAR_DECL)
                           )/* &&
                           !(phrase_nodes->is_valid(index-3) &&
                             (phrase_nodes->at(index-3)->type == ident::KEYWORD_TYPE)
                             )*/
                          ) ||
                         (phrase_nodes->at(index)->type == LABEL_STATEMENT) ||
                         (phrase_nodes->at(index)->type == GOTO_STATEMENT) ||
                         (phrase_nodes->at(index)->type == GOSUB_STATEMENT)
                         )
                {
                    setSuperType(phrase_nodes->at(index), STATEMENT);

                    return true;
                }
            }

            return false;
        }
    }
}
}

#endif // STATEMENT_H_INCLUDED
