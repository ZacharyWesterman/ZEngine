#pragma once
#ifndef BASIC_DICTIONARY_H_INCLUDED
#define BASIC_DICTIONARY_H_INCLUDED

#include "../core/string.h"
#include "../core/array.h"
#include "../core/stringUtils.h"

#include "../file/loadFileToMemory.h"

namespace dict
{
    template<typename CHAR>
    class basic_dictionary
    {
    private:
        core::array< core::string<CHAR> > words_in_dict;

        bool is_loaded;

        bool load_dict_from_file(const core::string<char>&);

    public:
        basic_dictionary() {is_loaded = false;}

        basic_dictionary(const core::string<char>& dict_file)
        { load_dict_from_file(dict_file); }

        bool load(const core::string<char>& dict_file)
        { return load_dict_from_file(dict_file); }


        const core::array< core::string<CHAR> >& list()
        { return words_in_dict; }

        bool good() { return is_loaded; }
    };

    template <typename CHAR>
    bool basic_dictionary<CHAR>::load_dict_from_file(const core::string<char>& dict_file)
    {
        core::string<CHAR> filedata;

        is_loaded = file::loadFileToMemory(dict_file, filedata);

        if (is_loaded)
        {
            core::tokenize((CHAR)10, filedata, words_in_dict); //tokenize with newline character

            for (uint i=0; i<words_in_dict.size(); i++)
                words_in_dict[i] = core::remove_whitespace(words_in_dict[i]);
        }


        return is_loaded;
    }
}

#endif // BASIC_DICTIONARY_H_INCLUDED
