/***************************************************************************
 *
 *            src/memory_manager.cpp
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


#include <cstdio>
#include <cassert>
#ifdef WIN32
    #include <io.h>
    #include <windows.h>
#else
    #include <unistd.h>
#endif

#include "bdd_internal.hpp"

#ifdef NO_DYNMEM
/*!
 * @brief Compute the memory available size.
 * @return The memory available size.
 */
static size_t mem_available()
{
#ifdef WIN32
    MEMORYSTATUSEX statex;
    GlobalMemoryStatusEx(&statex);

    return static_cast<size_t>(statex.ullTotalPhys);
#else
    long pages = sysconf(_SC_PHYS_PAGES);
    assert(pages != -1);

    long page_size = sysconf(_SC_PAGE_SIZE);
    assert(page_size != -1);

    return static_cast<size_t>(pages) * static_cast<size_t>(page_size);
#endif
}
#endif

namespace internal
{
    namespace manager
    {
        Cache cache;
        Tree nodes;

        struct ConstructorHack {
            ConstructorHack() {
                size_t cache_size = 0x20000000;
#ifdef NO_DYNMEM
                // Leave 256 MB for other people, taking at most 2 GB.
                size_t max_mem = 0x050000000;
                size_t extra_mem = 0x10000000;
#ifdef WIN32
                size_t mem_size = min(mem_available() - extra_mem, max_mem) - cache_size;
#else
                size_t mem_size = std::min(mem_available() - extra_mem, max_mem) - cache_size;
#endif
                nodes.init(mem_size);

#else
                nodes.init();
#endif
                cache.init(cache_size);
            }
        } hack;
    }
}
