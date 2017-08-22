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
 * Last modified:   21 Aug. 2017
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

            inline bool operator==(const varSignature& other) const
            { return (ID == other.ID); }

            varSignature(void* _ID, void* _type = NULL)
            {
                ID = _ID;
                type = _type;
            }
        };

        struct varScope
        {
            varScope* parent;

            core::array< varSignature > vars;

            core::array< varScope > children;


            varScope(varScope* _parent = NULL)
            { parent = _parent; }

            inline bool operator==(const varScope& other) const
            { return false; }

            error_flag addVar(const varSignature&);
            bool assignVar(const varSignature&);

            bool exists(const varSignature&);
        };

        error_flag varScope::addVar(const varSignature& _var)
        {
            for (int i=0; i<vars.size(); i++)
                if ((vars[i].ID == _var.ID) && (vars[i].type == _var.type))
                    return error::VARIABLE_REDECLARED;

            vars.add(_var);
            return error::NONE;
        }

        //returns true if variable was previously defined, false otherwise.
        bool varScope::assignVar(const varSignature& _var)
        {
            if (!exists(_var))
            {
                vars.add(_var);
                return true;
            }
            else
                return false;
        }

        bool varScope::exists(const varSignature& _var)
        {
            varScope* _scope = this;

            while (_scope->vars.find(_var) == -1)
            {
                if (_scope->parent)
                    _scope = _scope->parent;
                else
                    return false;
            }

            return true;
        }


        ///debug
        void printScope(const varScope& _scope, int padding = 0, int scopeNum = 0)
        {
            for (int i=0; i<_scope.vars.size(); i++)
            {
                cout << scopeNum << " ";

                for (int i=0; i<padding; i++)
                    cout << "  ";

                cout << "Tp=" << _scope.vars[i].type
                     << ",ID=" << _scope.vars[i].ID << endl;
            }

            for (int i=0; i<_scope.children.size(); i++)
                printScope(_scope.children[i], padding+1, ++scopeNum);
        }



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


            varScope global_scope;
            varScope* current_scope;




            void enter_node(int);
            void exit_node();


            void appendType();

            void enter_scope();
            void exit_scope();


            void analyze_variable_decl();
            void analyze_function_decl();
            void analyze_assignexpr();

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


                global_scope.parent = NULL;
                current_scope = &global_scope;
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


        template <typename CHAR>
        void semanticAnalyzer<CHAR>::enter_scope()
        {
            current_scope->children.add(varScope(current_scope));

            current_scope = &current_scope->children[current_scope->children.size() -1];
        }

        template <typename CHAR>
        void semanticAnalyzer<CHAR>::exit_scope()
        {
            if (current_scope->parent)
                current_scope = current_scope->parent;
        }


        ///Main semantic analysis function.
        //Returns true if finished, false otherwise.
        template <typename CHAR>
        bool semanticAnalyzer<CHAR>::analyze(const core::timeout& time)
        {
            while (!is_done && !time.timedOut())
            {
                //variable declaration
                if (root->type == phrase::VARIABLE_DECL)
                {
                    analyze_variable_decl();
                }
                //variable assignment
                else if (root->type == phrase::ASSIGNEXPR)
                {
                    analyze_assignexpr();
                }
                else if (root->type == phrase::FUNCTION_DECL)
                {
                    analyze_function_decl();
                }
                else
                {
                    if (index >= (root->children).size())
                        exit_node();
                    else
                        enter_node(index);
                }
            }

            ///debug
            if (is_done)
            {
                printScope(global_scope);
            }

            return is_done;
        }


        template <typename CHAR>
        void semanticAnalyzer<CHAR>::analyze_variable_decl()
        {
            error_flag err =
                current_scope->addVar(varSignature(root->children[0]->meta));

            if (err)
                error_buffer.add(parser_error<CHAR>(root->line,
                                                    root->column,
                                                    err,
                                    *((core::string<CHAR>*)root->children[0]->meta),
                                                    root->file));

            exit_node();
        }

        template <typename CHAR>
        void semanticAnalyzer<CHAR>::analyze_assignexpr()
        {
            if (index < 1)
            {
                varSignature _var (root->children[0]->children[0]->meta);

                //if the variable we are assigning to has not already been declared,
                //insert a variable declaration beforehand.
                if (current_scope->assignVar(_var))
                {
                    int prev_index;
                    index_stack.pop(prev_index);

                    //create new node (variable decl ID)
                    phrase_t<CHAR>* IDnode = new phrase_t<CHAR>();

                    IDnode->type = ident::IDENTIFIER;
                    IDnode->line = root->line;
                    IDnode->column = root->column;
                    IDnode->file = root->file;
                    IDnode->meta = root->children[0]->children[0]->meta;

                    //create new node (variable declaration)
                    phrase_t<CHAR>* varNode = new phrase_t<CHAR>();

                    varNode->type = phrase::VARIABLE_DECL;
                    varNode->line = root->line;
                    varNode->column = root->column;
                    varNode->file = root->file;

                    varNode->children.add(IDnode);
                    IDnode->parent = varNode;

                    root->parent->children.insert(varNode, prev_index-1);
                    varNode->parent = root->parent;

                    prev_index++;
                    index_stack.push(prev_index);

                    //varNode->children.add(phrase_nodes[index+1]);


                }

                enter_node(2);
            }
            else
                exit_node();
        }


        template <typename CHAR>
        void semanticAnalyzer<CHAR>::analyze_function_decl()
        {
            //TODO: register function signature

            if (index < 1)
            {
                enter_scope();
                enter_node(root->children.size() -1);
            }
            else
            {
                exit_scope();
                exit_node();
            }
        }


    }
}

#endif // SEMANTICANALYZER_H_INCLUDED
