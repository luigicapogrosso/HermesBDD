/***************************************************************************
 *
 *            src/bdd_internal.cpp
 *
 ****************************************************************************/

/*
 * MIT License
 *
 * Copyright (c) 2022 Luigi Capogrosso and Luca Geretti
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



#ifndef BDD_INTERNAL_HPP
#define BDD_INTERNAL_HPP


#include "node.hpp"
#include "tree.hpp"
#include "cache.hpp"

namespace internal
{
    // Cache line width (bytes) on x64.
    constexpr size_t cache_width = 64;

    namespace manager
    {
        // No concept of ordering/reordering.
        extern Tree nodes;
        extern Cache cache;
    }
}

#endif // BDD_INTERNAL_HPP
