/***************************************************************************
 *            tree.hpp
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



#ifndef TREE_HPP
#define TREE_HPP


#include <atomic>
#include <cstdio>
#include <cstdint>

#include "node.hpp"

namespace internal
{
    struct node_slot
    {
        Node node;
        bool exists;
        std::atomic_flag locked;
    };

    class Tree
    {
    public:
        node_slot *table;

        /*!
        * @brief Init tree.
        * @param mem_usage
        */
        void init(size_t mem_usage);

        /*!
        * @brief Checks (using the hash table) if there is a node 'v'.
        * @param node
        * @return 'true' if there is a node 'v', 'false' otherwise.
        */
        uint32_t lookup_or_create(const Node& node);

        std::atomic<uint32_t> count;
        
    private:
        uint32_t _elements;
    };
}

#endif // TREE_HPP
