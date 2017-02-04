#ifndef BASIC_EXPRESSION_H_INCLUDED
#define BASIC_EXPRESSION_H_INCLUDED

#include "../core/array.h"
#include "../core/dynamicStack.h"

#include "operators/oper_t.h"

#include <iostream>
using namespace std;

namespace script
{
    template <typename CHAR>
    class basic_expression
    {
    private:
        enum object_type
        {
            NONE = 0,
            IDENTIFIER,
            OPERATOR,
            UNKNOWN
        };

        ///internal object class to help with operations
        class object
        {
        public:
            core::string<CHAR> str;
            object_type type;

            bool operator==(const object& other) const
            {
                return ((type == other.type) &&
                        (str == other.str));
            }
        }; ///end of object class


        core::array< oper::oper_t<CHAR>* >* opers;

        core::string<CHAR> expr_string;
        core::string<CHAR> expr_value;

        error_flag expr_error;

        void evaluate_expression();

    public:
        //constructor REQUIRES all operators be set
        basic_expression(core::array< oper::oper_t<CHAR>* >& operators)
        {
            expr_error = error::NONE;

            opers = &operators;
        }

        //evaluate a basic expression (no parentheses)
        const core::string<CHAR>& operator=(const core::string<CHAR>& expr)
        {
            expr_string = expr;

            evaluate_expression();

            return expr_value;
        }


        void split_expr_into_objects(core::array<object>&);



        const core::string<CHAR>& string() const {return expr_string;}
        const core::string<CHAR>& value() const {return expr_value;}
        error_flag error() const {return expr_error;}
    };


    ///Convert the expression into a value
    template <typename CHAR>
    void basic_expression<CHAR>::evaluate_expression()
    {
        expr_error = error::NONE;


        core::array<object> object_list;

        split_expr_into_objects(object_list);


        for (int i=0; i<object_list.size(); i++)
        {
            cout << object_list[i].str.str() << " [";
            cout << (int)object_list[i].type << "]\n";
        }
    }


    ///split the given expression into its constituent parts
    template <typename CHAR>
    void basic_expression<CHAR>::split_expr_into_objects(core::array<object>& output)
    {
        object this_object;

        this_object.type = NONE;


        int beg = 0;
        int end = -1;

        bool in_string = false;


        object_type new_type = this_object.type;

        for (int i=0; i<expr_string.length(); i++)
        {
            if (core::is_alpha())



            //split if we encounter a different type of object
            if (core::is_white_space(expr_string[i]) ||
                (this_object.type != new_type))
            {
                //end of the string is before we encounter whitespace
                end = i - 1;

                if (end >= beg)
                {
                    this_object.str = expr_string.substr(beg, end);
                    this_object.type = new_type;

                    output.add(this_object);
                }

                //beginning of next string is after this whitespace
                beg = i + 1;
            }
        }

        //append any leftover characters in the input to the output
        if (expr_string.length()-1 >= beg)
        {
            this_object.str = expr_string.substr(beg, expr_string.length()-1);
            this_object.type = new_type;

            output.add(this_object);
        }
    }
}

#endif // BASIC_EXPRESSION_H_INCLUDED
