/**
 * File:            scan_iterator.h
 * Namespace:       z::script
 * Description:     A controller for the scanner that
 *                  replaces "include" statements with
 *                  the script in the specified file.
 *                  This process is performed in increments
 *                  so that scripts can be compiled during
 *                  runtime without making the program
 *                  unresponsive.
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   11 Mar. 2017
**/

#pragma once
#ifndef SCAN_ITERATOR_H_INCLUDED
#define SCAN_ITERATOR_H_INCLUDED

#include <z/core/array.h>
#include <z/file/loadFileToMemory.h>

#include "scanner.h"

namespace z
{
    namespace script
    {
        template <typename CHAR>
        class scan_iterator
        {
        private:
            scanner<CHAR>* _scanner;

            bool found_error;

        public:
            core::array< ident_t<CHAR> > identifiers;


            scan_iterator(core::sorted_array< core::string<CHAR> >* opers = NULL,
                          core::sorted_array< core::string<CHAR> >* cmds = NULL,
                          core::sorted_array< core::string<CHAR> >* funcs = NULL)
            {
                _scanner = new scanner<CHAR>(opers, cmds, funcs);

                found_error = false;
            }

            ~scan_iterator()
            {
                delete _scanner;
            }


            void set(const core::string<CHAR>&);


            bool scan();

            bool error();
        };


        ///set the input string for the scan iterator
        ///and scan that input (does not replace includes).
        template <typename CHAR>
        void scan_iterator<CHAR>::set(const core::string<CHAR>& input)
        {
            identifiers.clear();

            found_error = _scanner->scan(input);
            found_error |= _scanner->clean();

            identifiers = _scanner->identifiers;
        }


        ///template for iterator scanner. Scans the loaded
        ///input and replaces all includes in the script
        ///with the contents of the referenced file.
        //returns false if done scanning or unable to load
        //an included file, true otherwise.
        template <typename CHAR>
        bool scan_iterator<CHAR>::scan()
        {
            for (int i=0; i<identifiers.size(); i++)
                if (identifiers[i].type == ident::KEYWORD_INCLUDE)
                {
                    //include directories must exist and be a string literal.
                    if (identifiers.is_valid(i+1) &&
                        identifiers[i+1].type == ident::STRING_LITERAL)
                    {
                        core::string<CHAR> include_script;

                        //if the include file exists, load it.
                        if (false && file::loadFileToMemory(identifiers[i+1].name, include_script))
                        {

                        }
                        else
                        {

                        }

                    }
                    else
                    {
                        found_error = true;

                        identifiers[i].err = error::INVALID_INCLUDE;
                    }

                }

            return false;
        }
    }
}

#endif // SCAN_ITERATOR_H_INCLUDED
