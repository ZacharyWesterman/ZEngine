/**
 * File:            statementlist.h
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
 * Last modified:   20 Aug. 2017
**/

#pragma once
#ifndef STATEMENTLIST_H_INCLUDED
#define STATEMENTLIST_H_INCLUDED

namespace z
{
namespace script
{
    namespace compiler
    {

        bool lexer::statementlist()
        {
            if (phrase_nodes->is_valid(index+1) &&
                (phrase_nodes->at(index+1)->type == STATEMENT))
            {
                if (phrase_nodes->at(index)->type == STATEMENT)
                {
                    phrase_t* node = new phrase_t();

                    node->type = STATEMENTLIST;

                    node->line = phrase_nodes->at(index)->line;
                    node->column = phrase_nodes->at(index)->column;

                    phrase_nodes->at(index)->parent = node;
                    phrase_nodes->at(index+1)->parent = node;

                    node->children.add(phrase_nodes->at(index));
                    node->children.add(phrase_nodes->at(index+1));

                    node->file = phrase_nodes->at(index)->file;

                    phrase_nodes->replace(index, index+1, node);

                    return true;
                }
                else if (phrase_nodes->at(index)->type == STATEMENTLIST)
                {
                    phrase_nodes->at(index)->children.add(phrase_nodes->at(index+1));
                    phrase_nodes->at(index+1)->parent = phrase_nodes->at(index);
                    phrase_nodes->remove(index+1);

                    return true;
                }
            }

            return false;
        }
    }
}
}

#endif // STATEMENTLIST_H_INCLUDED
