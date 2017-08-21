/**
 * File:            semanticAnalyzer.h
 * Namespace:       z::script
 * Description:     Semantic analyzer template. Checks
 *                  for invalid semantics in the given
 *                  syntax tree. Additionally, indexes
 *                  variables, functions, types, and
 *                  commands for easy reference later on.
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   20 Aug. 2017
**/

#pragma once
#ifndef SEMANTICANALYZER_H_INCLUDED
#define SEMANTICANALYZER_H_INCLUDED

#include <z/core/array.h>
#include <z/core/dynamicStack.h>
#include <z/core/timeout.h>
#include "../errors.h"

#include "phrase.h"

#include "../command_t.h"
#include "../function_t.h"

#ifndef NULL
    #define NULL 0
#endif // NULL

#include <iostream>
using namespace std;

namespace z
{
    namespace script
    {
        struct varSignature
        {
            void* ID;
            void* type;
        };

        struct typeSignature
        {
            void* ID;

            core::array<varSignature> vars;
        };


        template <typename CHAR>
        class semanticAnalyzer
        {
        private:
            const core::array< command_t<CHAR>* >* commands;
            const core::array< function_t<CHAR>* >* functions;

            phrase_t<CHAR>* root;

            int index;
            core::dynamicStack<int> index_stack;

            bool is_done;



            core::array< void* > type_list;



            void enter_node(int);
            void exit_node();

            void appendType();

        public:
            core::array< parser_error<CHAR> > error_buffer;

            semanticAnalyzer(const core::array< command_t<CHAR>* >& _commands,
                     const core::array< function_t<CHAR>* >& _functions)
            {
                index = 0;
                root = NULL;

                is_done = true;

                commands = &_commands;
                functions = &_functions;
            };

            ~semanticAnalyzer(){};


            void setInput(phrase_t<CHAR>* new_root)
            {
                index_stack.dump();

                index = 0;
                root = new_root;

                is_done = (root == NULL);
            }

            inline bool error() {return (error_buffer.size() > 0);}

            inline bool done() {return is_done;}


            bool analyze(const core::timeout&);


        };



        template <typename CHAR>
        void semanticAnalyzer<CHAR>::enter_node(int entry)
        {
            root = root->children[entry];
            index_stack.push(entry+1);

            index = 0;
        }

        template <typename CHAR>
        void semanticAnalyzer<CHAR>::exit_node()
        {
            if (!index_stack.pop(index))
                is_done = true;
            else
                root = root->parent;
        }



        ///Main semantic analysis function.
        //Returns true if finished, false otherwise.
        template <typename CHAR>
        bool semanticAnalyzer<CHAR>::analyze(const core::timeout& time)
        {
            while (!is_done && !time.timedOut())
            {
                /*if (root->type == phrase::TYPEDECL)
                {
                    if ((root->children.size() > 1) &&
                        (index < 1))
                    {
                        type_list.add(root->children[0]->meta);
                        enter_node(1);
                    }
                    else
                        exit_node();
                }*/
                //else if (root->type == phrase::
                //else
                {
                    if (index >= (root->children).size())
                        exit_node();
                    else
                        enter_node(index);
                }
            }

            ///debug
            /*if (is_done)
            {
                cout << "VARS:\n";
                for (int i=0; i<type_list.size(); i++)
                {
                    cout << ((core::string<CHAR>*)type_list[i])->str() << endl;
                }

                cout << endl;
            }*/

            return is_done;
        }
    }
}

#endif // SEMANTICANALYZER_H_INCLUDED
