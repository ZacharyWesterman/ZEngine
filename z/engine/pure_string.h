#pragma once
#ifndef PURE_STRING_H_INCLUDED
#define PURE_STRING_H_INCLUDED

#include "../script/escape_sequences.h"
#include "../script/destringify.h"


namespace engine
{
    ///this function removes the quotes from around a string and replaces
    template <typename CHAR>
    core::string<CHAR> pure_string(const core::string<CHAR>& text)
    {
        core::string<CHAR> this_string = script::destringify(text);


        for (int i=0; i<this_string.length(); i++)
        {
            uint8_t esc_seq = script::what_esc_sequence(this_string, i);

            if (esc_seq)
            {


                core::string<CHAR> sequence;
                core::string<CHAR> equivalent;

                script::esc_sequence_name(esc_seq, sequence);
                script::esc_sequence_equiv(esc_seq, equivalent);

                this_string.replace(i, i+sequence.length()-1, equivalent);

                i = i - sequence.length() + equivalent.length() + 1;
            }
        }

        return this_string;
    }
}

#endif // PURE_STRING_H_INCLUDED
