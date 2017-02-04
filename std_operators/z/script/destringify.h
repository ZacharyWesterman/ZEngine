/**
 * File:            destringify.h
 * Namespace:       z::script
 * Description:     A template function to remove the
 *                  quotes on a script string, if there
 *                  are any. This is primarily used for
 *                  printing text and condensing strings.
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   3 Feb. 2017
**/

#pragma once
#ifndef DESTRINGIFY_H_INCLUDED
#define DESTRINGIFY_H_INCLUDED

#include "../core/string.h"
#include "escape_sequences.h"

namespace z
{
    namespace script
    {
        template <typename CHAR>
        core::string<CHAR> destringify(const core::string<CHAR>& input)
        {
            core::string<CHAR> text = core::remove_whitespace(input);

            core::string<CHAR> return_string;

            //define quote character and quote escape sequence
            core::string<CHAR> quote = (CHAR)34;

            core::string<CHAR> quote_esc;
            esc_sequence_name(ESC_SEQUENCE::QUOTE, quote_esc);


            bool lquote = text.beginsWith(quote);

            bool rquote = (text.foundAt(quote, text.length()-quote.length()) &&
                           !text.foundAt(quote_esc, text.length()-quote_esc.length()));


            int beg_index = 0;
            int end_index = text.length()-1;

            if (lquote)
                beg_index += quote.length();

            if (rquote)
                end_index -= quote.length();

            //if there is actually anything in the string
            //if (lquote < rquote-1)
                return_string = text.substr(beg_index, end_index);

            return return_string;
        }
    }
}

#endif // DESTRINGIFY_H_INCLUDED
