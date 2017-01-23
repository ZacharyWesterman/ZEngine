#pragma once
#ifndef COMMAND_H_INCLUDED
#define COMMAND_H_INCLUDED

namespace script
{
    class cmd
    {
    public:
        virtual ~cmd();

        virtual void doStuff() = 0;
    };
}

#endif // COMMAND_H_INCLUDED
