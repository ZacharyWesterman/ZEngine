#pragma once
#ifndef COMMAND_PRINT_H_INCLUDED
#define COMMAND_PRINT_H_INCLUDED

#include "command.h"

#include <iostream>

namespace script
{
    class cmd_print : public cmd
    {
    public:
        ~cmd_print();
        void doStuff() {std::cout << "This is a DLL test!\n";}
    };
}

#endif // COMMAND_PRINT_H_INCLUDED
