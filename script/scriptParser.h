#pragma once
#ifndef SCRIPTPARSER_H_INCLUDED
#define SCRIPTPARSER_H_INCLUDED

#include "../core/array.h"
#include "../core/string.h"
#include "../core/stringUtils/eval_string.h"
#include "../core/stringUtils/char_is_whiteSpace.h"
#include "../core/dynamicStack.h"

#include "commands.h"

#include "split_script_into_commands.h"
#include "is_valid_script_name.h"

#include "variableTree.h"

#include "functionalExpression.h"

#include "escape_sequences.h"



#include <iostream>
#include "../core/stringUtils/convert_type.h"
using namespace std;

namespace script
{
    template <typename CHAR>
    class scriptParser
    {
    private:
        core::array< command<CHAR> > parsed_script;
        core::dynamic_stack< variableTree<CHAR>* > scope;
        core::dynamic_stack<int> return_position;

        core::dynamic_stack<bool> prev_conditional_result;

        variableTree<CHAR>* global_scope;
        variableTree<CHAR>* current_scope;

        functionalExpression<CHAR> expression_parser;

        core::array< core::string<CHAR> > user_functions;
        core::array< int > user_func_positions;


        int entry_point;

        int current_cmd;

        int this_err_cmd;


        void empty_scope()
        {
            while (current_scope)
            {
                delete current_scope;
                current_scope = NULL;

                scope.pop(current_scope);
            }

            if (global_scope)
                delete global_scope;
        }

        void check_scoping();
        void find_entry_point();

        void link_conditionals();
        void link_loops();
        void link_for_loops();
        void link_labels();
        void link_subroutines();
        void link_breakages();

        void check_param_counts();
        void init_global_vars();

        void link_functions();


        void set_var_in_scope(variableTree<CHAR>*, command<CHAR>&);

        uint replace_vars(const core::string<CHAR>&, core::string<CHAR>&);
        bool get_var_array_index(const core::string<CHAR>&, int, core::string<CHAR>&, int&);

        uint replace_last_function();

        void unload();
        void preprocess();

    public:
        scriptParser() {current_scope = NULL; global_scope = NULL;}
        ~scriptParser() {empty_scope();}

        void parse(const core::string<CHAR>&);
        void parse(const core::array< core::string<CHAR> >&);
        void parse(const core::string<CHAR>*, int);

        bool next(command<CHAR>&);
        bool get_next_error(command<CHAR>&);

        bool good() const;
        bool done() const;

        void stop();
    };


    ///template function to unload any previously parsed script.
    ///
    template <typename CHAR>
    void scriptParser<CHAR>::unload()
    {
        //clear the parser of any previously parsed script
        parsed_script.clear();
        scope.dump();
        return_position.dump();
        prev_conditional_result.dump();

        user_functions.clear();
        user_func_positions.clear();

        if (current_scope)
            delete current_scope;
        current_scope = NULL;

        if (global_scope)
            delete global_scope;
        global_scope = NULL;

        current_cmd = 0;
        entry_point = 0;
        this_err_cmd = 0;
    }


    ///template function to process the current script
    ///and check for contextual errors
    template <typename CHAR>
    void scriptParser<CHAR>::preprocess()
    {
        //make sure all commands are in a valid scope
        check_scoping();
        //find the entry point
        find_entry_point();

        //process the conditionals
        link_conditionals();
        //process the loops
        link_loops();
        //process the numbered loops
        link_for_loops();
        //process the labels
        link_labels();
        //process the subroutines
        link_subroutines();
        //process any break commands
        link_breakages();

        //make sure there are the right amount of
        //parameters for each command
        check_param_counts();

        //gather all user-defined functions
        link_functions();


        for (int i=0; i<user_functions.size(); i++)
        {
            cout << "function '" << core::narrow(user_functions[i]).str();
            cout << "' at line " << user_func_positions[i] << endl;
        }

        //all variables outside any scope are global
        init_global_vars();

        current_cmd = entry_point + 1;
    }


    ///Template function to split a single string input into commands
    ///and check for command-based errors.
    ///Essentially a preprocessor for before each line is individually processed.
    template <typename CHAR>
    void scriptParser<CHAR>::parse(const core::string<CHAR>& input)
    {
        unload();

        //split the script into commands based on newline or colon characters.
        split_script_into_commands(input, parsed_script);

        preprocess();
    }


    ///Template function to split string array input into commands
    ///and check for command-based errors.
    ///Essentially a preprocessor for before each line is individually processed.
    template <typename CHAR>
    void scriptParser<CHAR>::parse(const core::array< core::string<CHAR> >& input)
    {
        unload();

        //split the script into commands based on newline or colon characters.
        for (uint i=0; i<input.size(); i++)
        {
            core::array< command<CHAR> > parsed_input;

            split_script_into_commands(input.at(i), parsed_input);

            for (uint j=0; j<parsed_input.size(); j++)
                parsed_script.add(parsed_input.at(j));
        }


        preprocess();
    }


    ///Template function to split string array input into commands
    ///and check for command-based errors.
    ///Essentially a preprocessor for before each line is individually processed.
    template <typename CHAR>
    void scriptParser<CHAR>::parse(const core::string<CHAR>* input, int size)
    {
        unload();

        //split the script into commands based on newline or colon characters.
        core::array< command<CHAR> > parsed_input;

        for (int i=0; i<size; i++)
        {
            parsed_input.clear();

            split_script_into_commands(input[i], parsed_input);

            for (uint j=0; j<parsed_input.size(); j++)
                parsed_script.add(parsed_input.at(j));
        }


        preprocess();
    }


    ///template function to get the next command that has an error after preprocessing.
    ///returns false if there are no preprocessor errors left.
    template <typename CHAR>
    bool scriptParser<CHAR>::get_next_error(command<CHAR>& bad_command)
    {
        for (int i=this_err_cmd; i<parsed_script.size(); i++)
        {
            this_err_cmd = i;

            if (parsed_script[i].error)
            {
                bad_command = parsed_script[i];
                this_err_cmd++;
                return true;
            }
        }

        //if we looked through all the commands and there
        //are no more errors
        return false;
    }


    ///template function to return the current command and move to the next one.
    ///parses the current command regardless of context.
    template <typename CHAR>
    bool scriptParser<CHAR>::next(command<CHAR>& this_command)
    {
        this_command.data.clear();
        this_command.type = COMMAND::NONE;
        this_command.error = ERROR::NONE;
        this_command.meta = 0;
        this_command.text_begin = -1;
        this_command.text_end = -1;

        if (good() && !done())
        {
            this_command.type = parsed_script.at(current_cmd).type;
            this_command.error = parsed_script.at(current_cmd).error;
            this_command.meta = parsed_script.at(current_cmd).meta;
            this_command.text_begin = parsed_script.at(current_cmd).text_begin;
            this_command.text_end = parsed_script.at(current_cmd).text_end;

            //cout << "##" << this_command.text_begin << endl;

            if (!this_command.error)
            {
                //if there is currently no scope
                if (!current_scope)
                {
                    variableTree<CHAR>* prev_scope = NULL;
                    if (!scope.isEmpty())
                        prev_scope = scope.peek();

                    if (!prev_scope)
                        prev_scope = global_scope;

                    current_scope = new variableTree<CHAR>(prev_scope);
                }


                //variables can be set within the current scope
                if ((parsed_script[current_cmd].type == COMMAND::SET_VARIABLE) ||
                    (parsed_script[current_cmd].type == COMMAND::SET_ARRAY_SIZE))
                {
                    core::string<CHAR> this_data;

                    core::array< core::string<CHAR> > parsed_data;

                    for (int c=1; c<parsed_script.at(current_cmd).data.size(); c++)
                    {
                        parsed_script.at(current_cmd).error |= replace_vars(parsed_script.at(current_cmd).data.at(c), this_data);

                        if (!parsed_script.at(current_cmd).error)
                            parsed_script.at(current_cmd).error |= expression_parser.process(this_data,
                                                                                         parsed_data);
                    }



                    if (parsed_script.at(current_cmd).data.is_valid(0))
                        this_command.data.add(parsed_script.at(current_cmd).data.at(0));

                    for (int i=0; i<parsed_data.size(); i++)
                        this_command.data.add(parsed_data.at(i));
                    //cout << "##" << core::narrow(parsed_data[0]).str() <<endl;

                    this_command.error = parsed_script.at(current_cmd).error;
                    this_command.meta = parsed_script.at(current_cmd).meta;
                    this_command.type = parsed_script.at(current_cmd).type;

                    set_var_in_scope(current_scope, this_command);


                    current_cmd++;
                }
                //any new conditional will add a new layer to the scope if it evaluates to true,
                //and may remove a layer that an appropriate conditional added.
                else if (parsed_script[current_cmd].type == COMMAND::CONDITIONAL_IF)
                {
                    //set a new scope for this conditional
                    scope.push(current_scope);
                    current_scope = new variableTree<CHAR>(scope.peek());


                    core::string<CHAR> this_data;

                    if (parsed_script.at(current_cmd).data.is_valid(0))
                        this_command.error |= replace_vars(parsed_script.at(current_cmd).data.at(0), this_data);

                    if (!this_command.error)
                        this_command.error |= expression_parser.process(this_data,
                                                                        this_command.data);

                    if (core::value(this_command.data.at(0)) != 0.f)
                    {
                        prev_conditional_result.push(true);
                        current_cmd++;
                    }
                    else
                    {
                        prev_conditional_result.push(false);
                        current_cmd = parsed_script.at(current_cmd).meta;
                    }
                }
                else if (parsed_script[current_cmd].type == COMMAND::CONDITIONAL_ELSE_IF)
                {
                    //get rid of the previous conditional's scope
                    if (current_scope)
                    {
                        delete current_scope;
                        current_scope = NULL;
                    }

                    //set a new scope for this conditional
                    current_scope = new variableTree<CHAR>(scope.peek());


                    bool prev_cond_result = false;
                    prev_conditional_result.pop(prev_cond_result);


                    if (prev_cond_result)
                    {
                        prev_conditional_result.push(true);
                        current_cmd = parsed_script.at(current_cmd).meta;
                    }
                    else
                    {
                        core::string<CHAR> this_data;

                        if (parsed_script.at(current_cmd).data.is_valid(0))
                            this_command.error |= replace_vars(parsed_script.at(current_cmd).data.at(0), this_data);

                        if (!this_command.error)
                            this_command.error |= expression_parser.process(this_data,
                                                                            this_command.data);

                        if (core::value(this_command.data.at(0)) != 0.f)
                        {
                            prev_conditional_result.push(true);
                            current_cmd++;
                        }
                        else
                        {
                            prev_conditional_result.push(false);
                            current_cmd = parsed_script.at(current_cmd).meta;
                        }
                    }
                }
                else if (parsed_script[current_cmd].type == COMMAND::CONDITIONAL_ELSE)
                {
                    //get rid of the previous conditional's scope
                    if (current_scope)
                    {
                        delete current_scope;
                        current_scope = NULL;
                    }

                    //set a new scope for this conditional
                    current_scope = new variableTree<CHAR>(scope.peek());


                    bool prev_cond_result = false;
                    prev_conditional_result.pop(prev_cond_result);


                    if (prev_cond_result)
                    {
                        prev_conditional_result.push(false);
                        current_cmd = parsed_script.at(current_cmd).meta;
                    }
                    else
                    {
                        prev_conditional_result.push(true);
                        current_cmd++;
                    }
                }
                else if (parsed_script[current_cmd].type == COMMAND::CONDITIONAL_END)
                {
                    bool prev_cond_result = false;
                    prev_conditional_result.pop(prev_cond_result);


                    if (current_scope)
                    {
                        delete current_scope;
                        current_scope = NULL;
                    }

                    scope.pop(current_scope);

                    current_cmd++;
                }
                //if we go to a subroutine, no new scope will be added,
                //but a new return point will be added.
                else if (parsed_script[current_cmd].type == COMMAND::SUBROUTINE_GOTO)
                {
                    return_position.push(current_cmd+1);

                    current_cmd = parsed_script.at(current_cmd).meta+1;
                }
                else if (parsed_script[current_cmd].type == COMMAND::SUBROUTINE_RETURN)
                {
                    return_position.pop(current_cmd);
                }
                //deal with label and goto commands
                else if (parsed_script[current_cmd].type == COMMAND::LABEL_DEFINE)
                {
                    current_cmd++;
                }
                else if (parsed_script[current_cmd].type == COMMAND::LABEL_GOTO)
                {
                    //by definition, labels should go to the base scope
                    while (!scope.isEmpty())
                    {
                        if (current_scope)
                            delete current_scope;
                        current_scope = NULL;
                        scope.pop(current_scope);
                    }

                    current_cmd = (int)(parsed_script.at(current_cmd).meta+1);
                }
                //if break command, leave the current loop
                else if (parsed_script[current_cmd].type == COMMAND::LEAVE_SCOPE)
                {
                    //when leaving the current loop, we should leave as many scopes
                    //as are between the break (current_cmd) and the break point (current_cmd meta)
                    //(at least one scope)

                    //calculate how many scopes need to be left
                    int shed_scopes = 1;

                    for (int i=current_cmd; i<(int)parsed_script[current_cmd].meta; i++)
                    {
                        if (parsed_script[i].type == COMMAND::CONDITIONAL_IF)
                            shed_scopes--;
                        else if (parsed_script[i].type == COMMAND::CONDITIONAL_END)
                            shed_scopes++;
                    }

                    //leave that number of scopes
                    for (int i=0; i<shed_scopes; i++)
                    {
                        if (!scope.isEmpty())
                        {
                            if (current_scope)
                                delete current_scope;
                            current_scope = NULL;
                            scope.pop(current_scope);
                        }
                    }

                    current_cmd = parsed_script.at(current_cmd).meta;
                }
                //deal with loops
                else if (parsed_script[current_cmd].type == COMMAND::LOOP_BEGIN)
                {
                    //regardless of context, add new scope
                    scope.push(current_scope);

                    current_scope = new variableTree<CHAR>(scope.peek());

                    //move forward regardless of context
                    current_cmd++;
                }
                else if (parsed_script[current_cmd].type == COMMAND::LOOP_BEGIN_CONDITION)
                {
                    core::string<CHAR> this_data;

                    //evaluate the expression
                    if (parsed_script.at(current_cmd).data.is_valid(0))
                        this_command.error |= replace_vars(parsed_script.at(current_cmd).data.at(0), this_data);

                    if (!this_command.error)
                        this_command.error |= expression_parser.process(this_data,
                                                                        this_command.data);

                    //don't loop if the expression evaluates to 0 (false)
                    if (core::value(this_command.data.at(0)) != 0.f)
                    {
                        scope.push(current_scope);

                        current_scope = new variableTree<CHAR>(scope.peek());

                        current_cmd++;
                    }
                    else
                    {
                        current_cmd = parsed_script.at(current_cmd).meta+1;
                    }
                }
                else if (parsed_script[current_cmd].type == COMMAND::LOOP_END)
                {
                    //regardless of context, remove the current scope
                    if (current_scope)
                        delete current_scope;
                    current_scope = NULL;

                    scope.pop(current_scope);

                    //go to the beginning of the loop regardless of context
                    current_cmd = parsed_script.at(current_cmd).meta;
                }
                else if (parsed_script[current_cmd].type == COMMAND::LOOP_END_CONDITION)
                {
                    core::string<CHAR> this_data;

                    //evaluate the expression
                    if (parsed_script.at(current_cmd).data.is_valid(0))
                        this_command.error |= replace_vars(parsed_script.at(current_cmd).data.at(0), this_data);

                    if (!this_command.error)
                        this_command.error |= expression_parser.process(this_data,
                                                                        this_command.data);

                    //don't loop if the expression evaluates to 0 (false)
                    if (core::value(this_command.data.at(0)) == 0.f)
                    {
                        current_cmd++;
                    }
                    else
                    {
                        current_cmd = parsed_script.at(current_cmd).meta;
                    }


                    //after evaluating the expression, leave the scope
                    if (current_scope)
                        delete current_scope;
                    current_scope = NULL;

                    scope.pop(current_scope);
                }
                else if (parsed_script[current_cmd].type == COMMAND::FOR_LOOP_BEGIN)
                {
                    //add new scope
                    scope.push(current_scope);

                    current_scope = new variableTree<CHAR>(scope.peek());

                    if (parsed_script[current_cmd].data.size() >= 3)
                    {
                        //add new variable (0th parameter)
                        core::string<CHAR> var_name = core::remove_whitespace(
                                                        parsed_script[current_cmd].data.at(0));

                        core::string<CHAR> init_value;
                        core::string<CHAR> check_value;
                        core::string<CHAR> incr_value;

                        //calculate the starting value for the variable
                        this_command.error |= replace_vars(parsed_script.at(current_cmd).data.at(1), init_value);

                        if (!this_command.error)
                            this_command.error |= expression_parser.process(init_value,
                                                                            this_command.data);

                        //calculate the end value for the variable
                        this_command.error |= replace_vars(parsed_script.at(current_cmd).data.at(2), check_value);

                        if (!this_command.error)
                            this_command.error |= expression_parser.process(check_value,
                                                                            this_command.data);

                        //calculate the value to increment the variable
                        if (parsed_script[current_cmd].data.is_valid(3))
                            this_command.error |= replace_vars(parsed_script.at(current_cmd).data.at(3),
                                                               incr_value);
                        else
                            incr_value = 1.f; //the increment value is 1 unless specified otherwise.

                        if (!this_command.error)
                            this_command.error |= expression_parser.process(incr_value,
                                                                            this_command.data);


                        //in "for V,x,y,i", if x + i <= y
                        bool good = false;

                        if (!this_command.error &&
                            (this_command.data.size() >= 3))
                        {
                            if (core::value(this_command.data.at(2)) < 0)
                            {
                                if ((core::value(this_command.data.at(0)) +
                                    core::value(this_command.data.at(2))) >=
                                    core::value(this_command.data.at(1)))
                                    good = true;
                            }
                            else
                            {
                                if ((core::value(this_command.data.at(0)) +
                                    core::value(this_command.data.at(2))) <=
                                    core::value(this_command.data.at(1)))
                                    good = true;
                            }
                        }

                        if (good)
                        {
                            command<CHAR> t_cmd;
                            t_cmd.type = COMMAND::SET_VARIABLE;
                            t_cmd.error = ERROR::NONE;
                            t_cmd.meta = 0;
                            //set the looping variable
                            t_cmd.data.add(var_name);
                            t_cmd.data.add(this_command.data.at(0));

                            //then we are going to be in this loop,
                            set_var_in_scope(current_scope, t_cmd);
                            this_command.error |= t_cmd.error;

                            current_cmd++;
                        }
                        else
                        {
                            //if we are already done with the loop,
                            //leave the unneeded scope
                            if (current_scope)
                                delete current_scope;
                            current_scope = NULL;

                            scope.pop(current_scope);


                            current_cmd = parsed_script.at(current_cmd).meta+1;
                        }
                    }
                    else
                    {
                        current_cmd++;
                    }
                }
                else if (parsed_script[current_cmd].type == COMMAND::FOR_LOOP_END)
                {
                    if ((int)parsed_script[current_cmd].meta < current_cmd)
                    {
                        //get the variable name
                        int loop_beg = (int)parsed_script[current_cmd].meta;

                        core::string<CHAR> var_name = core::remove_whitespace(
                                                        parsed_script[loop_beg].data.at(0));

                        core::string<CHAR> var_value;
                        core::string<CHAR> check_value;
                        core::string<CHAR> incr_value;

                        //get the current value for the variable
                        this_command.error |= replace_vars(var_name, var_value);


                        //calculate the end value for the variable
                        this_command.error |= replace_vars(parsed_script.at(loop_beg).data.at(2), check_value);

                        if (!this_command.error)
                            this_command.error |= expression_parser.process(check_value,
                                                                            this_command.data);

                        //calculate the value to increment the variable
                        if (parsed_script[loop_beg].data.is_valid(3))
                            this_command.error |= replace_vars(parsed_script.at(loop_beg).data.at(3),
                                                               incr_value);
                        else
                            incr_value = 1.f; //the increment value is 1 unless specified otherwise.

                        if (!this_command.error)
                            this_command.error |= expression_parser.process(incr_value,
                                                                            this_command.data);



                        //in "for V,x,y,i", loop if x + i < y
                        bool good = false;

                        if (!this_command.error &&
                            (this_command.data.size() >= 2))
                        {
                            if (core::value(this_command.data.at(1)) < 0)
                            {
                                if ((core::value(var_value) +
                                    core::value(this_command.data.at(1))) >
                                    core::value(this_command.data.at(0)))
                                    good = true;
                            }
                            else
                            {
                                if ((core::value(var_value) +
                                    core::value(this_command.data.at(1))) <
                                    core::value(this_command.data.at(0)))
                                    good = true;
                            }
                        }

                        if (good)
                        {
                            command<CHAR> t_cmd;
                            t_cmd.type = COMMAND::SET_VARIABLE;
                            t_cmd.error = ERROR::NONE;
                            t_cmd.meta = 0;
                            //increment the looping variable
                            var_value = (core::value(var_value) + core::value(this_command.data.at(1)));


                            t_cmd.data.add(var_name);
                            t_cmd.data.add(var_value);

                            //then we are going to be in this loop,
                            set_var_in_scope(current_scope, t_cmd);
                            this_command.error |= t_cmd.error;

                            current_cmd = parsed_script.at(current_cmd).meta+1;
                        }
                        else
                        {
                            //if we are done with the loop,
                            //leave the unneeded scope
                            if (current_scope)
                                delete current_scope;
                            current_scope = NULL;

                            scope.pop(current_scope);


                            current_cmd++;
                        }
                    }
                    else
                    {
                        current_cmd++;
                    }
                }
                else if (parsed_script[current_cmd].type == COMMAND::RETURN_VALUE)
                {
                    //if there is a return point in the stack
                    if (!return_position.isEmpty())
                    {
                        while (current_scope &&
                               (current_scope->get_parent() != global_scope))
                        {
                            delete current_scope;
                            current_scope = NULL;

                            scope.pop(current_scope);
                        }

                        return_position.pop(current_cmd);
                    }
                    else //we cannot return if we are in main
                    {
                        parsed_script.at(current_cmd).error |= ERROR::BAD_SCOPING;
                    }
                }
                else //for any other command, just process the data and return the command
                {
                    for (int i=0; i<parsed_script.at(current_cmd).data.size(); i++)
                    {


                        core::string<CHAR> this_data;
                        this_command.error |= replace_vars(parsed_script.at(current_cmd).data.at(i), this_data);


                        if (!this_command.error)
                            this_command.error |= expression_parser.process(this_data,
                                                                            this_command.data);

                    }

                    current_cmd++;
                }
            }
            else
            {
                current_cmd++;
            }

            return true;
        }
        else
        {
            return false;
        }
    }

    ///template to return whether the given input is valid.
    ///
    template <typename CHAR>
    bool scriptParser<CHAR>::good() const
    {
        if (parsed_script.size() > 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    ///template to return whether the script is done parsing.
    ///
    template <typename CHAR>
    bool scriptParser<CHAR>::done() const
    {
        if (parsed_script.is_valid(current_cmd))
        {
            if (parsed_script.at(current_cmd).type != COMMAND::EXIT_POINT)
                return false;
        }

        return true;
    }

    ///template to stop the script.
    ///essentially make done() return true.
    template <typename CHAR>
    void scriptParser<CHAR>::stop()
    {
        current_cmd = parsed_script.size();
    }


    ///template function to initialize global variables,
    ///and check for errors when initializing those variables.
    template <typename CHAR>
    void scriptParser<CHAR>::init_global_vars()
    {
        if (global_scope)
            delete global_scope;

        global_scope = new variableTree<CHAR>;


        bool in_function = false;
        bool in_main = false;
        bool in_subroutine = false;

        for (int i=0; i<parsed_script.size(); i++)
        {
            //if we are in a function, the scope is valid for loops
            if (parsed_script[i].type == COMMAND::FUNCTION_DEFINE)
            {
                in_function = true;
            }
            else if (parsed_script[i].type == COMMAND::FUNCTION_RETURN)
            {
                in_function = false;
            }

            //if we are in main, the scope is valid for loops
            else if (parsed_script[i].type == COMMAND::ENTRY_POINT)
            {
                in_main = true;
            }
            else if (parsed_script[i].type == COMMAND::EXIT_POINT)
            {
                in_main = false;
            }

            //if we are in a subroutine, the scope is valid for loops
            else if (parsed_script[i].type == COMMAND::SUBROUTINE_DEFINE)
            {
                in_subroutine = true;
            }
            else if (parsed_script[i].type == COMMAND::SUBROUTINE_RETURN)
            {
                in_subroutine = false;
            }


            //variables can be set outside of the scope (global variables), but no other commands
            else if (((parsed_script[i].type == COMMAND::SET_VARIABLE) ||
                      (parsed_script[i].type == COMMAND::SET_ARRAY_SIZE)) &&
                     !(in_function || in_main || in_subroutine))
            {
                core::string<CHAR> this_data;

                if (parsed_script.at(i).data.is_valid(1))
                    parsed_script.at(i).error |= replace_vars(parsed_script.at(i).data.at(1), this_data);


                core::array< core::string<CHAR> > parsed_data;

                if (!parsed_script.at(i).error)
                    parsed_script.at(i).error |= expression_parser.process(this_data,
                                                                           parsed_data);

                command<CHAR> cmd;
                if (parsed_script.at(i).data.is_valid(0))
                    cmd.data.add(parsed_script.at(i).data.at(0));
                if (parsed_data.is_valid(0))
                    cmd.data.add(parsed_data.at(0));
                cmd.error = parsed_script.at(i).error;
                cmd.meta = parsed_script.at(i).meta;
                cmd.type = parsed_script.at(i).type;

                set_var_in_scope(global_scope, cmd);
            }
        }
    }



    ///template function to check for errors with script scoping.
    template <typename CHAR>
    void scriptParser<CHAR>::check_scoping()
    {
        bool in_function = false;
        bool in_main = false;
        bool in_subroutine = false;

        for (int i=0; i<parsed_script.size(); i++)
        {
            //if we are in a function, the scope is valid for loops
            if (parsed_script[i].type == COMMAND::FUNCTION_DEFINE)
            {
                if (in_function || in_main || in_subroutine)
                    parsed_script[i].error |= ERROR::BAD_SCOPING;

                in_function = true;
            }
            else if (parsed_script[i].type == COMMAND::FUNCTION_RETURN)
            {
                if (!in_function || in_main || in_subroutine)
                    parsed_script[i].error |= ERROR::BAD_SCOPING;

                in_function = false;
            }

            //if we are in main, the scope is valid for loops
            else if (parsed_script[i].type == COMMAND::ENTRY_POINT)
            {
                if (in_function || in_main || in_subroutine)
                    parsed_script[i].error |= ERROR::BAD_SCOPING;

                in_main = true;
            }
            else if (parsed_script[i].type == COMMAND::EXIT_POINT)
            {
                if (in_function || !in_main || in_subroutine)
                    parsed_script[i].error |= ERROR::BAD_SCOPING;

                in_main = false;
            }

            //if we are in a subroutine, the scope is valid for loops
            else if (parsed_script[i].type == COMMAND::SUBROUTINE_DEFINE)
            {
                if (in_function || in_main || in_subroutine)
                    parsed_script[i].error |= ERROR::BAD_SCOPING;

                in_subroutine = true;
            }
            else if (parsed_script[i].type == COMMAND::SUBROUTINE_RETURN)
            {
                if (in_function || in_main || !in_subroutine)
                    parsed_script[i].error |= ERROR::BAD_SCOPING;

                in_subroutine = false;
            }


            //variables can be set outside of the scope (global variables), but no other commands
            //Comments are allowed.
            else if ((parsed_script[i].type != COMMAND::NONE) &&
                     (parsed_script[i].type != COMMAND::COMMENT) &&
                     (parsed_script[i].type != COMMAND::SET_VARIABLE) &&
                     (parsed_script[i].type != COMMAND::SET_ARRAY_SIZE))
            {
                if (!(in_function || in_main || in_subroutine))
                    parsed_script[i].error |= ERROR::BAD_SCOPING;
            }
        }
    }


    ///template function to locate the script entry point if it exists,
    ///and to check for entry point errors.
    template <typename CHAR>
    void scriptParser<CHAR>::find_entry_point()
    {
        //find the entry point
        bool found_entry_point = false;

        for (int i=0; i<parsed_script.size(); i++)
        {
            if (parsed_script[i].type == COMMAND::ENTRY_POINT)
            {
                if (!found_entry_point)
                {
                    entry_point = i;

                    found_entry_point = true;
                }
                else
                {
                    parsed_script[i].error |= ERROR::BAD_REDEFINITION;
                }
            }
        }


        if (!found_entry_point)
        {
            if (parsed_script.size() > 0)
                parsed_script[parsed_script.size()-1].error |= ERROR::NO_ENTRY_POINT;

            entry_point = parsed_script.size();
        }
    }



    ///template function to check for errors with script conditionals,
    ///and to link conditionals together for greater command speed.
    template <typename CHAR>
    void scriptParser<CHAR>::link_conditionals()
    {
        core::dynamic_stack<uint> prev_conditional;

        for (int i=0; i<parsed_script.size(); i++)
        {
            //conditional "if"
            if (parsed_script[i].type == COMMAND::CONDITIONAL_IF)
            {
                parsed_script[i].meta = i+1;

                prev_conditional.push(i);
            }
            //conditional "else if"
            else if (parsed_script[i].type == COMMAND::CONDITIONAL_ELSE_IF)
            {
                parsed_script[i].meta = i+1;

                if (prev_conditional.isEmpty())
                {
                    parsed_script[i].error |= ERROR::UNEXP_COMMAND;
                }
                else
                {
                    uint prev_cond;
                    prev_conditional.pop(prev_cond);

                    if (parsed_script[prev_cond].type == COMMAND::CONDITIONAL_ELSE)
                    {
                        parsed_script[i].error |= ERROR::UNEXP_COMMAND;
                    }
                    else
                    {
                        parsed_script[prev_cond].meta = i;
                    }
                }

                prev_conditional.push(i);
            }
            //conditional "else"
            else if (parsed_script[i].type == COMMAND::CONDITIONAL_ELSE)
            {
                parsed_script[i].meta = i+1;

                if (prev_conditional.isEmpty())
                {
                    parsed_script[i].error |= ERROR::UNEXP_COMMAND;
                }
                else
                {
                    uint prev_cond;
                    prev_conditional.pop(prev_cond);

                    if (parsed_script[prev_cond].type == COMMAND::CONDITIONAL_ELSE)
                    {
                        parsed_script[i].error |= ERROR::UNEXP_COMMAND;
                    }
                    else
                    {
                        parsed_script[prev_cond].meta = i;
                    }
                }

                prev_conditional.push(i);
            }
            //conditional "endif"
            else if (parsed_script[i].type == COMMAND::CONDITIONAL_END)
            {
                if (prev_conditional.isEmpty())
                {
                    parsed_script[i].error |= ERROR::UNEXP_COMMAND;
                }
                else
                {
                    uint prev_cond;
                    prev_conditional.pop(prev_cond);


                    parsed_script[prev_cond].meta = i;
                }
            }

        }


        if (!prev_conditional.isEmpty())
            parsed_script[parsed_script.size()-1].error |= ERROR::UNEXP_END_OF_SCRIPT;
    }


    ///template function to check for errors with script loops,
    ///and to link loops together for greater command speed.
    template <typename CHAR>
    void scriptParser<CHAR>::link_loops()
    {
        core::dynamic_stack<int> prev_loop;

        for (int i=0; i<parsed_script.size(); i++)
        {
            //loop begin/begin&condition command
            if ((parsed_script[i].type == COMMAND::LOOP_BEGIN) ||
                (parsed_script[i].type == COMMAND::LOOP_BEGIN_CONDITION))
            {
                parsed_script[i].meta = i+1;

                prev_loop.push(i);
            }
            //end loop/loop&condition command
            else if ((parsed_script[i].type == COMMAND::LOOP_END) ||
                     (parsed_script[i].type == COMMAND::LOOP_END_CONDITION))
            {
                parsed_script[i].meta = i+1;

                if (prev_loop.isEmpty())
                {
                    parsed_script[i].error |= ERROR::UNEXP_COMMAND;
                }
                else
                {
                    int prev_l;
                    prev_loop.pop(prev_l);


                    parsed_script[prev_l].meta = i;
                    parsed_script[i].meta = prev_l;
                }
            }

        }


        if (!prev_loop.isEmpty())
            parsed_script[parsed_script.size()-1].error |= ERROR::UNEXP_END_OF_SCRIPT;
    }


    ///template function to check for errors with numbered script loops,
    ///and to link numbered loops together for greater command speed.
    template <typename CHAR>
    void scriptParser<CHAR>::link_for_loops()
    {
        core::dynamic_stack<uint> prev_loop;

        for (int i=0; i<parsed_script.size(); i++)
        {
            //numbered loop begin command
            if (parsed_script[i].type == COMMAND::FOR_LOOP_BEGIN)
            {
                parsed_script[i].meta = i+1;

                prev_loop.push(i);
            }
            //end loop/loop&condition command
            else if (parsed_script[i].type == COMMAND::FOR_LOOP_END)
            {
                parsed_script[i].meta = i+1;

                if (prev_loop.isEmpty())
                {
                    parsed_script[i].error |= ERROR::UNEXP_COMMAND;
                }
                else
                {
                    uint prev_l;
                    prev_loop.pop(prev_l);


                    parsed_script[prev_l].meta = i;
                    parsed_script[i].meta = prev_l;
                }
            }

        }


        if (!prev_loop.isEmpty())
            parsed_script[parsed_script.size()-1].error |= ERROR::UNEXP_END_OF_SCRIPT;
    }


    ///template function to check for errors with labels,
    ///and to link label calls to labels for greater command speed.
    template <typename CHAR>
    void scriptParser<CHAR>::link_labels()
    {
        core::array< core::string<CHAR> > label_names;
        core::array< int > label_positions;

        int scope_layer = 0;
        int scope_begin_pos = 0;

        bool getting_labels = true;


        for (int i=0; i<parsed_script.size(); i++)
        {
            if ((parsed_script[i].type == COMMAND::FUNCTION_DEFINE) ||
                (parsed_script[i].type == COMMAND::ENTRY_POINT) ||
                (parsed_script[i].type == COMMAND::SUBROUTINE_DEFINE) ||
                (parsed_script[i].type == COMMAND::CONDITIONAL_IF) ||
                (parsed_script[i].type == COMMAND::LOOP_BEGIN) ||
                (parsed_script[i].type == COMMAND::LOOP_BEGIN_CONDITION) ||
                (parsed_script[i].type == COMMAND::FOR_LOOP_BEGIN))
            {
                scope_begin_pos = i;
                scope_layer++;
                getting_labels = true;
            }
            else if ((parsed_script[i].type == COMMAND::FUNCTION_RETURN) ||
                     (parsed_script[i].type == COMMAND::EXIT_POINT) ||
                     (parsed_script[i].type == COMMAND::SUBROUTINE_RETURN) ||
                     (parsed_script[i].type == COMMAND::CONDITIONAL_END) ||
                     (parsed_script[i].type == COMMAND::LOOP_END) ||
                     (parsed_script[i].type == COMMAND::LOOP_END_CONDITION) ||
                     (parsed_script[i].type == COMMAND::FOR_LOOP_END))
            {
                scope_layer--;
            }


            if (scope_layer == 1)
            {
                if (getting_labels)
                {
                    //label definition
                    if (parsed_script[i].type == COMMAND::LABEL_DEFINE)
                    {
                        if ((parsed_script[i].data.is_valid(0)) &&
                            is_valid_script_name(parsed_script[i].data.at(0)))
                        {
                            if (label_names.find(parsed_script[i].data.at(0)) > -1)
                            {
                                parsed_script[i].error |= ERROR::BAD_REDEFINITION;
                            }
                            else
                            {
                                label_names.add(parsed_script[i].data.at(0));
                                label_positions.add(i);
                            }
                        }
                        else
                        {
                            parsed_script[i].error |= ERROR::BAD_FORMATTING;
                        }
                    }
                }
                else
                {
                    //call to jump to a label
                    if (parsed_script[i].type == COMMAND::LABEL_GOTO)
                    {
                        parsed_script[i].meta = i+1;

                        int label_pos = -1;

                        if (parsed_script[i].data.is_valid(0))
                            label_pos = label_names.find(parsed_script[i].data.at(0));

                        if (label_pos <= -1)
                        {
                            parsed_script[i].error |= ERROR::UNKNOWN_LABEL;
                        }
                        else
                        {
                            parsed_script[i].meta = label_positions[label_pos];
                        }
                    }
                }
            }
            else if (scope_layer == 0)
            {
                if (getting_labels)
                {
                    i = scope_begin_pos;
                    getting_labels = false;
                }
                else if (label_names.size() > 0)
                {
                    label_names.clear();
                    label_positions.clear();
                }
            }

        }
    }


    ///template function to check for errors with subroutines,
    ///and to link calls to subroutines for greater command speed.
    template <typename CHAR>
    void scriptParser<CHAR>::link_subroutines()
    {
        core::array< core::string<CHAR> > subroutine_names;
        core::array< int > subroutine_positions;

        //find all the subroutines
        for (int i=0; i<parsed_script.size(); i++)
        {
            if (parsed_script[i].type == COMMAND::SUBROUTINE_DEFINE)
            {
                if (parsed_script[i].data.is_valid(0) &&
                    is_valid_script_name(parsed_script[i].data.at(0)))
                {
                    if (subroutine_names.find(parsed_script[i].data.at(0)) > -1)
                    {
                        parsed_script[i].error |= ERROR::BAD_REDEFINITION;
                    }
                    else
                    {
                        subroutine_names.add(parsed_script[i].data.at(0));
                        subroutine_positions.add(i);
                    }
                }
                else
                {
                    parsed_script[i].error |= ERROR::BAD_FORMATTING;
                }
            }
        }

        //link all the subroutine calls
        for (int i=0; i<parsed_script.size(); i++)
        {
            if (parsed_script[i].type == COMMAND::SUBROUTINE_GOTO)
            {
                parsed_script[i].meta = i+1;

                int sub_pos = -1;

                if (parsed_script[i].data.is_valid(0))
                    sub_pos = subroutine_names.find(parsed_script[i].data.at(0));

                if (sub_pos <= -1)
                {
                    parsed_script[i].error |= ERROR::UNKNOWN_SUBROUTINE;
                }
                else
                {
                    parsed_script[i].meta = subroutine_positions[sub_pos];
                }
            }

        }
    }


    ///template function to check for errors with break commands,
    ///and to link break commands to the appropriate scope end for greater command speed.
    template <typename CHAR>
    void scriptParser<CHAR>::link_breakages()
    {
        core::dynamic_stack< core::array<int> > scoped_break_pos;

        core::array<int> break_pos;

        for(int i=0; i<parsed_script.size(); i++)
        {
            if (parsed_script[i].type == COMMAND::LEAVE_SCOPE)
            {
                parsed_script[i].meta = (t_meta)(i+1);
                break_pos.add(i);
            }
            else if ((parsed_script[i].type == COMMAND::LOOP_BEGIN) ||
                     (parsed_script[i].type == COMMAND::LOOP_BEGIN_CONDITION))
            {
                scoped_break_pos.push(break_pos);
            }
            else if (parsed_script[i].type == COMMAND::FOR_LOOP_BEGIN)
            {
                scoped_break_pos.push(break_pos);
            }
            else if ((parsed_script[i].type == COMMAND::LOOP_END) ||
                     (parsed_script[i].type == COMMAND::LOOP_END_CONDITION) ||
                     (parsed_script[i].type == COMMAND::FOR_LOOP_END))
            {
                for (int b=0; b<break_pos.size(); b++)
                {
                    parsed_script[break_pos[b]].meta = (t_meta)(i+1);
                }

                break_pos.clear();

                scoped_break_pos.pop(break_pos);
            }

        }


        //Errors if there are any break commands out of a valid scope.
        for (int b=0; b<break_pos.size(); b++)
        {
            parsed_script[break_pos[b]].error |= ERROR::BAD_SCOPING;
        }

        break_pos.clear();

        while (!scoped_break_pos.isEmpty())
        {
            scoped_break_pos.pop(break_pos);

            for (int b=0; b<break_pos.size(); b++)
            {
                parsed_script[break_pos[b]].error |= ERROR::BAD_SCOPING;
            }

            break_pos.clear();
        }
    }


    ///template function to check the amount of parameters of each command,
    ///(check if there are too many, too few, or the right amount)
    template <typename CHAR>
    void scriptParser<CHAR>::check_param_counts()
    {
        //look at every command
        for (int i=0; i<parsed_script.size(); i++)
        {
            if ((parsed_script[i].data.size() < COMMAND::MIN_PARAMS[parsed_script[i].type]) &&
                (COMMAND::MIN_PARAMS[parsed_script[i].type] > -1))
            {
                parsed_script[i].error |= ERROR::TOO_FEW_PARAMS;
            }
            else if ((parsed_script[i].data.size() > COMMAND::MAX_PARAMS[parsed_script[i].type]) &&
                     (COMMAND::MAX_PARAMS[parsed_script[i].type] > -1))
            {
                parsed_script[i].error |= ERROR::TOO_MANY_PARAMS;
            }
        }
    }



    ///template to find all user-defined functions and index them,
    ///and check for errors in user-defined functions
    template <typename CHAR>
    void scriptParser<CHAR>::link_functions()
    {
        bool in_function = false;

        for (int i=0; i<parsed_script.size(); i++)
        {
            if (!parsed_script[i].error)
            {
                //if we are in a function, we can check for function-based errors
                if (parsed_script[i].type == COMMAND::FUNCTION_DEFINE)
                {
                    user_functions.add(parsed_script[i].data[0]);
                    user_func_positions.add(i);

                    in_function = true;
                }
                else if (parsed_script[i].type == COMMAND::FUNCTION_RETURN)
                {
                    in_function = false;
                }
                //variables can be set outside of the scope (global variables), but no other commands
                //Comments are allowed.
                /*else if (in_function &&
                         (parsed_script[i].type != COMMAND::NONE) &&
                         (parsed_script[i].type != COMMAND::COMMENT) &&
                         (parsed_script[i].type != COMMAND::SET_VARIABLE) &&
                         (parsed_script[i].type != COMMAND::SET_ARRAY_SIZE))
                {
                    if (!(in_function || in_main || in_subroutine))
                        parsed_script[i].error |= ERROR::BAD_SCOPING;
                }*/

                if (in_function){}
            }
        }
    }



    ///template function to set a variable in the given scope,
    ///and to check for errors with variable setting
    template <typename CHAR>
    void scriptParser<CHAR>::set_var_in_scope(variableTree<CHAR>* scope, command<CHAR>& cmd)
    {

        if (cmd.type == COMMAND::SET_VARIABLE)
        {
            if (cmd.data.size() == 2)   //variable name and single value
            {

                core::array< core::string<CHAR> > expr_values;

                cmd.error |= expression_parser.process(cmd.data.at(1), expr_values);



                core::string<CHAR> var_name;

                int end_index = 0;
                core::string<CHAR> array_index;

                if (expr_values.is_valid(0))
                {

                    //if we are setting an index in an array to a value
                    if (scope->found_variable(cmd.data.at(0), 0, var_name) &&
                        get_var_array_index(cmd.data.at(0), var_name.length(), array_index, end_index))
                    {
                        core::string<CHAR> index_novars;

                        cmd.error |= replace_vars(array_index, index_novars);

                        core::array< core::string<CHAR> > condensed_index;

                        cmd.error |= expression_parser.process(index_novars, condensed_index);


                        if (condensed_index.size() > 1)
                            cmd.error |= ERROR::TOO_MANY_PARAMS;
                        else if (condensed_index.size() < 1)
                            cmd.error |= ERROR::TOO_FEW_PARAMS;


                        if (!cmd.error)
                        {
                            cmd.error |= scope->set_array_value(var_name, (int)core::value(condensed_index.at(0)),
                                                                expr_values.at(0));
                        }
                    }
                    else //if we are setting variable to a single value
                    {
                        if (!cmd.error)
                        {
                            cmd.error |= scope->set_to_single_value(cmd.data.at(0), expr_values.at(0));
                        }
                    }
                }
            }
            else if (cmd.data.size() > 2) //variable name and multiple values
            {
                core::array< core::string<CHAR> > expr_values;

                for (int i=1; i<cmd.data.size(); i++)
                {
                    cmd.error |= expression_parser.process(cmd.data.at(i), expr_values);
                }


                if (!cmd.error)
                {
                    cmd.error |= scope->set_to_array(cmd.data.at(0), expr_values);
                }
            }
        }
        else if (cmd.type == COMMAND::SET_ARRAY_SIZE)
        {
            if (cmd.data.is_valid(0) && //has array name,
                cmd.data.is_valid(1))   //and array size
            {
                core::array< core::string<CHAR> > expr_values;
                cmd.error |= expression_parser.process(cmd.data.at(1), expr_values);


                if (!cmd.error && expr_values.is_valid(0))
                {
                    cmd.error |= scope->set_to_array_of_size(cmd.data.at(0), (int)core::value(expr_values.at(0)));
                }
            }
        }
    }


    ///template function to replace the variables in a given string with
    ///the values of corresponding variables in the current scope.
    template <typename CHAR>
    error_flag scriptParser<CHAR>::replace_vars(const core::string<CHAR>& input, core::string<CHAR>& output)
    {
        error_flag error = ERROR::NONE;

        output = input;

        variableTree<CHAR>* scope = current_scope;

        if (!scope)
            scope = global_scope;


        if (scope)
        {
            bool in_quote = false;
            core::string<CHAR> quote = (CHAR)34;

            core::string<CHAR> quote_esc;
            esc_sequence_name(ESC_SEQUENCE::QUOTE, quote_esc);


            for (int i=output.length()-1; i>=0; i--)
            {
                if (output.foundAt(quote_esc, i-1))
                {
                    i--;
                }
                else if(output.foundAt(quote, i))
                {
                    in_quote = !in_quote;
                }


                core::string<CHAR> var_name;

                if (!in_quote && scope->found_variable(output, i, var_name))
                {
                    core::string<CHAR> var_value;


                    core::string<CHAR> var_index; //determine if user is trying to access an array element
                    int var_end_index = -1;
                    if (get_var_array_index(output, i + var_name.length(), var_index, var_end_index))
                    {
                        core::array< core::string<CHAR> > condensed_index;

                        error |= expression_parser.process(var_index, condensed_index);


                        if (condensed_index.size() > 1)
                            error |= ERROR::TOO_MANY_PARAMS;
                        else if (condensed_index.size() < 1)
                            error |= ERROR::TOO_FEW_PARAMS;

                        if (!error)
                        {
                            error |= scope->get_array_value(var_name, (int)core::value(condensed_index.at(0)), var_value);
                        }
                    }
                    else //if user is trying to access a single variable
                    {
                        //cout << "s";
                        error |= scope->get_single_value(var_name, var_value);
                        var_end_index = i + var_name.length()-1;
                    }


                    if (!error)
                    {

                        output.replace(i, var_end_index, var_value);

                        /*cout << "## ";
                        for (int i=0; i<output.length(); i++)
                            cout << (char)output[i];
                        cout << "##\n";*/

                        //i += var_value.length() - var_name.length();
                    }
                }
            }
        }

        return error;
    }


    ///template function to get the value of index from var[index]
    ///
    template <typename CHAR>
    bool scriptParser<CHAR>::get_var_array_index(const core::string<CHAR>& input, int index_after_name,
                                                 core::string<CHAR>& output, int& end_index)
    {
        core::string<CHAR> open_bracket = (CHAR)91;
        core::string<CHAR> close_bracket = (CHAR)93;

        bool found_open = false;
        bool found_close = false;

        int open_index = -1;
        int close_index = -1;

        for (int i=index_after_name; i<input.length(); i++)
        {

            if (input.foundAt(open_bracket, i))
            {
                open_index = i+1;
                found_open = true;
            }
            else if (input.foundAt(close_bracket, i))
            {
                close_index = i-1;
                found_close = true;
            }
            else if (!found_open && !core::is_white_space(input.at(i)))
            {
                return false;
            }

            if (found_open && found_close)
            {
                end_index = close_index + 1;
                output = input.substr(open_index, close_index);

                return true;
            }
            else if (found_close)
            {
                return false;
            }
        }


        if (found_open)
        {
            close_index = input.length()-1;

            end_index = close_index;

            output = input.substr(open_index, close_index);

            return true;
        }


        return false;
    }

}

#endif // SCRIPTPARSER_H_INCLUDED
