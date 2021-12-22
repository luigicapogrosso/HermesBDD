/***************************************************************************
 *            cache.hpp
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



#ifndef CACHE_HPP
#define CACHE_HPP


#include <atomic>
#include <cstdint>

struct CacheItem
{
    struct
    {
        uint32_t a;
        uint32_t b;
        uint32_t c;
    } key;
    uint32_t result;
};

struct CacheSlot
{
    bool exists;
    std::atomic_flag locked;

    CacheItem data;
    CacheSlot()
        : locked(false)
    {
    }
};

class Cache
{
public:
    /*!
     * TODO: description.
     * @param a
     * @param b
     * @param c
     * @param result
     * @return
     */
    bool findITE(const uint32_t a,
                 const uint32_t b,
                 const uint32_t c,
                 uint32_t& result);

    /*!
     * TODO: description.
     * @param a
     * @param b
     * @param c
     * @param result
     */
    void insertITE(const uint32_t a,
                   const uint32_t b,
                   const uint32_t c,
                   uint32_t result);

    /*!
     * TODO: description.
     * @param mem_usage
     */
    void init(size_t mem_usage);

private:
    size_t elems;
    CacheSlot *table;

    /*!
     * TODO: description.
     * @param data
     * @return
     */
    bool find(CacheItem& data);

    /*!
     * TODO: description.
     * @param data
     */
    void insert(const CacheItem& data);
};

#endif // CACHE_HPP
