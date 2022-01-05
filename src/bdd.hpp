/***************************************************************************
 *            bdd.hpp
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



#ifndef BDD_HPP
#define BDD_HPP


#include <set>
#include <string>
#include <unordered_map>

class BDD
{
public:
    /*!
     * @brief '<bdd>' empty constructor.
     */
    BDD();

    /*!
     * @brief '<bdd>' constructor.
     * @param v
     */
    BDD(uint32_t v);

    /// Representation of constant 'true'.
    static BDD bdd_true;
    /// Representation of constant 'false'.
    static BDD bdd_false;

    /*!
     * @brief Compute '<a> == <b>'.
     * @param r
     * @return
     */
    bool operator==(BDD r);

    /*!
     * @brief Compute the negation of '<bdd>'.
     * @return
     */
    BDD operator!();

    /*!
     * @brief Compute '<a> and <b>'.
     * @param r
     * @return
     */
    BDD operator&(BDD r);
    /*!
     * @brief Compute '<a> and= <b>'.
     * @param r
     * @return
     */
    BDD operator&=(BDD r);

    /*!
     * @brief Compute '<a> or <b>'.
     * @param r
     * @return
     */
    BDD operator|(BDD r);
    /*!
     * @brief Compute '<a> or= <b>'.
     * @param r
     * @return
     */
    BDD operator|=(BDD r);

    /*!
     * @brief Compute '<a> xor <b>'.
     * @param r
     * @return
     */
    BDD operator^(BDD r);
    /*!
     * @brief Compute '<a> xor= <b>'.
     * @param r
     * @return
     */
    BDD operator^=(BDD r);

    /*!
     * @brief Compute '<a> then <b>'.
     * @param r
     * @return
     */
    BDD operator>(BDD r);
    /*!
     * @brief Compute '<a> then= <b>'.
     * @param r
     * @return
     */
    BDD operator>=(BDD r);

    /*!
     * @brief Compute '<b> then <a>'.
     * @param r
     * @return
     */
    BDD operator<(BDD r);
    /*!
     * @brief Compute '<b> then= <a>'.
     * @param r
     * @return
     */
    BDD operator<=(BDD r);

    /*!
     * @brief Print the '<bdd>' with a custom title.
     * @param title
     */
    void print(std::string title);

    /*!
     * @brief Compute the number of minterms (assignments that lead to True)
              for a function with <number_of_vars> variables; we don’t need to
              know the exact variables that may be in the BDD, just how many
              there are.
     * @param vars
     * @return
     */
    double count_sat(std::set<uint32_t> vars);

    /*!
     * @brief Extract a single path to True from the BDD.
     * @return
     */
    std::unordered_map<uint32_t, bool> one_sat();

    /*!
     * @brief Returns non-zero if this Bdd is true_node or false_node.
     * @return
     */
    bool is_constant();

    /*!
     * @brief Gets the node.
     * @return
     */
    uint32_t get_node();

    /*!
     * @brief Gets the dummy.
     * @return
     */
    bool get_dummy();

private:
    bool dummy;
    uint32_t node;

    /*!
     * @brief TODO: description.
     * @param node
     * @param dummy
     */
    BDD(uint32_t node, bool dummy);
};

#endif // BDD_HPP
