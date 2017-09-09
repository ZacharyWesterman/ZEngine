/**
 * File:            escapeSequences.h
 * Namespace:       z::script
 * Description:     The script escape sequences are defined here.
 *                  Currently, the only allowed escape sequences
 *                  are the double quote, tab, newline, and backslash.
 *                  ( \", \t, \n, and \\ respectively)
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   20 Aug. 2017
**/

#pragma once
#ifndef ESCAPESEQUENCES_H_INCLUDED
#define ESCAPESEQUENCES_H_INCLUDED

#include <z/core/string.h>

namespace z
{
namespace script
{
    namespace compiler
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

            static const core::string<char> SEQ_EQUIV[] =
            {
                "",//NONE
                "\"",//QUOTE (\")
                "\r\n",//NEWLINE (\n)
                "\t",//TAB (\t)
                "\\",//BACKSLASH (\)
                "", //NULL SYMBOL (\0)
            };
        }

        template <typename CHAR>
        void escSequenceName(int escseq, core::string<CHAR>& output)
        {
            if (escseq < ESC_SEQUENCE::ESC_SEQ_COUNT)
                output = ESC_SEQUENCE::SEQ_NAMES[escseq];
            else
                output = ESC_SEQUENCE::SEQ_NAMES[0];
        }

        template <typename CHAR>
        void escSequenceEquiv(int escseq, core::string<CHAR>& output)
        {
            if (escseq < ESC_SEQUENCE::ESC_SEQ_COUNT)
                output = ESC_SEQUENCE::SEQ_EQUIV[escseq];
            else
                output = ESC_SEQUENCE::SEQ_EQUIV[0];
        }

        template <typename CHAR>
        int whatEscSequence(const core::string<CHAR>& escseq, int position)
        {
            for (uint8_t i=ESC_SEQUENCE::ESC_SEQ_COUNT-1; i>0; i--)
            {
                if (escseq.foundAt(ESC_SEQUENCE::SEQ_NAMES[i], position))
                    return i;
            }

            return ESC_SEQUENCE::NONE;
        }

        template <typename CHAR>
        int whatEscSequence(const core::string<CHAR>& escseq)
        {
            for (uint8_t i=ESC_SEQUENCE::ESC_SEQ_COUNT-1; i>0; i--)
            {
                if (escseq == ESC_SEQUENCE::SEQ_NAMES[i])
                    return i;
            }

            return ESC_SEQUENCE::NONE;
        }

    }
}
}

#endif // ESCAPESEQUENCES_H_INCLUDED
