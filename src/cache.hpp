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

/*!
 * @brief Representation of a cache item.
 */
struct cache_item
{
    struct
    {
        uint32_t a;
        uint32_t b;
        uint32_t c;
    } key;
    uint32_t result;
};

/*!
 * @brief Representation of a cache slot.
 */
struct cache_slot
{
    bool exists;
    std::atomic_flag locked;

    cache_item data;
    cache_slot()
    {
        locked.clear();
    }
};

class Cache
{
public:
    /*!
     * @brief Find ITE in cache.
     * @param a
     * @param b
     * @param c
     * @param result
     * @return 'true' if ITE is in cache, 'false' otherwise.
     */
    bool findITE(const uint32_t a,
                 const uint32_t b,
                 const uint32_t c,
                 uint32_t& result);

    /*!
     * @brief Insert ITE in cache.
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
     * @brief Init cache.
     * @param mem_usage
     */
    void init(size_t mem_usage);

private:
    size_t elems;
    cache_slot *table;

    /*!
     * @brief Find item in cache.
     * @param data
     * @return 'true' if item is in cache, 'false' otherwise.
     */
    bool find(cache_item& data);

    /*!
     * @brief Insert item in cache.
     * @param data
     */
    void insert(const cache_item& data);
};

#endif // CACHE_HPP
