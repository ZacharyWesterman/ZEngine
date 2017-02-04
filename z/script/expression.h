#pragma once
#ifndef EXPRESSION_H_INCLUDED
#define EXPRESSION_H_INCLUDED

#include "../core/string.h"
#include "../core/stringUtils/eval_string.h"
#include "../core/stringUtils/char_is_whiteSpace.h"
#include "../core/stringUtils/string_remove_whiteSpace.h"
#include "../core/dynamicStack.h"
#include "../core/array.h"

#include <math.h>
#include "../math/factorial.h"

#include "operators.h"
#include "script_errors.h"
#include "escape_sequences.h"
#include "destringify.h"


namespace script
{
    template <typename CHAR>
    class expression
    {
    private:
        core::string<CHAR> expr_string;
        core::string<CHAR> expr_value;

        error_flag expr_error;

        void evaluate_expression();

        error_flag cumulative_operate(const core::array< core::string<CHAR> >&);

        error_flag single_operate(core::dynamic_stack< core::string<CHAR> >&, t_oper);

        error_flag separate_infix(core::array< core::string<CHAR> >&);

        error_flag to_postfix(const core::array< core::string<CHAR> >&,
                        core::array< core::string<CHAR> >&);

        error_flag correct_negatives(core::array< core::string<CHAR> >&);

        bool operand_is_string(const core::string<CHAR>&);
        core::string<CHAR> destringed_operand(const core::string<CHAR>&);

    public:

        expression() {expr_error = error::NONE;}
        expression(const core::string<CHAR>& expr)
        {
            expr_string = expr;

            evaluate_expression();
        }

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


    ///MEMBER FUNCTIONS///



    template <typename CHAR>
    void expression<CHAR>::evaluate_expression()
    {
        expr_error = error::NONE;


        core::array< core::string<CHAR> > infix;
        core::array< core::string<CHAR> > postfix;

        //separate the expression into an infix array
        expr_error = expr_error | separate_infix(infix);

        //certain '-'signs will negate a number. deal with this.
        expr_error = expr_error | correct_negatives(infix);

        //convert the infix array to a postfix array
        expr_error = expr_error | to_postfix(infix, postfix);
//for(uint i=0; i<postfix.size(); i++)
  //          cout << postfix.at(i).str() << endl;
        //calculate the expression value from the postfix array
        //cout << endl << "::";
        expr_error = expr_error | cumulative_operate(postfix);
        //cout << expr_error;
    }


    template <typename CHAR>
    error_flag expression<CHAR>::correct_negatives(core::array< core::string<CHAR> >& infix)
    {
        error_flag expression_error = error::NONE;

        core::string<CHAR> neg = (CHAR)45;
        core::string<CHAR> pos = (CHAR)43;


        for (int i=0; i<infix.size(); i++)
        {
            //if this is a '-'
            if (infix.at(i) == neg)
            {
                //if there is a '--' replace it with a '+'
                if (infix.is_valid(i+1) &&
                    infix.at(i+1) == neg)
                {
                    infix.remove(i+1);

                    infix.at(i) = pos;
                }

                //if we have a negative number with an operator or nothing else
                //(not a number being subtracted from another number)
                if (infix.is_valid(i+1) &&
                    (infix.at(i+1) == (CHAR)45))
                {
                    t_oper l_oper = OPERATOR::NONE;

                    if (infix.is_valid(i-1))
                        l_oper = what_operator(infix.at(i-1));


                    if (((l_oper != OPERATOR::CLOSE_PARENTH) &&
                         (l_oper != OPERATOR::NONE)) ||
                        !infix.is_valid(i-1))
                    {
                        //if we did not previously remove a '--',
                        //add a minus sign to the operand on the right.
                        if (what_operator(infix.at(i)) != OPERATOR::ADD)
                        {
                            //but don't add a minus sign to zero.
                            if (core::value(infix.at(i+1)) != 0.f)
                                infix.at(i+1) = infix.at(i) + infix.at(i+1);
                        }

                        infix.remove(i);
                    }
                }
            }
        }

        return expression_error;
    }

    template <typename CHAR>
    error_flag expression<CHAR>::cumulative_operate(const core::array< core::string<CHAR> >& postfix)
    {
        //cout << postfix.size() << endl;
        error_flag operation_error = error::NONE;

        core::dynamic_stack< core::string<CHAR> > operands;



        for (int i=0; i<postfix.size(); i++)
        {
            //check if we have an operator
            t_oper oper = what_operator(postfix.at(i));

            //if we do have an operator
            if (oper)
            {
                //perform operation.
                operation_error |= single_operate(operands, oper);
            }
            else // if we have an operand
            {
                //push it to the stack
                if (postfix.at(i).length() > 0)
                    operands.push(postfix.at(i));
            }
        }


        operands.pop(expr_value);

        if (!operands.isEmpty())
            operation_error |= error::MISSING_OPERATOR;

        //cout << operation_error;
        return operation_error;
    }


    template <typename CHAR>
    error_flag expression<CHAR>::single_operate(core::dynamic_stack< core::string<CHAR> >& operands, t_oper oper)
    {
        error_flag operation_error = error::NONE;

        /** Put code here to do a single operation **/

        return operation_error;
    }


    template <typename CHAR>
    error_flag expression<CHAR>::separate_infix(core::array< core::string<CHAR> >& infix)
    {
        error_flag expr_error = error::NONE;

        //define quote character and quote escape sequence
        core::string<CHAR> quote = (CHAR)34;

        core::string<CHAR> quote_esc;
        esc_sequence_name(ESC_SEQUENCE::QUOTE, quote_esc);

        //location of the previous operand
        int num_beg = 0;
        int num_end = -1;

        bool in_string = false;

        //for every character in the expression
        for (int i=0; i<expr_string.length(); i++)
        {
            num_end = i - 1;

            if (expr_string.foundAt(quote, i))
            {
                if (in_string && (num_end >= num_beg))
                {
                    core::string<CHAR> this_value = core::remove_whitespace(
                                                         expr_string.substr(num_beg, num_end+1));

                    if (this_value.length() > 0)
                        infix.add(this_value.str());

                    num_beg = i + 1;
                }

                in_string = !in_string;
            }
            else if (expr_string.foundAt(quote_esc, i))
            {
                i++;
            }

            //if we are at an operator
            t_oper oper = what_operator(expr_string, i);

            if (oper && !in_string)
            {
                //if there is a number before this operator,
                //append it to the output.
                if (num_end >= num_beg)
                {
                    core::string<CHAR> this_value = core::remove_whitespace(
                                                         expr_string.substr(num_beg, num_end));

                    if (this_value.length() > 0)
                        infix.add(this_value.str());
                }


                //append this operator to the output.
                core::string<CHAR> this_oper;
                operator_string(oper, this_oper);

                infix.add(this_oper.str());

                num_beg = i + this_oper.length();
                i = num_beg - 1;
            }
            else if (!in_string) //check for errors
            {
                if (!core::is_numeric(expr_string[i]) &&
                    (expr_string[i] != (CHAR)46) && //if character is not number related,
                    !core::is_white_space(expr_string[i]) && //or white space
                    (expr_string[i] != (CHAR)34)) //or a quote
                {
                    //it is invalid
                    expr_error |= error::INVALID_CHARACTER;
                }
            }

        }

        //if there is a number after the last operator, append it to the output.
        num_end = expr_string.length() - 1;

        if (num_end >= num_beg)
        {
            core::string<CHAR> this_value = core::remove_whitespace(
                                                 expr_string.substr(num_beg, num_end));
            if (this_value.length() > 0)
                infix.add(this_value.str());
        }


        return expr_error;
    }


    template <typename CHAR>
    error_flag expression<CHAR>::to_postfix(const core::array< core::string<CHAR> >& infix,
                                      core::array< core::string<CHAR> >& postfix)
    {
        error_flag expr_error = error::NONE;

        //stack for operators
        core::dynamic_stack< t_oper > operators;


        for (int i=0; i<infix.size(); i++)
        {
            infix.at(i);
            //check if we have an operator
            int oper = what_operator(infix.at(i));

            //if we do have an operator
            if (oper)
            {
                t_oper lastOper = OPERATOR::NONE;

                if (oper == OPERATOR::OPEN_PARENTH)
                {
                    //if the previous item was a number, assume multiplication
                    if (infix.is_valid(i-1))
                        if (!what_operator(infix.at(i-1)))
                            operators.push(OPERATOR::MULTIPLY);

                    operators.push(oper);
                }
                else if (oper == OPERATOR::CLOSE_PARENTH)
                {
                    bool didPop = operators.pop(lastOper);

                    while (didPop && (lastOper != OPERATOR::OPEN_PARENTH))
                    {
                        core::string<CHAR> last_oper;
                        operator_string(lastOper, last_oper);

                        postfix.add(last_oper);

                        lastOper = OPERATOR::NONE;
                        didPop = operators.pop(lastOper);
                    }
                }
                else
                {
                    bool didPop = operators.pop(lastOper);

                    while (didPop && (priority(lastOper) >= priority(oper)))
                    {
                        core::string<CHAR> last_oper;
                        operator_string(lastOper, last_oper);

                        postfix.add(last_oper);

                        lastOper = OPERATOR::NONE;
                        didPop = operators.pop(lastOper);
                    }

                    if (didPop)
                        operators.push(lastOper);

                    operators.push(oper);
                }
            }

            //if we don't have an operator,
            //then we have a value
            else
            {
                //if the previous item was a closed parentheses,
                // assume multiplication
                if (infix.is_valid(i-1))
                    if (what_operator(infix.at(i-1)) == OPERATOR::CLOSE_PARENTH)
                    {
                        core::string<CHAR> mul;
                        operator_string(OPERATOR::MULTIPLY, mul);

                        postfix.add(mul);
                    }

                //append to the output
                if (infix.at(i).length() > 0)
                    postfix.add(infix.at(i));
            }
        }


        //pop any operators still left on the stack
        t_oper oper = OPERATOR::NONE;
        core::string<CHAR> val;

        bool opers_left = operators.pop(oper);

        while (opers_left)
        {
            //then append its respective operator.
            core::string<CHAR> oper_str;
            operator_string(oper, oper_str);

            postfix.add(oper_str);


            opers_left = operators.pop(oper);
        }


        return expr_error;
    }


    template <typename CHAR>
    bool expression<CHAR>::operand_is_string(const core::string<CHAR>& operand)
    {
        core::string<CHAR> quote = (CHAR)34;

        return operand.beginsWith(quote);
    }

    template <typename CHAR>
    core::string<CHAR> expression<CHAR>::destringed_operand(const core::string<CHAR>& operand)
    {
        return destringify(operand);
    }
}

#endif // EXPRESSION_H_INCLUDED
