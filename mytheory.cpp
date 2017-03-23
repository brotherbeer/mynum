#include <ctime>
#include <cassert>
#if defined(_MSC_VER)
#include <windows.h>
#endif
#include "mytheory.h"


namespace mynum {

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
    size_t nw;
    word_t *pw, *pwe;
    byte_t *p = (byte_t*)vp, *pe;

    for (; n && ((word_t)p & (sizeof(word_t) - 1)) != 0; n--, p++)
    {
        *p = gen() & 0xff;
    }

    pe = p + n;
    if ((nw = n / sizeof(word_t)))
    {
        pw = (word_t*)p;
        pwe = pw + nw;
        while (pw != pwe)
        {
            *pw++ = gen();
        }
        p = (byte_t*)pw;
    }
    while (p != pe)
    {
        *p++ = gen() & 0xff;
    }
    return true;
}

XORSP_t::XORSP_t(): s0((state_t)time(NULL)), s1(s0)
{}

XORSP_t::XORSP_t(word_t seed): s0(seed), s1(s0)
{}

bool XORSP_t::gen_bytes(void* vp, size_t n)
{
    size_t nw;
    word_t *pw, *pwe;
    byte_t *p = (byte_t*)vp, *pe;
    state_t x, y;

    for (; n && ((word_t)p & (sizeof(word_t) - 1)) != 0; n--, p++)
    {
        x = s0;
        y = s1;
        s0 = y;
        x ^= x << 23;
        s1 = x ^ y ^ (x >> 17) ^ (y >> 26);
        *p = (s1 + y) & 0xff;
    }

    pe = p + n;
    if ((nw = n / sizeof(word_t)))
    {
        pw = (word_t*)p;
        pwe = pw + nw;
        while (pw != pwe)
        {
            x = s0;
            y = s1;
            s0 = y;
            x ^= x << 23;
            s1 = x ^ y ^ (x >> 17) ^ (y >> 26);
            *pw++ = word_t(s1 + y);
        }
        p = (byte_t*)pw;
    }
    while (p != pe)
    {
        x = s0;
        y = s1;
        s0 = y;
        x ^= x << 23;
        s1 = x ^ y ^ (x >> 17) ^ (y >> 26);
        *p++ = (s1 + y) & 0xff;
    }
    return true;
}

SRG_t::SRG_t()
{
    if (!CryptAcquireContext((HCRYPTPROV*)&handle, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT))
    {
        handle = 0;
    }
}

SRG_t::~SRG_t()
{
    if (handle)
    {
        CryptReleaseContext((HCRYPTPROV)handle, 0);
    }
}

word_t SRG_t::gen()
{
    word_t x;
    if (CryptGenRandom((HCRYPTPROV)handle, sizeof(x), (BYTE*)&x))
    {
        return x;
    }
    return 0;
}

bool SRG_t::gen_bytes(byte_t* p, size_t n)
{
    return CryptGenRandom((HCRYPTPROV)handle, (DWORD)n, p) == TRUE;   
}

bool SRG_t::valid() const
{
    return handle != 0;
}

static LCG_t default_rng((word_t)time(NULL));
static RNG* p_default_rng = &default_rng;

RNG& get_default_RNG()
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

unit_t rand_unit(RNG& rng)
{
    return rng.gen() & UNITMAX;
}

word_t rand_word()
{
    return p_default_rng->gen();
}

word_t rand_word(RNG& rng)
{
    return rng.gen();
}

bool rand(size_t bits, number_t& n, bool holdmsb)
{
    return rand(bits, n, *p_default_rng, holdmsb);
}

bool rand(size_t bits, number_t& n, RNG& rng, bool holdmsb)
{
    size_t m = bits % UNITBITS;
    size_t l = bits / UNITBITS + (m != 0);

    if (l)
    {
        n.clear();
        n.reserve(l);
        if (rng.gen_bytes(n.dat, l * sizeof(unit_t)))
        {
            if (m)
            {
                n.dat[l - 1] &= UNITMAX >> (UNITBITS - m);
            }
            if (holdmsb)
            {
                n.bit_set_one(bits - 1);
            }
            __trim_leading_zeros(n.dat, l);
            n.len = l;
            return true;
        }
    }
    return false;
}

bool rand(size_t bits, string_t& s, const string_t& chars, RNG& rng)
{
    if (bits)
    {
        s.clear();
        s.reserve(bits);
        while (bits--)
        {
            s.append(chars[rng.gen() % chars.length()]);
        }
        return true;
    }
    return false;
}

static unit_t __SMALL[] = 
{
    3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59,
    61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127,
    131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191,
    193, 197, 199, 211, 223, 227, 229, 233,
};

static const int __SMALL_SIZE = sizeof(__SMALL) / sizeof(unit_t);
static const int __SMALL_LAST = __SMALL[__SMALL_SIZE - 1];

bool __prime_test_roughly(const number_t& n)
{
    assert(n.is_pos() && n.is_odd());

    unit_t *p = __SMALL, *e;
    if (gt(n, __SMALL_LAST))
    {
        e = p + __SMALL_SIZE;
        while (p != e) if (!n.absrem_unit(UDM(*p++)))
        {
            return false;
        }

        number_t nd1(n), u, fm;
        nd1--;
        size_t t = nd1.tzbits_count();
        u.assign(nd1, t, nd1.bits_count());

        if (__MR_witness_unit(2, n, nd1, u, t)) return false;
        if (__MR_witness_unit(31, n, nd1, u, t)) return false;
        if (__MR_witness_unit(181, n, nd1, u, t)) return false;
        return true;
    }
    else
    {
        e = p + __SMALL_SIZE;
        while (p != e) if (n.dat[0] == *p++)
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
    else if (eq(n, 2))
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


void __pom(unit_t a, const number_t& b, const number_t& c, number_t& res)
{
    assert(a && b.is_pos() && !c.is_zero());

    number_t r(1);
    unit_t* p = b.dat + b.len - 1;
    unit_t* e = b.dat - 1, i, i0 = 1 << (UNITBITS - 1);

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
    unit_t *p = b.dat + b.len - 1;
    unit_t *e = b.dat - 1, i, i0 = 1 << (UNITBITS - 1);

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

bool __MR_witness_unit(unit_t b, const number_t& n, const number_t& nd1, const number_t& u, size_t t)
{
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

} // namespace end
