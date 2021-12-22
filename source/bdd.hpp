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
    BDD();

    /*!
     * TODO: description.
     * @param v
     */
    BDD(uint32_t v);

    static BDD bdd_true;
    static BDD bdd_false;

    // NOT.
    /*!
     * TODO: description.
     * @return
     */
    BDD operator!();

    // AND.
    /*!
     * TODO: description.
     * @param r
     * @return
     */
    BDD operator&(BDD r);
    /*!
     * TODO: description.
     * @param r
     * @return
     */
    BDD operator&=(BDD r);

    // OR.
    /*!
     * TODO: description.
     * @param r
     * @return
     */
    BDD operator|(BDD r);
    /*!
     * TODO: description.
     * @param r
     * @return
     */
    BDD operator|=(BDD r);

    // XOR.
    /*!
     * TODO: description.
     * @param r
     * @return
     */
    BDD operator^(BDD r);
    /*!
     * TODO: description.
     * @param r
     * @return
     */
    BDD operator^=(BDD r);

    // IMPLIES.
    /*!
     * TODO: description.
     * @param r
     * @return
     */
    BDD operator>(BDD r);
    /*!
     * TODO: description.
     * @param r
     * @return
     */
    BDD operator>=(BDD r);

    // REVERSE IMPLIES.
    /*!
     * TODO: description.
     * @param r
     * @return
     */
    BDD operator<(BDD r);
    /*!
     * TODO: description.
     * @param r
     * @return
     */
    BDD operator<=(BDD r);

    /*!
     * TODO: description.
     * @param title
     */
    void print(std::string title);

    /*!
     * TODO: description.
     * @param vars
     * @return
     */
    double count_sat(std::set<uint32_t> vars);

    /*!
     * TODO: description.
     * @return
     */
    std::unordered_map<uint32_t, bool> one_sat();

private:
    uint32_t node;

    /*!
     * TODO: description.
     * @param node
     * @param dummy
     */
    BDD(uint32_t node, bool dummy);
};

#endif // BDD_HPP
