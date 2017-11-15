/**
 * File:            function.h
 * Namespace:       z::script
 *
 * Description:     The base class for all script functions.
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   20 Aug. 2017
**/

#pragma once
#ifndef FUNCTION_H_INCLUDED
#define FUNCTION_H_INCLUDED

#include <z/core/string.h>
#include <z/core/array.h>
#include <z/core/timeout.h>

#include "generic.h"


namespace z
{
    namespace script
    {
        class function
        {
        private:
            void* graphics_engine;
            void* sound_engine;

            bool graphics_required;
            bool sound_required;

            core::string<char> func_name;

            int params_min;
            int params_max;

            bool is_constant;

        public:

            function(core::string<char> _name,
                       bool _constant = false,
                       int min_params = -1,
                       int max_params = -1,
                       void* _graphics_engine = NULL,
                       void* _sound_engine = NULL,
                       bool requires_graphics = false,
                       bool requires_sound = false)
            {
                func_name = _name;

                if (max_params < min_params)
                    min_params = max_params;

                params_min = min_params;
                params_max = max_params;
                is_constant = _constant;

                graphics_engine = _graphics_engine;
                sound_engine = _sound_engine;

                graphics_required = requires_graphics;
                sound_required = requires_sound;
            }

            virtual ~function() {}

            //call this script function (with timeout).
            //should return true if finished, false otherwise
            virtual bool call(const core::timeout&,
                              const generic<CPL_CHAR>&,//input parameter(s)
                              const generic<CPL_CHAR>&,//output value
                              void*, //temporary memory pointer
                              core::array<error>*) const = 0;


            inline void setGraphicsEngine(void* _engine, bool required = true)
            {
                graphics_engine = _engine;
                graphics_required = required;
            }

            inline void setSoundEngine(void* _engine, bool required = true)
            {
                sound_engine = _engine;
                sound_required = required;
            }


            inline bool requiresGraphics() const
            { return graphics_required; }

            inline bool requiresSound() const
            { return sound_required; }


            inline void* graphicsEngine() const
            { return graphics_engine; }

            inline void* soundEngine() const
            { return sound_engine; }


            inline const core::string<char>& name() const
            { return func_name; }

            inline const int maxParams() const
            { return params_max; }

            inline const int minParams() const
            { return params_min; }

            inline const bool constant() const
            { return is_constant; }


            bool operator==(const function& other) const
            {
                return (params_max == other.params_max) &&
                       (params_min == other.params_min) &&
                       (func_name == other.func_name);
            }

            inline bool operator!=(const function& other) const
            { return !operator==(other); }

            bool operator>(const function& other) const
            {
                return (func_name > other.func_name) ||
                       (params_max > other.params_max) ||
                       (params_min > other.params_min);
            }

            inline bool operator<=(const function& other) const
            { return !operator>(other); }

            bool operator<(const function& other) const
            {
                return (func_name < other.func_name) ||
                       (params_max < other.params_max) ||
                       (params_min < other.params_min);
            }

            inline bool operator>=(const function& other) const
            { return !operator<(other); }
        };


    }
}

#endif // FUNCTION_H_INCLUDED
