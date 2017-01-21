#pragma once
#ifndef SPLIT_SCRIPT_INTO_COMMANDS_H_INCLUDED
#define SPLIT_SCRIPT_INTO_COMMANDS_H_INCLUDED

#include "../core/array.h"
#include "../core/string.h"

#include "commands.h"

namespace script
{
    template <typename CHAR>
    void split_script_into_commands(const core::string<CHAR>& input,
                                    core::array< command<CHAR> >& output)
    {
        core::string<CHAR> new_line = (CHAR)10;
        core::string<CHAR> carr_ret = (CHAR)13;
        core::string<CHAR> new_cmd = (CHAR)59; //semicolon character
        //core::string<CHAR> new_cmd = (CHAR)58; //colon character


        int start_pos = 0;
        int end_pos = -1;

        for (int i=0; i<input.length(); i++)
        {
            if (input.foundAt(new_line, i) ||
                input.foundAt(carr_ret, i) ||
                input.foundAt(new_cmd, i))
            {
                if (i == start_pos)
                    start_pos++;

                end_pos = i-1;

                if (end_pos > start_pos)
                {
                    command<CHAR> t_cmd = getCommand(core::remove_whitespace(input.substr(start_pos, end_pos)));
                    t_cmd.text_begin = start_pos;
                    t_cmd.text_end = end_pos;


                    //ignore blank lines and comments
                    if ((t_cmd.type != cmd::COMMENT) &&
                        (t_cmd.error ||
                        (t_cmd.type != cmd::NONE)))
                    {

                        output.add(t_cmd);
                    }
                }

                start_pos = i + 1;
            }
        }

        if (input.length() > start_pos)
        {
            command<CHAR> t_cmd = getCommand(input.substr(start_pos, input.length() - 1));
            t_cmd.text_begin = start_pos;
            t_cmd.text_end = input.length() - 1;

            output.add(t_cmd);
        }
    }
}

#endif // SPLIT_SCRIPT_INTO_COMMANDS_H_INCLUDED
