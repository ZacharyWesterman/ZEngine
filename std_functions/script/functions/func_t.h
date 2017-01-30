#pragma once
#ifndef FUNC_T_H_INCLUDED
#define FUNC_T_H_INCLUDED

#include "../../core/string.h"
#include "../../core/array.h"

#include "../script_errors.h"

namespace script
{
    namespace func
    {
        template <typename CHAR>
        class func_t
        {
        private:
            core::string<CHAR> name;

            int min_params;
            int max_params;

        public:
            func_t (const core::string<CHAR>& func_name,
                    int min_p, int max_p)
            {
                name = func_name;

                min_params = min_p;
                max_params = max_p;
            }


            virtual error_flag call(const core::array< core::string<CHAR> >& params,
                                    core::string<CHAR>& output)
            {
                return error::NONE;
            }
        };
    }
}

#endif // FUNC_T_H_INCLUDED
