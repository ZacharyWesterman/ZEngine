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

        script::error newError(phrase_t* node, const core::string<char>& msg)
        {
            if (node->file)
                return script::error(msg, *(node->file),
                                     node->line, node->column);
            else
                return script::error(msg,
                                     node->line, node->column);
        }

        class syntaxRule
        {
        public:
            virtual ~syntaxRule() {}

            virtual bool apply(core::array< phrase_t* >*,
                               int,
                               core::array<error>*) = 0;
        };
    }
}
}

#endif // SYNTAXRULE_H_INCLUDED
