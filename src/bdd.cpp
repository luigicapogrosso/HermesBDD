/***************************************************************************
 *
 *            src/bdd.cpp
 *
 ****************************************************************************/

/*
 * MIT License
 *
 * Copyright (c) 2022 Luigi Capogrosso, Luca Geretti, Marco cristani, Franco Fummi, and Tiziano Villa.
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

static long double count_sat_helper(uint32_t node, int n, std::set<uint32_t>& vars)
{
    long double pow2 = pow(2, n);
    if (is_leaf(node))
    {
        return pow2;
    }

    // TODO: check cache now.

    Node *dnode = pointer(node);
    if (vars.count(dnode->root) == 0)
    {
        assert(false);
    }

    // TODO: this can be done in parallel.
    long double countT = count_sat_helper(dnode->branch_true, n, vars);
    long double countF = count_sat_helper(dnode->branch_false, n, vars);

    if (is_complemented(dnode->branch_true))
    {
        countT = pow2 - countT;
    }

    long double count = countT + (countF - countT) / 2;
    assert(count >= 0);

    // TODO: add to cache now.

    return count;
}

long double BDD::count_sat(std::set<uint32_t> vars)
{
    int n = vars.size();
    long double pow2 = pow(2, n);
    long double count = count_sat_helper(this->_node, n, vars);

    if (!is_complemented(this->_node))
    {
        count = pow2 - count;
    }

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
