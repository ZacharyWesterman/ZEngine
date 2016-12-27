#pragma once
#ifndef VARIABLETREE_H_INCLUDED
#define VARIABLETREE_H_INCLUDED

#include "variable.h"

#ifndef uint
    #define uint uint32_t
#endif // uint

#include <iostream>
using namespace std;

namespace script
{
    template <typename CHAR>
    class variableTree
    {
    private:
        variableTree<CHAR>* parent;

        struct node
        {
            variable<CHAR> data;
            node* left;
            node* right;
        };

        node* root;


        void delete_sub_nodes(node* rt_node)
        {
            if (rt_node)
            {
                if (rt_node->left)
                    delete rt_node->left;

                if (rt_node->right)
                    delete rt_node->right;

                delete rt_node;
            }
        }

        node* get_variable(node*, const core::string<CHAR>&);
        node* add_variable(node*&, const core::string<CHAR>&);

        node* find_variable(node*, const core::string<CHAR>&, const int);

    public:
        variableTree(variableTree* Parent = NULL) {root = NULL; parent = Parent;}
        ~variableTree() {delete_sub_nodes(root);}


        uint set_to_array(const core::string<CHAR>& name,
                          const core::array< core::string<CHAR> >& values);

        uint set_array_value(const core::string<CHAR>& name, const int index,
                             const core::string<CHAR>& value);

        uint set_to_array_of_size(const core::string<CHAR>& name, const int size);

        uint set_to_single_value(const core::string<CHAR>& name,
                                 const core::string<CHAR>& value);


        uint get_array_value(const core::string<CHAR>& name, const int index,
                             core::string<CHAR>& value);

        uint get_single_value(const core::string<CHAR>& name,
                              core::string<CHAR>& value);

        bool found_variable(const core::string<CHAR>& input, int position,
                            core::string<CHAR>& name);

        void clear() {delete_sub_nodes(root); root = NULL;}

        const variableTree<CHAR>* get_parent() const {return parent;}
    };



    template <typename CHAR>
    typename variableTree<CHAR>::node* variableTree<CHAR>::get_variable(node* root_node,
                                                                        const core::string<CHAR>& name)
    {
        if (root_node)
        {
            if (root_node->data.get_name() == name)
            {
                return root_node;
            }
            else if (root_node->data.get_name() > name)
            {
                return get_variable(root_node->left, name);
            }
            else
            {
                return get_variable(root_node->right, name);
            }
        }
        else
        {
            if (parent)
                return parent->get_variable(parent->root, name);
            else
                return NULL;
        }
    }


    template <typename CHAR>
    typename variableTree<CHAR>::node* variableTree<CHAR>::add_variable(node*& root_node,
                                                                        const core::string<CHAR>& name)
    {
        if (root_node)
        {
            if (root_node->data.get_name() == name)
            {
                return root_node;
            }
            else if (root_node->data.get_name() > name)
            {
                return add_variable(root_node->left, name);
            }
            else
            {
                return add_variable(root_node->right, name);
            }
        }
        else
        {
            node* t_node = NULL;

            if (parent)
                t_node = parent->get_variable(parent->root, name);

            if (t_node)
            {
                return t_node;
            }
            else
            {
                root_node = new node;

                root_node->data.rename(name);
                root_node->left = NULL;
                root_node->right = NULL;

                if (!root_node->data.is_valid())
                {
                    delete root_node;
                    root_node = NULL;
                }

                return root_node;
            }
        }
    }


    template <typename CHAR>
    typename variableTree<CHAR>::node* variableTree<CHAR>::find_variable(node* root_node,
                                                                         const core::string<CHAR>& input,
                                                                         const int index)
    {
        if (root_node)
        {
            if (input.foundAt(root_node->data.get_name(), index))
            {
                //cout << "&&" << root_node->data.get_name().str() << endl;

                CHAR left_char = input.at(index - 1);
                CHAR right_char = input.at(index + root_node->data.get_name().length());

                if (core::is_alphanumeric(left_char) || (left_char == (CHAR)95) ||
                    core::is_alphanumeric(right_char) || (right_char == (CHAR)95))
                {
                    if (parent)
                        return parent->find_variable(parent->root, input, index);
                    else
                        return NULL;
                }
                else
                {
                    return root_node;
                }
            }
            else
            {
                if (root_node->data.get_name() >
                    input.substr(index, index + root_node->data.get_name().length() - 1))
                {
                    return find_variable(root_node->left, input, index);
                }
                else
                {
                    return find_variable(root_node->right, input, index);
                }
            }
        }
        else
        {
            if (parent)
                return parent->find_variable(parent->root, input, index);
            else
                return NULL;
        }
    }





    template <typename CHAR>
    uint variableTree<CHAR>::set_array_value(const core::string<CHAR>& name, const int index,
                                             const core::string<CHAR>& value)
    {
        node* at_var = get_variable(root, name);


        if (at_var)
            return at_var->data.set_array_value(index, value);
        else
            return ERROR::UNKNOWN_VARIABLE;
    }


    template <typename CHAR>
    uint variableTree<CHAR>::set_to_array_of_size(const core::string<CHAR>& name,
                                                  const int size)
    {
        node* at_var = add_variable(root, name);

        if (at_var)
            return at_var->data.set_array_size(size);
        else
            return ERROR::UNKNOWN_VARIABLE;
    }


    template <typename CHAR>
    uint variableTree<CHAR>::set_to_single_value(const core::string<CHAR>& name,
                                                 const core::string<CHAR>& value)
    {
        node* at_var = add_variable(root, name);

        if (at_var)
            return at_var->data.set_to_single_value(value);
        else
            return ERROR::UNKNOWN_VARIABLE;
    }


    template <typename CHAR>
    uint variableTree<CHAR>::set_to_array(const core::string<CHAR>& name,
                                          const core::array< core::string<CHAR> >& values)
    {
        node* at_var = add_variable(root, name);

        if (at_var)
            return at_var->data.set_array_values(values);
        else
            return ERROR::UNKNOWN_VARIABLE;
    }


    template <typename CHAR>
    uint variableTree<CHAR>::get_single_value(const core::string<CHAR>& name,
                                              core::string<CHAR>& value)
    {
        node* at_var = get_variable(root, name);

        if (at_var)
            return at_var->data.get_single_value(value);
        else
            return ERROR::UNKNOWN_VARIABLE;
    }


    template <typename CHAR>
    uint variableTree<CHAR>::get_array_value(const core::string<CHAR>& name, const int index,
                                             core::string<CHAR>& value)
    {
        node* at_var = get_variable(root, name);

        if (at_var)
            return at_var->data.get_array_value(index, value);
        else
            return ERROR::UNKNOWN_VARIABLE;
    }




    template <typename CHAR>
    bool variableTree<CHAR>::found_variable(const core::string<CHAR>& input, int index,
                                            core::string<CHAR>& name)
    {
        node* at_var = find_variable(root, input, index);

        if (at_var)
        {
            name = at_var->data.get_name();
            return true;
        }
        else
        {
            return false;
        }
    }
}

#endif // VARIABLETREE_H_INCLUDED
