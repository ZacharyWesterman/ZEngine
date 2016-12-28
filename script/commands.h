#pragma once
#ifndef COMMANDS_H_INCLUDED
#define COMMANDS_H_INCLUDED

#include "../core/string.h"
#include "../core/stringUtils/string_remove_whiteSpace.h"
#include "../core/array.h"

#include "script_errors.h"
#include "split_parameters.h"

namespace script
{
    namespace COMMAND
    {
        enum CMD
        {
            NONE = 0,
            SET_VARIABLE, //will come after some variable name
            SET_ARRAY_SIZE,

            LABEL_DEFINE,
            LABEL_GOTO,

            SUBROUTINE_DEFINE,
            SUBROUTINE_GOTO,
            SUBROUTINE_RETURN,

            FUNCTION_DEFINE,
            FUNCTION_RETURN,

            CONDITIONAL_IF,
            CONDITIONAL_ELSE,
            CONDITIONAL_ELSE_IF,
            CONDITIONAL_END,

            LOOP_BEGIN,
            LOOP_BEGIN_CONDITION,
            LOOP_END_CONDITION,
            LOOP_END,

            FOR_LOOP_BEGIN,
            FOR_LOOP_END,

            LEAVE_SCOPE,
            RETURN_VALUE,

            ENTRY_POINT,
            EXIT_POINT,
            EXIT_PROGRAM,

            COMMENT,

            PRINT_TO_CONSOLE,

            SET_WINDOW_TITLE,

            SET_BACKGROUND_COLOR,

            DELAY_SCRIPT,

            COMMAND_COUNT
        };


        const core::string<char> CHAR_COMMANDS[] =
        {
            "",     //NONE
            "=",    //SET_VARIABLE
            "dim",  //SET_ARRAY_SIZE

            "label",    //LABEL_DEFINE
            "goto",     //LABEL_GOTO

            "sub",      //SUBROUTINE_DEFINE
            "gosub",    //SUBROUTINE_GOTO
            "endsub",   //SUBROUTINE_RETURN

            "function",     //FUNCTION_DEFINE
            "endfunction",  //FUNCTION_RETURN

            "if",       //CONDITIONAL_IF
            "else",     //CONDITIONAL_ELSE
            "else if",  //CONDITIONAL_ELSE_IF
            "endif",    //CONDITIONAL_END

            "do",   //LOOP_BEGIN
            "do while",//LOOP_BEGIN_CONDITION
            "while",//LOOP_END_CONDITION
            "loop", //LOOP_END

            "for",  //FOR_LOOP_BEGIN,
            "next", //FOR_LOOP_END,

            "break",//LEAVE_SCOPE
            "return",//RETURN_VALUE

            "main",//ENTRY_POINT,
            "endmain",//EXIT_POINT,
            "exit",//EXIT_PROGRAM,

            "#",//COMMENT

            "print",//PRINT_TO_CONSOLE

            "set window title", //SET_WINDOW_TITLE

            "set background color", //SET_BACKGROUND_COLOR

            "delay", //DELAY_SCRIPT
        };

        const core::string<wchar_t> WCHAR_T_COMMANDS[] =
        {
            L"",     //NONE
            L"=",    //SET_VARIABLE
            L"dim",  //SET_ARRAY_SIZE

            L"label",    //LABEL_DEFINE
            L"goto",     //LABEL_GOTO

            L"sub",      //SUBROUTINE_DEFINE
            L"gosub",    //SUBROUTINE_GOTO
            L"endsub",   //SUBROUTINE_RETURN

            L"function",     //FUNCTION_DEFINE
            L"endfunction",  //FUNCTION_RETURN

            L"if",       //CONDITIONAL_IF
            L"else",     //CONDITIONAL_ELSE
            L"else if",  //CONDITIONAL_ELSE_IF
            L"endif",    //CONDITIONAL_END

            L"do",   //LOOP_BEGIN
            L"do while",//LOOP_BEGIN_CONDITION
            L"while",//LOOP_END_CONDITION
            L"loop", //LOOP_END

            L"for",  //FOR_LOOP_BEGIN,
            L"next", //FOR_LOOP_END,

            L"break",//LEAVE_SCOPE
            L"return",//RETURN_VALUE

            L"main",//ENTRY_POINT,
            L"endmain",//EXIT_POINT,
            L"exit",//EXIT_PROGRAM,

            L"#",//COMMENT

            L"print",//PRINT_TO_CONSOLE

            L"set window title", //SET_WINDOW_TITLE

            L"set background color", //SET_BACKGROUND_COLOR

            L"delay", //DELAY_SCRIPT
        };


        typedef int8_t t_param_c;

        const t_param_c MIN_PARAMS[] =
        {
            0,//NONE,
            2,//SET_VARIABLE,   (min. 2 parameters: var name, var value)
            2,//SET_ARRAY_SIZE, (min. 2 parameters: array name, array size)

            1,//LABEL_DEFINE,
            1,//LABEL_GOTO,

            1,//SUBROUTINE_DEFINE,
            1,//SUBROUTINE_GOTO,
            0,//SUBROUTINE_RETURN,

            1,//FUNCTION_DEFINE, (min. 1 parameter: function name)
            0,//FUNCTION_RETURN,

            1,//CONDITIONAL_IF,
            0,//CONDITIONAL_ELSE,
            1,//CONDITIONAL_ELSE_IF,
            0,//CONDITIONAL_END,

            0,//LOOP_BEGIN,
            1,//LOOP_BEGIN_CONDITION,
            1,//LOOP_END_CONDITION,
            0,//LOOP_END,

            3,//FOR_LOOP_BEGIN,
            0,//FOR_LOOP_END,

            0,//LEAVE_SCOPE,
            1,//RETURN_VALUE,

            0,//ENTRY_POINT,
            0,//EXIT_POINT
            0,//EXIT_PROGRAM,

            -1,//COMMENT

            1,//PRINT_TO_CONSOLE

            1, //SET_WINDOW_TITLE

            1, //SET_BACKGROUND_COLOR

            1, //DELAY_SCRIPT
        };

        const t_param_c MAX_PARAMS[] =
        {
            0,//NONE,
            -1,//SET_VARIABLE, can be an array of any size
            2,//SET_ARRAY_SIZE,

            1,//LABEL_DEFINE,
            1,//LABEL_GOTO,

            1,//SUBROUTINE_DEFINE,
            1,//SUBROUTINE_GOTO,
            0,//SUBROUTINE_RETURN,

            -1,//FUNCTION_DEFINE, can have any number of parameters
            0,//FUNCTION_RETURN

            1,//CONDITIONAL_IF,
            0,//CONDITIONAL_ELSE,
            1,//CONDITIONAL_ELSE_IF,
            0,//CONDITIONAL_END,

            0,//LOOP_BEGIN,
            1,//LOOP_BEGIN_CONDITION,
            1,//LOOP_END_CONDITION,
            0,//LOOP_END,

            4,//FOR_LOOP_BEGIN,
            0,//FOR_LOOP_END,

            0,//LEAVE_SCOPE,
            1,//RETURN_VALUE,

            0,//ENTRY_POINT,
            0,//EXIT_POINT
            -1,//EXIT_PROGRAM,

            -1,//COMMENT

            1,//PRINT_TO_CONSOLE

            1, //SET_WINDOW_TITLE

            1, //SET_BACKGROUND_COLOR

            1, //DELAY_SCRIPT
        };
    }



    typedef int t_meta;
    typedef int cmd_flag;

    template <typename CHAR>
    class command
    {
    public:
        cmd_flag type;

        core::array< core::string<CHAR> > data;

        error_flag error;

        t_meta meta;


        int text_begin;
        int text_end;


        bool operator==(const command& other) const
        {
            return ((type == other.type) &&
                    (data == other.data) &&
                    (meta == other.meta));
        }
    };


    void commandName(const cmd_flag cmd, core::string<char>& name)
    {
        if (cmd < COMMAND::COMMAND_COUNT)
            name = COMMAND::CHAR_COMMANDS[cmd];
        else
            name = COMMAND::CHAR_COMMANDS[COMMAND::NONE];
    }

    void commandName(const cmd_flag cmd, core::string<wchar_t>& name)
    {
        if (cmd < COMMAND::COMMAND_COUNT)
            name = COMMAND::WCHAR_T_COMMANDS[cmd];
        else
            name = COMMAND::WCHAR_T_COMMANDS[COMMAND::NONE];
    }


    //template function to split a string into its appropriate command data
    template <typename CHAR>
    command<CHAR> getCommand(const core::string<CHAR>& input)
    {
        command<CHAR> output;
        output.type = COMMAND::NONE;
        output.error = ERROR::NONE;
        output.meta = 0;
        output.text_begin = -1;
        output.text_end = -1;


        core::string<CHAR> thisCMDname;
        core::string<CHAR> thisInput = core::remove_whitespace(input);

        for (cmd_flag i=COMMAND::COMMAND_COUNT-1; i>0; i--)
        {
            commandName(i, thisCMDname);

            if (thisCMDname.length() > 0)
            {
                //set variable command will be after a variable name
                if (i == COMMAND::SET_VARIABLE)
                {
                    int pos = thisInput.find(thisCMDname);

                    if (pos > -1)
                    {
                        output.type = i;

                        //append variable name
                        output.data.add(core::remove_whitespace(thisInput.substr(0, pos - 1)));

                        //append variable value(s)
                        split_parameters(core::remove_whitespace(thisInput.substr(thisCMDname.length() + pos,
                                                                              thisInput.length()-1)), output.data);

                        return output;
                    }
                }
                //define function command will behave like name(a,b,...)
                else if (i == COMMAND::FUNCTION_DEFINE)
                {
                    if (input.beginsWith(thisCMDname))
                    {
                        output.type = i;

                        core::string<CHAR> openParenth;
                        core::string<CHAR> closeParenth;

                        operator_string(OPERATOR::OPEN_PARENTH, openParenth);
                        operator_string(OPERATOR::CLOSE_PARENTH, closeParenth);

                        int funcNameBegin = thisCMDname.length();
                        int funcParamsBegin = thisInput.find(openParenth);

                        if (funcParamsBegin > -1)
                        {
                            //append function name
                            core::string<CHAR> funcName = core::remove_whitespace(
                                                                thisInput.substr(funcNameBegin,
                                                                funcParamsBegin-1));

                            if (funcName.length() > 0)
                                output.data.add(funcName);


                            //split function parameters
                            int funcParamsEnd = thisInput.find(closeParenth);

                            core::array< core::string<CHAR> > params;

                            split_parameters(thisInput.substr(funcParamsBegin+1,funcParamsEnd-1),
                                             params);

                            //and append them
                            for (int p=0; p<params.size(); p++)
                            {
                                core::string<CHAR> param = core::remove_whitespace(params.at(p));

                                //but only if the parameter has something
                                if (param.length() > 0)
                                    output.data.add(param);
                            }
                        }
                        else
                        {
                            //append function name
                            core::string<CHAR> funcName = core::remove_whitespace(
                                                                input.substr(funcNameBegin,
                                                                input.length()-1));

                            if (funcName.length() > 0)
                                output.data.add(funcName);
                        }

                        return output;
                    }
                }
                //comments can have no errors
                else if (i == COMMAND::COMMENT)
                {
                    if (thisInput.beginsWith(thisCMDname))
                    {
                        output.type = i;

                        output.data.add(thisInput.substr(thisCMDname.length(), thisInput.length()-1));

                        return output;
                    }
                }
                //all other commands behave the same as each other
                else
                {
                    if (thisInput.beginsWith(thisCMDname) &&
                        !core::is_alphanumeric(thisInput.at(thisCMDname.length())))
                    {
                        //cout << "##" << thisCMDname.str() << endl;
                        output.type = i;

                        split_parameters(core::remove_whitespace(thisInput.substr(thisCMDname.length(),
                                                                              thisInput.length()-1)), output.data);

                        return output;
                    }
                }
            }
        }


        if (thisInput.length() > 0)
        {
            output.data.add(thisInput);
            output.error |= ERROR::UNKNOWN_COMMAND;
        }

        return output;
    }
}

#endif // COMMANDS_H_INCLUDED
