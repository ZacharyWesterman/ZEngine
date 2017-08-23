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
        typedef unsigned long symID;

        struct varSignature
        {
            void* ID;
            void* type;

            symID uniqueID;

            inline bool operator==(const varSignature& other) const
            { return (ID == other.ID); }

            varSignature(void* _ID, unsigned long _uniqueID = 0, void* _type = NULL)
            {
                ID = _ID;
                type = _type;

                uniqueID = _uniqueID;
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

            symID getVarUniqueID(const varSignature&);
            void* getVarType(const varSignature&);

            varSignature getVariable(const varSignature&);
        };

        error_flag varScope::addVar(const varSignature& _var)
        {
            if (vars.find(_var) > -1)
                return error::VARIABLE_REDECLARED;

            vars.add(_var);
            return error::NONE;
        }

        //returns false if variable was previously defined, true otherwise.
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

            while (_scope->vars.find(_var) < 0)
            {
                if (_scope->parent)
                    _scope = _scope->parent;
                else
                    return false;
            }

            return true;
        }

        symID varScope::getVarUniqueID(const varSignature& _var)
        {
            varScope* _scope = this;

            int index = _scope->vars.find(_var);

            while (index < 0)
            {
                if (_scope->parent)
                    _scope = _scope->parent;
                else
                    return 0;

                index = _scope->vars.find(_var);
            }

            return _scope->vars[index].uniqueID;
        }

        void* varScope::getVarType(const varSignature& _var)
        {
            varScope* _scope = this;

            int index = _scope->vars.find(_var);

            while (index < 0)
            {
                if (_scope->parent)
                    _scope = _scope->parent;
                else
                    return NULL;

                index = _scope->vars.find(_var);
            }

            return _scope->vars[index].type;
        }

        varSignature varScope::getVariable(const varSignature& _var)
        {
            varScope* _scope = this;

            int index = _scope->vars.find(_var);

            while (index < 0)
            {
                if (_scope->parent)
                    _scope = _scope->parent;
                else
                    return varSignature(NULL);

                index = _scope->vars.find(_var);
            }

            return _scope->vars[index];
        }


        ///debug
        void printScope(const varScope& _scope, int padding = 0, int scopeNum = 0)
        {
            for (int i=0; i<_scope.vars.size(); i++)
            {
                cout << scopeNum << " ";

                for (int i=0; i<padding; i++)
                    cout << "  ";

                cout << "Tp=" << (_scope.vars[i].type ? ((core::string<char>*)_scope.vars[i].type)->str() : "var")
                     << ",\tNm=" << ((core::string<char>*)_scope.vars[i].ID)->str()
                     << ",\tID=" << _scope.vars[i].uniqueID << endl;
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

            symID uniqueID_current;


            core::dynamicStack<void*> typeStack;
            phrase_t<CHAR>* exprStart;

            void enter_node(int);
            void exit_node();


            void appendType();

            void enter_scope();
            void exit_scope();


            void analyze_variable_decl();
            void analyze_function_decl();
            void analyze_assignexpr();
            void analyze_variable();
            void analyze_expression();
            void analyze_typevar_decl();

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

                uniqueID_current = 1; //reserve 0 for NULL

                exprStart = NULL;
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
                else if (root->type == phrase::VARIABLE)
                {
                    analyze_variable();
                }
                else if ((root->type >= phrase::PARENTHEXPR) &&
                         (root->type <= phrase::SIZEOFEXPR))
                {
                    analyze_expression();
                }
                else if (root->type == phrase::TYPEVAR_DECL)
                {
                    analyze_typevar_decl();
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
                current_scope->addVar(varSignature(root->children[0]->meta, uniqueID_current++));

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
                varSignature _var (root->children[0]->children[0]->meta, uniqueID_current);

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
                    varNode->metaValue = uniqueID_current;

                    varNode->children.add(IDnode);
                    IDnode->parent = varNode;

                    root->parent->children.insert(varNode, prev_index-1);
                    varNode->parent = root->parent;

                    prev_index++;
                    index_stack.push(prev_index);


                    //root->children[0]->metaValue = uniqueID_current;

                    //increment uniqueID
                    uniqueID_current++;
                }

                enter_node(0);
            }
            else if (index < 2)
            {
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

        template <typename CHAR>
        void semanticAnalyzer<CHAR>::analyze_variable()
        {
            varSignature _var = current_scope->getVariable(
                                    varSignature(root->children[0]->meta) );

            symID varID = _var.uniqueID;

            if (!varID)
                error_buffer.add(parser_error<CHAR>(root->line,
                                                    root->column,
                                                    error::VARIABLE_UNDECLARED,
                                    *((core::string<CHAR>*)root->children[0]->meta),
                                                    root->file));
            else
            {
                root->metaValue = varID;
                typeStack.push(_var.type);
            }

            exit_node();
        }

        template <typename CHAR>
        void semanticAnalyzer<CHAR>::analyze_expression()
        {
            if (index >= (root->children).size())
            {
                if (exprStart == root)
                {
                    exprStart = NULL;

                    void* type1;
                    void* type2;
                    typeStack.pop(type1);

                    bool type_match = false;

                    while(typeStack.pop(type2) && type_match)
                    {
                        if (type2 != type1)
                            type_match = false;

                        type1 = type2;
                    }

                    if (!type_match)
                        error_buffer.add(parser_error<CHAR>(root->line,
                                                    root->column,
                                                    error::TYPE_MISMATCH,
                                                    NULL,
                                                    root->file));
                }
                //expressions with 3 children will always have 2 operands
                //also, POWEREXPRs with 2 children will have 2 operands
                else if ((root->children.size() >= 3) ||
                         ((root->type == phrase::POWEREXPR) &&
                          (root->children.size() == 2)) )
                {
                    void* type1;
                    void* type2;

                    if (typeStack.pop(type1) &&
                        typeStack.pop(type2))
                    {
                        if (type1 != type2))
                        {
                            error_buffer.add(parser_error<CHAR>(root->line,
                                                        root->column,
                                                        error::TYPE_MISMATCH,
                                                        NULL,
                                                        root->file));

                            typeStack.dump();
                        }

                        typeStack.push(type1);
                    }
                }
                else if (root->children.size() <= 2)
                {
                    void* type1;

                    if (typeStack.pop(type1))
                    {
                        if ((root->type == phrase::DIMENSIONEXPR) &&
                            (root->children[0]->type == ident::IDENTIFIER))
                        {
                            typeStack.push(root->children[0]->meta);
                        }
                    }
                }

                exit_node();
            }
            else
            {
                if (!exprStart)
                    exprStart = root;

                enter_node(index);
            }
        }

        template <typename CHAR>
        void semanticAnalyzer<CHAR>::analyze_typevar_decl()
        {
            error_flag err =
                current_scope->addVar(varSignature(root->children[1]->meta,
                                                   uniqueID_current++,
                                                   root->children[0]->meta));

            if (err)
                error_buffer.add(parser_error<CHAR>(root->line,
                                                    root->column,
                                                    err,
                                    *((core::string<CHAR>*)root->children[0]->meta),
                                                    root->file));

            exit_node();
        }
    }
}

#endif // SEMANTICANALYZER_H_INCLUDED
