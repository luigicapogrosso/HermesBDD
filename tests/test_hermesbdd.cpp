/***************************************************************************
 *            tests/test_hermesbdd.cpp
 *
 *  Copyright  2021  Luigi Capogrosso and Luca Geretti
 *
 ****************************************************************************/

/*
 * MIT License
 *
 * Copyright (c) 2021 Luigi Capogrosso and Luca Geretti
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
#include <cassert>
#include <unordered_map>

#include "bdd.hpp"
#include "test.hpp"

void test_constructors()
{
    HERMESBDD_PRINT_TEST_COMMENT("Testing constructors.");
    HERMESBDD_TEST_CALL(BDD set1(1));
}

void test_print()
{
    HERMESBDD_PRINT_TEST_COMMENT("Testing print().");

    BDD c(2);
    BDD d(2);
    BDD e(3);
    BDD f(4);
    BDD g(5);
    BDD h(6);
    BDD i(7);
    BDD j(8);

    BDD g1 = (((d | c) & (i & j)) ^ ((g > h) < i)) & ((e | f) | (c & f & j & i));

    g1.print("Simple SAT Instance");
}

void test_one_sat()
{
    HERMESBDD_PRINT_TEST_COMMENT("Testing one_sat().");

    BDD a(1);
    BDD b(2);

    std::unordered_map<uint32_t, bool> map = a.one_sat();
    HERMESBDD_TEST_ASSERT(map[1]);

    BDD c1 = a | b;
    map = c1.one_sat();
    HERMESBDD_TEST_ASSERT(map[1] || map[2]);

    BDD c2 = !a;
    map = c2.one_sat();
    HERMESBDD_TEST_ASSERT(!map[1]);

    BDD c3 = a & !b;
    map = c3.one_sat();
    HERMESBDD_TEST_ASSERT(map[1] && !map[2]);
}

void test_count_sat()
{
    BDD n1(1);
    BDD n2(2);
    BDD n3(3);
    BDD n4(4);
    BDD n5(5);
    BDD n6(6);
    std::set<uint32_t> s3({1, 2, 3, 4});

    BDD t1 = (n1 | n2) & (n3 | n4);
    HERMESBDD_TEST_EQUAL(t1.count_sat(s3), 9);

    BDD t2 = (n1 & n2) | (n3 & n4);
    HERMESBDD_TEST_EQUAL(t2.count_sat(s3), 7);
}


int main()
{
    test_constructors();
    test_print();
    test_one_sat();
    test_count_sat();

    return HERMESBDD_TEST_FAILURES;
}
