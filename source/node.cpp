/***************************************************************************
 *            node.cpp
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


#include <thread>
#include <iostream>

#include "bdd.hpp"
#include "node.hpp"
#include "bdd_internal.hpp"

// TODO: tune this.
static constexpr int granularity = 500;

/*!
 * TODO: description.
 * @param node
 * @return
 */
static inline bool is_leaf(uint32_t node)
{
    return node == Node::true_node || node == Node::false_node;
}

/*!
 * TODO: description.
 * @param node
 * @return
 */
static inline Node* pointer(uint32_t node)
{
    uint32_t index = 0x7FFFFFFF & node;

    return &internal::manager::nodes.table[index].node;
}

/*!
 * TODO: description.
 * @param node
 * @return
 */
static inline bool is_complemented(uint32_t node)
{
    return (node & 0x80000000);
}

/*!
 * TODO: description.
 * @param node
 * @return
 */
static inline uint32_t complement(uint32_t node)
{
    return node ^ 0x80000000;
}

/*!
 * TODO: description.
 * @param A
 * @param B
 * @return
 */
static inline bool equals_complement(uint32_t A, uint32_t B)
{
    return A == complement(B);
}

/*!
 * TODO: description.
 * @param A
 * @param B
 * @param C
 * @return
 */
static inline uint32_t top_variable(uint32_t A, uint32_t B, uint32_t C)
{
    auto var = [] (uint32_t x)
    {
        return (is_leaf(x) ? std::numeric_limits<uint32_t>::max() : pointer(x)->root);
    };

    return std::min(var(A), std::min(var(B), var(C)));
}

/*!
 * TODO: description.
 * @param node
 * @param var
 * @param value
 * @return
 */
static inline uint32_t evaluate_at(uint32_t node, uint32_t var, bool value)
{
    // Variable is above this node, nothing changes.
    if (is_leaf(node) || pointer(node)->root > var)
    {
        return node;
    }

    // Variable is exactly this node, choose appropriate branch
    if (pointer(node)->root == var)
    {
        uint32_t target = (value ? pointer(node)->branch_true : pointer(node)
                ->branch_false);

        return is_complemented(node) ? complement(target) : target;
    }
    // TODO: remove if we use evaluate in anything except ITE.
    // assert(false);

    uint32_t new_node = Node::make(var,
                                   evaluate_at(pointer(node)->branch_true,
                                               var, value),
                                   evaluate_at(pointer(node)->branch_false,
                                               var, value));

    new_node = is_complemented(node) ? complement(new_node) : new_node;

    return new_node;
}

Node::Node() { }

Node::Node(uint32_t root, uint32_t branch_true, uint32_t branch_false)
    : root(root)
    , branch_true(branch_true)
    , branch_false(branch_false)
{
    int size_true, size_false;

    if (is_leaf(branch_true))
    {
        size_true = 0;
    } else
    {
        size_true = pointer(branch_true)->size;
    }

    if (is_leaf(branch_false))
    {
        size_false = 0;
    } else
    {
        size_false = pointer(branch_false)->size;
    }

    size = size_true + size_false + 1;
}

uint32_t Node::make(uint32_t root, uint32_t branch_true, uint32_t branch_false)
{
    if (branch_true == branch_false)
    {
        return branch_false;
    }

    // Enforce canonicity (complement only on 1 edge).
    if (is_complemented(branch_false))
    {
        uint32_t new_true = complement(branch_true);
        uint32_t new_false = complement(branch_false);
        Node node(root, new_true, new_false);

        return complement(internal::manager::nodes.lookup_create(node));
    }

    Node node(root, branch_true, branch_false);

    return internal::manager::nodes.lookup_create(node);
}

uint32_t Node::ITE(uint32_t A, uint32_t B, uint32_t C)
{
    // Base cases.
    if (A == true_node)
    {
        return B;
    }

    if (A == false_node)
    {
        return C;
    }

    if (B == true_node && C == false_node)
    {
        return A;
    }

    if (B == false_node && C == true_node)
    {
        return complement(A);
    }

    if (B == C)
    {
        return B;
    }
    
    uint32_t result;
    
    // Check if this ITE has been done before in cache.
    if (internal::manager::cache.findITE(A, B, C, result))
    {
        return result;
    }

    // Normalization rules.
    if (A == B)
    {
        // ITE(A, A, C) -> ITE(A, 1, C).
        result = ITE(A, true_node, C);
    }
    else if (equals_complement(A, B))
    {
        // ITE(A, !A, C) -> ITE(A, 0, C).
        result = ITE(A, false_node, C);
    }
    else if (A == C)
    {
        // ITE(A, B, A) -> ITE(A, B, 0).
        result = ITE(A, B, false_node);
    }
    else if (equals_complement(A, C))
    {
        // ITE(A, B, !A) -> ITE(A, B, 1).
        result = ITE(A, B, true_node);
    }
    else if (B == true_node && pointer(C)->root < pointer(A)->root)
    {
        // ITE(A, 1, C) -> ITE(C, 1, A) if (C < A).
        result = ITE(C, true_node, A);
    }
    else if (B == false_node && pointer(C)->root < pointer(A)->root)
    {
        // ITE(A, 0, C) -> ITE(!C, 0, !A) if (C < A).
        result = ITE(complement(C), false_node, complement(A));
    }
    else if (C == true_node && pointer(B)->root < pointer(A)->root)
    {
        // ITE(A, B, 1) -> ITE(!B, !A, 1) if (B < A).
        result = ITE(complement(B), complement(A), true_node);
    }
    else if (C == false_node && pointer(B)->root < pointer(A)->root)
    {
        // ITE(A, B, 0) -> ITE(B, A, 0) if (B < A).
        result = ITE(B, A, false_node);
    }
    else if (equals_complement(B, C) && pointer(B)->root < pointer(C)->root)
    {
        // ITE(A, B, !B) -> ITE(B, A, !A).
        result = ITE(B, A, complement(A));
    }
    else if (is_complemented(A))
    {
        // ITE(A, B, C) -> ITE(!A, C, B) if (A complemented).
        result = ITE(complement(A), C, B);
    }
    else if (is_complemented(B))
    {
        // ITE(A, B, C) -> !ITE(A, !B, !C) if (B complemented).
        result = complement(ITE(A, complement(B), complement(C)));
    }
    else
    {
        // If no normalization applies.
        uint32_t x = top_variable(A, B, C);
        uint32_t A_false = evaluate_at(A, x, false);
        uint32_t B_false = evaluate_at(B, x, false);
        uint32_t C_false = evaluate_at(C, x, false);
        uint32_t A_true = evaluate_at(A, x, true);
        uint32_t B_true = evaluate_at(B, x, true);
        uint32_t C_true = evaluate_at(C, x, true);

        uint32_t R_false, R_true;
        if (pointer(A_false)->size +
            pointer(B_false)->size +
            pointer(C_false)->size > granularity)
        {
            // TODO: Do in parallel C++ standard library thread.
#pragma omp parallel
            R_false = ITE(A_false, B_false, C_false);
        }
        else
        {
            // TODO: Do in parallel with C++ standard library thread.
#pragma omp parallel
            R_false = ITE(A_false, B_false, C_false);
        }

        if (pointer(A_false)->size +
            pointer(B_false)->size +
            pointer(C_false)->size > granularity)
        {
            // TODO: Do in parallel C++ standard library thread.
#pragma omp parallel
            R_true = ITE(A_true, B_true, C_true);
        }
        else
        {
            // TODO: Do in parallel C++ standard library thread.
#pragma omp parallel
            R_true = ITE(A_true, B_true, C_true);
        }

        result = make(x, R_true, R_false);
    }

    // Put in cache.
    internal::manager::cache.insertITE(A, B, C, result);

    return result;
}

static void print_rec(uint32_t node, std::set<uint32_t>& visited, uint32_t uniq)
{
    node = (is_complemented(node) ? complement(node) : node);

    if (visited.count(node))
    {
        return;
    }

    if (is_leaf(node))
    {
        return;
    }
    Node *p = pointer(node);

    std::cout << ID(uniq, node) << " [label=\"" << p->root << "\"];\n";
    std::cout << ID(uniq, node) << " -> "
              << ID(uniq, p->branch_false) << " [style=dashed];\n";

    if (is_complemented(p->branch_true))
    {
        std::cout << ID(uniq, node) << " -> "
                  << ID(uniq, complement(p->branch_true))
                  << " [style=filled] [color=red];\n";
    }
    else
    {
        std::cout << ID(uniq, node) << " -> " << ID(uniq, p->branch_true)
                  << " [style=filled];\n";
    }

    visited.insert(node);

    print_rec(p->branch_true, visited, uniq);
    print_rec(p->branch_false, visited, uniq);
}

void Node::print(uint32_t node, std::string title)
{
    std::cout << "digraph \"G_" << node << "\" \n{\n";
    std::cout << "label = \"" << title << "\";\n";
    std::cout << "labelloc = \"t\";\n";

    std::cout << ID(node, false_node)
    << " [shape=box, label=\"false\", style=filled, height=0.3, width=0.3];\n";
    std::cout << ID(node, "f")
    << " [shape=triangle, label=\"f\", style=filled, height=0.3, width=0.3];\n";

    if (is_complemented(node))
    {
        std::cout << ID(node, "f")
                  << " -> " << ID(node, complement(node))
                  << " [style=filled] [color=red];\n";
    }
    else
    {
        std::cout << ID(node, "f")
                  << " -> " << ID(node, node)
                  << " [style=filled];\n";
    }

    std::set<uint32_t> visited;
    print_rec(node, visited, node);

    std::cout << "}\n";
    std::cout << "// Size: " << pointer(node)->size << "\n" << std::endl;
}
