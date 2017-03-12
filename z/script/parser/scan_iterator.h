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

#include "scanner.h"

namespace z
{
    namespace script
    {
        class scan_iterator
        {

        };
    }
}

#endif // SCAN_ITERATOR_H_INCLUDED
