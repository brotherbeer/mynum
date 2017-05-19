/* MYNUM LIBARAY SOURCE CODE
 *
 * This library provides addition, subtraction, multiplication, division,
 * modulo, exponentiation and bitwise operations for big integer arithmetic.
 * The author hopes mynum will be useful, but dose not make any warranty.
 *
 * If you have any questions, please contact <brotherbeer@163.com>
 */

#if defined(_WIN32)
#include <windows.h>
#else
#include <cstdio>
#include <sys/time.h>
#endif
#include <cassert>
#include "mytheory.h"


#if defined(_MSC_VER)
#define __force_inline(x) __forceinline x
#elif defined(__GNUC__)
#define __force_inline(x) __attribute__((always_inline)) inline x
#else
#define __force_inline(x) inline x
#endif

#if defined(_MSC_VER) && !defined(NO_INTRINSIC)
#include <intrin.h>
#endif

namespace mynum {

int jacobi(const number_t& a, const number_t& b)
{
    assert(b.is_odd());

    unit_t m;
    int t = 1, z;
    size_t tz;
    number_t u(a), v(b);
    UDM u8(8), u4(4);

    while (!u.is_zero())
    {
        m = v.absrem_unit(u8);
        z = m == 3 || m == 5? -1: 1;
        tz = u.tz_count();
        if (tz & 1)
        {
            t *= z;
        }
        u.shr(tz);
        if (u.absrem_unit(u4) == 3 && v.absrem_unit(u4) == 3)
        {
            t = -t;
        }
        swap(u, v);
        u.mod(v);
    }
    return v.is_one()? t: 0; 
}

int gcd(const number_t& a, const number_t& b, number_t& res)
{
    if (!a.is_zero() && !b.is_zero())
    {
        number_t tmp(b);
        res.assign(a);
        res.set_abs();
        tmp.set_abs();
        __EUCLID(res, tmp);
    }
    else if (a.is_zero() && !b.is_zero())
    {
        res.assign(b);
        res.set_abs();
    }
    else if (b.is_zero() && !a.is_zero())
    {
        res.assign(a);
        res.set_abs();
    }
    else
    {
        return 0;
    }
    return 1;
}

int gcd_ext(const number_t& a, const number_t& b, number_t& x, number_t& y, number_t& g)
{
    if (!a.is_zero() && !b.is_zero())
    {
        number_t b1(b);
        g.assign(a);
        g.set_abs();
        b1.set_abs();
        __EUCLID_ext(g, b1, x, y);
    }
    else if (a.is_zero() && !b.is_zero())
    {
        abs(b, g);
        x.set_zero();
        y.set_one();
    }
    else if (b.is_zero() && !a.is_zero())
    {
        abs(a, g);
        x.set_one();
        y.set_zero();
    }
    else
    {
        return 0;
    }

    if (a.is_neg())
    {
        x.set_neg();
    }
    if (b.is_neg())
    {
        y.set_neg();
    }
    return 1;
}

void lcm(const number_t& a, const number_t&b, number_t& res)
{
    number_t g;
    if (gcd(a, b, g))
    {
        kmul(a, b, res);
        res.div(g);
    }
    else
    {
        res.set_zero();
    }
}

int pom(const number_t& a, const number_t& b, const number_t& c, number_t& res)
{
    if (!c.is_zero() && !b.is_neg())
    {
        if (a.len && b.len)
        {
            __pom(a, b, c, res);
        }
        else if (!b.len)
        {
            res.assign(1 - (int)c.is_one());
        }
        else if (!a.len)
        {
            res.set_zero();
        }
        return 1;
    }
    return 0;
}

bool RNG::gen_bytes(void* vp, size_t n)
{
    byte_t* p = (byte_t*)vp;
    byte_t* e = p + n;
    word_t* w = (word_t*)p;
    word_t* we = w + n / sizeof(word_t);
    while (w != we)
    {
        *w++ = gen();
    }
    for (p = (byte_t*)w; p != e; p++)
    {
        *p = gen() & 0xff;
    }
    return true;
}

XORSP_t::XORSP_t(): s0(get_seed()), s1(s0)
{}

XORSP_t::XORSP_t(word_t seed): s0(seed), s1(s0)
{}

bool XORSP_t::gen_bytes(void* vp, size_t n)
{
    state_t x, y;
    byte_t *p = (byte_t*)vp, *e = p + n;

    for (; !algined(p) && n; n--, p++)
    {
        x = s0;
        y = s1;
        s0 = y;
        x ^= x << 23;
        s1 = x ^ y ^ (x >> 17) ^ (y >> 26);
        *p = (s1 + y) & 0xff;
    }
    word_t* w = (word_t*)p;
    word_t* we = w + n / sizeof(word_t);
    while (w != we)
    {
        x = s0;
        y = s1;
        s0 = y;
        x ^= x << 23;
        s1 = x ^ y ^ (x >> 17) ^ (y >> 26);
        *w++ = word_t(s1 + y);
    }
    for (p = (byte_t*)w; p != e; p++)
    {
        x = s0;
        y = s1;
        s0 = y;
        x ^= x << 23;
        s1 = x ^ y ^ (x >> 17) ^ (y >> 26);
        *p = (s1 + y) & 0xff;
    }
    return true;
}

#if defined(_WIN32)

unsigned long long __get_seed()
{
    LARGE_INTEGER PerfCnt;
    DWORD tid = GetCurrentThreadId();
    DWORD pid = GetCurrentProcessId();
    if (QueryPerformanceCounter(&PerfCnt))
    {
        PerfCnt.HighPart ^= tid;
        PerfCnt.HighPart ^= pid;
        return PerfCnt.QuadPart;
    }
    return 1;
}

CRNG_t::CRNG_t()
{
    if (!CryptAcquireContext((HCRYPTPROV*)&handle, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT))
    {
        handle = 0;
    }
}

CRNG_t::~CRNG_t()
{
    if (handle)
    {
        CryptReleaseContext((HCRYPTPROV)handle, 0);
    }
}

word_t CRNG_t::gen()
{
    word_t x;
    if (CryptGenRandom((HCRYPTPROV)handle, sizeof(x), (BYTE*)&x))
    {
        return x;
    }
    return 0;
}

bool CRNG_t::gen_bytes(void* p, size_t n)
{
    return CryptGenRandom((HCRYPTPROV)handle, (DWORD)n, (BYTE*)p) == TRUE;   
}

#else

unsigned long long __get_seed()
{
    struct timeval tv;
    if (gettimeofday(&tv, NULL) == 0)
    {
        return tv.tv_usec;
    }
    return 1;
}

CRNG_t::CRNG_t(): handle((word_t)fopen("/dev/urandom", "rb"))
{}

CRNG_t::~CRNG_t()
{
    if (handle)
    {
        fclose((FILE*)handle);
    }
}

word_t CRNG_t::gen()
{
    if (handle)
    {
        word_t x;
        fread(&x, sizeof(x), 1, (FILE*)handle);
        return x;
    }
    return 0;
}

bool CRNG_t::gen_bytes(void* p, size_t n)
{
    if (handle)
    {
        return fread(p, 1, n, (FILE*)handle) == n;
    }
    return false;
}

#endif

bool CRNG_t::valid() const
{
    return handle != 0;
}

word_t get_seed()
{
    static word_t n = 1;
    unsigned long long x = __get_seed() * n++;

    x ^= x << 16;
    x ^= x << 31;
    x ^= x >> 7;
    x ^= x << 17;
    if (x)
    {
        return (x * 0x2545F4914F6CDD3B) & WORDMAX;
    }
    return n;
}

static XORSP_t default_rng;
static RNG* p_default_rng = &default_rng;

RNG& default_RNG()
{
    return *p_default_rng;
}

void set_default_RNG(RNG& rng)
{
    p_default_rng = &rng;
}

unit_t rand_unit()
{
    return p_default_rng->gen() & UNITMAX;
}

bool chance(size_t x)
{
    return p_default_rng->chance(x);
}

word_t rand_word()
{
    return p_default_rng->gen();
}

bool rand(size_t maxbits, number_t& n)
{
    return rand(maxbits, default_RNG(), n);
}

bool rand(size_t maxbits, RNG& rng, number_t& n)
{
    size_t m = maxbits % UNITBITS;
    size_t l = maxbits / UNITBITS + (m != 0);

    n.clear_and_reserve(l);
    if (rng.gen_bytes(n.dat, l * sizeof(unit_t)))
    {
        if (m)
        {
            n.dat[l - 1] &= UNITMAX >> (UNITBITS - m);
        }
        __trim_leading_zeros(n.dat, l);
        n.len = l;
        return true;
    }
    return false;
}

bool rand(size_t maxbits, bool holdmsb, number_t& n)
{
    return rand(maxbits, default_RNG(), holdmsb, n);
}

bool rand(size_t maxbits, RNG& rng, bool holdmsb, number_t& n)
{
    if (!holdmsb)
    {
        return rand(maxbits, rng, n);
    }
    else if (!maxbits)
    {
        n.set_zero();
        return true;
    }
    else while (rand(maxbits, rng, n))
    {
        if (n.bit_at(maxbits - 1))
        {
            return true;
        }
    }
    return false;
}

bool rand(size_t length, const string_t& chars, string_t& s)
{
    return rand(length, chars, default_RNG(), s);
}

bool rand(size_t length, const string_t& chars, RNG& rng, string_t& s)
{
    if (length)
    {
        s.clear();
        s.reserve(length);
        while (length--)
        {
            s.append(chars[rng.gen() % chars.length()]);
        }
        return true;
    }
    return false;
}

static unit_t __SMALL[] = 
{
    0x003, 0x005, 0x007, 0x00b, 0x00d, 0x011, 0x013, 0x017,
    0x01d, 0x01f, 0x025, 0x029, 0x02b, 0x02f, 0x035, 0x03b,
    0x03d, 0x043, 0x047, 0x049, 0x04f, 0x053, 0x059, 0x061,
    0x065, 0x067, 0x06b, 0x06d, 0x071, 0x07f, 0x083, 0x089,
    0x08b, 0x095, 0x097, 0x09d, 0x0a3, 0x0a7, 0x0ad, 0x0b3,
    0x0b5, 0x0bf, 0x0c1, 0x0c5, 0x0c7, 0x0d3, 0x0df, 0x0e3,
    0x0e5, 0x0e9, 0x0ef, 0x0f1, 0x0fb, 0x101, 0x107, 0x10d,
    0x10f, 0x115, 0x119, 0x11b, 0x125, 0x133, 0x137, 0x139,
    0x13d, 0x14b, 0x151, 0x15b, 0x15d, 0x161, 0x167, 0x16f,
    0x175, 0x17b, 0x17f, 0x185, 0x18d, 0x191, 0x199, 0x1a3,
    0x1a5, 0x1af, 0x1b1, 0x1b7, 0x1bb, 0x1c1, 0x1c9, 0x1cd,
    0x1cf, 0x1d3, 0x1df, 0x1e7, 0x1eb, 0x1f3, 0x1f7, 0x1fd,
    0x209, 0x20b, 0x21d, 0x223, 0x22d, 0x233, 0x239, 0x23b,
    0x241, 0x24b, 0x251, 0x257, 0x259, 0x25f, 0x265, 0x269,
    0x26b, 0x277, 0x281, 0x283, 0x287, 0x28d, 0x293, 0x295,
    0x2a1, 0x2a5, 0x2ab, 0x2b3, 0x2bd, 0x2c5, 0x2cf, 0x2d7,
    0x2dd, 0x2e3, 0x2e7, 0x2ef, 0x2f5, 0x2f9, 0x301, 0x305,
    0x313, 0x31d, 0x329, 0x32b, 0x335, 0x337, 0x33b, 0x33d,
    0x347, 0x355, 0x359, 0x35b, 0x35f, 0x36d, 0x371, 0x373,
    0x377, 0x38b, 0x38f, 0x397, 0x3a1, 0x3a9, 0x3ad, 0x3b3,
    0x3b9, 0x3c7, 0x3cb, 0x3d1, 0x3d7, 0x3df, 0x3e5, 0x3f1,
    0x3f5, 0x3fb, 0x3fd, 0x407, 0x409, 0x40f, 0x419, 0x41b,
    0x425, 0x427, 0x42d, 0x43f, 0x443, 0x445, 0x449, 0x44f,
    0x455, 0x45d, 0x463, 0x469, 0x47f, 0x481, 0x48b, 0x493,
    0x49d, 0x4a3, 0x4a9, 0x4b1, 0x4bd, 0x4c1, 0x4c7, 0x4cd,
    0x4cf, 0x4d5, 0x4e1, 0x4eb, 0x4fd, 0x4ff, 0x503, 0x509,
    0x50b, 0x511, 0x515, 0x517, 0x51b, 0x527, 0x529, 0x52f,
    0x551, 0x557, 0x55d, 0x565, 0x577, 0x581, 0x58f, 0x593,
    0x595, 0x599, 0x59f, 0x5a7, 0x5ab, 0x5ad, 0x5b3, 0x5bf,
    0x5c9, 0x5cb, 0x5cf, 0x5d1, 0x5d5, 0x5db, 0x5e7, 0x5f3,
    0x5fb, 0x607, 0x60d, 0x611, 0x617, 0x61f, 0x623, 0x62b,
    0x62f, 0x63d, 0x641, 0x647, 0x649, 0x64d, 0x653, 0x655,
};

static const int __SMALL_SIZE = sizeof(__SMALL) / sizeof(unit_t);
static const int __SMALL_LAST = __SMALL[__SMALL_SIZE - 1];

bool __prime_test_roughly(const number_t& n)
{
    assert(n.is_pos() && n.is_odd());

    unit_t *p = __SMALL, *e;
    if (gt(n, __SMALL_LAST))
    {
        size_t top = n.bits_count() / 2;
        if (top > __SMALL_SIZE)
        {
            top = __SMALL_SIZE;
        }
        e = p + top;

        while (p != e) if (!n.absrem_unit(UDM(*p++)))
        {
            return false;
        }

        number_t nd1(n), u, fm;
        nd1--;
        size_t t = nd1.tz_count();
        u.assign(nd1, t, nd1.bits_count());

        if (__MR_witness_unit(2, n, nd1, u, t)) return false;
        if (__MR_witness_unit(31, n, nd1, u, t)) return false;
        if (__MR_witness_unit(181, n, nd1, u, t)) return false;
        return true;
    }
    else
    {
        e = p + __SMALL_SIZE;
        while (p != e) if (*n.dat == *p++)
        {
            return true;
        }
        return false;
    }
}

bool prime_test_roughly(const number_t& n)
{
    if (n.is_pos() && n.is_odd() && !n.is_one())
    {
        return __prime_test_roughly(n);
    }
    else if (n.is_two())
    {
        return true;
    }
    return false;
}

void prime_next_roughly(const number_t& n, number_t& res)
{
    if (gt(n, 1))
    {
        number_t m(n);
        m.add_unit(n.is_even()? 1: 2);
        while (!__prime_test_roughly(m))
        {
            m.add_unit(2);
        }
        res.steal(m);
    }
    else
    {
        res.assign(2);
    }
}

void prime_prev_roughly(const number_t& n, number_t& res)
{
    if (gt(n, 3))
    {
        number_t m(n);
        m.sub_unit(n.is_even()? 1: 2);
        while (!__prime_test_roughly(m))
        {
            m.sub_unit(2);
        }
        res.steal(m);
    }
    else if (eq(n, 3))
    {
        res.assign(2);
    }
}

bool MR_prime_test(const number_t & n, size_t times)
{
    if (gt(n, 3) && n.is_odd())
    {
        size_t nb, rb, tz;
        number_t nd1(n), u, base;

        nd1--;
        tz = nd1.tz_count();
        nb = n.bits_count();
        u.assign(nd1, tz, nd1.bits_count());

        while (times--)
        {
            do
            {
                rb = rand_word();
                rand(rb % nb, base);
            }
            while (base.is_zero() || base.is_one());

            if (__MR_witness(base, n, nd1, u, tz))
            {
                return false;
            }
        }
        return true;
    }
    else if(eq(n, 2) || eq(n, 3))
    {
        return true;
    }
    return false;
}

int inv(const number_t& a, const number_t& m, number_t& res)
{
    if (a.is_pos() && gt(m, 1))
    {
        number_t x, &px = res;
        number_t u(a), v(m);
        number_t q, r, t0, t1;

        px.set_one();
        while (!v.is_zero())
        {
            div(u, v, q, r);

            t0.steal(x);
            kmul(q, t0, t1);
            sub(px, t1, x);
            px.steal(t0);

            u.steal(v);
            v.steal(r);
        }

        if (u.is_one())
        {
            px.mod(m);
            if (px.is_neg())
            {
                px.add(m);
            }
            return 1;
        }
    }
    return 0;
}

word_t hash(const number_t& a)
{
    word_t hash = (word_t)a.len;

    for (const unit_t* p = a.unit(); p != a.unit_end(); p++)
    {
        hash = ((hash << 5) ^ (hash >> 27)) ^ *p;
    }
    return hash;
}

template <class OP>
void __bit_shift_op(const number_t& a, const number_t& b, size_t shift, OP& op, number_t& res)
{
    assert(res.is_not(b));

    res.assign(a);
    if (shift + b.bits_count() > res.bits_count())
    {
        res.bits_reserve(shift + b.bits_count());
        res.fill_unused_capacity(0);
    }

    const unit_t *pb = b.dat, *be = b.unit_end();
    unit_t *pr = res.dat + shift / UNITBITS, *re = res.unit_end();
    dunit_t dunit, carry = 0;
    size_t sh = shift % UNITBITS, lr = res.unit_count();

    for (; pb != be; pb++, pr++)
    {
        dunit = (dunit_t)*pb << sh;
        op(*pr, (carry | dunit) & UNITMAX);
        carry = dunit >> UNITBITS;
    }
    if (carry)
    {
        op(*pr++, carry);
    }
    if (pr > re)
    {
        lr = pr - res.dat;
    }
    __trim_leading_zeros(res.dat, lr);
    res.len = lr * res.sign();
}

struct __OR
{
    void operator () (unit_t& a, dunit_t b) { a |= b; }
};

void bit_shift_or(const number_t& a, const number_t& b, size_t shift, number_t& res)
{
    __OR op;
    if (b.is_not(res))
    {
        __bit_shift_op(a, b, shift, op, res);
    }
    else
    {
        number_t tmp(b);
        __bit_shift_op(a, tmp, shift, op, res);
    }
}

struct __XOR
{
    void operator () (unit_t& a, dunit_t b) { a ^= b; }
};

void bit_shift_xor(const number_t& a, const number_t& b, size_t shift, number_t& res)
{
    __XOR op;
    if (b.is_not(res))
    {
        __bit_shift_op(a, b, shift, op, res);
    }
    else
    {
        number_t tmp(b);
        __bit_shift_op(a, tmp, shift, op, res);
    }
}

size_t log2(const number_t& a)
{
    return a.bits_count() - 1;
}

#if UNITBITS == 32

const dunit_t P = 0xf600000000000001; // primitive root 7
const dunit_t v = 0x0a6810a6810a680f; // (B**2 - 1) / P - B  B is 1 << 64

const dunit_t W[] = {
    0xf600000000000000, 0x89b683c828b96fa5, 0xcb3aadaec85e0018, 0x24893e9ef1c93b2e,
    0xa877dbbf219d70dd, 0xeb2c25238a4522f9, 0xe2a5aa8f9bb3b563, 0x5ef0fe4bb75d1946,
    0x31a0ad62cc44e46a, 0xa31ac7a1cdec774b, 0x3c0fdc1fb444c680, 0xa6a942f163f1f351,
    0x7ef2001e6f6f9fe0, 0xa7575d8d03de4226, 0x2f3397990e49f7c6, 0x647fb6ae1a9f64a5,
    0xe261ff7e80f14ccd, 0x06663576c6fcdeef, 0xd30a32ed94ab6a04, 0x6d36f5f33316f6cf,
    0x5b04ed109714a96e, 0x0363f2e546035237, 0xc4012b83d7f91efe, 0x70044ddd5edcfbda,
};

const dunit_t RW[] = {
    0xf600000000000000, 0x6c497c37d746905c, 0x5c7a6bf1ed029a5c, 0xea70197b9fbe3d9d,
    0x2f6805fe995d4c9d, 0xd7c394079ba2d542, 0xdb2bce7bef4314ec, 0x2e46715506e0eec1,
    0x47e9aa55faf6293d, 0x073f98debf1b9387, 0x47d5b24732cd6fb0, 0xd13a0c3ba42661f2,
    0x0db7ff2bb18c1b20, 0xcaa2485f79a410ac, 0x357ebbb6d9cd4026, 0x12ce6a02c94a9df4,
    0x8e3dd2d5d0ac250c, 0x9285208621b6a8dd, 0xa9a71fc2a3e0b9c5, 0x1f5daaa7c0415626,
    0x073d3d99ee084e84, 0xaa611344e70d40a9, 0x6930e7c22bbd7f33, 0x6a8ce61ccbfc3c93,
};

const dunit_t RN[] = {
    0x7b00000000000001, 0xb880000000000001, 0xd740000000000001, 0xe6a0000000000001,
    0xee50000000000001, 0xf228000000000001, 0xf414000000000001, 0xf50a000000000001,
    0xf585000000000001, 0xf5c2800000000001, 0xf5e1400000000001, 0xf5f0a00000000001,
    0xf5f8500000000001, 0xf5fc280000000001, 0xf5fe140000000001, 0xf5ff0a0000000001,
    0xf5ff850000000001, 0xf5ffc28000000001, 0xf5ffe14000000001, 0xf5fff0a000000001,
    0xf5fff85000000001, 0xf5fffc2800000001, 0xf5fffe1400000001, 0xf5ffff0a00000001,
};

typedef unsigned short hunit_t;
const unit_t HUNITBASE = 65536;

#elif UNITBITS == 16

const dunit_t NTT::P = 0xc0000001; // primitive root 5
const dunit_t NTT::V = 0x55555553; // (B**2 - 1) / P - B  B is 1 << 32

const dunit_t NTT::W[] = {
    0xc0000000, 0x3c6f986f, 0x3d771377, 0x96990787,
    0x03fd4a80, 0x26d9e65a, 0x4d088bd6, 0x74bc4044,
    0x2d93ac34, 0x6e950ac8, 0x45ab3b3c, 0x0c97ec70,
    0x67620637, 0x56233758, 0x53c794bc, 0x962aa97d,
    0x6b4fb1ca, 0x50f7f530, 0x1e8119a0, 0xb36523bd,
    0x1ab6df09, 0xbdb3bc14, 0x0863aacf, 0x201a6811,
};

const dunit_t NTT::RW[] = {
    0xc0000000, 0x83906792, 0x248fd720, 0x2c2ecdc1,
    0xb8b2a5ce, 0x2a03bb71, 0x2d91ad89, 0xaefba6fd,
    0x920d56a5, 0x1fb8c962, 0xa8ac163d, 0x5eb4be40,
    0x36cbba5f, 0x677a2bea, 0xb2da7cbf, 0xa67c9122,
    0xa113d5de, 0x24a9f0a2, 0x7a850295, 0x24dc667e,
    0x2d7a6825, 0x27fb3db5, 0xae60de57, 0x466ffb17,
};

const dunit_t NTT::RN[] = {
    0x60000001, 0x90000001, 0xa8000001, 0xb4000001,
    0xba000001, 0xbd000001, 0xbe800001, 0xbf400001,
    0xbfa00001, 0xbfd00001, 0xbfe80001, 0xbff40001,
    0xbffa0001, 0xbffd0001, 0xbffe8001, 0xbfff4001,
    0xbfffa001, 0xbfffd001, 0xbfffe801, 0xbffff401,
    0xbffffa01, 0xbffffd01, 0xbffffe81, 0xbfffff41,
};

typedef byte_t hunit_t;
const unit_t HUNITBASE = 256;

#endif

static const byte_t __bit_rev_table[] = 
{
    0x00, 0x80, 0x40, 0xC0, 0x20, 0xA0, 0x60, 0xE0,
    0x10, 0x90, 0x50, 0xD0, 0x30, 0xB0, 0x70, 0xF0, 
    0x08, 0x88, 0x48, 0xC8, 0x28, 0xA8, 0x68, 0xE8,
    0x18, 0x98, 0x58, 0xD8, 0x38, 0xB8, 0x78, 0xF8, 
    0x04, 0x84, 0x44, 0xC4, 0x24, 0xA4, 0x64, 0xE4,
    0x14, 0x94, 0x54, 0xD4, 0x34, 0xB4, 0x74, 0xF4, 
    0x0C, 0x8C, 0x4C, 0xCC, 0x2C, 0xAC, 0x6C, 0xEC,
    0x1C, 0x9C, 0x5C, 0xDC, 0x3C, 0xBC, 0x7C, 0xFC, 
    0x02, 0x82, 0x42, 0xC2, 0x22, 0xA2, 0x62, 0xE2,
    0x12, 0x92, 0x52, 0xD2, 0x32, 0xB2, 0x72, 0xF2, 
    0x0A, 0x8A, 0x4A, 0xCA, 0x2A, 0xAA, 0x6A, 0xEA,
    0x1A, 0x9A, 0x5A, 0xDA, 0x3A, 0xBA, 0x7A, 0xFA,
    0x06, 0x86, 0x46, 0xC6, 0x26, 0xA6, 0x66, 0xE6,
    0x16, 0x96, 0x56, 0xD6, 0x36, 0xB6, 0x76, 0xF6, 
    0x0E, 0x8E, 0x4E, 0xCE, 0x2E, 0xAE, 0x6E, 0xEE,
    0x1E, 0x9E, 0x5E, 0xDE, 0x3E, 0xBE, 0x7E, 0xFE,
    0x01, 0x81, 0x41, 0xC1, 0x21, 0xA1, 0x61, 0xE1,
    0x11, 0x91, 0x51, 0xD1, 0x31, 0xB1, 0x71, 0xF1,
    0x09, 0x89, 0x49, 0xC9, 0x29, 0xA9, 0x69, 0xE9,
    0x19, 0x99, 0x59, 0xD9, 0x39, 0xB9, 0x79, 0xF9, 
    0x05, 0x85, 0x45, 0xC5, 0x25, 0xA5, 0x65, 0xE5,
    0x15, 0x95, 0x55, 0xD5, 0x35, 0xB5, 0x75, 0xF5,
    0x0D, 0x8D, 0x4D, 0xCD, 0x2D, 0xAD, 0x6D, 0xED,
    0x1D, 0x9D, 0x5D, 0xDD, 0x3D, 0xBD, 0x7D, 0xFD,
    0x03, 0x83, 0x43, 0xC3, 0x23, 0xA3, 0x63, 0xE3,
    0x13, 0x93, 0x53, 0xD3, 0x33, 0xB3, 0x73, 0xF3, 
    0x0B, 0x8B, 0x4B, 0xCB, 0x2B, 0xAB, 0x6B, 0xEB,
    0x1B, 0x9B, 0x5B, 0xDB, 0x3B, 0xBB, 0x7B, 0xFB,
    0x07, 0x87, 0x47, 0xC7, 0x27, 0xA7, 0x67, 0xE7,
    0x17, 0x97, 0x57, 0xD7, 0x37, 0xB7, 0x77, 0xF7, 
    0x0F, 0x8F, 0x4F, 0xCF, 0x2F, 0xAF, 0x6F, 0xEF,
    0x1F, 0x9F, 0x5F, 0xDF, 0x3F, 0xBF, 0x7F, 0xFF,
};

static __force_inline(size_t) __log2(slen_t x);
static __force_inline(dunit_t) __mul_mod_P(dunit_t x, dunit_t y);
static __force_inline(dunit_t) __add_mod_P(dunit_t x, dunit_t y);

NTT::roots_pool_t::roots_pool_t(const dunit_t roots[])
{
    int s = 1, i = 0;
    dunit_t *p, *e, w, x;

    pool = new dunit_t[65535];
    while (s < 65536)
    {
        x = 1;
        w = roots[i++];
        p = pool + s - 1;
        e = p + s;   
        for (*p++ = 1; p != e; p++)
        {
            *p = x = __mul_mod_P(x, w);
        }
        s <<= 1;
    }
}

NTT::roots_pool_t::~roots_pool_t()
{
    delete[] pool;
}

NTT::roots_pool_t* NTT::pool0 = NULL;
NTT::roots_pool_t* NTT::pool1 = NULL;

void NTT::init_roots_pool()
{
    pool0 = new roots_pool_t(W);
    pool1 = new roots_pool_t(RW);
}

void NTT::release_roots_pool()
{
    delete pool0;
    delete pool1;
    pool0 = pool1 = NULL;
}

NTT::NTT(): dat(NULL), n(0), lgn(0)
{}

NTT::~NTT()
{
    release();
}

void NTT::release()
{
    mem::deallocate(dat);
    n = lgn = 0;
}

void NTT::forward(const number_t& a)
{
    slen_t l = a.unit_count();
    hunit_t* p = (hunit_t*)a.dat;
    hunit_t* e = p + l * 2;

    n = l * 2 * 2;
    lgn = __log2(n);
    if (n & (n - 1))
    {
        lgn++;
        n = 1 << lgn;
    }

    sig = a.sign();
    dat = (dunit_t*)mem::allocate(n, sizeof(dunit_t));
    memset(dat, 0, n * sizeof(dunit_t));

    size_t i, ih, il, s = 16 - lgn;
    for (i = 0; p != e; i++, p++)
    {
        ih = __bit_rev_table[i >> 8];
        il = __bit_rev_table[i & 0xff];
        dat[((il << 8) | ih) >> s] = *p;
    }
    if (pool0)
    {
        __fft(pool0);
    }
    else
    {
        __fft(W);
    }
}

void NTT::mul(const NTT& another)
{
    dunit_t* p = dat;
    dunit_t* e = dat + n;
    dunit_t* q = another.dat;

    for (; p != e; p++, q++)
    {
        *p = __mul_mod_P(*p, *q);
    }
}

void NTT::backward()
{
    dunit_t tmp;
    size_t i, ih, il, k;
    size_t s = 16 - lgn;

    for (i = 0; i < n; i++)
    {
        ih = __bit_rev_table[i >> 8];
        il = __bit_rev_table[i & 0xff];
        k = ((il << 8) | ih) >> s;
        if (i > k)
        {
            tmp = dat[i];
            dat[i] = dat[k];
            dat[k] = tmp;
        }
    }
    if (pool1)
    {
        __fft(pool1);
    }
    else
    {
        __fft(RW);
    }

    dunit_t R = RN[lgn - 1];
    dunit_t* p = dat;
    dunit_t* e = dat + n;
    for (; p != e; p++)
    {
        *p = __mul_mod_P(*p, R);
    }
}

void NTT::to_number(number_t& res)
{
    slen_t len = n / 2;
    unit_t *tmp = (unit_t*)mem::allocate(len, sizeof(unit_t));
    hunit_t *p = (hunit_t*)tmp;
    dunit_t *q = dat, *e = q + n, carry = 0;

    while (q != e)
    {
        carry += *q++;
        *p++ = carry % HUNITBASE;
        carry /= HUNITBASE;
    }

    __trim_leading_zeros(tmp, len);
    res.release();
    res.len = len;
    res.dat = tmp;
}

void NTT::__fft(const dunit_t root[])
{
    size_t k, s, h;
    dunit_t *p, *q, *e;
    dunit_t wm, m, t, w, u;

    for (s = 0; s < lgn; s++)
    {
        h = 1 << s;
        m = h << 1;
        wm = root[s];
        for (k = 0; k < n; k += m)
        {
            w = 1;

            p = dat + k;
            e = p + h;
            for (; p != e; p++)
            {
                u = *p;
                q = p + h;

                t = __mul_mod_P(w, *q);
               *p = __add_mod_P(u, t);
               *q = __add_mod_P(u, P - t);
                w = __mul_mod_P(w, wm);
            }
        }
    }
}

void NTT::__fft(const roots_pool_t* pool)
{
    size_t k, s, h;
    dunit_t m, t, w, u;
    dunit_t *p, *q, *e;
    const dunit_t *pp, *pw;

    p = dat;
    e = p + n;
    while (p != e)
    {
        q = p + 1;
        u = *p;
        t = *q;
       *p = __add_mod_P(u, t);
       *q = __add_mod_P(u, P - t);
        p += 2;
    }

    for (s = 1; s < lgn; s++)
    {
        pp = pool->get(s);

        h = 1 << s;
        m = h << 1;
        for (k = 0; k < n; k += m)
        {
            w = 1;
            pw = pp;
            p = dat + k;
            e = p + h;
            for (; p != e; p++)
            {
                u = *p;
                q = p + h;

                t = __mul_mod_P(w, *q);
               *p = __add_mod_P(u, t);
               *q = __add_mod_P(u, P - t);
                w = *++pw;
            }
        }
    }
}

void fsqr(const number_t& a, number_t& res)
{
    NTT ntta;

    ntta.forward(a);
    ntta.mul(ntta);
    ntta.backward();
    ntta.to_number(res);
}

void fmul(const number_t& a, const number_t& b, number_t& res)
{
    NTT ntta, nttb;

    ntta.forward(a);
    nttb.forward(b);
    ntta.mul(nttb);
    ntta.backward();
    ntta.to_number(res);
}

void __EUCLID(number_t& a, number_t& b)
{
    assert(a.is_pos() && b.is_pos());

    number_t tmp;
    if (cmp(a, b) < 0)
    {
        swap(a, b);
    }
    while (!b.is_zero())
    {
        mod(a, b, tmp);
        a.steal(b);
        b.steal(tmp);
    }
}

void __EUCLID_ext(number_t& a, number_t& b, number_t& px, number_t& py)
{
    assert(a.is_pos() && b.is_pos());

    number_t x, y(1);
    number_t q, r, t0, t1;

    px.set_one();
    py.set_zero();
    while (!b.is_zero())
    {
        div(a, b, q, r);

        t0.steal(x);
        kmul(q, t0, t1);
        sub(px, t1, x);
        px.steal(t0);

        t0.steal(y);
        kmul(q, t0, t1);
        sub(py, t1, y);
        py.steal(t0);

        a.steal(b);
        b.steal(r);
    }
}

void __pom_unit(unit_t a, const number_t& b, const number_t& c, number_t& res)
{
    assert(a && b.is_pos() && !c.is_zero());

    number_t r(1);
    const unit_t* p = b.unit_last();
    const unit_t* e = b.unit_rend();
    unit_t i, i0 = 1 << (UNITBITS - 1);

    for (; p != e; p--)
    {
        for (i = i0; i != 0; i >>= 1)
        {
            r.ksqr();
            r.mod(c);
            if (*p & i)
            {
                r.mul_unit(a);
                r.mod(c);
            }
        }
    }
    res.steal(r);
}

void __pom(const number_t& a, const number_t& b, const number_t& c, number_t& res)
{
    assert(a.len && b.is_pos() && !c.is_zero());

    number_t r(1), m(a);
    const unit_t *p = b.unit_last();
    const unit_t *e = b.unit_rend();
    unit_t i, i0 = 1 << (UNITBITS - 1);

    m.mod(c);
    for (; p != e; p--)
    {
        for (i = i0; i != 0; i >>= 1)
        {
            r.ksqr();
            r.mod(c);
            if (*p & i)
            {
                r.kmul(m);
                r.mod(c);
            }
        }
    }
    res.steal(r);
}

bool __MR_witness(const number_t& b, const number_t& n, const number_t& nd1, const number_t& u, size_t t)
{
    assert(lt(b, n));
    assert(!b.is_zero() && !b.is_one());

    bool cond;
    number_t x;

    __pom(b, u, n, x);
    for (size_t i = 1; i <= t; i++)
    {
        cond = !x.is_one() && neq(x, nd1);
        x.ksqr();
        x.mod(n);
        if (x.is_one() && cond)
        {
            return true;
        }
    }
    if (!x.is_one())
    {
        return true;
    }
    return false;
}

bool __MR_witness_unit(unit_t b, const number_t& n, const number_t& nd1, const number_t& u, size_t t)
{
    bool cond;
    number_t x;

    __pom_unit(b, u, n, x);
    for (size_t i = 1; i <= t; i++)
    {
        cond = !x.is_one() && neq(x, nd1);
        x.ksqr();
        x.mod(n);
        if (x.is_one() && cond)
        {
            return true;
        }
    }
    if (!x.is_one())
    {
        return true;
    }
    return false;
}

#if defined(__GNUC__) && !defined(NO_INTRINSIC)

size_t __log2(slen_t x)
{
    assert(x > 0);

    return 32 - __builtin_clz((unsigned int)x) - 1;
}

#elif defined(_MSC_VER) && !defined(NO_INTRINSIC)

#if UNITBITS == 16

size_t __log2(slen_t x)
{
    assert(x > 0);

    unsigned long b;
    _BitScanReverse(&b, x);
    return b;
}

dunit_t __mul_mod_P(dunit_t x, dunit_t y)
{
    unsigned __int64 U, Q;
    dunit_t u1, u0, q1, q0, r;
    const dunit_t P = NTT::P;
    const dunit_t V = NTT::V;

    U = __emulu(x, y);
    u1 = U >> 32;
    u0 = U & 0xFFFFFFFF;

    Q = __emulu(V, u1) + U;   
    q1 = Q >> 32;
    q0 = Q & 0xFFFFFFFF;

    r = u0 - ++q1 * P;

    if (r > q0)
    {
        r += P;
    }
    if (r >= P)
    {
        r -= P;
    }
    return r;
}

dunit_t __add_mod_P(dunit_t x, dunit_t y)
{
    dunit_t z = NTT::P - y;
    return x - z + NTT::P * (z > x);
}

#elif UNITBITS == 32

size_t __log2(slen_t x)
{
    assert(x > 0);

    unsigned long b;
    _BitScanReverse64(&b, x);
    return b;
}

#endif

#else

// TODO

#endif

} // namespace end
