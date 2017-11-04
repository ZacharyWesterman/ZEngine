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
        void deleteNode(phrase_t* root)
        {
            for (int i=0; i<(root->children.size()); i++)
            {
                deleteNode(root->children[i]);
            }

            delete root;
        }

        void setSuperType(phrase_t* node, int newType)
        {
            if (node->orig_type == ident::NONE)
                node->orig_type = node->type;

            node->type = newType;
        }

        class syntaxRule
        {
        public:
            virtual ~syntaxRule() {}

            virtual bool apply(core::array< phrase_t* >* phrase_nodes,
                               int index) {return false;}
        };
    }
}
}

#endif // SYNTAXRULE_H_INCLUDED
