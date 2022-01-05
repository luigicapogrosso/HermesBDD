/***************************************************************************
 *            tree.cpp
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
#include "tree.hpp"

namespace internal
{
    /*!
     * @brief Compute the hash128 of the cache item.
     * @param data
     * @return
     */
    static inline size_t hash(const Node& node)
    {
        return hash128(&node, sizeof(Node));
    }

    /*!
     * @brief Copy node src in dest.
     * @param src
     * @param dest
     */
    static void copy_node(const Node& src, Node& dest)
    {
        dest.root = src.root;
        dest.branch_true = src.branch_true;
        dest.branch_false = src.branch_false;
        dest.size = src.size;
    }

    void Tree::init(size_t mem_usage)
    {
        elements = mem_usage / sizeof(node_slot);
        assert(elements <std::numeric_limits<int32_t>::max());

        // Faster than running constructors.
        table = (node_slot *) calloc(elements, sizeof(node_slot));

        assert(table != nullptr);
        table[0].exists = true;
    }

    /*!
     * @brief A lock guard around nodes
     */
    struct lock_protector
    {
    public:
        lock_protector(node_slot& slot) : _slot(slot)
        {
            while (_slot.locked.test_and_set(std::memory_order_acquire));
        }

        ~lock_protector()
        {
            _slot.locked.clear(std::memory_order_release);
        }

    private:
        node_slot& _slot;
    };

    uint32_t Tree::lookup_or_create(const Node& node)
    {
        uint32_t hashed = hash(node);

        for (uint32_t offset = 0; offset < elements; offset++)
        {
            uint32_t index = (hashed + offset) % elements;

            node_slot& current = table[index];
            lock_protector lock(current);

            if (!current.exists)
            {
                copy_node(node, current.node);

                // Maintain data structure.
                count.fetch_add(1, std::memory_order_relaxed);
                current.exists = true;

                return index;
            }

            if (node.root == current.node.root               &&
                node.branch_true == current.node.branch_true &&
                node.branch_false == current.node.branch_false)
            {
                // TODO: increase reference count.
                return index;
            }
        }

        return 0;
    }
}
