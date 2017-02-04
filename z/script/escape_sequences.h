/**
 * File:            escape_sequences.h
 * Namespace:       z::script
 * Description:     The script escape sequences are defined here.
 *                  Currently, the only allowed escape sequences
 *                  are the double quote, tab, newline, and backslash.
 *                  ( \", \t, \n, and \\ respectively)
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   3 Feb. 2017
**/

#pragma once
#ifndef ESCAPE_SEQUENCES_H_INCLUDED
#define ESCAPE_SEQUENCES_H_INCLUDED

#include <stdint.h>

#include "../core/string.h"

namespace z
{
    namespace script
    {
        namespace ESC_SEQUENCE
        {
            enum SEQUENCE
            {
                NONE = 0,
                QUOTE,
                NEWLINE,
                TAB,
                BACKSLASH,

                ESC_SEQ_COUNT
            };

            const core::string<char> CHAR_SEQ_NAMES[] =
            {
                "",//NONE
                "\\\"",//QUOTE (\")
                "\\n",//NEWLINE (\n)
                "\\t",//TAB (\t)
                "\\\\",//BACKSLASH (\)
            };

            const core::string<wchar_t> WCHAR_T_SEQ_NAMES[] =
            {
                L"",//NONE
                L"\\\"",//QUOTE (\")
                L"\\n",//NEWLINE (\n)
                L"\\t",//TAB (\t)
                L"\\\\",//BACKSLASH (\)
            };

            const core::string<char> CHAR_SEQ_EQUIV[] =
            {
                "",//NONE
                "\"",//QUOTE (\")
                "\n",//NEWLINE (\n)
                "\t",//TAB (\t)
                "\\",//BACKSLASH (\)
            };

            const core::string<wchar_t> WCHAR_T_SEQ_EQUIV[] =
            {
                L"",//NONE
                L"\"",//QUOTE (\")
                L"\n",//NEWLINE (\n)
                L"\t",//TAB (\t)
                L"\\",//BACKSLASH (\)
            };
        }

        void esc_sequence_name(uint8_t escseq, core::string<char>& output)
        {
            if (escseq < ESC_SEQUENCE::ESC_SEQ_COUNT)
                output = ESC_SEQUENCE::CHAR_SEQ_NAMES[escseq];
            else
                output = ESC_SEQUENCE::CHAR_SEQ_NAMES[0];
        }

        void esc_sequence_equiv(uint8_t escseq, core::string<char>& output)
        {
            if (escseq < ESC_SEQUENCE::ESC_SEQ_COUNT)
                output = ESC_SEQUENCE::CHAR_SEQ_EQUIV[escseq];
            else
                output = ESC_SEQUENCE::CHAR_SEQ_EQUIV[0];
        }

        uint8_t what_esc_sequence(const core::string<char>& escseq, int position)
        {
            for (uint8_t i=ESC_SEQUENCE::ESC_SEQ_COUNT-1; i>0; i--)
            {
                if (escseq.foundAt(ESC_SEQUENCE::CHAR_SEQ_NAMES[i], position))
                    return i;
            }

            return ESC_SEQUENCE::NONE;
        }

        uint8_t what_esc_sequence(const core::string<char>& escseq)
        {
            for (uint8_t i=ESC_SEQUENCE::ESC_SEQ_COUNT-1; i>0; i--)
            {
                if (escseq == ESC_SEQUENCE::CHAR_SEQ_NAMES[i])
                    return i;
            }

            return ESC_SEQUENCE::NONE;
        }



        void esc_sequence_name(uint8_t escseq, core::string<wchar_t>& output)
        {
            if (escseq < ESC_SEQUENCE::ESC_SEQ_COUNT)
                output = ESC_SEQUENCE::WCHAR_T_SEQ_NAMES[escseq];
            else
                output = ESC_SEQUENCE::WCHAR_T_SEQ_NAMES[0];
        }

        void esc_sequence_equiv(uint8_t escseq, core::string<wchar_t>& output)
        {
            if (escseq < ESC_SEQUENCE::ESC_SEQ_COUNT)
                output = ESC_SEQUENCE::WCHAR_T_SEQ_EQUIV[escseq];
            else
                output = ESC_SEQUENCE::WCHAR_T_SEQ_EQUIV[0];
        }

        uint8_t what_esc_sequence(const core::string<wchar_t>& escseq, int position)
        {
            for (uint8_t i=ESC_SEQUENCE::ESC_SEQ_COUNT-1; i>0; i--)
            {
                if (escseq.foundAt(ESC_SEQUENCE::WCHAR_T_SEQ_NAMES[i], position))
                    return i;
            }

            return ESC_SEQUENCE::NONE;
        }

        uint8_t what_esc_sequence(const core::string<wchar_t>& escseq)
        {
            for (uint8_t i=ESC_SEQUENCE::ESC_SEQ_COUNT-1; i>0; i--)
            {
                if (escseq == ESC_SEQUENCE::WCHAR_T_SEQ_NAMES[i])
                    return i;
            }

            return ESC_SEQUENCE::NONE;
        }
    }
}

#endif // ESCAPE_SEQUENCES_H_INCLUDED
