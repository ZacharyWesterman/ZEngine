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
#include <z/core/timeout.h>
#include "../error.h"

#include "phrase.h"

#include "../command.h"
#include "../function.h"

#include "semanticScope.h"
#include "semanticRule.h"

#include "signatures.h"
#include "varScope.h"

#ifndef NULL
    #define NULL 0
#endif // NULL

#include <iostream>
using namespace std;

namespace z
{
namespace script
{
    namespace compiler
    {
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

        void genFuncSigString(const funcSignature& _func, core::string<char>& msg)
        {
            if (_func.returnType)
                msg = *((core::string<CPL_CHAR>*)_func.returnType);
            else
                msg = "var";

            msg += ' ';
            if (_func.inType)
            {
                msg += *((core::string<CPL_CHAR>*)_func.inType);
                msg += '.';
            }
            msg += *((core::string<CPL_CHAR>*)_func.ID);


            msg += '(';


            for(int i=0; i<_func.params.size(); i++)
            {
                if (i)
                    msg += ',';

                if (_func.paramTypes[i])
                    msg += *((core::string<CPL_CHAR>*)_func.paramTypes[i]);
                else
                    msg += "var";


            }

            msg += ')';
        }


        class semanticAnalyzer
        {
        private:
            const core::array< command* >* commands;
            const core::array< function* >* functions;

            const core::array< semanticRule* >* rules;

            phrase_t* root;

            int index;
            core::dynamicStack<int> index_stack;
            bool is_done;

            semanticScope semantics;


            void enter_node(int);
            void exit_node();


            void appendType();

        public:
            core::array< error > error_buffer;

            semanticAnalyzer(const core::array< command* >* _commands,
                             const core::array< function* >* _functions,
                             const core::array< semanticRule* >* _rules)
            {
                index = 0;
                root = NULL;

                is_done = true;

                commands = _commands;
                functions = _functions;
                rules = _rules;
            };

            ~semanticAnalyzer(){};


            void setInput(phrase_t* new_root)
            {
                error_buffer.clear();

                index_stack.dump();

                index = 0;
                root = new_root;

                is_done = ((root == NULL) ||
                           (commands == NULL) ||
                           (functions == NULL) ||
                           (rules == NULL));

                semantics.clear();
            }

            inline bool good() {return (error_buffer.size() == 0);}
            inline bool bad() {return (error_buffer.size() != 0);}

            inline bool done() {return is_done;}


            bool analyze(const core::timeout&);


        };




        void semanticAnalyzer::enter_node(int entry)
        {
            root = root->children[entry];
            index_stack.push(entry+1);

            index = 0;
        }


        void semanticAnalyzer::exit_node()
        {
            if (!index_stack.pop(index) ||
                !(root->parent))
                is_done = true;
            else
                root = root->parent;
        }


        ///Main semantic analysis function.
        //Returns true if finished, false otherwise.

        bool semanticAnalyzer::analyze(const core::timeout& time)
        {
            while (!is_done && !time.timedOut())
            {
                //check each rule,
                int r = 0;
                while ((r < (rules->size())) &&
                       !(rules->at(r)->check(commands,
                                            functions,
                                            &semantics,
                                            root,
                                            index,
                                            &error_buffer))
                        )
                {
                    r++;
                }

                //then enter the next available node.
                if ((index >= (root->children).size()) ||
                    (index < 0)
                    )
                    exit_node();
                else
                    enter_node(index);
            }

            ///debug
            if (is_done)
            {
                printScope(semantics.globalScope);
                //print functions
                for (int i=0; i<semantics.functionList.size(); i++)
                {
                    cout << endl;
                    core::string<char> msg;
                    genFuncSigString(semantics.functionList[i], msg);
                    cout << msg.str() << ", \tID="
                         << semantics.functionList[i].uniqueID;
                }
                //print types
                for (int i=0; i<semantics.typeList.size(); i++)
                {
                    cout << endl;
                    cout << "type "
                         << ((core::string<CPL_CHAR>*)semantics.typeList[i].type)->narrow().str()
                         << "{ vars=";
                    for (int v=0; v<semantics.typeList[i].vars.size(); v++)
                    {
                        if (v)
                            cout << ',';
                        cout << semantics.typeList[i].vars[v];
                    }
                    cout << "  funcs=";
                    for (int f=0; f<semantics.typeList[i].funcs.size(); f++)
                    {
                        if (f)
                            cout << ',';
                        cout << semantics.typeList[i].funcs[f];
                    }
                    cout << " }";
                }
            }

            return is_done;
        }


        /*
        void semanticAnalyzer::analyze_variable_decl()
        {
            varSignature _var (root->children[0]->meta, uniqueID_current);


            if (!current_scope->addVar(_var))
            {
                core::string<CHAR> msg;
                if (current_type)
                {
                    msg = *((core::string<CHAR>*)current_type);
                    msg += '.';
                }

                msg += *((core::string<CHAR>*)root->children[0]->meta);

                error_buffer.add(error(root->line,
                                                    root->column,
                                        error::VARIABLE_REDECLARED,
                                                    msg,
                                                    root->file));
            }
            else if (current_type)
                type_var_list.add(uniqueID_current);

            uniqueID_current++;

            exit_node();
        }


        void semanticAnalyzer::analyze_assignexpr()
        {
            if ((root->children[0]->type == phrase::VARIABLE) &&
                (index < 1))
            {
                varSignature _var (root->children[0]->children[0]->meta, uniqueID_current);

                //if the variable we are assigning to has not already been declared,
                //insert a variable declaration beforehand.
                if (current_scope->assignVar(_var))
                {
                    int prev_index;
                    index_stack.pop(prev_index);

                    //create new node (variable decl ID)
                    phrase_t* IDnode = new phrase_t();

                    IDnode->type = ident::IDENTIFIER;
                    IDnode->line = root->line;
                    IDnode->column = root->column;
                    IDnode->file = root->file;
                    IDnode->meta = root->children[0]->children[0]->meta;

                    //create new node (variable declaration)
                    phrase_t* varNode = new phrase_t();

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


        void semanticAnalyzer::analyze_function_decl()
        {
            if (index < (root->children).size())
            {
                if (index == 0)
                    enter_scope();

                enter_node(index);
            }
            else
            {
                typeSignature _type (root->children[0]->meta, NULL);

                if (type_list.find(_type) > -1) //search types
                {
                    error_buffer.add(error(root->line,
                                                        root->column,
                                                        error::FUNCTION_ALREADY_TYPE,
                                        *((core::string<CHAR>*)_type.type),
                                                        root->file));
                }
                else //search functions
                {
                    funcSignature _func (root->children[0]->meta,
                                         root->meta,
                                         uniqueID_current++,
                                         current_type);

                    for (int i=0; i<param_list.size(); i++)
                    {
                        varSignature _var (current_scope->getVariable(param_list[i]));
                        _func.paramTypes.add(_var.type);
                        _func.params.add(_var.uniqueID);
                    }


                    if (function_list.find(_func) <= -1)
                    {
                        function_list.add(_func);

                        if (current_type)
                            type_func_list.add(_func.uniqueID);
                    }
                    else
                    {
                        core::string<CHAR> msg;
                        genFuncSigString(_func, msg);

                        error_buffer.add(error(root->line,
                                                            root->column,
                                                            error::FUNCTION_REDECLARED,
                                                            msg,
                                                            root->file));
                    }
                }


                param_list.clear();

                exit_scope();
                exit_node();
            }
        }


        void semanticAnalyzer::analyze_variable()
        {
            varSignature _var = current_scope->getVariable(
                                    varSignature(root->children[0]->meta) );

            symID varID = _var.uniqueID;

            if (!varID)
                error_buffer.add(error(root->line,
                                                    root->column,
                                                    error::VARIABLE_UNDECLARED,
                                    *((core::string<CHAR>*)root->children[0]->meta),
                                                    root->file));
            else
            {
                root->metaValue = varID;

                if (exprStart)
                    typeStack.push(_var.type);
            }

            exit_node();
        }


        void semanticAnalyzer::analyze_literal()
        {
            if (exprStart)
                typeStack.push(NULL); //literals are always of type "var"

            exit_node();
        }


        void semanticAnalyzer::analyze_expression()
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
                        error_buffer.add(error(root->line,
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
                        if (type1 != type2)
                        {
                            error_buffer.add(error(root->line,
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
                        if (root->type == phrase::DIMENSIONEXPR)
                        {
                            if (root->children[0]->type == ident::IDENTIFIER)
                            {
                                typeStack.push(root->children[0]->meta);
                            }

                            if (type1)
                            {
                                error_buffer.add(error(root->line,
                                                        root->column,
                                                        error::TYPE_DISALLOWED,
                                                        NULL,
                                                        root->file));

                                typeStack.dump();
                            }
                        }
                        else
                            typeStack.push(type1);
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


        void semanticAnalyzer::analyze_typevar_decl()
        {
            //check if the type exists
            typeSignature _type (root->children[0]->meta, NULL);

            if (type_list.find(_type) <= -1)
            {
                error_buffer.add(error(root->line,
                                                    root->column,
                                                    error::TYPE_UNDEFINED,
                                    *((core::string<CHAR>*)_type.type),
                                                    root->file));
            }

            //add typed-var to current scope
            bool good = current_scope->addVar(varSignature(root->children[1]->meta,
                                                   uniqueID_current,
                                                   root->children[0]->meta));

            if (!good)
            {
                error_buffer.add(error(root->line,
                                                    root->column,
                                        error::VARIABLE_REDECLARED,
                                    *((core::string<CHAR>*)root->children[1]->meta),
                                                    root->file));
            }
            else if (current_type)
                type_var_list.add(uniqueID_current);

            uniqueID_current++;

            exit_node();
        }


        void semanticAnalyzer::analyze_for_statement()
        {
            if (index < root->children.size())
            {
                if (index == 0)
                    enter_scope();

                enter_node(index);
            }
            else
            {
                exit_scope();
                exit_node();
            }
        }


        void semanticAnalyzer::analyze_typedecl()
        {
            if (index < root->children.size())
            {
                if (index == 0)
                {
                    //find the correct scope
                    int i = type_list.find(typeSignature(
                                            root->children[0]->meta,
                                            NULL));

                    if (i > -1)
                    {
                        current_scope = type_list[i].scope;
                    }
                    else
                        enter_scope();

                    current_type = root->children[0]->meta;
                }

                enter_node(index);
            }
            else
            {
                //make sure no functions with this name exist
                void* ID = root->children[0]->meta;

                int i = 0;
                while ((i < function_list.size()) &&
                       (function_list[i].ID != ID))
                {
                    i++;
                }

                if (i < function_list.size())
                {
                    error_buffer.add(error(root->line,
                                                        root->column,
                                                error::TYPE_ALREADY_FUNCTION,
                                                *((core::string<CHAR>*)ID),
                                                        root->file));
                }


                //register to the appropriate type
                typeSignature _type (current_type, current_scope);

                i = type_list.find(_type);
                if (i <= -1)
                {
                    i = type_list.add(_type);
                }

                type_list[i].funcs.add(type_func_list);
                type_list[i].vars.add(type_var_list);


                current_type = NULL;
                type_func_list.clear();
                type_var_list.clear();

                exit_scope();
                exit_node();
            }
        }


        void semanticAnalyzer::analyze_foreach_statement()
        {
            if (index < root->children.size())
            {
                if (index == 0)
                    enter_scope();

                enter_node(index);
            }
            else
            {
                exit_scope();
                exit_node();
            }
        }


        void semanticAnalyzer::analyze_formaldecl()
        {
            if (root->type == phrase::FORMALTYPEDECL)
            {
                bool good = current_scope->addVar(varSignature(root->children[1]->meta,
                                                   uniqueID_current++,
                                                   root->children[0]->meta));

                if (!good)
                    error_buffer.add(error(root->line,
                                                        root->column,
                                            error::VARIABLE_REDECLARED,
                                        *((core::string<CHAR>*)root->children[1]->meta),
                                                        root->file));

                param_list.add(root->children[1]->meta);
            }
            else
            {
                bool good = current_scope->addVar(varSignature(root->children[0]->meta,
                                                   uniqueID_current++));

                if (!good)
                    error_buffer.add(error(root->line,
                                                        root->column,
                                            error::VARIABLE_REDECLARED,
                                        *((core::string<CHAR>*)root->children[0]->meta),
                                                        root->file));

                param_list.add(root->children[0]->meta);
            }

            exit_node();
        }
        */

    }
}
}

#endif // SEMANTICANALYZER_H_INCLUDED
