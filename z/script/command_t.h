/**
 * File:            command_t.h
 * Namespace:       z::script
 *
 * Description:     The base class for all script commands.
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   2 Aug. 2017
**/

#pragma once
#ifndef COMMAND_T_H_INCLUDED
#define COMMAND_T_H_INCLUDED

#include <z/core/string.h>
#include <z/core/array.h>
#include <z/core/timeout.h>

#include "data_t.h"

namespace z
{
    namespace script
    {

        template <typename CHAR>
        class command_t
        {
        private:
            void* graphics_engine;

            core::array< core::string<CHAR> > cmd_name;

            int params_min;
            int params_max;

        protected:
            core::array< data_t<CHAR> > params;

        public:
            command_t(const core::array< core::string<CHAR> >& _name,
                      int min_params = 0,
                      int max_params = 0,
                      void* _engine = NULL)
            {
                cmd_name = _name;

                if (max_params < min_params)
                    min_params = max_params;

                params_min = min_params;
                params_max = max_params;

                graphics_engine = _engine;
            }

            virtual ~command_t() {}


            virtual error_flag addParam(const data_t<CHAR>& next_param)
            {
                params.add(next_param);

                return error::NONE;
            }

            //call this script function (with timeout).
            //should return true if finished, false otherwise
            virtual bool call(const core::timeout&) = 0;


            inline void setEngine(void* _engine)
            {
                graphics_engine = _engine;
            }

            inline void clear()
            {
                params.clear();
            }


            inline const core::array< core::string<CHAR> >& name() const
            { return cmd_name; }

            inline const int maxParams() const
            { return params_max; }

            inline const int minParams() const
            { return params_min; }


            error_flag paramCountError() const
            {
                if (params_max < 0)
                    return error::NONE;

                if (params.size() > params_max)
                    return error::TOO_MANY_PARAMS;
                else if (params.size() < params_min)
                    return error::TOO_FEW_PARAMS;
                else
                    return error::NONE;
            }


            bool operator==(const command_t<CHAR>& other) const
            {
                return (params_max == other.params_max) &&
                       (params_min == other.params_min) &&
                       (cmd_name == other.cmd_name);
            }

            inline bool operator!=(const command_t<CHAR>& other) const
            { return !operator==(other); }

            bool operator>(const command_t<CHAR>& other) const
            {
                return (cmd_name > other.cmd_name) ||
                       (params_max > other.params_max) ||
                       (params_min > other.params_min);
            }

            inline bool operator<=(const command_t<CHAR>& other) const
            { return !operator>(other); }

            bool operator<(const command_t<CHAR>& other) const
            {
                return (cmd_name < other.cmd_name) ||
                       (params_max < other.params_max) ||
                       (params_min < other.params_min);
            }

            inline bool operator>=(const command_t<CHAR>& other) const
            { return !operator<(other); }
        };
    }
}

#endif // COMMAND_T_H_INCLUDED
