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
 * TODO: description.
 * @param data
 * @return
 */
static uint64_t hash(const CacheItem& data)
{
    return hash128(&data, sizeof(CacheItem::key));
}

/*!
 * A lock guard around nodes.
 */
struct LockProtector
{
public:
    LockProtector(CacheSlot& slot) : _slot(slot)
    {
        while (_slot.locked.test_and_set(std::memory_order_acquire));
    }

    ~LockProtector() {
        _slot.locked.clear(std::memory_order_release);
    }

private:
    CacheSlot& _slot;
};

void Cache::init(size_t mem_usage)
{
    elems = mem_usage / sizeof(CacheSlot);
    table = new CacheSlot[elems];

    assert(table != nullptr);
}

void Cache::insert(const CacheItem& data)
{
    uint64_t index = hash(data) % elems;

    CacheSlot& current = table[index];
    LockProtector lock(current);

    current.data = data;
    current.exists = true;
}

bool Cache::find(CacheItem& item)
{
    uint64_t index = hash(item) % elems;

    CacheSlot& current = table[index];
    LockProtector lock(current);

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
    CacheItem data;

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
    CacheItem data;

    data.key.a = a;
    data.key.b = b;
    data.key.c = c;
    data.result = result;

    insert(data);
}