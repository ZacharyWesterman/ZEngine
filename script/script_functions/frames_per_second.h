#pragma once
#ifndef FRAMES_PER_SECOND_H_INCLUDED
#define FRAMES_PER_SECOND_H_INCLUDED

#include "../../core/array.h"
#include "../../core/string.h"
#include "../script_errors.h"

#include "../../engine/global/fps.h"


namespace script
{
    namespace FUNCTION
    {
        //fps function takes no parameters.
        template <typename CHAR>
        error_flag frames_per_second(const core::array< core::string<CHAR> >& params, core::string<CHAR>& output)
        {
            error_flag func_error = ERROR::NONE;

            if (params.size() > 0)
            {
                func_error |= ERROR::TOO_MANY_PARAMS;
            }
            else
            {
                output = engine::global::FPS;
            }

            return func_error;
        }
        //end of fps

    }
}

#endif // FRAMES_PER_SECOND_H_INCLUDED
