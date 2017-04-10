/**
 * File:            scanner_include_tree.h
 * Namespace:       z::script
 * Description:     Iterates through the included files and
 *                  adds to the included list if not
 *                  already included.
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   13 Mar. 2017
**/

#pragma once
#ifndef SCANNER_INCLUDE_TREE_H_INCLUDED
#define SCANNER_INCLUDE_TREE_H_INCLUDED

#include <z/core/array.h>
#include <z/core/string.h>

#include <z/file/loadFileToMemory.h>

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

            core::array< core::string<char> > include_list;


            void delete_node(include_tree_node<CHAR>*);


            void get_children_loc(include_tree_node<CHAR>*);

            bool add_include(include_tree_node<CHAR>*, int);

        public:
            scanner_include_tree(scanner<CHAR>*,
                                 const core::string<CHAR>&,
                                 const core::string<char>&);


            ~scanner_include_tree();


            bool traverse();
        };


        ///constructor
        template <typename CHAR>
        scanner_include_tree<CHAR>::scanner_include_tree(scanner<CHAR>* _scanner,
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



        ///destructor
        template <typename CHAR>
        scanner_include_tree<CHAR>::~scanner_include_tree()
        {
            delete_node(root);
        }


        //recursive destructor function
        template <typename CHAR>
        void scanner_include_tree<CHAR>::delete_node(include_tree_node<CHAR>* node)
        {
            if (node)
            {
                for (int i=0; i<(node->children.size()); i++)
                    delete_node(node->children[i]);

                delete node;
            }
        }


        ///traverse the includes iteratively
        ///that is, add one include per call
        //returns false when done, true otherwise
        template <typename CHAR>
        bool scanner_include_tree<CHAR>::traverse()
        {
            if (!current)
                return false;


            //get the list of include indexes
            if (current->children_loc.size() == 0)
                get_children_loc(current);


            if (current->children.size() <
                current->children_loc.size())
            {
                if (!add_include(current, children_loc[children.size()]))
                {
                    current->found_error = true;


                    current->identifiers[children_loc[children.size()]].err = \
                                error::INCLUDE_LOAD_FAILED;

                    return false;
                }
                else
                {
                    current = current->children[children.size() - 1];

                    return true;
                }

            }
            else
            {
                if (current->parent)
                {
                    current = current->parent;

                    return true;
                }
                else
                {
                    return false;
                }
            }

        }


        template <typename CHAR>
        void scanner_include_tree<CHAR>::get_children_loc(include_tree_node<CHAR>* node)
        {
            //look for every include keyword
            for (int i=0; i<(node->identifiers.size()); i++)
            {
                if (node->identifiers[i].type == ident::KEYWORD_INCLUDE)
                {
                    //include directories must exist and be a string literal.
                    if (node->identifiers.is_valid(i+1) &&
                        node->identifiers[i+1].type == ident::STRING_LITERAL)
                    {

                        //add the include string index to the child list.
                        node->children_loc.add(i+1);
                    }
                    else
                    {
                        node->found_error = true;

                        node->identifiers[i].err = error::INVALID_INCLUDE;
                    }

                }
            }
        }


        template <typename CHAR>
        bool scanner_include_tree<CHAR>::add_include(include_tree_node<CHAR>* parent, int index)
        {
            core::string<CHAR> fileData;


            core::string<char> dir = parent->dir;
            core::string<char> filename = parent->identifiers[index].name;


            core::string<char> full_incl_filename = dir + filename;

            if (!file::loadFileToMemory(full_incl_filename, fileData))
            {
                return false;
            }
            else
            {

            }
        }

    }
}

#endif // SCANNER_INCLUDE_TREE_H_INCLUDED
