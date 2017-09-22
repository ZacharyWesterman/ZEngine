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

        template<typename CHAR>
        void genFuncSigString(const funcSignature& _func, core::string<CHAR>& msg)
        {
            if (_func.returnType)
                msg = *((core::string<CHAR>*)_func.returnType);
            else
                msg = "var";

            msg += ' ';
            if (_func.inType)
            {
                msg += *((core::string<CHAR>*)_func.inType);
                msg += '.';
            }
            msg += *((core::string<CHAR>*)_func.ID);


            msg += '(';


            for(int i=0; i<_func.params.size(); i++)
            {
                if (i)
                    msg += ',';

                if (_func.paramTypes[i])
                    msg += *((core::string<CHAR>*)_func.paramTypes[i]);
                else
                    msg += "var";


            }

            msg += ')';
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

            core::array<funcSignature> function_list;
            core::array< void* > param_list;

            core::array<typeSignature> type_list;

            core::dynamicStack<void*> typeStack;
            phrase_t<CHAR>* exprStart;

            void* current_type;
            core::array<symID> type_func_list;
            core::array<symID> type_var_list;


            void enter_node(int);
            void exit_node();


            void appendType();

            void enter_scope();
            void exit_scope();


            void analyze_variable_decl();
            void analyze_function_decl();
            void analyze_assignexpr();
            void analyze_variable();
            void analyze_literal();
            void analyze_expression();
            void analyze_list();

            void analyze_typevar_decl();

            void analyze_for_statement();
            void analyze_foreach_statement();
            void analyze_if_statement();

            void analyze_typedecl();

            void analyze_formaldecl();
            void analyze_funccall();

        public:
            core::array< parserError<CHAR> > error_buffer;

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

                uniqueID_current = 1;

                exprStart = NULL;

                current_type = NULL;
            };

            ~semanticAnalyzer(){};


            void setInput(phrase_t<CHAR>* new_root)
            {
                index_stack.dump();

                index = 0;
                root = new_root;

                is_done = (root == NULL);

                global_scope.vars.clear();
                global_scope.children.clear();
                current_scope = &global_scope;

                uniqueID_current = 1; //reserve 0 for NULL

                function_list.clear();
                type_list.clear();

                typeStack.dump();
                exprStart = NULL;

                current_type = NULL;
                type_func_list.clear();
                type_var_list.clear();
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
            if (!index_stack.pop(index) ||
                !(root->parent))
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
                else if (root->type == ident::LITERAL)
                {
                    analyze_literal();
                }
                else if (root->type == phrase::TYPEVAR_DECL)
                {
                    analyze_typevar_decl();
                }
                else if (root->type == phrase::TYPEDECL)
                {
                    analyze_typedecl();
                }
                else if (root->type == phrase::FOR_STATEMENT)
                {
                    analyze_for_statement();
                }
                else if (root->type == phrase::FOREACH_STATEMENT)
                {
                    analyze_foreach_statement();
                }
                else if ((root->type == phrase::FORMALVARDECL) ||
                         (root->type == phrase::FORMALTYPEDECL))
                {
                    analyze_formaldecl();
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
                cout << endl;
                //print functions
                for (int i=0; i<function_list.size(); i++)
                {
                    core::string<CHAR> msg;
                    genFuncSigString(function_list[i], msg);
                    cout << msg.str() << ", \tID="
                         << function_list[i].uniqueID << endl;
                }
                cout << endl;
                //print types
                for (int i=0; i<type_list.size(); i++)
                {
                    cout << "type "
                         << ((core::string<CHAR>*)type_list[i].type)->str()
                         << "{ vars=";
                    for (int v=0; v<type_list[i].vars.size(); v++)
                    {
                        if (v)
                            cout << ',';
                        cout << type_list[i].vars[v];
                    }
                    cout << "  funcs=";
                    for (int f=0; f<type_list[i].funcs.size(); f++)
                    {
                        if (f)
                            cout << ',';
                        cout << type_list[i].funcs[f];
                    }
                    cout << " }";
                }
                cout << endl;
            }

            return is_done;
        }


        template <typename CHAR>
        void semanticAnalyzer<CHAR>::analyze_variable_decl()
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

                error_buffer.add(parserError<CHAR>(root->line,
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

        template <typename CHAR>
        void semanticAnalyzer<CHAR>::analyze_assignexpr()
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
                    error_buffer.add(parserError<CHAR>(root->line,
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

                        error_buffer.add(parserError<CHAR>(root->line,
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

        template <typename CHAR>
        void semanticAnalyzer<CHAR>::analyze_variable()
        {
            varSignature _var = current_scope->getVariable(
                                    varSignature(root->children[0]->meta) );

            symID varID = _var.uniqueID;

            if (!varID)
                error_buffer.add(parserError<CHAR>(root->line,
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

        template <typename CHAR>
        void semanticAnalyzer<CHAR>::analyze_literal()
        {
            if (exprStart)
                typeStack.push(NULL); //literals are always of type "var"

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
                        error_buffer.add(parserError<CHAR>(root->line,
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
                            error_buffer.add(parserError<CHAR>(root->line,
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
                                error_buffer.add(parserError<CHAR>(root->line,
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

        template <typename CHAR>
        void semanticAnalyzer<CHAR>::analyze_typevar_decl()
        {
            //check if the type exists
            typeSignature _type (root->children[0]->meta, NULL);

            if (type_list.find(_type) <= -1)
            {
                error_buffer.add(parserError<CHAR>(root->line,
                                                    root->column,
                                                    error::TYPE_UNDEFINED,
                                    *((core::string<CHAR>*)_type.type),
                                                    root->file));
            }

            //add typed-var to current scope
            errorFlag err =
                current_scope->addVar(varSignature(root->children[1]->meta,
                                                   uniqueID_current,
                                                   root->children[0]->meta));

            if (err)
            {
                error_buffer.add(parserError<CHAR>(root->line,
                                                    root->column,
                                                    err,
                                    *((core::string<CHAR>*)root->children[1]->meta),
                                                    root->file));
            }
            else if (current_type)
                type_var_list.add(uniqueID_current);

            uniqueID_current++;

            exit_node();
        }

        template <typename CHAR>
        void semanticAnalyzer<CHAR>::analyze_for_statement()
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

        template <typename CHAR>
        void semanticAnalyzer<CHAR>::analyze_typedecl()
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
                    error_buffer.add(parserError<CHAR>(root->line,
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

        template <typename CHAR>
        void semanticAnalyzer<CHAR>::analyze_foreach_statement()
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

        template <typename CHAR>
        void semanticAnalyzer<CHAR>::analyze_formaldecl()
        {
            if (root->type == phrase::FORMALTYPEDECL)
            {
                errorFlag err = current_scope->addVar(varSignature(root->children[1]->meta,
                                                   uniqueID_current++,
                                                   root->children[0]->meta));

                if (err)
                    error_buffer.add(parserError<CHAR>(root->line,
                                                        root->column,
                                                        err,
                                        *((core::string<CHAR>*)root->children[1]->meta),
                                                        root->file));

                param_list.add(root->children[1]->meta);
            }
            else
            {
                errorFlag err = current_scope->addVar(varSignature(root->children[0]->meta,
                                                   uniqueID_current++));

                if (err)
                    error_buffer.add(parserError<CHAR>(root->line,
                                                        root->column,
                                                        err,
                                        *((core::string<CHAR>*)root->children[0]->meta),
                                                        root->file));

                param_list.add(root->children[0]->meta);
            }

            exit_node();
        }

    }
}
}

#endif // SEMANTICANALYZER_H_INCLUDED
