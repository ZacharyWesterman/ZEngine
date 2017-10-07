#pragma once
#ifndef SYNTAXRULE_H_INCLUDED
#define SYNTAXRULE_H_INCLUDED

#include "error.h"
#include "phrase.h"

#include <z/core/array.h>


namespace z
{
namespace script
{
    namespace compiler
    {
        template <typename CHAR>
        void deleteNode(phrase_t<CHAR>* root)
        {
            for (int i=0; i<(root->children.size()); i++)
            {
                deleteNode(root->children[i]);
            }

            delete root;
        }

        template <typename CHAR>
        class syntaxRule
        {
        public:
            virtual ~syntaxRule() {}

            virtual bool apply(core::array< phrase_t<CHAR>* >* phrase_nodes,
                               int index) {return false;}
        };
    }
}
}

#endif // SYNTAXRULE_H_INCLUDED
