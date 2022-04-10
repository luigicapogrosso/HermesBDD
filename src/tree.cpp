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
#include <cstring>

#include "tree.hpp"

namespace internal
{
    int64_t pos;

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

    void Tree::init()
    {
        pos = 1;
        _elements = 1000;

        // Faster than running constructors.
		table = (node_slot*) calloc(_elements, sizeof(node_slot));
		for (uint32_t i = 0; i < _elements; i++)
		{
			table[i].node.id = -1;
		}

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
		if (node.id == -1)
		{
			node_slot& current = table[pos];

			current.node.id = pos;
			pos += 1;
			copy_node(node, current.node);

			if (pos > _elements - 1)
			{
				_elements = _elements * 2;

				node_slot* table2 = (node_slot*) calloc(_elements, sizeof(node_slot));
				std::memcpy(table2, table, _elements / 2 * sizeof(node_slot));
				
				table = table2;

				for (uint64_t i = _elements / 2; i < _elements; i++)
				{
					table[i].node.id = -1;
				}
			}

			// Maintain data structure.
			count.fetch_add(1, std::memory_order_relaxed);
			current.exists = true;

			return current.node.id;
		}

		return node.id;
	}
}
