#pragma once
#ifndef TYPEDECL_SEM_H_INCLUDED
#define TYPEDECL_SEM_H_INCLUDED

#include <z/script/compiler/semanticRule.h>

namespace z
{
namespace script
{
    namespace compiler
    {
        class typedecl_sem : public semanticRule
        {
        public:
            //only activate on TYPEDECL nodes
            typedecl_sem() : semanticRule(TYPEDECL) {}

            ~typedecl_sem() {}

            void apply(const core::array< command* >*, //list of commands
                       const core::array< function* >*, //list of functions
                       semanticScope*,
                       phrase_t*, //current node
                       int&,//next node index to enter
                       core::array<error>*);
        };


        void typedecl_sem::apply(const core::array< command* >* commands,
                       const core::array< function* >* functions,
                       semanticScope* semantics,
                       phrase_t* node, //current node
                       int& index,//next node index to enter
                       core::array<error>* error_buffer)
        {
            if (index < node->children.size())
            {
                if (index == 0)
                {
                    //find the correct scope
                    int i = semantics->typeList.find(typeSignature(
                                            node->children[0]->meta,
                                            NULL));

                    if (i > -1)
                    {
                        semantics->currentScope = semantics->typeList[i].scope;
                    }
                    else
                        enter_scope(semantics->currentScope);

                    semantics->currentType = node->children[0]->meta;
                }
            }
            else
            {
                //make sure no functions with this name exist
                void* ID = node->children[0]->meta;

                int i = 0;
                while ((i < semantics->functionList.size()) &&
                       (semantics->functionList[i].ID != ID))
                {
                    i++;
                }

                if (i < semantics->functionList.size())
                {
                    core::string<char> msg = "Type '";
                    msg += *((core::string<CPL_CHAR>*)ID);
                    msg += "' shadows function declaration";

                    error_buffer->add(error(msg,
                                           *(node->file),
                                           node->line,
                                           node->column));
                }


                //register to the appropriate type
                typeSignature _type (semantics->currentType, semantics->currentScope);

                i = semantics->typeList.find(_type);
                if (i <= -1)
                {
                    i = semantics->typeList.add(_type);
                }

                semantics->typeList[i].funcs.add(semantics->typeFuncList);
                semantics->typeList[i].vars.add(semantics->typeVarList);


                semantics->currentType = NULL;
                semantics->typeFuncList.clear();
                semantics->typeVarList.clear();

                exit_scope(semantics->currentScope);
            }
        }
    }
}
}

#endif // TYPEDECL_SEM_H_INCLUDED
