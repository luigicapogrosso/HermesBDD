/***************************************************************************
 *
 *            profile/profile_replaceme.cpp
 *
 ****************************************************************************/

/*
 * MIT License
 *
 * Copyright (c) 2023 Luigi Capogrosso, Luca Geretti,
 *                    Marco cristani, Franco Fummi, and Tiziano Villa.
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


#include <cmath>
#include <cassert>
#include <iostream>

#include "bdd.hpp"
#include "profile.hpp"

// Global static for board size, used by counting_solution.
static int N;

long long counting_solution(int n_board_size)
{
    N = n_board_size;

    BDD **X;
    BDD queen = BDD::bdd_true;

    // Build variable array.
    X = new BDD*[static_cast<unsigned long>(N)];
    for (int k = 0; k < N; k++)
    {
        X[k] = new BDD[static_cast<unsigned long>(N)];
    }

    for (int row = 0; row < N; row++)
    {
        for (int col = 0 ; col < N ; col++)
        {
            X[row][col] = BDD((uint32_t)(row * N + col));
        }
    }

    // Place a queen in each row.
    for (int row = 0; row < N; row++)
    {
        BDD e = BDD::bdd_false;
        for (int col = 0; col < N; col++)
        {
            e |= X[row][col];
        }
        queen &= e;
    }

    // Build requirements for each variable (field).
    for (int row = 0; row < N; row++)
    {
        for (int col = 0; col < N; col++)
        {
            BDD a = BDD::bdd_true;
            for (int x = 0 ; x < N ; x++)
            {
                if (x != col)
                {
                    a &= (X[row][col] > !X[row][x]);
                }
            }

            BDD b = BDD::bdd_true;
            for (int y = 0; y < N; y++)
            {
                if (y != row)
                {
                    b &= (X[row][col] > !X[y][col]);
                }
            }

            BDD c = BDD::bdd_true;
            for (int k = 0 ; k < N ; k++)
            {
                int ll = k - row + col;
                if ((ll >= 0) && (ll < N))
                {
                    if (k != row)
                    {
                        c &= (X[row][col] > !X[k][ll]);
                    }
                }
            }

            BDD d = BDD::bdd_true;
            for (int k = 0; k < N; k++)
            {
                int ll = row + col - k;
                if (ll>=0 && ll<N)
                {
                    if (k != row)
                    {
                        d &= (X[row][col] > !X[k][ll]);
                    }
                }
            }

            queen &= (a & b & c & d);
        }
    }

    std::set<uint32_t> vars;
    for (int i = 0; i < N * N; i++)
    {
        vars.insert((unsigned int)i);
    }

    return static_cast<long long>(std::round(queen.count_sat(vars)));
}

struct ProfileNQueens : public Profiler
{
    ProfileNQueens() : Profiler(1e5) { }

    void run()
    {
        profile_nqueens_7x7();
        profile_nqueens_8x8();
    }

    void profile_nqueens_7x7()
    {
        int num_samples = 50;

        profile_on_total("Profiling N-queens (7x7) problem",
        [&](auto)
            {
                counting_solution(7);
            },
        num_samples);
    }

    void profile_nqueens_8x8()
    {
        int num_samples = 50;

        profile_on_total("Profiling N-queens (8x8) problem",
        [&](auto)
            {
                counting_solution(8);
            },
        num_samples);
    }
};


int main()
{
    ProfileNQueens().run();
}
