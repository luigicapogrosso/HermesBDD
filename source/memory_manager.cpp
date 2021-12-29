/***************************************************************************
 *            memory_manager.cpp
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



#include <cstdio>
#include <cassert>
#include <unistd.h>

#include "bdd_internal.hpp"


static size_t mem_available()
{
    long pages = sysconf(_SC_PHYS_PAGES);
    long page_size = sysconf(_SC_PAGE_SIZE);

    assert(pages != -1);
    assert(page_size != -1);

    return static_cast<size_t>(pages) * static_cast<size_t>(page_size);
}

namespace internal
{
    namespace manager
    {
        // TODO: we need to set the sizes of these somewhere.
        // std::unordered_map<Query, Node*> cache;
        Cache cache;
        Tree nodes;

        struct ConstructorHack {
            ConstructorHack() {
                // Leave 256 MB for other people, taking at most 16 GB.
                size_t max_mem = 0x400000000;

                //size_t max_mem = 0x40000000;
                size_t extra_mem = 0x10000000;

                size_t cache_size = 0x20000000;
                size_t mem = std::min(mem_available() - extra_mem,
                                      max_mem) - cache_size;

                nodes.init(mem);
                cache.init(cache_size);
            }
        } hack;
    }
}