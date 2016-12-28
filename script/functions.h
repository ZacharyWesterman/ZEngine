#pragma once
#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

#include "../core/string.h"
#include "../core/array.h"

namespace script
{
    namespace FUNCTION
    {
        template <typename CHAR>
        class function
        {
        public:
            core::string<CHAR> name;

            COMMAND::t_param_c min_params;
            COMMAND::t_param_c max_params;

            //pointer to real function.
            //every script function must take an array of strings as parameters,
            //and a single string as output. It should return any error encountered.
            error_flag (*func)(const core::array< core::string<CHAR> >&, core::string<CHAR>&);


            function() {}
            function(const core::string<CHAR>& Name,
                     const COMMAND::t_param_c minParams, const COMMAND::t_param_c maxParams,
                     error_flag (*Func)(const core::array< core::string<CHAR> >&, core::string<CHAR>&))
            {
                name = Name;
                min_params = minParams;
                max_params = maxParams;
                func = Func;
            }
        };


        enum FUNCTIONS
        {
            CEILING,
            FLOOR,
            ROUND,

            RAND_F,
            RAND_I,

            LOG,
            SQUARE_ROOT,

            SINE,
            COSINE,
            TANGENT,

            DEG_TO_RAD,
            RAD_TO_DEG,

            ARC_SINE,
            ARC_COSINE,
            ARC_TANGENT,

            ABSOLUTE_VAL,
            FPS,

            RGBA,
            RGB,
            RED_RGB,
            GREEN_RGB,
            BLUE_RGB,
            ALPHA_RGB,

            FUNCTION_COUNT
        };

        const COMMAND::t_param_c MIN_PARAM[] =
        {
            1,//CEILING,
            1,//FLOOR,
            1,//ROUND,

            2,//RAND_F,
            2,//RAND_I,

            1,//LOG,
            1,//SQUARE_ROOT

            1,//SINE
            1,//COSINE
            1,//TANGENT

            1,//DEG_TO_RAD
            1,//RAD_TO_DEG

            1,//ARC_SINE
            1,//ARC_COSINE
            1,//ARC_TANGENT

            1,//ABSOLUTE_VAL
            0,//FPS

            4,//RGBA
            3,//RGB
            1,//RED_RGB
            1,//GREEN_RGB
            1,//BLUE_RGB
            1,//ALPHA_RGB
        };

        const COMMAND::t_param_c MAX_PARAM[] =
        {
            1,//CEILING,
            1,//FLOOR,
            1,//ROUND,

            2,//RAND_F,
            2,//RAND_I,

            2,//LOG,
            1,//SQUARE_ROOT

            1,//SINE
            1,//COSINE
            1,//TANGENT

            1,//DEG_TO_RAD
            1,//RAD_TO_DEG

            1,//ARC_SINE
            1,//ARC_COSINE
            1,//ARC_TANGENT

            1,//ABSOLUTE_VAL
            0,//FPS

            4,//RGBA
            3,//RGB
            1,//RED_RGB
            1,//GREEN_RGB
            1,//BLUE_RGB
            1,//ALPHA_RGB
        };


        const core::string<char> CHAR_FUNC_NAME[] =
        {
            "ceil",
            "floor",
            "round",

            "frand",
            "irand",

            "log",
            "sqrt",

            "sin",
            "cos",
            "tan",

            "rad",
            "deg",

            "arcsin",
            "arccos",
            "arctan",

            "abs",
            "fps",

            "rgba",
            "rgb",
            "red",
            "green",
            "blue",
            "alpha",
        };

        const core::string<wchar_t> WCHAR_T_FUNC_NAME[] =
        {
            L"ceil",
            L"floor",
            L"round",

            L"frand",
            L"irand",

            L"log",
            L"sqrt",

            L"sin",
            L"cos",
            L"tan",

            L"rad",
            L"deg",

            L"arcsin",
            L"arccos",
            L"arctan",

            L"abs",
            L"fps",

            L"rgba",
            L"rgb",
            L"red",
            L"green",
            L"blue",
            L"alpha",
        };


        bool function_name(int func, core::string<char>& name)
        {
            if ((func < FUNCTION_COUNT) &&
                (func >= 0))
            {
                name = CHAR_FUNC_NAME[func];
                return true;
            }
            else
            {
                return false;
            }
        }

        bool function_name(int func, core::string<wchar_t>& name)
        {
            if ((func < FUNCTION_COUNT) &&
                (func >= 0))
            {
                name = WCHAR_T_FUNC_NAME[func];
                return true;
            }
            else
            {
                return false;
            }
        }

    }



}

#endif // FUNCTIONS_H_INCLUDED
