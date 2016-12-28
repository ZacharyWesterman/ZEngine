#pragma once
#ifndef FUNCTIONALEXPRESSION_H_INCLUDED
#define FUNCTIONALEXPRESSION_H_INCLUDED

#include "../core/string.h"
#include "../core/array.h"

#include "functions.h"
#include "expression.h"
#include "split_parameters.h"
#include "script_errors.h"


//script functions
#include "script_functions/ceiling.h"
#include "script_functions/floor.h"
#include "script_functions/round.h"
#include "script_functions/rand_f.h"
#include "script_functions/rand_i.h"
#include "script_functions/log.h"
#include "script_functions/square_root.h"
#include "script_functions/sine.h"
#include "script_functions/cosine.h"
#include "script_functions/tangent.h"
#include "script_functions/degrees_to_radians.h"
#include "script_functions/radians_to_degrees.h"
#include "script_functions/arc_sine.h"
#include "script_functions/arc_cosine.h"
#include "script_functions/arc_tangent.h"
#include "script_functions/absolute_val.h"
#include "script_functions/frames_per_second.h"
#include "script_functions/rgb.h"
#include "script_functions/rgba.h"
#include "script_functions/red_rgba.h"
#include "script_functions/green_rgba.h"
#include "script_functions/blue_rgba.h"
#include "script_functions/alpha_rgba.h"


namespace script
{
    ///FUNCTIONAL EXPRESSION HANDLER///
    template <typename CHAR>
    class functionalExpression
    {
    private:
        FUNCTION::function<CHAR> funcs[FUNCTION::FUNCTION_COUNT];

        error_flag get_func_value(const core::string<CHAR>&, int, int, core::string<CHAR>&);
        int get_func_range(const core::string<CHAR>&, int);

    public:
        functionalExpression();

        error_flag process(const core::string<CHAR>&, core::array< core::string<CHAR> >&);
    };


    ///functionalExpression constructor
    template <typename CHAR>
    functionalExpression<CHAR>::functionalExpression()
    {
        //assign the function data
        core::string<CHAR> names[FUNCTION::FUNCTION_COUNT];
        for (int i=0; i<FUNCTION::FUNCTION_COUNT; i++)
            FUNCTION::function_name(FUNCTION::FUNC_ID(i), names[i]);



        ///TO DO: replace this mess with something prettier.
        funcs[0] = FUNCTION::function<CHAR>(names[0], FUNCTION::MIN_PARAM[0], FUNCTION::MAX_PARAM[0], FUNCTION::ceiling);
        funcs[1] = FUNCTION::function<CHAR>(names[1], FUNCTION::MIN_PARAM[1], FUNCTION::MAX_PARAM[1], FUNCTION::floor);
        funcs[2] = FUNCTION::function<CHAR>(names[2], FUNCTION::MIN_PARAM[2], FUNCTION::MAX_PARAM[2], FUNCTION::round);

        funcs[3] = FUNCTION::function<CHAR>(names[3], FUNCTION::MIN_PARAM[3], FUNCTION::MAX_PARAM[3], FUNCTION::rand_f);
        funcs[4] = FUNCTION::function<CHAR>(names[4], FUNCTION::MIN_PARAM[4], FUNCTION::MAX_PARAM[4], FUNCTION::rand_i);

        funcs[5] = FUNCTION::function<CHAR>(names[5], FUNCTION::MIN_PARAM[5], FUNCTION::MAX_PARAM[5], FUNCTION::log);
        funcs[6] = FUNCTION::function<CHAR>(names[6], FUNCTION::MIN_PARAM[6], FUNCTION::MAX_PARAM[6], FUNCTION::square_root);

        funcs[7] = FUNCTION::function<CHAR>(names[7], FUNCTION::MIN_PARAM[7], FUNCTION::MAX_PARAM[7], FUNCTION::sine);
        funcs[8] = FUNCTION::function<CHAR>(names[8], FUNCTION::MIN_PARAM[8], FUNCTION::MAX_PARAM[8], FUNCTION::cosine);
        funcs[9] = FUNCTION::function<CHAR>(names[9], FUNCTION::MIN_PARAM[9], FUNCTION::MAX_PARAM[9], FUNCTION::tangent);

        funcs[10]= FUNCTION::function<CHAR>(names[10],FUNCTION::MIN_PARAM[10],FUNCTION::MAX_PARAM[10],FUNCTION::deg_to_rad);
        funcs[11]= FUNCTION::function<CHAR>(names[11],FUNCTION::MIN_PARAM[11],FUNCTION::MAX_PARAM[11],FUNCTION::rad_to_deg);

        funcs[12]= FUNCTION::function<CHAR>(names[12],FUNCTION::MIN_PARAM[12],FUNCTION::MAX_PARAM[12],FUNCTION::arc_sine);
        funcs[13]= FUNCTION::function<CHAR>(names[13],FUNCTION::MIN_PARAM[13],FUNCTION::MAX_PARAM[13],FUNCTION::arc_cosine);
        funcs[14]= FUNCTION::function<CHAR>(names[14],FUNCTION::MIN_PARAM[14],FUNCTION::MAX_PARAM[14],FUNCTION::arc_tangent);

        funcs[15]= FUNCTION::function<CHAR>(names[15],FUNCTION::MIN_PARAM[15],FUNCTION::MAX_PARAM[15],FUNCTION::absolute_val);
        funcs[16]= FUNCTION::function<CHAR>(names[16],FUNCTION::MIN_PARAM[16],FUNCTION::MAX_PARAM[16],FUNCTION::frames_per_second);

        funcs[17]= FUNCTION::function<CHAR>(names[17],FUNCTION::MIN_PARAM[17],FUNCTION::MAX_PARAM[17],FUNCTION::rgba);
        funcs[18]= FUNCTION::function<CHAR>(names[18],FUNCTION::MIN_PARAM[18],FUNCTION::MAX_PARAM[18],FUNCTION::rgb);
        funcs[19]= FUNCTION::function<CHAR>(names[19],FUNCTION::MIN_PARAM[19],FUNCTION::MAX_PARAM[19],FUNCTION::red_rgba);
        funcs[20]= FUNCTION::function<CHAR>(names[20],FUNCTION::MIN_PARAM[20],FUNCTION::MAX_PARAM[20],FUNCTION::green_rgba);
        funcs[21]= FUNCTION::function<CHAR>(names[21],FUNCTION::MIN_PARAM[21],FUNCTION::MAX_PARAM[21],FUNCTION::blue_rgba);
        funcs[22]= FUNCTION::function<CHAR>(names[22],FUNCTION::MIN_PARAM[22],FUNCTION::MAX_PARAM[22],FUNCTION::alpha_rgba);
    }


    template <typename CHAR>
    error_flag functionalExpression<CHAR>::process(const core::string<CHAR>& input_line,
                                             core::array< core::string<CHAR> >& solved_params)
    {
        error_flag expr_error = ERROR::NONE;

        //split all parameters and evaluate separately
        core::array< core::string<CHAR> > params;
        split_parameters(input_line, params);


        //define quote character and quote escape sequence
        core::string<CHAR> quote = (CHAR)34;

        core::string<CHAR> quote_esc;
        esc_sequence_name(ESC_SEQUENCE::QUOTE, quote_esc);

        //evaluate each parameter separately
        for (int p=0; p<params.size(); p++)
        {
            bool in_quote = false;
            //for every character in this parameter
            for (int i=0; i<params[p].length(); i++)
            {
                if (params[p].foundAt(quote, i))
                {
                    in_quote = !in_quote;
                }
                //if we hit the quote escape sequence inside of quotes, ignore it
                else if (in_quote && params[p].foundAt(quote_esc, i))
                {
                    i++;
                }

                //as long as we're not in quotes,
                if (!in_quote)
                {
                    //consecutively check for each function
                    for (int f=0; f<FUNCTION::FUNCTION_COUNT; f++)
                    {
                        //if we found a function at this index,
                        if (!in_quote && params[p].foundAt(funcs[f].name, i))
                        {
                            //get the length the function encompasses
                            int func_len = get_func_range(params[p], i);

                            //get the function's value
                            core::string<CHAR> func_value;
                            expr_error |= get_func_value(params[p],
                                                         f,
                                                         i + funcs[f].name.length(),
                                                         func_value);

                            if (!expr_error)
                                params[p].replace(i, i + func_len-1, func_value);
                            //else
                              // return expr_error;
                        }
                    }
                }

            }//end for every character in this parameter


            //once the functions have been replaced,
            //evaluate their contents
            if (!expr_error)
            {
                expression<CHAR> expr = params[p];

                expr_error |= expr.error();

                //and append each parameter to the output.
                solved_params.add(expr.value());
            }
        }


        return expr_error;
    }



    template <typename CHAR>
    error_flag functionalExpression<CHAR>::get_func_value(const core::string<CHAR>& input_expr,
                                                    int func_number,
                                                    int past_func_name,
                                                    core::string<CHAR>& output_value)
    {
        error_flag expr_error = ERROR::NONE;

        ///we are going to get the positions inside the function parentheses
        int layer = 0;

        core::string<CHAR> open_parenth;
        operator_string(OPERATOR::OPEN_PARENTH, open_parenth);

        core::string<CHAR> close_parenth;
        operator_string(OPERATOR::CLOSE_PARENTH, close_parenth);


        int start_pos = past_func_name;
        //search for the first open parentheses
        while ((start_pos < input_expr.length()) && !input_expr.foundAt(open_parenth, start_pos))
        {
            start_pos++;
        }


        if (start_pos >= input_expr.length())
            expr_error |= ERROR::MISSING_L_PARENTH;


        if (!expr_error)
        {
            int end_pos = start_pos+1;
            layer++;
            //search for the matching close parentheses on the same layer
            while ((end_pos < input_expr.length()) && (layer != 0))
            {
                if (input_expr.foundAt(close_parenth, end_pos))
                    layer--;
                else if (input_expr.foundAt(open_parenth, end_pos))
                    layer++;

                if (layer > 0)
                    end_pos++;
            }


            ///now we have found the range,
            //get the function parameters string
            core::string<CHAR> inside = input_expr.substr(start_pos+1, end_pos-1);
            //cout <<"'"<< inside.str() <<"'"<< endl;

            core::array< core::string<CHAR> > params;

            expr_error |= process(inside, params);

            //solve the function
            if (!expr_error)
                expr_error |= funcs[func_number].func(params, output_value);
        }


        return expr_error;
    }


    template <typename CHAR>
    int functionalExpression<CHAR>::get_func_range(const core::string<CHAR>& input_expr,
                                                    int func_name_start)
    {
        int layer = 0;

        int pos = func_name_start;

        core::string<CHAR> open_parenth;
        operator_string(OPERATOR::OPEN_PARENTH, open_parenth);

        core::string<CHAR> close_parenth;
        operator_string(OPERATOR::CLOSE_PARENTH, close_parenth);


        //search for the first open parentheses
        while ((pos < input_expr.length()) && !input_expr.foundAt(open_parenth, pos))
        {
            pos++;
        }

        if (pos >= input_expr.length())
            return (pos - func_name_start);


        pos++;
        layer++;
        //search for the matching close parentheses on the same layer
        while ((pos < input_expr.length()) && (layer != 0))
        {
            if (input_expr.foundAt(close_parenth, pos))
                layer--;
            else if (input_expr.foundAt(open_parenth, pos))
                layer++;

            pos++;
        }

        //return the length the function takes up
        return (pos - func_name_start);
    }
}

#endif // FUNCTIONALEXPRESSION_H_INCLUDED
