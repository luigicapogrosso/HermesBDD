/***************************************************************************
 *
 *            include/hash.hpp
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



#ifndef HASH_HPP
#define HASH_HPP


#include <cstdint>
#include <cstddef>

#ifdef WIN32
    #define FORCE_INLINE static inline __forceinline
#else
    #define FORCE_INLINE static inline __attribute__((always_inline))
#endif

static inline uint64_t rotl64 (uint64_t x, int8_t r)
{
    return (x << r) | (x >> (64 - r));
}

#define ROTL64(x,y) rotl64(x,y)

#define BIG_CONSTANT(x) (x##LLU)

// Block read - if your platform needs to do endian-swapping or can only
// handle aligned reads, do the conversion here.

FORCE_INLINE uint64_t getblock64 (const uint64_t * p, int i)
{
    return p[i];
}

// Finalization mix - force all bits of a hash block to avalanche.

FORCE_INLINE uint64_t fmix64 (uint64_t k)
{
    k ^= k >> 33;
    k *= BIG_CONSTANT(0xff51afd7ed558ccd);
    k ^= k >> 33;
    k *= BIG_CONSTANT(0xc4ceb9fe1a85ec53);
    k ^= k >> 33;

    return k;
}

static uint64_t hash128(const void* key, const uint64_t len)
{
    const uint8_t * data = (const uint8_t*)key;
    const int nblocks = len / 16;

    uint64_t h1 = ~1337UL;
    uint64_t h2 = 0x69696969;

    const uint64_t c1 = BIG_CONSTANT(0x87c37b91114253d5);
    const uint64_t c2 = BIG_CONSTANT(0x4cf5ad432745937f);

    // Body.

    const uint64_t * blocks = (const uint64_t *)(data);

    for(int i = 0; i < nblocks; i++)
    {
        uint64_t k1 = getblock64(blocks,i*2+0);
        uint64_t k2 = getblock64(blocks,i*2+1);

        k1 *= c1; k1  = ROTL64(k1,31); k1 *= c2; h1 ^= k1;

        h1 = ROTL64(h1,27); h1 += h2; h1 = h1*5+0x52dce729;

        k2 *= c2; k2  = ROTL64(k2,33); k2 *= c1; h2 ^= k2;

        h2 = ROTL64(h2,31); h2 += h1; h2 = h2*5+0x38495ab5;
    }

    // Tail.

    const uint8_t * tail = (const uint8_t*)(data + nblocks*16);

    uint64_t k1 = 0;
    uint64_t k2 = 0;

    switch(len & 15)
    {
        case 15: k2 ^= ((uint64_t)tail[14]) << 48; [[clang::fallthrough]];
        case 14: k2 ^= ((uint64_t)tail[13]) << 40; [[clang::fallthrough]];
        case 13: k2 ^= ((uint64_t)tail[12]) << 32; [[clang::fallthrough]];
        case 12: k2 ^= ((uint64_t)tail[11]) << 24; [[clang::fallthrough]];
        case 11: k2 ^= ((uint64_t)tail[10]) << 16; [[clang::fallthrough]];
        case 10: k2 ^= ((uint64_t)tail[ 9]) << 8;  [[clang::fallthrough]];
        case  9: k2 ^= ((uint64_t)tail[ 8]) << 0;
            k2 *= c2; k2  = ROTL64(k2,33); k2 *= c1; h2 ^= k2; [[clang::fallthrough]];
        case  8: k1 ^= ((uint64_t)tail[ 7]) << 56; [[clang::fallthrough]];
        case  7: k1 ^= ((uint64_t)tail[ 6]) << 48; [[clang::fallthrough]];
        case  6: k1 ^= ((uint64_t)tail[ 5]) << 40; [[clang::fallthrough]];
        case  5: k1 ^= ((uint64_t)tail[ 4]) << 32; [[clang::fallthrough]];
        case  4: k1 ^= ((uint64_t)tail[ 3]) << 24; [[clang::fallthrough]];
        case  3: k1 ^= ((uint64_t)tail[ 2]) << 16; [[clang::fallthrough]];
        case  2: k1 ^= ((uint64_t)tail[ 1]) << 8; [[clang::fallthrough]];
        case  1: k1 ^= ((uint64_t)tail[ 0]) << 0;
            k1 *= c1; k1  = ROTL64(k1,31); k1 *= c2; h1 ^= k1;
        default: // do nothing;
            break;
    };

    // Finalization.

    h1 ^= len; h2 ^= len;

    h1 += h2;
    h2 += h1;

    h1 = fmix64(h1);
    h2 = fmix64(h2);

    h1 += h2;
    h2 += h1;

    return h1 ^ h2;
}

#endif // HASH_HPP
