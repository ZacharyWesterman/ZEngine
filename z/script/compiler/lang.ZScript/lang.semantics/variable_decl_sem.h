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
            return false;
        }
    }
}
}

#endif // VARIABLE_DECL_SEM_H_INCLUDED
