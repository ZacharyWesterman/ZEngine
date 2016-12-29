#pragma once
#ifndef DEVICESETTINGS_H_INCLUDED
#define DEVICESETTINGS_H_INCLUDED

#include "../core/string.h"

namespace engine
{
    namespace mode
    {
        enum DEBUG_MODE
        {
            DEBUG,
            TEST,
            RELEASE
        };
    }


    class deviceSettings
    {
    public:
        bool fullScreen;

        mode::DEBUG_MODE Mode;



        deviceSettings();

        load(const core::string<char>&);
    };


    deviceSettings::deviceSettings()
    {
        fullScreen = false;

        Mode = mode::DEBUG;


    }

    deviceSettings::load()
}


#endif // DEVICESETTINGS_H_INCLUDED
