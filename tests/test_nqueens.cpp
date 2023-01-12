/***************************************************************************
 *
 *            tests/test_nqueens.cpp
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



#include <set>
#include <ctime>
#include <chrono>
#include <cassert>
#include <unordered_map>

#include "bdd.hpp"
#include "test.hpp"

static int N;

int counting_solution(int n)
{
    N = n;

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
        for (int col=0 ; col < N ; col++)
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
                if (ll >= 0 && ll < N)
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
        vars.insert((unsigned int) i);
    }

    return queen.count_sat(vars);
}

void nqueens_solution_1()
{
    HERMESBDD_PRINT_TEST_COMMENT("Testing N-queen problem using a 1x1 chessboard.");

    int s = counting_solution(1);

    HERMESBDD_TEST_ASSERT(s == 1);
}

void nqueens_solution_2()
{
    HERMESBDD_PRINT_TEST_COMMENT("Testing N-queen problem using a 2x2 chessboard.");

    int s = counting_solution(2);

    HERMESBDD_TEST_ASSERT(s == 0);
}

void nqueens_solution_4()
{
    HERMESBDD_PRINT_TEST_COMMENT("Testing N-queen problem using a 4x4 chessboard.");

    int s = counting_solution(4);

    HERMESBDD_TEST_ASSERT(s == 2);
}

void nqueens_solution_5()
{
    HERMESBDD_PRINT_TEST_COMMENT("Testing N-queen problem using a 5x5 chessboard.");

    int s = counting_solution(5);

    HERMESBDD_TEST_ASSERT(s == 10);
}

void nqueens_solution_6()
{
    HERMESBDD_PRINT_TEST_COMMENT("Testing N-queen problem using a 6x6 chessboard.");

    int s = counting_solution(6);

    HERMESBDD_TEST_ASSERT(s == 4);
}

void nqueens_solution_7()
{
    HERMESBDD_PRINT_TEST_COMMENT("Testing N-queen problem using a 7x7 chessboard.");

    int s = counting_solution(7);

    HERMESBDD_TEST_ASSERT(s == 40);
}


int main()
{
    nqueens_solution_1();
    nqueens_solution_2();
    nqueens_solution_4();
    nqueens_solution_5();
    nqueens_solution_6();
    nqueens_solution_7();

    return HERMESBDD_TEST_FAILURES;
}
