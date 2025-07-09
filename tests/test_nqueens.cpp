/***************************************************************************
 *
 *            tests/test_nqueens.cpp
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


#include <set>
#include <ctime>
#include <cmath>
#include <chrono>
#include <cassert>
#include <unordered_map>

#include "bdd.hpp"
#include "test.hpp"

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

    return static_cast<long long>(std::round(queen.count_sat(vars)));
}

void nqueens_solution_1()
{
    HERMESBDD_PRINT_TEST_COMMENT("Testing N-queen problem using a 1x1 chessboard.");
    long long s = counting_solution(1);
    HERMESBDD_TEST_ASSERT(s == 1LL);
}

void nqueens_solution_2()
{
    HERMESBDD_PRINT_TEST_COMMENT("Testing N-queen problem using a 2x2 chessboard.");
    long long s = counting_solution(2);
    HERMESBDD_TEST_ASSERT(s == 0LL);
}

void nqueens_solution_3()
{
    HERMESBDD_PRINT_TEST_COMMENT("Testing N-queen problem using a 3x3 chessboard.");
    long long s = counting_solution(3);
    HERMESBDD_TEST_ASSERT(s == 0LL);
}

void nqueens_solution_4()
{
    HERMESBDD_PRINT_TEST_COMMENT("Testing N-queen problem using a 4x4 chessboard.");
    long long s = counting_solution(4);
    HERMESBDD_TEST_ASSERT(s == 2LL);
}

void nqueens_solution_5()
{
    HERMESBDD_PRINT_TEST_COMMENT("Testing N-queen problem using a 5x5 chessboard.");
    long long s = counting_solution(5);
    HERMESBDD_TEST_ASSERT(s == 10LL);
}

void nqueens_solution_6()
{
    HERMESBDD_PRINT_TEST_COMMENT("Testing N-queen problem using a 6x6 chessboard.");
    long long s = counting_solution(6);
    HERMESBDD_TEST_ASSERT(s == 4LL);
}

void nqueens_solution_7()
{
    HERMESBDD_PRINT_TEST_COMMENT("Testing N-queen problem using a 7x7 chessboard.");
    long long s = counting_solution(7);
    HERMESBDD_TEST_ASSERT(s == 40LL);
}

void nqueens_solution_8()
{
    HERMESBDD_PRINT_TEST_COMMENT("Testing N-queen problem using a 8x8 chessboard.");
    long long s = counting_solution(8);
    HERMESBDD_TEST_ASSERT(s == 92LL);
}

void nqueens_solution_9()
{
    HERMESBDD_PRINT_TEST_COMMENT("Testing N-queen problem using a 9x9 chessboard.");
    long long s = counting_solution(9);
    HERMESBDD_TEST_ASSERT(s == 352LL);
}

void nqueens_solution_10()
{
    HERMESBDD_PRINT_TEST_COMMENT("Testing N-queen problem using a 10x10 chessboard.");
    long long s = counting_solution(10);
    HERMESBDD_TEST_ASSERT(s == 724LL);
}

void nqueens_solution_11()
{
    HERMESBDD_PRINT_TEST_COMMENT("Testing N-queen problem using a 11x11 chessboard.");
    long long s = counting_solution(11);
    HERMESBDD_TEST_ASSERT(s == 2680LL);
}

void nqueens_solution_12()
{
    HERMESBDD_PRINT_TEST_COMMENT("Testing N-queen problem using a 12x12 chessboard.");
    long long s = counting_solution(12);
    HERMESBDD_TEST_ASSERT(s == 14200LL);
}

void nqueens_solution_13()
{
    HERMESBDD_PRINT_TEST_COMMENT("Testing N-queen problem using a 13x13 chessboard.");
    long long s = counting_solution(13);
    HERMESBDD_TEST_ASSERT(s == 73712LL);
}

void nqueens_solution_14()
{
    HERMESBDD_PRINT_TEST_COMMENT("Testing N-queen problem using a 14x14 chessboard.");
    long long s = counting_solution(14);
    HERMESBDD_TEST_ASSERT(s == 365596LL);
}

void nqueens_solution_15()
{
    HERMESBDD_PRINT_TEST_COMMENT("Testing N-queen problem using a 15x15 chessboard.");
    long long s = counting_solution(15);
    HERMESBDD_TEST_ASSERT(s == 2279184LL);
}


int main()
{
    nqueens_solution_1();
    nqueens_solution_2();
    nqueens_solution_3();
    nqueens_solution_4();
    nqueens_solution_5();
    nqueens_solution_6();
    nqueens_solution_7();
    nqueens_solution_8();
    nqueens_solution_9();
    nqueens_solution_10();

    // Disabling tests for N > 10 by default due to long execution times.
    // To enable them, uncomment the lines below.

    // HERMESBDD_PRINT_TEST_COMMENT("Running extended N-Queens tests (N=11 to N=15). These may take some time.");
    // nqueens_solution_11();
    // nqueens_solution_12();
    // nqueens_solution_13();
    // nqueens_solution_14();
    // nqueens_solution_15();

    return HERMESBDD_TEST_FAILURES;
}
