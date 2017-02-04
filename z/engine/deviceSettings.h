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
        int width;
        int height;

        int bits;

        bool fullscreen;
        bool shadows;

        bool vsync;

        mode::DEBUG_MODE Mode;



        deviceSettings();

        bool load(const core::string<char>&);
    };


    deviceSettings::deviceSettings()
    {
        width = 800;
        height = 600;

        bits = 16;

        fullscreen = false;
        shadows = false;

        vsync = false;

        Mode = mode::DEBUG;
    }

    bool deviceSettings::load(const core::string<char>& fileName) {return false;}
}


#endif // DEVICESETTINGS_H_INCLUDED
