#pragma once
#ifndef DYNAMICSTACK_H_INCLUDED
#define DYNAMICSTACK_H_INCLUDED


namespace core
{
    //template for dynamic stack
    template <typename T>
    class dynamic_stack
    {
    private:
        struct ll_node
        {
            T data;
            ll_node* next;
        };

        ll_node* head;

    public:
        //constructor
        dynamic_stack() {head = NULL;}

        //destructor
        ~dynamic_stack()
        {
            while (head != NULL)
            {
                ll_node* nodePtr = head->next;
                delete head;
                head = nodePtr;
            }
        }

        //push an object to the top of the stack.
        void push(T data)
        {
            ll_node* newNode = new ll_node;

            newNode->data = data;
            newNode->next = head;

            head = newNode;
        }

        //pop an object from the top of the stack.
        //returns whether pop was successful.
        bool pop(T& data)
        {
            if (head == NULL)
            {
                return false;
            }
            else
            {
                data = head->data;

                ll_node* nodePtr = head->next;
                delete head;
                head = nodePtr;

                return true;
            }
        }

        T peek()
        {
            return head->data;
        }

        //return whether the stack is empty.
        bool isEmpty() {return (head == NULL);}


        void dump()
        {
            while (head != NULL)
            {
                ll_node* nodePtr = head->next;
                delete head;
                head = nodePtr;
            }
        }
    };
}


#endif // DYNAMICSTACK_H_INCLUDED
