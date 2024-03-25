/***************************************************************************
 *
 *            include/bdd.hpp
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


#ifndef BDD_HPP
#define BDD_HPP


#include <set>
#include <string>
#include <unordered_map>
#include <cstdint>

class BDD
{
public:
    /*!
     * @brief '<bdd>' empty constructor.
     */
    BDD();

    /*!
     * @brief Representation of literal '<v>'.
     * @param v
     */
    BDD(uint32_t v);

    /// Representation of constant 'true'.
    static BDD bdd_true;
    /// Representation of constant 'false'.
    static BDD bdd_false;

    /*!
     * @brief Compute '<bdd_a> == <bdd_b>'.
     * @param r
     * @return 'true' if '<bdd_a> == <bdd_b>', 'false' otherwise.
     */
    bool operator==(BDD r);

    /*!
     * @brief Compute the negation of <bdd_a>.
     * @return The negation of <bdd_a>.
     */
    BDD operator!();

    /*!
     * @brief Compute '<bdd_a> and <bdd_b>'.
     * @param r
     * @return '<bdd_a> and <bdd_b>'
     */
    BDD operator&(BDD r);
    /*!
     * @brief Compute '<bdd_a> and eq. <bdd_b>'.
     * @param r
     * @return '<bdd_a> and eq. <bdd_b>'
     */
    BDD operator&=(BDD r);

    /*!
     * @brief Compute '<bdd_a> or <bdd_b>'.
     * @param r
     * @return '<bdd_a> or <bdd_b>'
     */
    BDD operator|(BDD r);
    /*!
     * @brief Compute '<bdd_a> or eq. <bdd_b>'.
     * @param r
     * @return '<bdd_a> or eq. <bdd_b>'
     */
    BDD operator|=(BDD r);

    /*!
     * @brief Compute '<bdd_a> xor <bdd_b>'.
     * @param r
     * @return '<bdd_a> xor <bdd_b>'
     */
    BDD operator^(BDD r);
    /*!
     * @brief Compute '<bdd_a> xor eq. <bdd_b>'.
     * @param r
     * @return '<bdd_a> xor eq. <bdd_b>'
     */
    BDD operator^=(BDD r);

    /*!
     * @brief Compute '<bdd_a> then <bdd_b>'.
     * @param r
     * @return '<bdd_a> then <bdd_b>'
     */
    BDD operator>(BDD r);
    /*!
     * @brief Compute '<bdd_a> then eq. <bdd_b>'.
     * @param r
     * @return '<bdd_a> then eq. <bdd_b>'
     */
    BDD operator>=(BDD r);

    /*!
     * @brief Compute '<bdd_b> then <bdd_a>'.
     * @param r
     * @return '<bdd_b> then <bdd_a>'
     */
    BDD operator<(BDD r);
    /*!
     * @brief Compute '<bdd_b> then eq. <bdd_a>'.
     * @param r
     * @return '<bdd_b> then eq. <bdd_a>'
     */
    BDD operator<=(BDD r);

    /**
     * @brief Representation of the negation of literal '<v>'.
     * @return The negation of literal '<v>'.
     */
    BDD nithvar(uint32_t v);

    /*!
     * @brief Print the '<bdd>' with a custom title.
     * @param title
     */
    void print(std::string title);

    /*!
     * @brief Compute the number of minterms (assignments that lead to true)
     *        for a function with <number_of_vars> variables; we don’t need to
     *        know the exact variables that may be in the BDD, just how many
     *        there are.
     * @param vars
     * @return
     */
    long double count_sat(std::set<uint32_t> vars);

    /*!
     * @brief Computes a satisfying truth assignment of a BDD.
     * @return The result is an unordered map that assigns some of the BDD
     *         indices a boolean truth values. If an index vi is not mapped to
     *         a boolean value, then vi is a don't care value in this
     *         assignment.
     */
    std::unordered_map<uint32_t, bool> one_sat();

    /*!
     * @brief Returns non-zero if this BDD is 'true_node' or 'false_node'.
     * @return 'true' if this BDD is 'true_node', 'false' otherwise.
     */
    bool is_constant();

    /*!
     * @brief Gets the node.
     * @return The node.
     */
    uint32_t get_node();

    /*!
     * @brief Gets the dummy.
     * @return The dummy.
     */
    bool get_dummy();

private:
    bool _dummy;
    uint32_t _node;

    /*!
     * @brief '<bdd>' constructor that does not require creation of a node.
     * @param node
     * @param dummy
     */
    BDD(uint32_t node, bool dummy);
};

#endif // BDD_HPP
