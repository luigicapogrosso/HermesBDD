/***************************************************************************
 *
 *            profile/profile.hpp
 *
 ****************************************************************************/

/*
 * MIT License
 *
 * Copyright (c) 2022 Luigi Capogrosso, Luca Geretti, Marco cristani, Franco Fummi, and Tiziano Villa.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */



#ifndef PROFILE_HPP
#define PROFILE_HPP


#include <iomanip>
#include <functional>

#include "stopwatch.hpp"

struct Randomiser
{
    static double get(double min, double max)
    {
        // TODO: rand() has limited randomness; use C++11 random library.
        return ((max - min) * rand() / RAND_MAX + min);
    }
};

inline bool _init_randomiser()
{
    srand(time(nullptr));

    return true;
}

static const bool init_randomiser = _init_randomiser();

class Profiler
{
public:
    Profiler(int num_tries) : _num_tries(num_tries) { }

    int num_tries() const { return _num_tries; }

    [[nodiscard]] Randomiser const& rnd() const { return _rnd; }

    void profile_on_average(std::string msg,
                 std::function<void(int)> function,
                 int num_tries)
    {
        _ussw.restart();
        for (int i = 0; i < num_tries; ++i) function(i);
        _ussw.click();
        std::cout << msg << " completed in "
                  << ((double)_ussw.duration().count())/num_tries*1000
                  << " us on average" << std::endl;
    }

    void profile_on_total(std::string msg,
                 std::function<void(int)> function,
                 int num_tries)
    {
        _mssw.restart();
        for (int i = 0; i < num_tries; ++i) function(i);
        _mssw.click();
        std::cout << msg << " completed in "
                  << ((double)_mssw.duration().count())
                  << " ms on total" << std::endl;
    }

    void profile(std::string msg, std::function<void(int)> function)
    {
        profile_on_average(msg, function, _num_tries);
    }

private:
    Stopwatch<Microseconds> _ussw;
    Stopwatch<Milliseconds> _mssw;
    Randomiser _rnd;
    const int _num_tries;
};

#endif // PROFILE_HPP
