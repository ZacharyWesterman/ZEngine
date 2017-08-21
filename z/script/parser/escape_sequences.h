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

#include <z/core/string.h>

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
                NULLSYM,

                ESC_SEQ_COUNT
            };

            static const core::string<char> SEQ_NAMES[] =
            {
                "",//NONE
                "\\\"",//QUOTE (\")
                "\\n",//NEWLINE (\n)
                "\\t",//TAB (\t)
                "\\\\",//BACKSLASH (\)
                "\\0",//NULL SYMBOL (\0)
            };

            /*const core::string<wchar_t> WCHAR_T_SEQ_NAMES[] =
            {
                L"",//NONE
                L"\\\"",//QUOTE (\")
                L"\\n",//NEWLINE (\n)
                L"\\t",//TAB (\t)
                L"\\\\",//BACKSLASH (\)
            };*/

            static const core::string<char> SEQ_EQUIV[] =
            {
                "",//NONE
                "\"",//QUOTE (\")
                "\r\n",//NEWLINE (\n)
                "\t",//TAB (\t)
                "\\",//BACKSLASH (\)
                "", //NULL SYMBOL (\0)
            };

            /*const core::string<wchar_t> WCHAR_T_SEQ_EQUIV[] =
            {
                L"",//NONE
                L"\"",//QUOTE (\")
                L"\r\n",//NEWLINE (\n)
                L"\t",//TAB (\t)
                L"\\",//BACKSLASH (\)
            };*/
        }

        template <typename CHAR>
        void esc_sequence_name(uint8_t escseq, core::string<CHAR>& output)
        {
            if (escseq < ESC_SEQUENCE::ESC_SEQ_COUNT)
                output = ESC_SEQUENCE::SEQ_NAMES[escseq];
            else
                output = ESC_SEQUENCE::SEQ_NAMES[0];
        }

        template <typename CHAR>
        void esc_sequence_equiv(uint8_t escseq, core::string<CHAR>& output)
        {
            if (escseq < ESC_SEQUENCE::ESC_SEQ_COUNT)
                output = ESC_SEQUENCE::SEQ_EQUIV[escseq];
            else
                output = ESC_SEQUENCE::SEQ_EQUIV[0];
        }

        template <typename CHAR>
        uint8_t what_esc_sequence(const core::string<CHAR>& escseq, int position)
        {
            for (uint8_t i=ESC_SEQUENCE::ESC_SEQ_COUNT-1; i>0; i--)
            {
                if (escseq.foundAt(ESC_SEQUENCE::SEQ_NAMES[i], position))
                    return i;
            }

            return ESC_SEQUENCE::NONE;
        }

        template <typename CHAR>
        uint8_t what_esc_sequence(const core::string<CHAR>& escseq)
        {
            for (uint8_t i=ESC_SEQUENCE::ESC_SEQ_COUNT-1; i>0; i--)
            {
                if (escseq == ESC_SEQUENCE::SEQ_NAMES[i])
                    return i;
            }

            return ESC_SEQUENCE::NONE;
        }



        /*
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
        }*/
    }
}

#endif // ESCAPE_SEQUENCES_H_INCLUDED
