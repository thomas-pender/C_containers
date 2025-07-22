/**
 * @file bit_sets.h
 * @brief Public interface of <tt>bit_sets</tt> class
 * @author Thomas Pender
 */
# ifndef INCLUDED_BIT_SETS_H
# define INCLUDED_BIT_SETS_H

# include <stdio.h>
# include <stddef.h>
# include <stdint.h>

# define _WORDSIZE 64

/**
 * @brief Bit representations of powers of 2.
 *
 * Bit representations of powers of 2.
 */
static uint64_t _BITT[64] = {
  1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,32768,65536,131072,262144,
  524288,1048576,2097152,4194304,8388608,16777216,33554432,67108864,134217728,
  268435456,536870912,1073741824,2147483648,4294967296,8589934592,17179869184,
  34359738368,68719476736,137438953472,274877906944,549755813888,1099511627776,
  2199023255552,4398046511104,8796093022208,17592186044416,35184372088832,
  70368744177664,140737488355328,281474976710656,562949953421312,1125899906842624,
  2251799813685248,4503599627370496,9007199254740992,18014398509481984,
  36028797018963968,72057594037927936,144115188075855872,288230376151711744,
  576460752303423488,1152921504606846976,2305843009213693952,4611686018427387904,
  9223372036854775808ULL,
};

/**
 * @brief Bit counts for the possible 256 bytes.
 *
 * Bit counts for the possible 256 bytes.
 */
static uint64_t _BITTCOUNT[256] = {
  0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,1,2,2,3,2,3,3,4,
  2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,
  2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,
  4,5,5,6,5,6,6,7,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
  2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,2,3,3,4,3,4,4,5,
  3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
  4,5,5,6,5,6,6,7,5,6,6,7,6,7,7,8,
};

/**
 * @brief Rightmost bits for 256 possible bytes.
 *
 * Rightmost bits for 256 possible bytes.
 */
static uint64_t _RIGHTBITT[256] = {
  _WORDSIZE,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,5,0,1,0,
  2,0,1,0,3,0,1,0,2,0,1,0,4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,6,0,1,0,2,0,1,0,3,0,1,0,
  2,0,1,0,4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,5,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,0,1,0,
  2,0,1,0,3,0,1,0,2,0,1,0,7,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,0,1,0,2,0,1,0,3,0,1,0,
  2,0,1,0,5,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,6,0,1,0,
  2,0,1,0,3,0,1,0,2,0,1,0,4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,5,0,1,0,2,0,1,0,3,0,1,0,
  2,0,1,0,4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
};

typedef uint64_t setwords_t, sets_t;

/**
 * @brief Return quotient of <tt>pos</tt> with respect to <tt>setwords_t</tt>.
 *
 * Return quotient of <tt>pos</tt> with respect to <tt>setwords_t</tt>.
 */
# define _SETWD(pos) ((pos)>>6)

/**
 * @brief Set bit at position <tt>pos</tt> in <tt>setword_t</tt>.
 *
 * Set bit at position <tt>pos</tt> in <tt>setword_t</tt>.
 */
# define _SETBT(pos) ((pos)&0x3F)

/**
 * @brief Multiply by <tt>_WORDSIZE</tt>.
 *
 * Multiply by <tt>_WORDSIZE</tt>.
 */
# define _TIMESWORDSIZE(w) ((w)<<6)

/**
 * @brief Number of <tt>setwords_t</tt> required to hold a set of <tt>n</tt>
 * elements.
 *
 * Number of <tt>setwords_t</tt> required to hold a set of <tt>n</tt>
 * elements.
 */
# define _SETWORDSNEEDED(n) ((((n)-1)>>6)+1)

/**
 * @brief Add element indexed by <tt>pos</tt> to set.
 *
 * Add element indexed by <tt>pos</tt> to set.
 */
# define _ADDELEMENT(setadd, pos)  ((setadd)[_SETWD(pos)] |= _BITT[_SETBT(pos)])

/**
 * @brief Remove element indexed by <tt>pos</tt> from set.
 *
 * Remove element indexed by <tt>pos</tt> from set.
 */
# define _DELELEMENT(setadd, pos)  ((setadd)[_SETWD(pos)] &= ~_BITT[_SETBT(pos)])

/**
 * @brief Flip membership od element indexed by <tt>pos</tt>.
 *
 * Remove element indexed by <tt>pos</tt> from set if it is not already
 * included. If it is included, remove it.
 */
# define _FLIPELEMENT(setadd, pos) ((setadd)[_SETWD(pos)] ^= _BITT[_SETBT(pos)])

/**
 * @brief Test if element indexed by <tt>pos</tt> is in the set.
 *
 * Test if element indexed by <tt>pos</tt> is in the set.
 */
# define _ISELEMENT(setadd, pos) (((setadd)[_SETWD(pos)] & _BITT[_SETBT(pos)])!= 0)

/**
 * @brief Dump contents of set.
 *
 * Dump contents of set.
 */
# define _EMPTYSET(setadd, m) {                         \
    for ( size_t _i = 0; _i < m; _i++ ) setadd[_i] = 0; \
  }

/**
 * @brief Add every possible element to the set.
 *
 * Add every possible element to the set.
 */
# define _ALLBITS(setadd, m) {                      \
    for ( size_t _i = 0; _i < m; _i++ )             \
      setadd[_i] = 0xFFFFFFFFFFFFFFFFUL;            \
  }

/**
 * @brief Copy contents of set <tt>s2</tt> into <tt>s1</tt>.
 *
 * Copy contents of set <tt>s2</tt> into <tt>s1</tt>.
 */
# define _SETCPY(s1, s2, m) {                                 \
    for ( size_t _i = 0; _i < m; _i++ ) (s1)[_i] = (s2)[_i];  \
  }

# define MSK032      0x00000000FFFFFFFFUL
# define MSK00016    0x000000000000FFFFUL
# define MSK01600    0x0000FFFF00000000UL
# define MSK00000008 0x00000000000000FFUL
# define MSK00000800 0x0000000000FF0000UL
# define MSK00080000 0x000000FF00000000UL
# define MSK08000000 0x00FF000000000000UL
# define MSK64       0xFFFFFFFFFFFFFFFEUL

/**
 * @brief Number of elements in the set.
 *
 * Number of elements in the set.
 */
# define _POPCOUNT(x) (_BITTCOUNT[(x)>>56 & 0xFF] + _BITTCOUNT[(x)>>48 & 0xFF] \
                      + _BITTCOUNT[(x)>>40 & 0xFF] + _BITTCOUNT[(x)>>32 & 0xFF] \
                      + _BITTCOUNT[(x)>>24 & 0xFF] + _BITTCOUNT[(x)>>16 & 0xFF] \
                      + _BITTCOUNT[(x)>>8 & 0xFF]  + _BITTCOUNT[(x) & 0xFF])

/**
 * @brief Return the smallest element of the set.
 *
 * Return the smallest element of the set. If the set is empty, <tt>_WORDSIZE</tt>
 * is returned.
 */
# define _FIRSTBIT(x) ((x) ? ((x) & MSK032 ? ((x) & MSK00016 ? ((x) & MSK00000008 \
                      ? (_RIGHTBITT[(x)&0xFF]) : (8+_RIGHTBITT[((x)>>8)&0xFF])) : \
                      ((x) & MSK00000800 ? (16+_RIGHTBITT[((x)>>16)&0xFF]) : \
                      (24+_RIGHTBITT[((x)>>24)&0xFF]))) : ((x) & MSK01600 ? \
                      ((x) & MSK00080000 ? (32+_RIGHTBITT[((x)>>32)&0xFF]) \
                      : (40+_RIGHTBITT[((x)>>40)&0xFF])) : ((x) & MSK08000000 \
                      ? (48+_RIGHTBITT[((x)>>48)&0xFF]) : \
                      (56+_RIGHTBITT[((x)>>56)&0xFF])))) : _WORDSIZE)

/**
 * @brief Return the smallest element of the set.
 *
 * Return the smallest element of the set. Assumes the set is nonempty.
 */
# define _FIRSTBITNZ(x) ((x) & MSK032 ? ((x) & MSK00016 ? ((x) & MSK00000008 \
                      ? (_RIGHTBITT[(x)&0xFF]) : (8+_RIGHTBITT[((x)>>8)&0xFF])) : \
                      ((x) & MSK00000800 ? (16+_RIGHTBITT[((x)>>16)&0xFF]) : \
                      (24+_RIGHTBITT[((x)>>24)&0xFF]))) : ((x) & MSK01600 ? \
                      ((x) & MSK00080000 ? (32+_RIGHTBITT[((x)>>32)&0xFF]) \
                      : (40+_RIGHTBITT[((x)>>40)&0xFF])) : ((x) & MSK08000000 \
                      ? (48+_RIGHTBITT[((x)>>48)&0xFF]) : \
                      (56+_RIGHTBITT[((x)>>56)&0xFF]))))

/**
 * @brief Set 64-bit mask <tt>x</tt> positions to the right.
 *
 * Set 64-bit mask <tt>x</tt> positions to the right.
 */
# define _BITMASK(x)  (MSK64 << (x))

/**
 * @brief Return intersection of two <tt>setwords_t</tt>.
 *
 * Return intersection of two <tt>setwords_t</tt>.
 */
# define _WINTERSECT(word1, word2) ((word1) & (word2))

/**
 * @brief Return size of intersection of two <tt>setwords_t</tt>.
 *
 * Return size of intersection of two <tt>setwords_t</tt>.
 */
# define _WINTERSECTSIZE(word1, word2) _POPCOUNT(_WINTERSECT(word1, word2))

/**
 * @brief Takes and removes smallest element of <tt>setwords_t</tt>.
 *
 * Takes and removes smallest element of <tt>setwords_t</tt>.
 */
# define _TAKEBIT(_i, word) {_i = _FIRSTBITNZ(word); (word) ^= _BITT[_i];}

/**
 * @brief Return element of set.
 *
 * Returns smallest element of set or negative value if empty.
 *
 * @param[in] set1 Set object being looped over.
 * @param[in] m Number of setwords in the set object.
 * @param[in] pos Element of set or -1.
 *
 * @retval int Next element of set after <tt>pos</tt> or first element if
 * <tt>pos</tt> is -1.
 */
extern int bit_nextelement(const sets_t set1[static 1], size_t m, int pos);

/**
 * @brief Applies permutation to set.
 *
 * Applies permutation to set and places the result in first argument.
 *
 * @param[in] s1 Desitination for image of set under permutation.
 * @param[in] s2 Set being permuted.
 * @param[in] p Permutation being applied to set.
 * @param[in] m Number of setwords in set.
 */
extern void bit_permset(sets_t s1[restrict static 1],
                        const sets_t s2[restrict static 1],
                        const uint32_t p[static 1], size_t m);

/**
 * @brief Test if permutation fixeds the set.
 *
 * Test if permutation fixeds the set.
 *
 * @param[in] s1 Set being acted upon by permutation.
 * @param[in] p Permutation acting on set.
 * @param[in] m Number of setwords in set.
 */
extern int bit_permaut(const sets_t s1[static 1],
                       const uint32_t p[static 1], size_t m);

/**
 * @brief Check if cardinality of intersection of two sets is a given value.
 *
 * Check if cardinality of intersection of two sets is a given value.
 *
 * <strong>Unchecked Runtime Errors</strong>
 *    - It is undefined for <tt>s1</tt> and <tt>s2</tt> to be aliases.
 *
 * @param[in] s1 First conjunct of intersection.
 * @param[in] s2 Second conjunct of intersection.
 * @param[in] m Number of setwords in set.
 * @param[in] num Size of intersection to test.
 *
 * @retval int Returns 1 if intersection is size <tt>num</tt>. Returns -1
 * otherwise.
 */
static inline
int bit_intersect(const sets_t s1[restrict static 1],
                  const sets_t s2 [restrict static 1],
                  size_t m, size_t num)
{
  size_t count = 0;
  for ( size_t i = 0; i < m; i++ ) {
    count += _WINTERSECTSIZE(s1[i], s2[i]);
    if ( count > num ) return -1;
  }
  return 1;
}

/**
 * @brief Returns size of set.
 *
 * Returns size of set.
 *
 * @param[in] s Set being counted.
 * @param[in] m Number of setwords in set object.
 *
 * @retval size_t Number of elements in set.
 */
static inline
size_t bit_setsize(sets_t s[static 1], size_t m)
{
  if ( m == 1 ) return _POPCOUNT(*s);

  size_t count = 0;
  for ( long int i = m - 1; i >= 0; i-- ) count += _POPCOUNT(s[i]);
  return count;
}

# endif
