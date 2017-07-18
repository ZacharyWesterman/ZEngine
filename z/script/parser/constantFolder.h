/**
 * File:            constantFolder.h
 * Namespace:       z::script
 * Description:     A template class for optimizing a given
 *                  syntax tree via constant folding. The
 *                  folding is performed incrementally so
 *                  that script AST(s) can be optimized during
 *                  run-time without making the program
 *                  unresponsive.
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   18 Jul. 2017
**/

#pragma once
#ifndef CONSTANTFOLDER_H_INCLUDED
#define CONSTANTFOLDER_H_INCLUDED

#include <z/core/dynamicStack.h>
#include <z/core/timeout.h>
#include "../errors.h"

#include "phrase.h"

#ifndef NULL
    #define NULL 0
#endif // NULL

namespace z
{
    namespace script
    {
        template <typename CHAR>
        class constantFolder
        {
        private:
            core::array< parser_error<CHAR> > error_buffer;

            phrase_t<CHAR>* root;

            int index;

            core::dynamic_stack<int> index_stack;

            bool is_done;

        public:
            constantFolder()
            {
                index = 0;
                root = NULL;
            };

            ~constantFolder(){};


            void setInput(phrase_t<CHAR>* new_root)
            {
                index_stack.dump();

                index = 0;
                root = new_root;
            }

            inline bool error() {return (error_buffer.size() > 0);}

            inline bool done() {return (is_done || !root);}


            bool fold(const core::timeout& time);
        };
    }
}

#endif // CONSTANTFOLDER_H_INCLUDED
