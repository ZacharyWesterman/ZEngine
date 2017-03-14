#pragma once
#ifndef SCANNER_INCLUDE_TREE_H_INCLUDED
#define SCANNER_INCLUDE_TREE_H_INCLUDED

#include <z/core/array.h>
#include <z/core/string.h>

#include "identity.h"
#include "scanner.h"

namespace z
{
    namespace script
    {
        template <typename CHAR>
        struct include_tree_node
        {
            //data about this node
            core::string<char> dir;
            core::string<char> filename;
            core::array< ident_t<CHAR> > identifiers;

            bool found_error;


            //data about the following consecutive nodes
            //(included files)
            core::array<int> children_loc;
            core::array<include_tree_node*> children;

            //data about the previous node
            include_tree_node* parent;
        };


        ///scanner include tree class
        template <typename CHAR>
        class scanner_include_tree
        {
        private:
            include_tree_node<CHAR>* root;
            include_tree_node<CHAR>* current;

            scanner<CHAR>* scanner_ptr;


        public:
            scanner_include_tree(scanner<CHAR>*,
                                 const core::string<CHAR>&,
                                 const core::string<char>&);
        };


        template <typename CHAR>
        scanner_include_tree::scanner_include_tree(scanner<CHAR>* _scanner,
                                                   const core::string<CHAR>& input,
                                                   const core::string<char>& dir = core::string<char>())
        {
            scanner_ptr = _scanner;

            root = NULL;


            if (scanner_ptr)
            {
                root = new include_tree_node;

                //scan into the node
                root->found_error = scanner_ptr->scan(input);
                root->found_error |= scanner_ptr->clean();

                root->identifiers = scanner_ptr->identifiers;


                root->dir = dir;

                root->parent = NULL;
            }

            current = root;
        }
    }
}

#endif // SCANNER_INCLUDE_TREE_H_INCLUDED
