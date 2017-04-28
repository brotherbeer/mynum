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
    res.len = lr * sign(res);
}

struct __OR
{
    void operator () (unit_t& a, dunit_t b) { a |= b; }
};

void bit_shift_or(const number_t& a, const number_t& b, size_t shift, number_t& res)
{
    __OR op;
    __bit_shift_op(a, b, shift, op, res);
}

struct __XOR
{
    void operator () (unit_t& a, dunit_t b) { a ^= b; }
};

void bit_shift_xor(const number_t& a, const number_t& b, size_t shift, number_t& res)
{
    __XOR op;
    __bit_shift_op(a, b, shift, op, res);
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

} // namespace end
