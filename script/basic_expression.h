#ifndef BASIC_EXPRESSION_H_INCLUDED
#define BASIC_EXPRESSION_H_INCLUDED

#include "../core/array.h"
#include "../core/dynamicStack.h"

#include "operators/oper_t.h"

namespace script
{
    template <typename CHAR>
    class basic_expression
    {
    private:
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






        const core::string<CHAR>& string() const {return expr_string;}
        const core::string<CHAR>& value() const {return expr_value;}
        error_flag error() const {return expr_error;}
    };


    template <typename CHAR>
    void basic_expression<CHAR>::evaluate_expression()
    {
        expr_error = error::NONE;

        core::dynamic_stack< core::string<CHAR> > operands;
    }
}

#endif // BASIC_EXPRESSION_H_INCLUDED
