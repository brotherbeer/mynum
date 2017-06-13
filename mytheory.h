/* MYNUM LIBARAY HEADER */
#pragma once

#include "mynum.h"


namespace mynum {

int jacobi(const number_t& a, const number_t& b);

int gcd(const number_t& a, const number_t& b, number_t& res);
int gcd_ext(const number_t& a, const number_t& b, number_t& x, number_t& y, number_t& g);

void lcm(const number_t& a, const number_t& b, number_t& res);

int pom(const number_t& a, const number_t& b, const number_t& c, number_t& res);

bool prime_test_roughly(const number_t& n);
void prime_next_roughly(const number_t& n, number_t& res);
void prime_prev_roughly(const number_t& n, number_t& res);
bool MR_prime_test(const number_t& n, size_t times);

int inv(const number_t& a, const number_t& m, number_t& res);  // Modular multiplicative inverse

void bit_shift_or(const number_t& a, const number_t& b, size_t shift, number_t& res);
void bit_shift_xor(const number_t& a, const number_t& b, size_t shift, number_t& res);

size_t log2(const number_t& a);

struct NTT
{
    struct roots_pool_t
    {
        dunit_t* pool;
        dunit_t lgmax;

        roots_pool_t(): pool(NULL), lgmax(0)
        {}

       ~roots_pool_t();

        void init(const dunit_t W[], size_t lgm);

        bool valid() const
        {
            return pool != NULL;
        }

        const dunit_t* get(size_t i) const
        {
            return pool + (size_t(1) << i) - 1;
        }

        void release();
    };

    static const dunit_t P;
    static const dunit_t V;

    static const dunit_t W[];
    static const dunit_t RW[];
    static const dunit_t RN[];

    static roots_pool_t pool0;
    static roots_pool_t pool1;

    static void init_roots_pool();
    static void init_roots_pool(size_t lgm);
    static void reset_roots_pool(size_t lgm);
    static void release_roots_pool();

    static bool suitable(const number_t&);

    dunit_t* dat;
    size_t n;
    size_t lgn;
    size_t cap;

    NTT();

    NTT(const NTT&);

   ~NTT();

    void set_up(size_t);
    
    void release();

    void forward(const number_t&);

    void mul(const NTT&);

    void mul(const NTT&, NTT&);

    void backward();

    void to_number(number_t&);

    void __fft(const dunit_t[]);
    void __fft(const roots_pool_t&);
};

void fsqr(const number_t& a, number_t& res);
void fmul(const number_t& a, const number_t& b, number_t& res);

struct RNG;

RNG& default_RNG();
void set_default_RNG(RNG& rng);

word_t get_seed();

unit_t rand_unit();
word_t rand_word();
bool chance(size_t);

bool rand(size_t maxbits, number_t& n);
bool rand(size_t maxbits, bool holdmsb, number_t& n);
bool rand(size_t minbits, size_t maxbits, number_t& n);
bool rand(size_t maxbits, RNG& rng, number_t& n);
bool rand(size_t maxbits, RNG& rng, bool holdmsb, number_t& n);
bool rand(size_t minbits, size_t maxbits, RNG& rng, number_t& n);

bool rand(size_t length, const string_t& chars, string_t& s);
bool rand(size_t length, const string_t& chars, RNG& rng, string_t& s);

inline bool algined(void* p) { return ((word_t)p & (sizeof(word_t) - 1)) == 0; }

struct RNG  // Random Number Generator
{
    virtual word_t gen() = 0;

    virtual bool valid() const = 0;

    virtual bool gen_bytes(void* vp, size_t n);

    word_t operator () () { return gen(); }

    virtual ~RNG() {}

    bool chance(size_t x) { return x && (gen() % x == 1); }
};

template <word_t A, word_t C>
struct _LCG_t: public RNG  // Linear Congruential Generator
{
    word_t seed;

    _LCG_t(): seed(get_seed())
    {}

    _LCG_t(word_t s): seed(s)
    {}

    word_t gen()
    {
        return seed = A * seed + C;
    }

    bool gen_bytes(void* vp, size_t n)
    {
        byte_t* p = (byte_t*)vp;
        byte_t* e = p + n;
        for (; !algined(p) && n; n--)
        {
            *p++ = (seed = A * seed + C) & 0xff;
        }
        word_t* w = (word_t*)p;
        word_t* we = w + n / sizeof(word_t);
        while (w != we)
        {
            *w++ = seed = A * seed + C;
        }
        if ((p = (byte_t*)w) != e)
        {
            word_t x = seed = A * seed + C;
            while (p != e)
            {
                *p++ = (x >>= 8) & 0xff;
            }
        }
        return true;
    }

    bool valid() const
    {
        return true;
    }
};

template <word_t A, word_t C>
struct _XLCG_t: public RNG
{
    word_t seed;

    _XLCG_t(): seed(get_seed())
    {}

    _XLCG_t(word_t s): seed(s)
    {}

    word_t gen()
    {
        seed = A * seed + C;
        return seed ^ (seed >> UNITBITS);
    }

    bool gen_bytes(void* vp, size_t n)
    {
        byte_t* p = (byte_t*)vp;
        byte_t* e = p + n;

        for (; !algined(p) && n; n--)
        {
            seed = A * seed + C;
            *p++ = (seed >>= UNITBITS) & 0xff;
        }
        word_t* w = (word_t*)p;
        word_t* we = w + n / sizeof(word_t);
        while (w != we)
        {
            seed = A * seed + C;
            *w++ = seed ^ (seed >> UNITBITS);
        }
        for (p = (byte_t*)w; p != e; p++)
        {
            seed = A * seed + C;
            *p = (seed >>= UNITBITS) & 0xff;
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
typedef _XLCG_t<0x9d832a31, 0x11> XLCG_t;
#else
typedef _LCG_t<0x5851f42d4c957f2d, 0x11> LCG_t;
typedef _XLCG_t<0x5851f42d4c957f2d, 0x11> XLCG_t;
#endif

struct XORSP_t: public RNG  // XOR Shift Plus generator
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

struct CRNG_t: public RNG  // Cryptographical RNG
{
    word_t handle;

    CRNG_t();

   ~CRNG_t();

    word_t gen();

    bool gen_bytes(void* p, size_t n);

    bool valid() const;
};

word_t hash(const number_t& a);

/** inner functions */
void __EUCLID(number_t& a, number_t& b);
void __EUCLID_ext(number_t& a, number_t& b, number_t& c, number_t& d);
void __pom_unit(unit_t a, const number_t& b, const number_t& c, number_t& res);
void __pom(const number_t& a, const number_t& b, const number_t& c, number_t& res);
bool __MR_witness_unit(unit_t b, const number_t& n, const number_t& nd1, const number_t& u, size_t t);
bool __MR_witness(const number_t& b, const number_t& n, const number_t& nd1, const number_t& u, size_t t);

}
