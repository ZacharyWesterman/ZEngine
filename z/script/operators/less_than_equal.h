/**
 * File:            less_than_equal.h
 * Namespace:       z::script::oper
 * Description:     '<=' script operator.
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   3 Feb. 2017
**/

#pragma once
#ifndef LESS_THAN_EQUAL_H_INCLUDED
#define LESS_THAN_EQUAL_H_INCLUDED

#include "oper_t.h"

#include "../is_script_string.h"
#include "../destringify.h"

namespace z
{
    namespace script
    {
        namespace oper
        {
            template <typename CHAR>
            class less_than_equal : public oper_t<CHAR>
            {
            public:
                ///LT equality uses '<=' and has a priority of 2.
                less_than_equal() : oper_t<CHAR>(core::string<char>("<="), 2, false) {}


                ///Perform operation
                error_flag operate(core::dynamic_stack< core::string<CHAR> >& operands) const
                {
                    error_flag operation_error = error::NONE;

                    core::string<CHAR> arg1;
                    core::string<CHAR> arg2;

                    if (operands.pop(arg2))
                    {
                        if (operands.pop(arg1))
                        {
                            bool arg1_is_string = is_script_string(arg1);
                            bool arg2_is_string = is_script_string(arg2);

                            if (arg1_is_string || arg2_is_string)
                            {
                                //we are simply checking string equality
                                //(even our "numbers" are strings.)

                                //so just remove any quotes from strings
                                if (arg1_is_string)
                                    arg1 = destringify(arg1);

                                if (arg2_is_string)
                                    arg2 = destringify(arg2);


                                double result = (double)(arg1 <= arg2); ///check string equality

                                operands.push(result);
                            }
                            else
                            {
                                double val1 = core::value(arg1);
                                double val2 = core::value(arg2);

                                double result = (val1 <= val2); ///check number equality

                                operands.push(result);
                            }

                        }
                        else
                        {
                            operation_error |= error::MISSING_OPERAND;
                        }
                    }
                    else
                    {
                        operation_error |= error::MISSING_OPERAND;
                    }

                    return operation_error;
                }

            };
        }
    }
}

#endif // LESS_THAN_EQUAL_H_INCLUDED
