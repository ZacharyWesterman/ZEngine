#pragma once
#ifndef FPS_H_INCLUDED
#define FPS_H_INCLUDED

#include "../../core/timer.h"

namespace engine
{
    namespace global
    {
        core::timer frame;

        double FPS(-1);
    }
}

#endif // GLOBAL_FPS_H_INCLUDED
