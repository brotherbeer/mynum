#if defined(_WIN32)
#include <windows.h>
#else
#include <cstdio>
#include <sys/time.h>
#endif
#include <cassert>
#include "mytheory.h"


namespace mynum {

void gcd(const number_t& a, const number_t& b, number_t& res)
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
}

void gcdext(const number_t& a, const number_t& b, number_t& x, number_t& y, number_t& g)
{
    if (!a.is_zero() && !b.is_zero())
    {
        number_t aa(a), bb(b);
        aa.set_abs();
        bb.set_abs();
        __extended_EUCLID(aa, bb, x, y, g);
    }
    else if (a.is_zero() && !b.is_zero())
    {
        g.assign(b);
        x.set_zero();
        y.set_one();
    }
    else if (b.is_zero() && !a.is_zero())
    {
        g.assign(a);
        x.set_one();
        y.set_zero();
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

word_t rand_word()
{
    return p_default_rng->gen();
}

bool rand(size_t maxbits, number_t& n)
{
    return rand(maxbits, *p_default_rng, n);
}

bool rand(size_t maxbits, RNG& rng, number_t& n)
{
    size_t m = maxbits % UNITBITS;
    size_t l = maxbits / UNITBITS + (m != 0);

    n.clear();
    n.reserve(l);
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
        tz = nd1.tzbits_count();
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
    // now, a is the GCD
}

void __extended_EUCLID(number_t& a, number_t& b, number_t& c, number_t& d, number_t& g)
{
    assert(a.is_pos() && b.is_pos());

    number_t px(1), py;
    number_t x, y(1);
    number_t q, r, t0, t1;

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
    g.steal(a);
    c.steal(px);
    d.steal(py);
}

void __pom_unit(unit_t a, const number_t& b, const number_t& c, number_t& res)
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

} // namespace end
