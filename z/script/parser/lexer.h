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

            core::array< phrase_t<CHAR> > phrase_list;

            bool initial_scan;

            int index;
            bool found_error;

        public:
            lexer()
            {
                input_ident = NULL;
                initial_scan = true;

                index = 0;
            }

            void setInput(core::array< ident_t<CHAR> >& identifiers)
            {
                input_ident = &identifiers;
                phrase_list.clear();

                initial_scan = true;

                index = 0;
            }

            bool lex(const core::timeout&);

            inline bool error() {return found_error;}
        };


        template <typename CHAR>
        bool lexer<CHAR>::lex(const core::timeout& time)
        {
            while (!time.timedOut())
            {
                if (initial_scan)
                {
                    phrase_list.add(phrase_t<CHAR>());
                    phrase_list[index].data = input_ident[index];

                    index++;

                    if (index >= input_ident.size())
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
