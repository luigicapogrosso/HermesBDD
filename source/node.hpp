/***************************************************************************
 *            node.hpp
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



#ifndef NODE_HPP
#define NODE_HPP


#include <limits>
#include <string>
#include <cstdint>

#define ID(uniq, x) "\"" << uniq << "_" << x << "\""

class Node
{
public:
    static constexpr uint32_t true_node = 0x80000000;
    static constexpr uint32_t false_node = 0x00000000;

    // Uniquely identifying BDDs in canonical form.
    int size;
    uint32_t root;
    uint32_t branch_true;
    uint32_t branch_false;

    /*!
     * Creates node on stack, should be ONLY used in make to get heap pointer.
     * @param root
     * @param branch_true
     * @param branch_false
     */
    Node(uint32_t root, uint32_t branch_true, uint32_t branch_false);

    /*!
     * Creates node on the heap, this is the pointer that should be used in
     * other operations.
     * @param root
     * @param branch_true
     * @param branch_false
     * @return
     */
    static uint32_t make(uint32_t root,
                        uint32_t branch_true,
                        uint32_t branch_false);

    /*!
     * TODO: description.
     * @param A
     * @param B
     * @param C
     * @return
     */
    static uint32_t ITE(uint32_t A, uint32_t B, uint32_t C);

     /*!
     * TODO: description.
     * @param A
     * @param B
     * @param C
     * @return
     */
    static uint32_t ITE_without_cache(uint32_t A, uint32_t B, uint32_t C);

    /*!
     * TODO: description.
     * @param A
     * @param B
     * @param C
     * @return
     */
    static uint32_t ITE_with_thread(uint32_t A, uint32_t B, uint32_t C, int level);

    /*!
     * For debug purposes, will print a full graph of the tree.
     * @param node
     * @param title
     */
    static void print(uint32_t node, std::string title);
};

#endif // NODE_HPP
