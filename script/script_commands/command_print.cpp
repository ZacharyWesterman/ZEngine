#include "command_print.h"

namespace script
{
    __declspec (dllexport) cmd* loadCommand()
    {
        cmd* thisCommand = new cmd_print;

        return thisCommand;
    }
}
