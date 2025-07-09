/***************************************************************************
 *
 *            src/bdd.cpp
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


#include <map>
#include <cmath>
#include <cassert>
#include <iostream>

#include "bdd.hpp"
#include "node.hpp"
#include "bdd_internal.hpp"

/*!
 * @brief Compute the complement of node.
 * @param node
 * @return The complement of node.
 */
static inline uint32_t complement(uint32_t node)
{
    return node ^ 0x80000000;
}

/*!
 * @brief Compute if node is complemented.
 * @param node
 * @return 'true' if node is complemented, 'false' otherwise.
 */
static inline bool is_complemented(uint32_t node)
{
    return (node & 0x80000000);
}

/*!
 * @brief Compute if node is a leaf.
 * @param node
 * @return 'true' if node is a leaf, 'false' otherwise.
 */
static inline bool is_leaf(uint32_t node)
{
    return node == Node::true_node || node == Node::false_node;
}

/*!
 * @brief Get the pointer of node.
 * @param node
 * @return The pointer of node.
 */
static inline Node *pointer(uint32_t node)
{
    // TODO: set lowest order bit to 0.
    uint32_t index = 0x7FFFFFFF & node;

    return &internal::manager::nodes.table[index].node;
}

BDD::BDD()
{

}

BDD::BDD(uint32_t v)
{
    _node = Node::make(v, Node::true_node, Node::false_node);
}

BDD BDD::bdd_true(Node::true_node, true);

BDD BDD::bdd_false(Node::false_node, true);

BDD::BDD(uint32_t node, bool dummy)
    : _dummy(dummy)
    , _node(node)
{

}

bool BDD::operator==(BDD r)
{
    return this->_node == r._node;
}

BDD BDD::operator!()
{
    return BDD(complement(this->_node), true);
}

BDD BDD::operator&(BDD r)
{
    return BDD(Node::ITE(this->_node, r._node, Node::false_node),
               true);
}

BDD BDD::operator&=(BDD r)
{
    *this = *this & r;
    return *this;
}

BDD BDD::operator|(BDD r)
{
    return BDD(Node::ITE(this->_node, Node::true_node, r._node),
               true);
}

BDD BDD::operator|=(BDD r)
{
    *this = *this | r;
    return *this;
}

BDD BDD::operator^(BDD r)
{
    return BDD(Node::ITE(this->_node, complement(r._node), r._node),
               true);
}

BDD BDD::operator^=(BDD r)
{
    *this = *this ^ r;
    return *this;
}

BDD BDD::operator>(BDD r)
{
    return BDD(Node::ITE(this->_node, r._node, Node::true_node),
               true);
}

BDD BDD::operator>=(BDD r)
{
    *this = *this > r;
    return *this;
}

BDD BDD::operator<(BDD r)
{
    return BDD(Node::ITE(this->_node, Node::true_node, complement(r._node)),
               true);
}

BDD BDD::operator<=(BDD r)
{
    *this = *this > r;
    return *this;
}

BDD BDD::nithvar(uint32_t v)
{
    return !(BDD(v));
}

void BDD::print(std::string title)
{
    Node::print(this->_node, title);
}

// Using std::map for cache. Key: node ID, Value: computed count.
static long double count_sat_helper(uint32_t node_ref,
                                    int n_total_vars,
                                    const std::set<uint32_t>& vars_set,
                                    long double pow2_n_total_vars,
                                    std::map<uint32_t, long double>& cache)
{
    // 1. Handle base cases: TrueNode and FalseNode.
    if (node_ref == Node::true_node)
    {
        return pow2_n_total_vars;
    }
    if (node_ref == Node::false_node)
    {
        return 0.0L;
    }

    // 2. Check cache.
    auto it = cache.find(node_ref);
    if (it != cache.end())
    {
        return it->second;
    }

    // Get the actual node data (strips complement from node_ref).
    Node *dnode_ptr = pointer(node_ref);

    // 3. Assert that the variable of this node is in the vars_set.
    assert(vars_set.count(dnode_ptr->root) != 0 && "Variable of BDD node not in specified variable set for count_sat");

    // 4. Recursive calls for children.
    // The `n_total_vars` and `pow2_n_total_vars` are passed down unchanged,
    // as they represent the total context for scaling.
    long double count_for_true_branch = count_sat_helper(dnode_ptr->branch_true,
                                                         n_total_vars,
                                                         vars_set,
                                                         pow2_n_total_vars,
                                                         cache);

    long double count_for_false_branch = count_sat_helper(dnode_ptr->branch_false,
                                                          n_total_vars,
                                                          vars_set,
                                                          pow2_n_total_vars,
                                                          cache);

    // 5. Adjust counts if branches are complemented.
    // This logic assumes count_sat_helper returns the count for the specific node_ref passed to it.
    // If dnode_ptr->branch_true is a complemented ref (e.g., !G),
    // then count_for_true_branch is already CountSat(!G).
    // No further adjustment needed here based on branch's complement bit.
    // The recursive call handles the complement of its argument.

    // Recursive step based on Shannon expansion: C(f) = (C(f|v=0) + C(f|v=1))/2, scaled by 2^N.
    // Or, if C means full count: C(f) = (C(f|v=0) + C(f|v=1))/2 if var v is part of N.
    // Here, count_for_true_branch is C(TrueBranchFunction, N vars).
    // and count_for_false_branch is C(FalseBranchFunction, N vars).
    long double result_for_dnode_ptr = (count_for_true_branch + count_for_false_branch) / 2.0L;

    // 6. Handle if the current node_ref itself was complemented.
    long double final_result_for_node_ref;
    if (is_complemented(node_ref))
    {
        final_result_for_node_ref = pow2_n_total_vars - result_for_dnode_ptr;
    }
    else
    {
        final_result_for_node_ref = result_for_dnode_ptr;
    }

    // Allow for small floating point inaccuracies if near zero.
    assert(final_result_for_node_ref >= -1e-9);

    // 7. Store in cache and return.
    cache[node_ref] = final_result_for_node_ref;
    return final_result_for_node_ref;
}


long double BDD::count_sat(std::set<uint32_t> vars)
{
    int n = vars.size();
    if (n == 0 && (this->_node == Node::true_node || this->_node == Node::false_node)) {
        // Special case: if no variables, True node has 1 satisfying assignment (empty assignment), False has 0.
        // However, the problem is usually defined over a set of variables.
        // If vars is empty, pow(2,0)=1.
        // True node -> 1. False node -> 0.
        if (this->_node == Node::true_node) return 1.0L;
        if (this->_node == Node::false_node) return 0.0L;
        // If node is not a terminal and n=0, it's ill-defined.
        // The helper expects dnode->root to be in vars.
        // For safety, if called with n=0 and non-terminal node, could return error or 0.
        // Current structure will proceed, pow2=1. Assertion might fail.
        // Let's assume n > 0 if node is not terminal, or vars contains all support vars.
    }


    long double pow2_n = std::pow(2.0L, n); // Use long double for pow.
    std::map<uint32_t, long double> cache;  // Create cache for this call.

    // The helper function will now correctly calculate based on the node_ref it receives, including handling its own complement bit.
    long double count = count_sat_helper(this->_node, n, vars, pow2_n, cache);

    return count;
}

static bool one_sat_helper(uint32_t node,
                           bool p,
                           std::unordered_map<uint32_t, bool>& map)
{
    if (is_leaf(node))
    {
        return !p;
    }

    Node* dnode = pointer(node);

    map[dnode->root] = false;
    if (one_sat_helper(dnode->branch_false, p, map))
    {
        return true;
    }

    map[dnode->root] = true;
    if (is_complemented(dnode->branch_true))
    {
        p = !p;
    }

    return one_sat_helper(dnode->branch_true, p, map);
}

std::unordered_map<uint32_t, bool> BDD::one_sat()
{
    std::unordered_map<uint32_t, bool> map;

    if (one_sat_helper(this->_node, !is_complemented(this->_node), map))
    {
        return map;
    }
    else
    {
        map.clear();

        return map;
    }
}

bool BDD::is_constant()
{
    return _node == Node::true_node || Node::false_node;
}

uint32_t BDD::get_node()
{
    return _node;
}

bool BDD::get_dummy()
{
    return _dummy;
}
