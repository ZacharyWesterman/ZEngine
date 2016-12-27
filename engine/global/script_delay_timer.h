#ifndef DELAY_TIMER_H_INCLUDED
#define DELAY_TIMER_H_INCLUDED

#include "../../core/timer.h"

namespace engine
{
    namespace global
    {
        core::timer SCRIPT_DELAY_TIMER(true);

        double SCRIPT_DELAY_MILLIS (0.f);
    }
}

#endif // DELAY_TIMER_H_INCLUDED
