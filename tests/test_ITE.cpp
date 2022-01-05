/***************************************************************************
 *            tests/test_ITE.cpp
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

void test_not()
{
    HERMESBDD_PRINT_TEST_COMMENT("Testing NOT operator.");

    BDD a(1);
    BDD b(2);

    BDD c = !a;
    std::unordered_map<uint32_t, bool> map = c.one_sat();

    HERMESBDD_TEST_ASSERT(!map[1]);
}

void test_and()
{
    HERMESBDD_PRINT_TEST_COMMENT("Testing AND operator.");
    BDD a(1);
    BDD b(2);

    BDD c = a & !b;
    std::unordered_map<uint32_t, bool> map = c.one_sat();

    HERMESBDD_TEST_ASSERT(map[1] && !map[2]);
}

void test_and_eq()
{
    HERMESBDD_PRINT_TEST_COMMENT("Testing AND EQUAL operator.");
    BDD a(1);
    BDD b(2);

    BDD c = a &= b;
    std::unordered_map<uint32_t, bool> map = c.one_sat();

    HERMESBDD_TEST_EQUAL(map[1], map[2]);
}

void test_or()
{
    HERMESBDD_PRINT_TEST_COMMENT("Testing OR operator.");

    BDD a(1);
    BDD b(2);

    BDD c = a | b;
    std::unordered_map<uint32_t, bool> map = c.one_sat();

    HERMESBDD_TEST_ASSERT(map[1] || map[2]);
}

void test_or_eq()
{
    HERMESBDD_PRINT_TEST_COMMENT("Testing OR EQUAL operator.");

    BDD a(1);
    BDD b(2);

    BDD c = a |= b;
    std::unordered_map<uint32_t, bool> map = c.one_sat();

    HERMESBDD_TEST_EQUAL(map[1], !map[2]);
}

void test_xor()
{
    HERMESBDD_PRINT_TEST_COMMENT("Testing XOR operator.");

    BDD a(1);
    BDD b(2);

    BDD c = (a ^ b);
    std::unordered_map<uint32_t, bool> map = c.one_sat();

    HERMESBDD_TEST_ASSERT(map[1] ^ map[2]);
}

void test_xor_eq()
{
    HERMESBDD_PRINT_TEST_COMMENT("Testing XOR EQUAL operator.");

    BDD a(1);
    BDD b(2);

    BDD c = (a ^= b);
    std::unordered_map<uint32_t, bool> map = c.one_sat();

    HERMESBDD_TEST_EQUAL(map[1], !map[2]);
}

void test_implies()
{
    HERMESBDD_PRINT_TEST_COMMENT("Testing IMPLIES operator.");

    BDD a(1);
    BDD b(2);
    std::set<uint32_t> s({1, 2});

    BDD c = a > b;

    HERMESBDD_TEST_ASSERT(c.count_sat(s) == 3);
}

void test_implies_eq()
{
    HERMESBDD_PRINT_TEST_COMMENT("Testing IMPLIES EQUAL operator.");

    BDD a(1);
    BDD b(2);
    std::set<uint32_t> s({1, 2});

    BDD c = a >= b;
    std::unordered_map<uint32_t, bool> map = c.one_sat();

    HERMESBDD_TEST_EQUAL(map[1], map[2]);
}

void test_reverse_implies()
{
    HERMESBDD_PRINT_TEST_COMMENT("Testing reverse IMPLIES operator.");

    BDD a(1);
    BDD b(2);
    std::set<uint32_t> s({1, 2});

    BDD c = a < b;

    HERMESBDD_TEST_ASSERT(c.count_sat(s) == 3);
}

void test_reverse_implies_eq()
{
    HERMESBDD_PRINT_TEST_COMMENT("Testing reverse IMPLIES EQUAL operator.");

    BDD a(1);
    BDD b(2);
    std::set<uint32_t> s({1, 2});

    BDD c = a <= b;
    std::unordered_map<uint32_t, bool> map = c.one_sat();

    HERMESBDD_TEST_EQUAL(map[1], map[2]);
}


int main()
{
    test_not();

    test_and();
    test_and_eq();

    test_or();
    test_or_eq();

    test_xor();
    test_xor_eq();

    test_implies();
    test_implies_eq();

    test_reverse_implies();
    test_reverse_implies_eq();

    return HERMESBDD_TEST_FAILURES;
}
