/***************************************************************************
 *
 *            profile/profile_replaceme.cpp
 *
 ****************************************************************************/

/*
 * MIT License
 *
 * Copyright (c) 2023 Luigi Capogrosso, Luca Geretti, Marco cristani, Franco Fummi, and Tiziano Villa.
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


#include <cassert>
#include <iostream>

#include "bdd.hpp"
#include "profile.hpp"

struct ProfileITE : public Profiler
{
    ProfileITE() : Profiler(1e5) { }

    void run()
    {
        profile_not();

        profile_and();
        profile_and_eq();

        profile_or();
        profile_or_eq();

        profile_xor();
        profile_xor_eq();

        profile_implies();
        profile_implies_eq();

        profile_reverse_implies();
        profile_reverse_implies_eq();
    }

    void profile_not()
    {
        int num_samples = 10000;

        profile_on_average("Profiling NOT operator",
        [&](uint32_t i)
            {
                BDD a(i);
                BDD b(i + 1);

                BDD c = !a;
                std::unordered_map<uint32_t, bool> map = c.one_sat();
            },
        num_samples);
    }

    void profile_and()
    {
        int num_samples = 10000;

        profile_on_average("Profiling AND operator",
        [&](uint32_t i)
            {
                BDD a(i);
                BDD b(i + 1);

                BDD c = a & !b;
                std::unordered_map<uint32_t, bool> map = c.one_sat();
            },
        num_samples);
    }

    void profile_and_eq()
    {
        int num_samples = 10000;

        profile_on_average("Profiling AND EQUAL operator",
        [&](uint32_t i)
            {
                BDD a(i);
                BDD b(i + 1);

                BDD c = a &= b;
                std::unordered_map<uint32_t, bool> map = c.one_sat();
            },
        num_samples);
    }

    void profile_or()
    {
        int num_samples = 10000;

        profile_on_average("Profiling OR operator",
        [&](uint32_t i)
            {
                BDD a(i);
                BDD b(i + 1);

                BDD c = a | b;
                std::unordered_map<uint32_t, bool> map = c.one_sat();
            },
        num_samples);
    }

    void profile_or_eq()
    {
        int num_samples = 10000;

        profile_on_average("Profiling OR EQUAL operator",
        [&](uint32_t i)
            {
                BDD a(i);
                BDD b(i + 1);

                BDD c = a |= b;
                std::unordered_map<uint32_t, bool> map = c.one_sat();
            },
        num_samples);
    }

    void profile_xor()
    {
        int num_samples = 10000;

        profile_on_average("Profiling XOR operator",
        [&](uint32_t i)
            {
                BDD a(i);
                BDD b(i + 1);

                BDD c = a ^ b;
                std::unordered_map<uint32_t, bool> map = c.one_sat();
            },
        num_samples);
    }

    void profile_xor_eq()
    {
        int num_samples = 10000;

        profile_on_average("Profiling XOR EQUAL operator",
        [&](uint32_t i)
            {
                BDD a(i);
                BDD b(i + 1);

                BDD c = a ^= b;
                std::unordered_map<uint32_t, bool> map = c.one_sat();
            },
        num_samples);
    }

    void profile_implies()
    {
        int num_samples = 10000;

        profile_on_average("Profiling IMPLIES operator",
        [&](uint32_t i)
            {
                BDD a(i);
                BDD b(i + 1);

                BDD c = a > b;
                std::unordered_map<uint32_t, bool> map = c.one_sat();
            },
        num_samples);
    }

    void profile_implies_eq()
    {
        int num_samples = 10000;

        profile_on_average("Profiling IMPLIES EQUAL operator",
        [&](uint32_t i)
            {
                BDD a(i);
                BDD b(i + 1);

                BDD c = a >= b;
                std::unordered_map<uint32_t, bool> map = c.one_sat();
            },
        num_samples);
    }

    void profile_reverse_implies()
    {
        int num_samples = 10000;

        profile_on_average("Profiling reverse IMPLIES operator",
        [&](uint32_t i)
            {
                BDD a(i);
                BDD b(i + 1);

                BDD c = a < b;
                std::unordered_map<uint32_t, bool> map = c.one_sat();
            },
        num_samples);
    }

    void profile_reverse_implies_eq()
    {
        int num_samples = 10000;

        profile_on_average("Profiling reverse IMPLIES EQUAL operator",
        [&](uint32_t i)
            {
                BDD a(i);
                BDD b(i + 1);

                BDD c = a <= b;
                std::unordered_map<uint32_t, bool> map = c.one_sat();
            },
        num_samples);
    }
};


int main()
{
    ProfileITE().run();
}
