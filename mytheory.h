#pragma once

#include "mynum.h"


namespace mynum {
    
void gcd(const number_t& a, const number_t& b, number_t& res);
void gcdext(const number_t& a, const number_t& b, number_t& x, number_t& y, number_t& g);

int pom(const number_t& a, const number_t& b, const number_t& c, number_t& res);

bool prime_test_roughly(const number_t& n);
void prime_next_roughly(const number_t& n, number_t& res);
void prime_prev_roughly(const number_t& n, number_t& res);

bool MR_prime_test(const number_t& n, size_t times);

struct RNG;

RNG& default_RNG();
void set_default_RNG(RNG& rng);

word_t get_seed();

unit_t rand_unit();
unit_t rand_unit(RNG& rng);
word_t rand_word();
word_t rand_word(RNG& rng);

bool rand(size_t maxbits, number_t& n);
bool rand(size_t maxbits, RNG& rng, number_t& n);
bool rand(size_t bits, string_t& s, const string_t& chars, RNG& rng);

struct RNG  // random number generator interface
{
    virtual word_t gen() = 0;

    virtual bool valid() const = 0;

    virtual bool gen_bytes(void* vp, size_t n);

    word_t operator () () { return gen(); }

    virtual ~RNG() {}
};

template <word_t A, word_t C> struct _LCG_t: public RNG
{
    word_t seed;

    _LCG_t(): seed(get_seed())
    {}

    _LCG_t(word_t s): seed(s)
    {}

    word_t gen()
    {
        return seed = A * seed + C;  // M = 1 << WORDBITS
    }

    bool gen_bytes(void* vp, size_t n)
    {
        size_t nw;
        word_t *pw, *pwe;
        byte_t *p = (byte_t*)vp, *pe;

        for (; n && ((word_t)p & (sizeof(word_t) - 1)) != 0; n--, p++)
        {
            seed = A * seed + C;
            *p = seed & 0xff;
        }

        pe = p + n;
        if ((nw = n / sizeof(word_t)))
        {
            pw = (word_t*)p;
            pwe = pw + nw;
            while (pw != pwe)
            {
                seed = A * seed + C;
                *pw++ = seed;
            }
            p = (byte_t*)pw;
        }
        while (p != pe)
        {
            seed = A * seed + C;
            *p++ = seed & 0xff;
        }
        return true;
    }

    bool valid() const
    {
        return true;
    }
};

#if UNITBITS == 16
typedef _LCG_t<0x9d832a31, 0x11> LCG_t;
#else
typedef _LCG_t<0x5851f42d4c957f2d, 0x1> LCG_t;
#endif

// Vigna, Sebastiano (April 2014). "Further scramblings of Marsaglia's xorshift generators"
// http://vigna.di.unimi.it/ftp/papers/xorshiftplus.pdf

struct XORSP_t: public RNG   // xor shift plus
{
    typedef unsigned long long state_t;

    state_t s0, s1;

    XORSP_t();

    XORSP_t(word_t seed);

    word_t gen()
    {
	    state_t x = s0, y = s1;
	    s0 = y;
	    x ^= x << 23;
	    s1 = x ^ y ^ (x >> 17) ^ (y >> 26);
	    return word_t(s1 + y);
    }

    bool gen_bytes(void* vp, size_t n);

    bool valid() const
    {
        return true;
    }
};

struct SRNG_t: public RNG
{
    word_t handle;

    SRNG_t();

    ~SRNG_t();

    word_t gen();

    bool gen_bytes(void* p, size_t n);

    bool valid() const;
};

/** inner functions */
void __EUCLID(number_t& a, number_t& b);
void __extended_EUCLID(number_t& a, number_t& b, number_t& c, number_t& d, number_t& g);
void __pom_unit(unit_t a, const number_t& b, const number_t& c, number_t& res);
void __pom(const number_t& a, const number_t& b, const number_t& c, number_t& res);
bool __MR_witness_unit(unit_t b, const number_t& n, const number_t& nd1, const number_t& u, size_t t);
bool __MR_witness(const number_t& b, const number_t& n, const number_t& nd1, const number_t& u, size_t t);
}
