#ifndef TIMER_H_INCLUDED
#define TIMER_H_INCLUDED

#include <chrono>

namespace core
{
    class timer
    {
        typedef std::chrono::high_resolution_clock high_resolution_clock;
        typedef std::chrono::milliseconds milliseconds;

    private:
        high_resolution_clock::time_point start;

    public:
        explicit timer (bool run = false)
        {
            if (run)
                reset();
        }

        void reset()
        {
            start = high_resolution_clock::now();
        }


        double elapsed() const
        {
            return (double)(std::chrono::duration_cast<milliseconds>
                    (high_resolution_clock::now() - start)).count();
        }
    };
}


#endif // TIMER_H_INCLUDED
