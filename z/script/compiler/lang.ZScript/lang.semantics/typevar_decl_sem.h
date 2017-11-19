#pragma once
#ifndef TYPEVAR_DECL_SEM_H_INCLUDED
#define TYPEVAR_DECL_SEM_H_INCLUDED

#include <z/script/compiler/semanticRule.h>

namespace z
{
namespace script
{
    namespace compiler
    {
        class typevar_decl_sem : public semanticRule
        {
        public:
            //only activate on TYPEVAR_DECL nodes
            typevar_decl_sem() : semanticRule(TYPEVAR_DECL) {}

            ~typevar_decl_sem() {}

            void apply(const core::array< command* >*, //list of commands
                       const core::array< function* >*, //list of functions
                       semanticScope*,
                       phrase_t*, //current node
                       int&,//next node index to enter
                       core::array<error>*);
        };


        void typevar_decl_sem::apply(const core::array< command* >* commands,
                       const core::array< function* >* functions,
                       semanticScope* semantics,
                       phrase_t* node, //current node
                       int& index,//next node index to enter
                       core::array<error>* error_buffer)
        {
            //add typed variable to scope after checking any initialization expression(s)
            if (index >= node->children.size())
            {
                //check if the type exists
                typeSignature _type (node->children[0]->meta, NULL);

                if (semantics->typeList.find(_type) < 0)
                {
                    core::string<char> msg = "Undefined type '";
                    msg += *((core::string<CPL_CHAR>*)_type.type);
                    msg += "'";

                    error_buffer->add(error(msg,
                                            *(node->file),
                                            node->line,
                                            node->column));
                }

                //add typed-var to current scope
                varSignature type_var (node->children[1]->meta,
                                       semantics->uniqueID_Current,
                                       node->children[0]->meta);

                bool good = semantics->currentScope->addVar(type_var);

                if (!good)
                {
                    core::string<char> msg = "Variable '";
                    if (semantics->currentType)
                    {
                        msg = *((core::string<CPL_CHAR>*)semantics->currentType);
                        msg += '.';
                    }

                    msg += *((core::string<CPL_CHAR>*)node->children[1]->meta);
                    msg += "' is already defined";

                    error_buffer->add(error(msg,
                                        *(node->file),
                                        node->line,
                                        node->column));
                }
                else if (semantics->currentType)
                    semantics->typeVarList.add(semantics->uniqueID_Current);

                semantics->uniqueID_Current++;
            }
        }
    }
}
}

#endif // TYPEVAR_DECL_SEM_H_INCLUDED
