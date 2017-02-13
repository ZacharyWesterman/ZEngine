/**
 * File:            load_operators.h
 * Namespace:       z::script
 * Description:     Functions for loading operator libraries
 *                  Note that these libraries will never be
 *                  unloaded because the program may call
 *                  methods in them at any time.
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   5 Feb. 2017
**/

#pragma once
#ifndef LOAD_OPERATORS_H_INCLUDED
#define LOAD_OPERATORS_H_INCLUDED

#include "operators/oper_t.h"

#include "../file/list_files_in_dir.h"

#include "../core/string.h"
#include "../core/sorted_array.h"
#include "../core/loadLibrary.h"


#ifdef __linux__
    #error "load_operators.h" is not compatible with the target OS.
#elif _WIN32

namespace z
{
    namespace script
    {

        ///Load operators with narrow character
        bool load_oper_lib(core::sorted_array< script::oper::oper_t<char>* >* output,
                           const core::string<char>& file_name)
        {
            typedef void (__stdcall *_func) (core::sorted_array< script::oper::oper_t<char>* >*);

            core::library::lib_t lib = NULL;
            lib = core::library::load(file_name.str());


            if (!lib)
            {
                return false;
            }
            else
            {
                _func get_opers = (_func)core::library::get_function(lib, "get_opers_c");

                if (!get_opers)
                {
                    return false;
                }
                else
                {
                    get_opers(output);
                    return true;
                }
            }
        }


        ///Load operators with wide characters
        bool load_oper_lib(core::sorted_array< script::oper::oper_t<wchar_t>* >* output,
                           const core::string<char>& file_name)
        {
            typedef void (__stdcall *_func) (core::sorted_array< script::oper::oper_t<wchar_t>* >*);

            core::library::lib_t lib = NULL;
            lib = core::library::load(file_name.str());


            if (!lib)
            {
                return false;
            }
            else
            {
                _func get_opers = (_func)core::library::get_function(lib, "get_opers_w");

                if (!get_opers)
                {
                    return false;
                }
                else
                {
                    get_opers(output);
                    return true;
                }
            }
        }


        ///Load all of the libraries in the "./operators/" directory (narrow chars)
        void load_operators(core::sorted_array< script::oper::oper_t<char>* >* output,
                            const core::string<char>& folder_name)
        {
            core::array< core::string<char> > files;

            file::list_files_in_dir(folder_name, "dll", files);

            for(int i=0; i<files.size(); i++)
            {
                core::string<char> file_name = folder_name;
                file_name += '/';
                file_name += files[i];

                load_oper_lib(output, file_name);
            }
        }

        ///Load all of the libraries in the "./operators/" directory (wide chars)
        void load_operators(core::sorted_array< script::oper::oper_t<wchar_t>* >* output,
                            const core::string<char>& folder_name)
        {
            core::array< core::string<char> > files;

            file::list_files_in_dir(folder_name, "dll", files);

            for(int i=0; i<files.size(); i++)
            {
                core::string<char> file_name = folder_name;
                file_name += '/';
                file_name += files[i];

                load_oper_lib(output, file_name);
            }
        }
    }
}
#else
    #error "load_operators.h" is not compatible with the target OS.
#endif

#endif // LOAD_OPERATORS_H_INCLUDED
