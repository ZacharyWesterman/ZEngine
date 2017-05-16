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

#include "phrase.h"

namespace z
{
    namespace script
    {
        template <typename CHAR>
        class lexer
        {
        private:
            core::array< ident_t<CHAR> >* input_ident;

            core::array< phrase<CHAR> > syntax_tree;

            bool initial_scan;

            int current_ident;
            int current_index;

        public:
            lexer()
            {
                input_ident = NULL;
                initial_scan = true;

                current_ident = -1;
                current_index = -1;
            }

            void setInput(core::array< ident_t<CHAR> >& identifiers)
            {
                input_ident = &identifiers;
                syntax_tree.clear();

                initial_scan = true;
            }

            bool lex(const core::timeout&);
        };


        template <typename CHAR>
        bool lexer<CHAR>::lex(const core::timeout& time)
        {
            while (!time.timedOut())
            {
                if (initial_scan)
                {
                    if ()

                    initial_scan = false;
                }
                else
                {

                }
            }

            return !time.timedOut();
        }
    }
}

#endif // LEXER_H_INCLUDED
