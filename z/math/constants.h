/**
 * File:            constants.h
 * Namespace:       z::math
 *
 * Description:     Some global mathematical constants.
 *                  IEEE doubles only have ~15 decimal
 *                  place precision, but to avoid
 *                  rounding errors I have made these
 *                  constants as accurate as I
 *                  reasonably can.
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   3 Mar. 2017
**/

#pragma once
#ifndef CONSTANTS_H_INCLUDED
#define CONSTANTS_H_INCLUDED

namespace z
{
    namespace math
    {
        //pi to the 31st decimal place.
        const double pi     = 3.1415926535897932384626433832795;

        //e to the 31st decimal place.
        const double e      = 2.7182818284590452353602874713527;

        //the square root of 2 to the 65th decimal place
        const double sqrt2  = 1.41421356237309504880168872420969807856967187537694807317667973799;
    }
}

#endif // CONSTANTS_H_INCLUDED
