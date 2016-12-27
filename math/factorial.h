#pragma once
#ifndef FACTORIAL_H_INCLUDED
#define FACTORIAL_H_INCLUDED

namespace math
{
    long int factorial(int n)
    {
        long int output = 1;

        for (long int i=2; i<=(long int)n; i++)
        {
            output *= i;
        }

        return output;
    }
}

#endif // FACTORIAL_H_INCLUDED
