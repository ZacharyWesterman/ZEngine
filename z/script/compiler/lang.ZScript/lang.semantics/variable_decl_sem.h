#ifndef VARIABLE_DECL_SEM_H_INCLUDED
#define VARIABLE_DECL_SEM_H_INCLUDED

#include <z/script/compiler/semanticRule.h>

namespace z
{
namespace script
{
    namespace compiler
    {
        class variable_decl_sem : public semanticRule
        {
            ~variable_decl_sem() {}

            //return TRUE if node is of correct type
            //(don't check every rule if we found the right one)
            bool check(const core::array< command* >*, //list of commands
                       const core::array< function* >*, //list of functions
                       semanticScope*,
                       phrase_t*, //current node
                       int&,//next node index to enter
                       core::array<error>*);
        };


        bool variable_decl_sem::check(const core::array< command* >* commands,
                       const core::array< function* >* functions,
                       semanticScope* semantics,
                       phrase_t* node, //current node
                       int& index,//next node index to enter
                       core::array<error>* error_buffer)
        {
            if (node->type == VARIABLE_DECL)
            {
                //add variable to scope after checking any initialization expression
                if (index >= node->children.size())
                {
                    //std::cout << "vardecl exit!!\n";

                    varSignature _var (node->children[0]->meta,
                                    semantics->uniqueID_Current);


                    if (!((semantics->currentScope)->addVar(_var)) )
                    { //error: variable already declared in scope
                        core::string<char> msg = "Variable '";
                        if (semantics->currentType)
                        {
                            msg = *((core::string<CPL_CHAR>*)semantics->currentType);
                            msg += '.';
                        }

                        msg += *((core::string<CPL_CHAR>*)node->children[0]->meta);

                        msg += "' is already defined";

                        error_buffer->add(error(msg,
                                                *(node->file),
                                                node->line,
                                                node->column));
                    }
                    else if (semantics->currentType)
                    {
                        semantics->typeVarList.add(semantics->uniqueID_Current);
                    }

                    semantics->uniqueID_Current++;
                }
                else
                {
                    //index++;//traverse possible initialization expression first
                    //std::cout << "vardecl!!\n";
                }

                return true;
            }
            else
                return false;
        }
    }
}
}

#endif // VARIABLE_DECL_SEM_H_INCLUDED
