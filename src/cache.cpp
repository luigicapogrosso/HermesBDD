/***************************************************************************
 *            cache.cpp
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



#include <cassert>

#include "hash.hpp"
#include "cache.hpp"

/*!
 * @brief Lock guard around nodes.
 */
struct lock_protector
{
public:
    lock_protector(cache_slot& slot) : slot(slot)
    {
        while (slot.locked.test_and_set(std::memory_order_acquire));
    }

    ~lock_protector() {
        slot.locked.clear(std::memory_order_release);
    }

private:
    cache_slot& slot;
};

/*!
 * @brief Compute the hash128 of the cache item.
 * @param data
 * @return
 */
static uint64_t hash(const cache_item& data)
{
    return hash128(&data, sizeof(cache_item::key));
}

void Cache::init(size_t mem_usage)
{
    elems = mem_usage / sizeof(cache_slot);
    table = new cache_slot[elems];

    assert(table != nullptr);
}

void Cache::insert(const cache_item& data)
{
    uint64_t index = hash(data) % elems;

    cache_slot& current = table[index];
    lock_protector lock(current);

    current.data = data;
    current.exists = true;
}

bool Cache::find(cache_item& item)
{
    uint64_t index = hash(item) % elems;

    cache_slot& current = table[index];
    lock_protector lock(current);

    if (!current.exists)
    {
        return false;
    }

    if (current.data.key.a != item.key.a)
    {
        return false;
    }

    if (current.data.key.b != item.key.b)
    {
        return false;
    }

    if (current.data.key.c != item.key.c)
    {
        return false;
    }

    item.result = current.data.result;

    return true;
}

bool Cache::findITE(const uint32_t a,
                    const uint32_t b,
                    const uint32_t c,
                    uint32_t& result)
{
    cache_item data;

    data.key.a = a;
    data.key.b = b;
    data.key.c = c;

    if (find(data))
    {
        result = data.result;

        return true;
    }

    return false;
}

void Cache::insertITE(const uint32_t a,
                      const uint32_t b,
                      const uint32_t c,
                      uint32_t result)
{
    cache_item data;

    data.key.a = a;
    data.key.b = b;
    data.key.c = c;
    data.result = result;

    insert(data);
}
