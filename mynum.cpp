/* MYNUM LIBARAY SOURCE CODE
 * 
 * This library provides addition, subtraction, multiplication, division,
 * modulo, involution, power, root  and bit operations  of large integers.
 * No restrictions on the dissemination and modification of this source code,
 * but should accord with socially beneficial purpose. 
 * This library is distributed in the hope that it will be useful, but without any warranty,
 * if you have any questions, please contact <brotherbeer@163.com>
 *
 * NOTICE!! This library is currently only available on the LITTLE-ENDIAN machines.
 */

#include <cstdlib>
#include <cstring>
#include <cmath>
#include "mynum.h"

#ifdef __GNUC__
#define __always_inline(x)  __attribute__((always_inline)) x
#elif defined(_MSC_VER)
#define __always_inline(x)  __forceinline x
#endif

namespace mynum {

const float LN_8 = 2.0794f;   // log(8)
const float LN_10 = 2.3026f;  // log(10)
const unit_t  SHIFT = sizeof(unit_t) << 3;
const dunit_t BASE = (dunit_t)1 << SHIFT;
const unit_t  MASK = ~unit_t(0);
const int KMUL_THRESHOLD = 80;
const int KSQR_THRESHOLD = 120;

#if UNITBITS == 16
const int INNERDEC_BASE_DIGITS = 4;
const int INNEROCT_BASE_DIGITS = 5;
const unit_t INNERDEC_BASE = 10000;
const unit_t INNEROCT_BASE = 0100000;
const float LN_BASE = 11.0903f;            // log(BASE) 
const float LN_INNERDEC_BASE = 9.2103f;    // log(INNERDEC_BASE)
const float LN_INNEROCT_BASE = 10.3972f;   // log(INNEROCT_BASE)

#elif UNITBITS == 32
const int INNERDEC_BASE_DIGITS = 9;
const int INNEROCT_BASE_DIGITS = 10;
const unit_t INNERDEC_BASE = 1000000000;
const unit_t INNEROCT_BASE = 010000000000;
const float LN_BASE = 22.1807f;
const float LN_INNERDEC_BASE = 20.7233f;
const float LN_INNEROCT_BASE = 20.7944f;
#endif

typedef unsigned char byte_t;

struct mem 
{
    static __always_inline(void*) allocate(size_t s, size_t u)
    {
        return malloc(s * u);
    }

    static __always_inline(void) deallocate(void* p)
    {
        free(p);
    }

private:
    mem() {}
    mem(const mem&) {}
    ~mem() {}
};

__always_inline(int) __max_base();

int max_base()
{
    return __max_base();
}

struct _radix_t
{
    unit_t base;
    unit_t inner_base_digits;
    unit_t inner_base;
    float ln_base;
    float ln_inner_base;

    _radix_t(int n): base(n), inner_base_digits(1), inner_base(n)
    {
        assert(n > 2 && n <= __max_base());

        while ((dunit_t)inner_base * base < (dunit_t)MASK)
        {
            inner_base_digits++;
            inner_base *= base;
        }
        ln_base = log((float)base);
        ln_inner_base = log((float)inner_base);
    }
};

static __always_inline(unit_t*) __allocate_units(slen_t s)
{
    return (unit_t*)mem::allocate(s, sizeof(unit_t));
}

static __always_inline(void) __deallocate_units(unit_t* p)
{
    return mem::deallocate(p);
}

static __always_inline(unit_t*) __set_units_zero(unit_t* p, slen_t l)
{
    return (unit_t*)memset(p, 0, l * sizeof(unit_t));
}

static __always_inline(unit_t*) __copy_units(unit_t* d, const unit_t* s, slen_t l)
{
    return (unit_t*)memcpy(d, s, l * sizeof(unit_t));
}

static __always_inline(int) __char_digit(char c);
static __always_inline(bool) __char_digit_valid(char c, int base);
static __always_inline(slen_t) __vbits_count(unit_t x);

#define __sign_shift(x) ((x) >> ((sizeof(slen_t) << 3) - 1))

static __always_inline(slen_t) __abs(slen_t x)
{
    slen_t y = __sign_shift(x);
    return (x + y) ^ y;
}

static __always_inline(slen_t) __sign(slen_t x)
{
    return __sign_shift(x) | 1;
}

static __always_inline(slen_t) __sign(slen_t x, slen_t y)
{
    return __sign_shift(x ^ y) | 1;
}

static __always_inline(bool) __same_sign(slen_t x, slen_t y)
{
    return __sign_shift(x ^ y) == 0;
}

/** class number_t implementation */

#define __trim_leading_zeros(dat, len) do\
    {\
        const unit_t *e = dat - 1, *p = e + __abs(len); \
        while (p != e && !*p) {p--;} \
        len = slen_t((p - e) * __sign(len)); \
    } while (0)

number_t::number_t(const char* s)
{
    if (s)
    {
        slen_t l = (slen_t)strlen(s);
        if (l > 0)
        {
            __construct_from_xbase_string(s, l, 10, LN_10, INNERDEC_BASE, INNERDEC_BASE_DIGITS);
            __trim_leading_zeros(dat, len);
        }
    }
}

number_t::number_t(const char* s, int base)
{
    if (s && base <= __max_base())
    {
        slen_t l = (slen_t)strlen(s);
        if (l > 0)
        {
            __construct_from_string(s, l, base);
            __trim_leading_zeros(dat, len);
        }
    }
}

number_t::number_t(const char* s, size_t l, int base)
{
    if (s && slen_t(l) > 0 && base <= __max_base())
    {
        __construct_from_string(s, l, base);
        __trim_leading_zeros(dat, len);
    }
}

number_t::number_t(const number_t& another)
{
    __copy(another);
}

number_t& number_t::assign(const number_t& x)     { copy(x); return *this; }
number_t& number_t::assign(int x)                 { __release(); __assign(x); return *this;  }
number_t& number_t::assign(long x)                { __release(); __assign(x); return *this;  }
number_t& number_t::assign(long long x)           { __release(); __assign(x); return *this;  }
number_t& number_t::assign(unsigned int x)        { __release(); __uassign(x); return *this; }
number_t& number_t::assign(unsigned long x)       { __release(); __uassign(x); return *this; }
number_t& number_t::assign(unsigned long long x)  { __release(); __uassign(x); return *this; }

number_t& number_t::assign(const char* s, int base)
{
    __release();
    if (s && base <= __max_base())
    {
        slen_t l = (slen_t)strlen(s);
        if (l > 0)
        {
            __construct_from_string(s, l, base);
            __trim_leading_zeros(dat, len);
        }
    }
    return *this;
}

number_t& number_t::assign(const char* s, size_t l, int base)
{
    __release();
    if (s && slen_t(l) > 0 && base <= __max_base())
    {
        __construct_from_string(s, l, base);
        __trim_leading_zeros(dat, len);
    }
    return *this;
}

number_t::~number_t()
{
    __release();
}

string_t number_t::to_bin_string() const
{
    string_t res;
    return __to_bin_string(res);
}

string_t& number_t::to_bin_string(string_t& res) const
{
    return __to_bin_string(res);
}

string_t number_t::to_oct_string() const
{
    string_t res;
    return __to_xbase_string(res, 8, INNEROCT_BASE, INNEROCT_BASE_DIGITS, LN_INNEROCT_BASE);
}

string_t& number_t::to_oct_string(string_t& res) const
{
    return __to_xbase_string(res, 8, INNEROCT_BASE, INNEROCT_BASE_DIGITS, LN_INNEROCT_BASE);
}

string_t number_t::to_dec_string() const
{
    string_t res;
    return __to_xbase_string(res, 10, INNERDEC_BASE, INNERDEC_BASE_DIGITS, LN_INNERDEC_BASE);
}

string_t& number_t::to_dec_string(string_t& res) const
{
    return __to_xbase_string(res, 10, INNERDEC_BASE, INNERDEC_BASE_DIGITS, LN_INNERDEC_BASE);
}

string_t number_t::to_hex_string() const
{
    string_t res;
    return __to_hex_string(res);
}

string_t& number_t::to_hex_string(string_t& res) const
{
    return __to_hex_string(res);
}

string_t number_t::to_string(int base) const
{
    string_t res;
    return to_string(res, base);
}

string_t& number_t::to_string(string_t& res, int base) const
{
    if (base > 1) switch (base)
    {
        case  2: return __to_bin_string(res);
        case  8: return __to_xbase_string(res, 8,  INNEROCT_BASE, INNEROCT_BASE_DIGITS, LN_INNEROCT_BASE);
        case 10: return __to_xbase_string(res, 10, INNERDEC_BASE, INNERDEC_BASE_DIGITS, LN_INNERDEC_BASE);
        case 16: return __to_hex_string(res);
        default: if (base <= __max_base())
        {
            _radix_t r(base);
            return __to_xbase_string(res, base, r.inner_base, r.inner_base_digits, r.ln_inner_base);
        }
    }
    res.release();
    return res;
}

number_t& number_t::abs()
{
    mynum::abs(*this);
    return *this;
}

number_t& number_t::neg()
{
    mynum::neg(*this);
    return *this;
}

number_t& number_t::add(const number_t& x)
{
    mynum::add(*this, x, *this);
    return *this;
}

number_t& number_t::sub(const number_t& x)
{
    mynum::sub(*this, x, *this);
    return *this;
}

number_t& number_t::mul(const number_t& x)
{
    mynum::mul(*this, x, *this);
    return *this;
}

number_t& number_t::kmul(const number_t& x)
{
    mynum::kmul(*this, x, *this);
    return *this;
}

number_t& number_t::div(const number_t& x)
{
    mynum::div(*this, x, *this);
    return *this;
}

number_t& number_t::div(const number_t& x, number_t& r)
{
    mynum::div(*this, x, *this, r);
    return *this;
}

number_t& number_t::mod(const number_t& x)
{
    number_t dummy;
    mynum::div(*this, x, dummy, *this);
    return *this;
}

number_t& number_t::shr(size_t x)
{
    mynum::shr(*this, x, *this);
    return *this;
}

number_t& number_t::shl(size_t x)
{
    mynum::shl(*this, x, *this);
    return *this;
}

number_t& number_t::bit_or(const number_t& x)
{
    mynum::bit_or(*this, x, *this);
    return *this;
}

number_t& number_t::bit_and(const number_t& x)
{
    mynum::bit_and(*this, x, *this);
    return *this;
}

number_t& number_t::bit_xor(const number_t& x)
{
    mynum::bit_xor(*this, x, *this);
    return *this;
}

number_t& number_t::bit_not()
{
    mynum::bit_not(*this, *this);
    return *this;
}

number_t& number_t::sqr()
{
    mynum::sqr(*this, *this);
    return *this;
}

number_t& number_t::ksqr()
{
    mynum::ksqr(*this, *this);
    return *this;
}

number_t& number_t::sqrt()
{
    mynum::sqrt(*this, *this);
    return *this;
}

number_t& number_t::pow(const number_t& x)
{
    mynum::pow(*this, x, *this);
    return *this;
}

number_t& number_t::pom(const number_t& x, const number_t& y)
{
    mynum::pom(*this, x, y, *this);
    return *this;
}

int number_t::bit_at(size_t x) const
{
    return len? __bit_at(x): 0;
}

size_t number_t::bits_count() const
{
    return len? __vbits_count(): 0;
}

bool number_t::is_power2() const
{
    return mynum::is_power2(*this);
}

bool number_t::is_odd() const
{
    return mynum::is_odd(*this);
}

bool number_t::is_even() const
{
    return mynum::is_even(*this);
}

void number_t::copy(const number_t& another)
{
    if (this != &another)
    {
        __release();
        __copy(another);
    }
}

void number_t::steal(number_t& other)
{
    if (is_not(other))
    {
        __release();
        dat = other.dat;
        len = other.len;
        other.dat = NULL;
        other.len = 0;
    }
}

void number_t::set_zero()
{
    __release();
}

void number_t::set_one()
{
    __release();
    __reserve(1);
    *dat = 1;
    len = 1;
}

number_t& number_t::operator = (const number_t& x)    { return assign(x); }
number_t& number_t::operator = (short x)              { return assign(x); }
number_t& number_t::operator = (long x)               { return assign(x); }
number_t& number_t::operator = (long long x)          { return assign(x); }
number_t& number_t::operator = (char x)               { return assign(x); }
number_t& number_t::operator = (int x)                { return assign(x); }
number_t& number_t::operator = (bool x)               { return assign(x); }
number_t& number_t::operator = (unsigned short x)     { return assign(x); }
number_t& number_t::operator = (unsigned long x)      { return assign(x); }
number_t& number_t::operator = (unsigned long long x) { return assign(x); }
number_t& number_t::operator = (unsigned char x)      { return assign(x); }
number_t& number_t::operator = (unsigned int x)       { return assign(x); }

number_t number_t::operator + () const
{
    return *this;
}

number_t number_t::operator - () const
{
    number_t res(*this);
    res.len = 0 - res.len;
    return res;
}

number_t& number_t::operator ~ ()
{
    return this->bit_not();
}

number_t& number_t::operator ++ ()
{
    return this->add(number_t(1));
}

number_t& number_t::operator -- ()
{
    return this->sub(number_t(1));
}

number_t& number_t::operator ++ (int)
{
    return this->add(number_t(1));
}

number_t& number_t::operator -- (int)
{
    return this->sub(number_t(1));
}

number_t& number_t::operator += (const number_t& x)
{
    return this->add(x);
}

number_t& number_t::operator -= (const number_t& x)
{
    return this->sub(x);
}

number_t& number_t::operator *= (const number_t& x)
{
    return this->mul(x);
}

number_t& number_t::operator /= (const number_t& x)
{
    return this->div(x);
}

number_t& number_t::operator %= (const number_t& x)
{
    return this->mod(x);
}

number_t& number_t::operator |= (const number_t& x)
{
    return this->bit_or(x);
}

number_t& number_t::operator &= (const number_t& x)
{
    return this->bit_and(x);
}

number_t& number_t::operator ^= (const number_t& x)
{
    return this->bit_xor(x);
}

number_t& number_t::operator <<= (int x)
{
    return this->shl(x);
}

number_t& number_t::operator >>= (int x)
{
    return this->shr(x);
}

number_t::operator bool () const
{
    return !is_zero();
}

bool number_t::operator ! () const
{
    return is_zero();
}

void number_t::__copy(const number_t& another)
{
    slen_t l = __abs(another.len);
    dat = __allocate_units(l);
    memcpy(dat, another.dat, l * sizeof(unit_t));
    len = another.len;
}

void number_t::__reserve(slen_t units)
{
    dat = __allocate_units(units);
}

slen_t number_t::__bits_reserve(slen_t n)
{
    slen_t units = (n + SHIFT - 1) / SHIFT;
    __reserve(units);
    return units;
}

int number_t::__bit_at(size_t x) const
{
    return dat[x / SHIFT] & ((unit_t)1 << x % SHIFT);
}

slen_t number_t::__vbits_count() const
{
    return mynum::__vbits_count(dat[len - 1]) + (len - 1) * SHIFT;
}

void number_t::__release()
{
    __deallocate_units(dat);
    dat = NULL;
    len = 0;
}

void number_t::__add(unit_t x)
{
    if (len)
    {
        dunit_t carry = 0;
        carry = (dunit_t)dat[0] + x;
        dat[0] = carry & MASK;
        carry >>= SHIFT;
        for (slen_t i = 1; i < len && carry != 0; i++)
        {
            carry += dat[i];
            dat[i] = carry & MASK;
            carry >>= SHIFT;
        }
        if (carry)
        {
            dat[len++] = carry & MASK;
        }
    }
    else
    {
        dat[len++] = x;
    }
}

void number_t::__mul(unit_t x)
{
    unit_t* p = dat;
    dunit_t carry = 0;
    for (slen_t i = 0; i < len; i++)
    {
        carry += (dunit_t)dat[i] * x;
        *p++ = carry & MASK;
        carry >>= SHIFT;
    }
    if (carry)
    {
        *p++ = carry & MASK;
    }
    len = slen_t(p - dat);
}

static __always_inline(unit_t) __strbin_to_unit(const char* p, int l)
{
    unit_t x = 0;
    const unit_t A = 1 << (UNITBITS - 1);
    for (int i = 0; i < l; i++)
    {
        if (p[i] == '1')
        {
            x |= A >> i;
        }
    }
    return x >> (UNITBITS - l);
}

void number_t::__construct_from_bin_string(const char* s, slen_t l)
{
    assert(len == 0 && dat == NULL);

    int sign = 1;
    if (*s == '-' || *s == '+')
    {
        sign = *s == '-'? -1: 1;
        s++;
        l--;
    }

    const char *p0 = s + l - UNITBITS;
    const char *p1 = s + l % UNITBITS;
    __reserve((l + UNITBITS - 1) / UNITBITS);
    for (; p0 >= p1; p0 -= UNITBITS)
    {
        dat[len++] = __strbin_to_unit(p0, UNITBITS);
    }
    if (l % UNITBITS)
    {
        dat[len++] = __strbin_to_unit(s, l % UNITBITS);
    }
    len *= sign;
}

static __always_inline(unit_t) __strhex_to_unit(const char* p, int l)
{
    unit_t x = 0;
    while (l--)
    {
        x <<= 4;
        x |= __char_digit(*p++);
    }
    return x;
}

void number_t::__construct_from_hex_string(const char* s, slen_t l)
{
    assert(len == 0 && dat == NULL);

    int sign = 1;
    if (*s == '-' || *s == '+')
    {
        sign = *s == '-'? -1: 1;
        s++;
        l--;
    }

    const int k = sizeof(unit_t) << 1;
    const char *p0 = s + l - k;
    const char *p1 = s + l % k;

    __reserve(l + k - 1 / k);
    for (; p0 >= p1; p0 -= k)
    {
        dat[len++] = __strhex_to_unit(p0, k);
    }
    if (l % k)
    {
        dat[len++] = __strhex_to_unit(s, l % k);
    }
    len *= sign;
}

static __always_inline(unit_t) __str_to_unit(const char* p, int base, int l)
{
    unit_t x = 0;
    while (l--)
    {
        x *= base;
        x += __char_digit(*p++);
    }
    return x;
}

void number_t::__construct_from_xbase_string(const char* s, slen_t l, unit_t base, float ln_base, unit_t inner_base, unit_t inner_base_digits)
{
    assert(len == 0 && dat == NULL && l >= 0 && base <= __max_base());

    unit_t u;
    int i = 0, sign = 1, d;
    __reserve(int(ln_base * l / LN_BASE + 1));

    if (*s == '-' || *s == '+')
    {
        sign = *s == '-'? -1: 1;
        s++;
        l--;
    }
    for (; i < l - l % inner_base_digits; i += inner_base_digits)
    {
        if ((u = __str_to_unit(s + i, base, inner_base_digits)) < inner_base)
        {
            __mul(inner_base);
            __add(u);
        }
    }
    for (; i != l; i++)
    {
        if ((d = __char_digit(s[i])) < base)
        {
            __mul(base);
            __add(d);
        }
    }
    len *= sign;
}

void number_t::__construct_from_string(const char* s, slen_t l, int base)
{
    assert(len == 0 && dat == NULL && l >= 0 && base <= __max_base());

    switch (base)
    {
        case 2:
            __construct_from_bin_string(s, l);
            break;
        case 8:
            __construct_from_xbase_string(s, l, 8,  LN_8, INNEROCT_BASE, INNEROCT_BASE_DIGITS);
            break;
        case 10:
            __construct_from_xbase_string(s, l, 10, LN_10, INNERDEC_BASE, INNERDEC_BASE_DIGITS);
            break;
        case 16:
            __construct_from_hex_string(s, l);
            break;
        default:
        {
            _radix_t r(base);
            __construct_from_xbase_string(s, l, r.base, r.ln_base, r.inner_base, r.inner_base_digits);
            break;
        }
    }
}

static const char* __B[16] =
{
    "0000", "0001", "0010", "0011", "0100", "0101", "0110", "0111",
    "1000", "1001", "1010", "1011", "1100", "1101", "1110", "1111",
};

string_t& number_t::__to_bin_string(string_t& res) const
{
    slen_t l = __abs(len);
    res.release();
    if (l)
    {
        char* str, *ps;
        byte_t* p = (byte_t*)(dat + l - 1) + sizeof(unit_t) - 1;
        byte_t* e = (byte_t*)dat - 1;

        while (!*p) p--;
        ps = str = (char*)mem::allocate(l * UNITBITS + 1 + 1, sizeof(char));
        if (len < 0)
        {
            *ps++ = '-';
        }
        for (int i = 0; i < 8; i++)
        {
            if ((128 >> i) & *p)
            {
                while (i < 8)
                {
                    *ps++ = ((128 >> i++) & *p)? '1': '0';
                }
                break;
            }
        }
        p--;
        while (p != e)
        {
            *((int*)ps + 0) = *(int*)__B[*p >> 4];
            *((int*)ps + 1) = *(int*)__B[*p & 0x0F];
            ps += 8;
            p--;
        }
        *ps = '\0';
        res.take(str, ps - str);
        return res;
    }
    res.__copy("0", 1);
    return res;
}

char __DIGIT_CHAR[36] = 
{
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
    'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
    'u', 'v', 'w', 'x', 'y', 'z',
};

int __max_base()
{
    return sizeof(__DIGIT_CHAR);
}

string_t& number_t::__to_hex_string(string_t& res) const
{
    slen_t l = __abs(len);
    res.release();
    if (l)
    {
        char* str, *ps;
        byte_t* p = (byte_t*)(dat + l - 1) + sizeof(unit_t) - 1;
        byte_t* e = (byte_t*)dat - 1;

        while (!*p)
        {
            p--;
        }

        ps = str = (char*)mem::allocate(l * sizeof(unit_t) * 2 + 1 + 1, sizeof(char));

        if (len < 0)
        {
            *ps++ = '-';
        }
        if (*p & 0xF0)
        {
            *ps++ = __DIGIT_CHAR[*p >> 4];
        }
        *ps++ = __DIGIT_CHAR[*p-- & 0x0F];

        while (p != e)
        {
            *ps++ = __DIGIT_CHAR[*p >> 4];
            *ps++ = __DIGIT_CHAR[*p-- & 0x0F];
        }
        *ps = '\0';
        res.take(str, ps - str);
        return res;
    }
    res.__copy("0", 1);
    return res;
}

static __always_inline(dunit_t) __make_dunit(unit_t high, unit_t low)
{
    return (dunit_t)high << SHIFT | low;
}

static __always_inline(void) __unit_to_str(unit_t x, char* str, int base, int width)
{
    assert(base > 0 && base <= __max_base());

    char* p = str + width - 1;
    while (x)
    {
        *p-- = __DIGIT_CHAR[x % base];
        x /= base;
    }
    while (p != str - 1)
    {
        *p-- = '0';
    }
}

string_t& number_t::__to_xbase_string(string_t& res, unit_t base, unit_t inner_base, unit_t inner_base_digits, float ln_inner_base) const
{
    slen_t l = __abs(len);
    res.release();
    if (l)
    {
        slen_t size = 0;
        char* str = NULL, *ps;

        unit_t* tmp = __allocate_units(slen_t(LN_BASE * l / ln_inner_base + 1));
        for (slen_t i = l - 1; i >= 0; i--)
        {
            unit_t unit = dat[i];
            for (slen_t j = 0; j < size; j++)
            {
                dunit_t dunit = __make_dunit(tmp[j], unit);
                tmp[j] = unit_t(dunit % inner_base);
                unit = unit_t(dunit / inner_base);
            }
            while (unit)
            {
                tmp[size++] = unit % inner_base;
                unit /= inner_base;
            }
        }
        if (size)
        {
            char buf[32];
            unit_t* p = tmp + size - 1;
            unit_t* e = tmp - 1;

            __unit_to_str(*p--, buf, base, inner_base_digits);
            ps = str = (char*)mem::allocate(size * inner_base_digits + 1 + 1, sizeof(char));

            if (len < 0)
            {
                *ps++ = '-';
            }

            slen_t i = 0;
            while (buf[i] == '0')
            {
                i++;
            }
            for (; i < inner_base_digits; i++)
            {
                *ps++ = buf[i];
            }
            while (p != e)
            {
                __unit_to_str(*p--, ps, base, inner_base_digits);
                ps += inner_base_digits;
            }
        }
        __deallocate_units(tmp);
        if (str)
        {
            *ps = '\0';
            res.take(str, ps - str);
            return res;
        }
    }
    res.__copy("0", 1);
    return res;
}

void number_t::__trim()
{
    __trim_leading_zeros(dat, len);
}

number_t abs(const number_t& a)                         { number_t res; abs(a, res); return res; }
number_t neg(const number_t& a)                         { number_t res; neg(a, res); return res; }
number_t add(const number_t& a, const number_t& b)      { number_t res; add(a, b, res); return res; }
number_t sub(const number_t& a, const number_t& b)      { number_t res; sub(a, b, res); return res; }
number_t mul(const number_t& a, const number_t& b)      { number_t res; mul(a, b, res); return res; }
number_t sqr(const number_t& a)                         { number_t res; sqr(a, res); return res; }
number_t ksqr(const number_t& a)                        { number_t res; ksqr(a, res); return res; }
number_t kmul(const number_t& a, const number_t& b)     { number_t res; kmul(a, b, res); return res; }
number_t div(const number_t& a, const number_t& b)      { number_t res, dummy; div(a, b, res, dummy); return res; }
number_t mod(const number_t& a, const number_t& b)      { number_t res, dummy; div(a, b, dummy, res); return res; }
number_t shr(const number_t& a, size_t b)               { number_t res; shr(a, b, res); return res; }
number_t shl(const number_t& a, size_t b)               { number_t res; shl(a, b, res); return res; }
number_t pow(const number_t& a, const number_t& b)      { number_t res; pow(a, b, res); return res; }
number_t sqrt(const number_t& a)                        { number_t res; sqrt(a, res); return res; }
number_t bit_and(const number_t& a, const number_t& b)  { number_t res; bit_and(a, b, res); return res; }
number_t bit_or(const number_t& a, const number_t& b)   { number_t res; bit_or(a, b, res); return res; }
number_t bit_xor(const number_t& a, const number_t& b)  { number_t res; bit_xor(a, b, res); return res; }
number_t bit_not(const number_t& a)                     { number_t res; bit_not(a, res); return res; }

/** class string_t implementation */

string_t::string_t(const char* p): dat(NULL), len(0)
{
    if (p)
    {
        __copy(p, strlen(p));
    }
}

string_t::string_t(const char* p, size_t l): dat(NULL), len(0)
{
    if (p)
    {
        __copy(p, l);
    }
}

string_t::string_t(const string_t& another): dat(NULL), len(0)
{
    if (another.dat)
    {
        __copy(another.dat, another.len);
    }
}

string_t::~string_t()
{
    release();
}

string_t& string_t::operator = (const string_t& another)
{
    if (this != &another)
    {
        release();
        if (another.dat)
        {
            __copy(another.dat, another.len);
        }
    }
    return *this;
}

string_t& string_t::operator = (const char* p)
{
    if (dat != p)
    {
        release();
        if (p)
        {
            __copy(p, strlen(p));
        }
    }
    return *this;
}

void string_t::release()
{
    mem::deallocate(dat);
    dat = NULL;
    len = 0;
}

int string_t::cmp(const string_t& another) const
{
    return mynum::cmp(*this, another);
}

__always_inline(void) __to_lower(char* p, char* e)
{
    for (; p != e; p++) if (*p >= 'A' && *p <= 'Z')
    {
        *p += 32;
    }
}

__always_inline(void) __to_upper(char* p, char* e)
{
    for (; p != e; p++) if (*p >= 'a' && *p <= 'z')
    {
        *p -= 32;
    }
}

string_t& string_t::to_lower()
{
    __to_lower(dat, dat + len);
    return *this;
}

string_t& string_t::to_lower(string_t& res)
{
    res = *this;
    __to_lower(res.dat, res.dat + res.len);
    return res;
}

string_t& string_t::to_upper()
{
    __to_upper(dat, dat + len);
    return *this;
}

string_t& string_t::to_upper(string_t& res)
{
    res = *this;
    __to_upper(res.dat, res.dat + res.len);
    return res;
}

void string_t::__copy(const char* p, size_t l)
{
    len = l;
    dat = (char*)mem::allocate(len + 1, sizeof(char));
    memcpy(dat, p, len);
    dat[len] = '\0';
}

int cmp(const string_t& a, const string_t& b)
{
    size_t l = a.len > b.len? b.len: a.len;
    for (size_t i = 0; i != l; i++)
    {
        if (a.dat[i] > b.dat[i])
        {
            return 1;
        }
        else if (a.dat[i] < b.dat[i])
        {
            return -1;
        }
    }
    if (a.len > b.len)
    {
        return 1;
    }
    else if (a.len < b.len)
    {
        return -1;
    }
    return 0;
}

int check(const char* p, int base)
{
    const char* q = p;
    while (*p != '\0' && __char_digit_valid(*p, base)) p++;
    return p - q;
}

int check(const char* p, const char* e, int base)
{
    const char* q = p;
    while (p != e && __char_digit_valid(*p, base)) p++;
    return p - q;
}

const char* find_valid(const char* p, const char* e, int base, size_t* size)
{
    return NULL;
}

/** algorithms implementation */

static void __sub(const number_t& a, const number_t& b, number_t& res);
static void __mul(const unit_t* x, slen_t lx, const unit_t* y, slen_t ly, number_t& res);
static void __kmul(const unit_t* x, slen_t lx, const unit_t* y, slen_t ly, number_t& res);
static void __sqr(const unit_t* x, slen_t lx, number_t& res);
static void __ksqr(const unit_t* x, slen_t lx, number_t& res);
static void __div(const unit_t* a, slen_t la, const unit_t* b, slen_t lb, number_t& q, number_t& r);
static void __div(const unit_t* a, slen_t la, const unit_t* b, slen_t lb, number_t& q);
static bool __neq_core(const unit_t* x, slen_t lx, const unit_t* y, slen_t ly);
static slen_t __cmp_core(const unit_t* x, slen_t lx, const unit_t* y, slen_t ly);
static slen_t __add_core(const unit_t* x, slen_t lx, const unit_t* y, slen_t ly, unit_t* res);
static slen_t __sub_core(const unit_t* x, slen_t lx, const unit_t* y, slen_t ly, unit_t* res);
static slen_t __mul_core(const unit_t* x, slen_t lx, const unit_t* y, slen_t ly, unit_t* res);
static slen_t __sqr_core(const unit_t* x, slen_t lx, unit_t* res);
static void __div_unit_core(const unit_t* x, slen_t lx, unit_t y, unit_t* q, slen_t* lq, unit_t* r, slen_t* lr);
static unit_t __guess_quotient(unit_t x1, unit_t x2, unit_t x3, unit_t y1, unit_t y2);
static void __guess_sqrt(const number_t& a, number_t& res);
static unit_t __truing_quotient(unit_t* x, const unit_t* y, slen_t len, unit_t trial);
static slen_t __div_core(unit_t* x, slen_t lx, const unit_t* y, slen_t ly, unit_t* q);
static void __shl_units(number_t& a, slen_t b);
static slen_t __shl_core(unit_t* x, slen_t lx, int d);
static unit_t __shr_core(unit_t* x, slen_t lx, int d);
static slen_t __bit_and_core(const unit_t* x, const unit_t* y, slen_t lx, unit_t* res);
static slen_t __bit_or_core(const unit_t* x, slen_t lx, const unit_t* y, slen_t ly, unit_t* res);
static slen_t __bit_xor_core(const unit_t* x, slen_t lx, const unit_t* y, slen_t ly, unit_t* res);
static slen_t __bit_not_core(const unit_t* x, slen_t lx, unit_t* res);

int cmp(const number_t& a, const number_t& b)
{
    if (a.is_not(b))
    {
        if (a.len - b.len)
        {
            return a.len - b.len > 0? 1: -1;        
        }
        slen_t sign = __sign(a.len);
        slen_t la = __abs(a.len);
        slen_t lb = __abs(b.len);
        return __cmp_core(a.dat, la, b.dat, lb) * sign;
    }
    return 0;
}

bool neq(const number_t& a, const number_t& b)
{
    if (a.is_not(b))
    {
        if (!__same_sign(a.len, b.len))
        {
            return true;
        }
        slen_t la = __abs(a.len);
        slen_t lb = __abs(b.len);
        return __neq_core(a.dat, la, b.dat, lb);
    }
    return false;
}

void abs(const number_t& a, number_t& res)
{
    res.copy(a);
    res.len = __abs(res.len);
}

void neg(const number_t& a, number_t& res)
{
    res.copy(a);
    res.len = 0 - res.len;
}

void add(const number_t& a, const number_t& b, number_t& res)
{
    unit_t* tmp;
    slen_t lr, la, lb, sign;

    la = __abs(a.len);
    lb = __abs(b.len);

    if (__same_sign(a.len, b.len))
    {
        if (la > lb)
        {
            tmp = __allocate_units(la + 1);
            lr = __add_core(a.dat, la, b.dat, lb, tmp);
        }
        else
        {
            tmp = __allocate_units(lb + 1);
            lr = __add_core(b.dat, lb, a.dat, la, tmp);
        }
        sign = __sign(a.len);
    }
    else
    {
        if ((sign = __cmp_core(a.dat, la, b.dat, lb)) == 1)
        {
            tmp = __allocate_units(la);
            lr = __sub_core(a.dat, la, b.dat, lb, tmp);
        }
        else
        {
            tmp = __allocate_units(lb);
            lr = __sub_core(b.dat, lb, a.dat, la, tmp);
        }
        sign *= __sign(a.len);
    }

    res.__release();
    res.dat = tmp;
    res.len = lr * sign;
}

void sub(const number_t& a, const number_t& b, number_t& res)
{
    unit_t* tmp;
    slen_t lr, la, lb, sign;
    
    la = __abs(a.len);
    lb = __abs(b.len);

    if (__same_sign(a.len, b.len))
    {
        if ((sign = __cmp_core(a.dat, la, b.dat, lb)) == 1)
        {
            tmp = __allocate_units(la);
            lr = __sub_core(a.dat, la, b.dat, lb, tmp);
        }
        else
        {
            tmp = __allocate_units(lb);
            lr = __sub_core(b.dat, lb, a.dat, la, tmp);
        }
        sign *= __sign(a.len);
    }
    else
    {
        if (la > lb)
        {
            tmp = __allocate_units(la + 1);
            lr = __add_core(a.dat, la, b.dat, lb, tmp);
        }
        else
        {
            tmp = __allocate_units(lb + 1);
            lr = __add_core(b.dat, lb, a.dat, la, tmp);
        }
        sign = __sign(a.len);
    }

    res.__release();
    res.dat = tmp;
    res.len = lr * sign;
}

void mul(const number_t& a, const number_t& b, number_t& res)
{
    __mul(a.dat, __abs(a.len), b.dat, __abs(b.len), res);
    res.len *= __sign(a.len, b.len);
}

void kmul(const number_t& a, const number_t& b, number_t& res)
{
    __kmul(a.dat, __abs(a.len), b.dat, __abs(b.len), res);
    res.len *= __sign(a.len, b.len);
}

void sqr(const number_t& a, number_t& res)
{
    __sqr(a.dat, __abs(a.len), res);
}

void ksqr(const number_t& a, number_t& res)
{
    __ksqr(a.dat, __abs(a.len), res);
}

int div(const number_t& a, const number_t& b, number_t& q, number_t& r)
{
    if (!b.is_zero())
    {
        slen_t la = __abs(a.len);
        slen_t lb = __abs(b.len);
        if (la >= lb)
        {
            __div(a.dat, la, b.dat, lb, q, r);
            r.len *= __sign(a.len);
            q.len *= __sign(a.len, b.len);
        }
        else
        {
            q.set_zero();
            r.assign(a);
        }
        return 1;
    }
    return 0;
}

int div(const number_t& a, const number_t& b, number_t& q)
{
    if (!b.is_zero())
    {
        slen_t la = __abs(a.len);
        slen_t lb = __abs(b.len);
        if (la >= lb)
        {
            __div(a.dat, la, b.dat, lb, q);
            q.len *= __sign(a.len, b.len);
        }
        else
        {
            q.set_zero();
        }
        return 1;
    }
    return 0;
}

int floor_div(const number_t& a, const number_t& b, number_t& q, number_t& r)
{
    if (!b.is_zero())
    {
        slen_t la = __abs(a.len);
        slen_t lb = __abs(b.len);
        if (la >= lb)
        {
            __div(a.dat, la, b.dat, lb, q, r);
        }
        else
        {
            q.set_zero();
            abs(a, r);
        }

        slen_t sign = __sign(a.len);
        if (__same_sign(a.len, b.len))
        {
            r.len *= sign;
        }
        else
        {
            if (!r.is_zero())
            {
                add(q, 1, q);
                unit_t* tmp = __allocate_units(lb);
                slen_t lr = __sub_core(b.dat, lb, r.dat, r.len, tmp);
                r.__release();
                r.dat = tmp;
                r.len = lr;
            }
            q.len *= -1;
            r.len *= -sign;
        }
        return 1;
    }
    return 0;
}

void shr(const number_t& a, size_t b, number_t& res)
{
    slen_t n = b / SHIFT;
    slen_t m = b % SHIFT;
    slen_t l = __abs(a.len) - n;
    if (l > 0)
    {
        unit_t* tmp = __allocate_units(l);
        __copy_units(tmp, a.dat + n, l);
        __shr_core(tmp, l, m);
        res.__release();
        res.dat = tmp;
        res.len = l * __sign(a.len);
        res.__trim();
    }
    else
    {
        res.set_zero();
    }
}

void shl(const number_t& a, size_t b, number_t& res)
{
    slen_t n = b / SHIFT;
    slen_t m = b % SHIFT;
    slen_t l = __abs(a.len) + n;
    unit_t* tmp = __allocate_units(l + 1);
    __set_units_zero(tmp, n);
    __copy_units(tmp + n, a.dat, a.len);
    if (m && __shl_core(tmp + n, a.len, m) > a.len)
    {
        l++;
    }
    res.__release();
    res.dat = tmp;
    res.len = l * __sign(a.len);
    res.__trim();
}

void pow(const number_t& a, const number_t& b, number_t& res)
{
    if (a.len && b.len)
    {
        number_t tmp(1);
        unit_t* p = b.dat + b.len - 1;
        unit_t* e = b.dat - 1;
        while (p != e)
        {
            unit_t u = 1 << (SHIFT - 1);
            while (u != 0)
            {
                ksqr(tmp, tmp);
                if (*p & u)
                {
                    kmul(tmp, a, tmp);
                }
                u >>= 1;
            }
            p--;
        }
        res.steal(tmp);
    }
    else if (!b.len)
    {
        res.set_one();
    }
    else if (!a.len)
    {
        res.set_zero();
    }
}

int pom(const number_t& a, const number_t& b, const number_t& c, number_t& res)
{
    if (!c.is_zero())
    {
        if (a.len && b.len)
        {
            number_t tmp(1), q, r;
            unit_t* p = b.dat + b.len - 1;
            unit_t* e = b.dat - 1;
            while (p != e)
            {
                unit_t u = 1 << (SHIFT - 1);
                while (u != 0)
                {
                    ksqr(tmp, tmp);
                    div(tmp, c, q, tmp);

                    if (*p & u)
                    {
                        kmul(tmp, a, tmp);
                        div(tmp, c, q, tmp);
                    }
                    u >>= 1;
                }
                p--;
            }
            res.steal(tmp);
        }
        else if (!b.len)
        {
            res.set_one();
        }
        else if (!a.len)
        {
            res.set_zero();
        }
        return 1;
    }
    return 0;
}

unit_t sqrt(dunit_t x)
{
    static byte_t __sqrt_table[] = {
        0, 1, 1, 1, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 
        5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 7,
        7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 8, 8, 8, 8, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
        10, 10, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
        11, 11, 11, 11, 11, 11, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
        12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 13, 13, 13, 13, 13, 13, 13,
        13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
        13, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
        14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 15, 15, 15, 15, 15, 15, 15, 15,
        15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
        15, 15, 15, 15
    };

    if (x)
    {
        dunit_t t0, t1;
        byte_t* p = (byte_t*)&x;
        t0  = __sqrt_table[*p++];
        t0 |= __sqrt_table[*p++] << 4;
        t0 |= __sqrt_table[*p++] << 8;
        t0 |= __sqrt_table[*p] << 12;
#if UNITBITS == 32
        t0 |= __sqrt_table[*p++] << 16;
        t0 |= __sqrt_table[*p++] << 20;
        t0 |= __sqrt_table[*p++] << 24;
        t0 |= __sqrt_table[*p] << 28;
#endif

        /* newton iteration: t0 = (t0 + x / t0) >> 1; */
        ITERATION:
        {
            t1 = x / t0;
            switch (t0 - t1)
            {
                case 0:
                case ~0:
                    return (unit_t)t0;
                case 1:
                    return (unit_t)t1;
                default:
                    t0 = (t0 + t1) >> 1;
            }
        }
        goto ITERATION;
    }
    return 0;
}

int sqrt(const number_t& a, number_t& res)
{
    if (a.len > 2)
    {
        number_t res1, rem, d;
        __guess_sqrt(a, res);

        ITERATION:
        {
            div(a, res, res1, rem);
            sub(res1, res, d);
            res.add(res1).shr(1);
            if (d.is_zero() || d.is_one())
            {
                return 1;
            }
        }
        goto ITERATION;
    }
    else switch (a.len)
    {
        case 0:
            res.set_zero();
            return 1;
        case 1:
            res.assign(sqrt((dunit_t)a.dat[0]));
            return 1;
        case 2:
            res.assign(sqrt(*(dunit_t*)a.dat));
            return 1;
    }
    return 0;
}

void bit_and(const number_t& a, const number_t& b, number_t& res)
{
    unit_t* tmp;
    slen_t la = __abs(a.len), lb = __abs(b.len), lr;
    if (la >= lb)
    {
        tmp = __allocate_units(lb);
        lr = __bit_and_core(a.dat, b.dat, lb, tmp);
    }
    else
    {
        tmp = __allocate_units(la);
        lr = __bit_and_core(a.dat, b.dat, la, tmp);
    }
    res.__release();
    res.dat = tmp;
    res.len = lr * __sign(a.len, b.len);
}

void bit_or(const number_t& a, const number_t& b, number_t& res)
{
    unit_t* tmp;
    slen_t la = __abs(a.len), lb = __abs(b.len), lr;
    if (la > lb)
    {
        tmp = __allocate_units(la);
        lr = __bit_or_core(a.dat, la, b.dat, lb, tmp);
    }
    else
    {
        tmp = __allocate_units(lb);
        lr = __bit_or_core(b.dat, lb, a.dat, la, tmp);  
    }
    res.__release();
    res.dat = tmp;
    res.len = lr * __sign(a.len, b.len);
}

void bit_xor(const number_t& a, const number_t& b, number_t& res)
{
    unit_t* tmp;
    slen_t la = __abs(a.len), lb = __abs(b.len), lr;
    if (la > lb)
    {
        tmp = __allocate_units(la);
        lr = __bit_xor_core(a.dat, la, b.dat, lb, tmp);
    }
    else
    {
        tmp = __allocate_units(lb);
        lr = __bit_xor_core(b.dat, lb, a.dat, la, tmp);  
    }
    res.__release();
    res.dat = tmp;
    res.len = lr * __sign(a.len, b.len);
}

void bit_not(const number_t& a, number_t& res)
{
    if (a.len)
    {
        slen_t la = __abs(a.len);
        unit_t* tmp = __allocate_units(la);
        slen_t lr = __bit_not_core(a.dat, la, tmp);
        res.__release();
        res.dat = tmp;
        res.len = lr * __sign(a.len);
    }
}

void swap(number_t& a, number_t& b)
{
    unit_t* tmp = a.dat;
    a.dat = b.dat;
    b.dat = tmp;
    a.len ^= b.len;
    b.len ^= a.len;
    a.len ^= b.len;
}

void bits_reserve_1(number_t& a, int n)
{
    if (n)
    {
        a.__release();
        a.len = a.__bits_reserve(n);
        slen_t extra = a.len * SHIFT - n;
        const unit_t max_shift = unit_t(1) << (SHIFT - 1);
        __set_units_zero(a.dat, a.len);
        a.dat[a.len - 1] = max_shift >> extra;
    }
    else
    {
        a.set_zero();
    }
}

void bits_reserve_max(number_t& a, int n)
{
    if (n)
    {
        a.__release();
        a.len = a.__bits_reserve(n);
        slen_t extra = a.len * SHIFT - n;
        unit_t* p = a.dat;
        unit_t* e = a.dat + a.len - 1;
        while (p != e)
        {
            *p++ = MASK;
        }
        *p = MASK;
        *p <<= extra;
        *p >>= extra;
    }
    else
    {
        a.set_zero();
    }
}

bool is_power2(const number_t& a)
{
    if (!a.is_zero())
    {
        const unit_t* p = a.dat;
        const unit_t* e = a.dat + a.len - 1;

        while (p != e)
        {
            if (*p++ != 0)
            {
                return false;
            }
        }
        return (*p & (*p - 1)) == 0;
    }
    return false;
}

bool is_odd(const number_t& a)
{
    if (!a.is_zero())
    {
        return a.dat[0] & 1;
    }
    return false;
}

bool is_even(const number_t& a)
{
    return !is_odd(a);
}

void __sub(const number_t& a, const number_t& b, number_t& res)
{
    assert(cmp(a, b) >= 0);

    unit_t* tmp = __allocate_units(a.len);
    slen_t lr = __sub_core(a.dat, a.len, b.dat, b.len, tmp);
    res.__release();
    res.dat = tmp;
    res.len = lr;
}

void __mul(const unit_t* x, slen_t lx, const unit_t* y, slen_t ly, number_t& res)
{
    assert (lx >= 0 && ly >= 0);

    if (lx && ly)
    {
        unit_t* tmp = __allocate_units(lx + ly);
        __set_units_zero(tmp, lx + ly);
        slen_t lr = __mul_core(x, lx, y, ly, tmp);
        res.__release();
        res.len = lr;
        res.dat = tmp;
    }
    else
    {
        res.set_zero();
    }
}

void __kmul(const unit_t* x, slen_t lx, const unit_t* y, slen_t ly, number_t& res)
{
    if (lx <= KMUL_THRESHOLD || ly <= KMUL_THRESHOLD)
    {
        __mul(x, lx, y, ly, res);
        return;
    }

    slen_t n, la, lb, lc, ld;
    const unit_t *a, *b, *c, *d;

    n = lx >= ly? lx >> 1: ly >> 1;
    b = x;
    d = y;
    a = x + n;
    c = y + n;
    lb = n < lx? n: lx;
    ld = n < ly? n: ly;
    la = lx - n > 0? lx - n: 0;
    lc = ly - n > 0? ly - n: 0;

    __trim_leading_zeros(a, la);
    __trim_leading_zeros(b, lb);
    __trim_leading_zeros(c, lc);
    __trim_leading_zeros(d, ld);

    number_t ac, bd, a_bd_c;
    
    __kmul(a, la, c, lc, ac);
    __kmul(b, lb, d, ld, bd);

    unit_t* tmp = __allocate_units(lx + ly + 1);

    slen_t s0, l0 = 0, s1, l1 = 0;
    if ((s0 = __cmp_core(a, la, b, lb)) > 0)
    {
        l0 = __sub_core(a, la, b, lb, tmp);
    }
    else if (s0 < 0)
    {
        l0 = __sub_core(b, lb, a, la, tmp);
    }

    if ((s1 = __cmp_core(d, ld, c, lc)) > 0)
    {
        l1 = __sub_core(d, ld, c, lc, tmp + l0);
    }
    else if (s1 < 0)
    {
        l1 = __sub_core(c, lc, d, ld, tmp + l0);
    }

    __kmul(tmp, l0, tmp + l0, l1, a_bd_c);

    __set_units_zero(tmp, lx + ly + 1);
    __copy_units(tmp, bd.dat, bd.len);
    __copy_units(tmp + 2 * n, ac.dat, ac.len);

    __add_core(tmp + n, lx + ly + 1 - n, ac.dat, ac.len, tmp + n);
    __add_core(tmp + n, lx + ly + 1 - n, bd.dat, bd.len, tmp + n);

    if (s0 == s1)
    {
        __add_core(tmp + n, lx + ly + 1 - n, a_bd_c.dat, a_bd_c.len, tmp + n);
    }
    else
    {
        __sub_core(tmp + n, lx + ly + 1 - n, a_bd_c.dat, a_bd_c.len, tmp + n);
    }

    res.__release();
    res.len = lx + ly + 1;
    res.dat = tmp;
    res.__trim();
}

void __sqr(const unit_t* x, slen_t lx, number_t& res)
{
    if (lx)
    {
        unit_t* tmp = __allocate_units(2 * lx);
        slen_t lr = __sqr_core(x, lx, tmp);
        res.__release();
        res.len = lr;
        res.dat = tmp;
    }
    else
    {
        res.set_zero();
    }
}

void __ksqr(const unit_t* x, slen_t lx, number_t& res)
{
    if (lx <= KSQR_THRESHOLD)
    {
        __sqr(x, lx, res);
        return;
    }

    slen_t n, la, lb;
    const unit_t *a, *b;

    n = lx >> 1;
    b = x;
    a = x + n;
    lb = n;
    la = lx - n;
    __trim_leading_zeros(a, la);
    __trim_leading_zeros(b, lb);

    number_t aa, ab, bb;

    __ksqr(a, la, aa);
    __kmul(a, la, b, lb, ab);
    __ksqr(b, lb, bb);

    unit_t* tmp = __allocate_units(2 * lx);

    __set_units_zero(tmp, 2 * lx);
    __copy_units(tmp, bb.dat, bb.len);
    __copy_units(tmp + 2 * n, aa.dat, aa.len);

    __add_core(tmp + n, 2 * lx - n, ab.dat, ab.len, tmp + n);
    __add_core(tmp + n, 2 * lx - n, ab.dat, ab.len, tmp + n);

    res.__release();
    res.len = 2 * lx;
    res.dat = tmp;
    res.__trim();
}

void __div(const unit_t* a, slen_t la, const unit_t* b, slen_t lb, number_t& q, number_t& r)
{
    assert(la >= lb && lb > 0);

    if (lb > 1)
    {
        slen_t lx = la;
        slen_t ly = lb;
        unit_t* x = __allocate_units(lx + 1);
        unit_t* y = __allocate_units(ly);
        __copy_units(x, a, lx);
        __copy_units(y, b, ly);

        slen_t n = 0;
        if (*(x + lx - 1) >= *(y + ly - 1))
        {
            n = SHIFT - __vbits_count(*(y + ly - 1));
            lx = __shl_core(x, lx, n);
            ly = __shl_core(y, ly, n);
            if (*(x + lx - 1) >= *(y + ly - 1))
            {
                x[lx++] = 0;
            }
        }

        unit_t* tmp = __allocate_units(lx - ly);
        slen_t lr = __div_core(x, lx, y, ly, tmp);
        q.__release();
        q.dat = tmp;
        q.len = lr;

        if (n)
        {
            __shr_core(x, ly, n);
        }
        __copy_units(y, x, ly);
        r.__release();
        r.dat = y;
        r.len = ly;
        r.__trim();

        __deallocate_units(x);
    }
    else
    {
        unit_t* tmpq = __allocate_units(la);
        unit_t* tmpr = __allocate_units(1);
        slen_t lq, lr;
        __div_unit_core(a, la, *b, tmpq, &lq, tmpr, &lr);
        q.__release();
        r.__release();
        q.dat = tmpq;
        q.len = lq;
        r.dat = tmpr;
        r.len = lr;
    }
}

void __div(const unit_t* a, slen_t la, const unit_t* b, slen_t lb, number_t& q)
{
    assert(la >= lb && lb > 0);

    if (lb > 1)
    {
        slen_t lx = la;
        slen_t ly = lb;
        unit_t* x = __allocate_units(lx + 1);
        unit_t* y = __allocate_units(ly);
        __copy_units(x, a, lx);
        __copy_units(y, b, ly);

        slen_t n = 0;
        if (*(x + lx - 1) >= *(y + ly - 1))
        {
            n = SHIFT - __vbits_count(*(y + ly - 1));
            lx = __shl_core(x, lx, n);
            ly = __shl_core(y, ly, n);
            if (*(x + lx - 1) >= *(y + ly - 1))
            {
                x[lx++] = 0;
            }
        }

        unit_t* tmp = __allocate_units(lx - ly);
        slen_t lr = __div_core(x, lx, y, ly, tmp);
        q.__release();
        q.dat = tmp;
        q.len = lr;
        __deallocate_units(x);
        __deallocate_units(y);
    }
    else
    {
        slen_t lq, lr;
        unit_t* tmpq = __allocate_units(la);
        unit_t* tmpr = __allocate_units(1);
        __div_unit_core(a, la, *b, tmpq, &lq, tmpr, &lr);
        q.__release();
        q.dat = tmpq;
        q.len = lq;
        __deallocate_units(tmpr);
    }
}

void __shl_units(number_t& a, int b)
{
    assert(a.len >= 0);

    unit_t* tmp = __allocate_units(a.len + b);
    __set_units_zero(tmp, b);
    __copy_units(tmp + b, a.dat, a.len);
    a.__release();
    a.dat = tmp;
    a.len = a.len + b;
    a.__trim();
}

void __guess_sqrt(const number_t& a, number_t& res)
{
    assert(a.len > 2);

    slen_t n = a.__vbits_count();
    n = n & 1? (n + 1) >> 1 : n >> 1;
    res.__release();
    res.len = res.__bits_reserve(n);
    __set_units_zero(res.dat, res.len);

    dunit_t h;
    dunit_t *p, *e = (dunit_t*)a.dat - 1;
    unit_t* pr = res.dat + res.len - 1;
    if (a.len & 1)
    {
        h = *(a.dat + a.len - 1);
        p = (dunit_t*)(a.dat + a.len - 3);
    }
    else
    {
        h = *(dunit_t*)(a.dat + a.len - 2);
        p = (dunit_t*)(a.dat + a.len - 4);
    }

    *pr = sqrt(h);
    unit_t x;
    number_t rem, q, r, est(*pr), est2b;
    rem.__reserve(4);
    rem.len = 4;
    *(dunit_t*)rem.dat = *p;
    *((dunit_t*)rem.dat + 1) = h - *pr * *pr;
    rem.__trim();

    slen_t times = a.len / 10? a.len / 10: 4;
    do
    {
        shl(est, SHIFT + 1, est2b);  // est2b = est * 2 * BASE
        div(rem, est2b, q, r);

        if (q.len == 1)
        {
            x = q.dat[0];
        }
        else if (q.len > 1)
        {
            x = MASK;
        }
        else
        {
            x = 0;
        }

        __shl_units(est, 1);

        if (x) while (1)
        {
            /*
             tmp = (est * 2 * BASE + x) * x
             */
            number_t tmp;
            tmp.__reserve(est2b.len + 1);
            __copy_units(tmp.dat, est2b.dat, est2b.len);
            tmp.len = est2b.len;
            tmp.dat[0] = x;
            tmp.__mul(x);

            if (cmp(tmp, rem) <= 0)
            {
                est.dat[0] = x;
                __sub(rem, tmp, rem);
                break;
            }
            else
            {
                __sub(tmp, rem, tmp);

                number_t tmp1;
                tmp1.__reserve(est2b.len + 1);
                __copy_units(tmp1.dat, est2b.dat, est2b.len);
                tmp1.len = est2b.len;
                tmp1.dat[0] = x;
                tmp1.__add(x);

                div(tmp, tmp1, q, r);

                if (!q.is_zero())
                {
                    x -= q.dat[0];
                }
                else if (!tmp.is_zero())
                {
                    x--;
                }
            }
        }

        if (--p != e)
        {
            __shl_units(rem, 2);
            *(dunit_t*)(rem.dat) = *p;
        }
    }
    while (--times > 0 && p != e);

    unit_t* pe = est.dat + est.len - 1;
    while (pe >= est.dat)
    {
        *pr-- = *pe--;
    }
}

bool __neq_core(const unit_t* x, slen_t lx, const unit_t* y, slen_t ly)
{
    assert (lx >= 0 && ly >= 0);

    if (lx - ly)
    {
        return true;
    }
    if (lx)
    {
        if (x[lx - 1] != y[lx - 1] || x[0] != y[0])
        {
            return true;
        }
        else
        {
            slen_t i = (x[0] * 19937 + 1) % lx;
            if (x[i] != y[i])
            {
                return true;
            }        
        }
    }

    const dunit_t* p1 = (dunit_t*)x;
    const dunit_t* p2 = (dunit_t*)y;
    const dunit_t* pe = lx & 1? (dunit_t*)(x + lx - 1): (dunit_t*)(x + lx);

    while (p1 != pe)
    {  
        if (*p1++ != *p2++)
        {
            return true;
        }
    }
    return false;
}

slen_t __cmp_core(const unit_t* x, slen_t lx, const unit_t* y, slen_t ly)
{
    assert(lx >= 0 && ly >= 0);

    if (lx - ly)
    {
        return lx - ly > 0? 1: -1;
    }
    if (lx & 1)
    {
        lx--;
        if (x[lx] > y[lx])
        {
            return 1;
        }
        else if (x[lx] < y[lx])
        {
            return -1;
        }
    }

    const dunit_t* p1 = (dunit_t*)(x + lx - 2);
    const dunit_t* p2 = (dunit_t*)(y + lx - 2);
    const dunit_t* pe = (dunit_t*)(x - 2);

    for (dunit_t d; p1 != pe; p1--, p2--)
    {  
        if ((d = *p1 - *p2))
        {
            return d <= *p1? 1: -1;
        }
    }
    return 0;
}

slen_t __add_core(const unit_t* x, slen_t lx, const unit_t* y, slen_t ly, unit_t* res)
{
    assert(lx >= ly);

    slen_t i = 0;
    dunit_t carry = 0;
    while (i < ly)
    {
        carry += (dunit_t)x[i] + y[i];
        res[i++] = carry & MASK;
        carry >>= SHIFT;
    }
    while (i < lx)
    {
        carry += x[i];
        res[i++] = carry & MASK;
        carry >>= SHIFT;
    }
    if (carry)
    {
        res[i++] = (unit_t)carry;
    }
    __trim_leading_zeros(res, i);
    return i;
}

slen_t __sub_core(const unit_t* x, slen_t lx, const unit_t* y, slen_t ly, unit_t* res)
{
    dunit_t borrow = 0;
    slen_t i = 0;
    for (; i < ly; i++)
    {
        borrow = (dunit_t)x[i] - y[i] - borrow;
        res[i] = borrow & MASK;
        borrow >>= SHIFT;
        borrow &= 1;
    }
    for (; i < lx; i++)
    {
        borrow = (dunit_t)x[i] - borrow;
        res[i] = borrow & MASK;
        borrow >>= SHIFT;
        borrow &= 1;
    }
    __trim_leading_zeros(res, i);
    return i;
}

slen_t __mul_core(const unit_t* x, slen_t lx, const unit_t* y, slen_t ly, unit_t* res)
{
    unit_t* pr;
    const unit_t* py;       
    for (slen_t i = 0; i < lx; i++)
    {
        pr = res + i;
        py = y;
        dunit_t carry = 0;
        while (py < y + ly)
        {
            carry += *pr + (dunit_t)*py++ * x[i];
            *pr++ = carry & MASK;
            carry >>= SHIFT;
        }
        if (carry)
        {
            *pr = (unit_t)carry;
        }
    }
    lx += ly;
    __trim_leading_zeros(res, lx);
    return lx;
}

slen_t __sqr_core(const unit_t* x, slen_t lx, unit_t* res)
{
    __set_units_zero(res, 2 * lx);

    unit_t cflag;
    dunit_t m, carry;
    const unit_t* p, *q;
    const unit_t* pend = x + lx - 1;
    const unit_t* qend = x + lx;
    unit_t* r;

    slen_t i = 0;
    for (p = x; p != pend; p++, i++)
    {
        r = res + (i << 1);
        carry = *r + (dunit_t)*p * *p;
        *r++ = carry & MASK;
        carry >>= SHIFT;

        m = (dunit_t)*p << 1;
        cflag = m >> UNITBITS;
        m &= MASK;
        for (q = p + 1; q != qend; q++, r++)
        {
            carry += *r + (dunit_t)*q * m;
            *r = carry & MASK;
            carry = (carry >> SHIFT) + ((dunit_t)*q * cflag);
        }
        if (carry)
        {
            *(dunit_t*)r += carry;
        }
    }

    lx <<= 1;
    r = res + lx - 2;
    carry = *r + (dunit_t)*p * *p;
    *r++ = carry & MASK;
    *r += carry >> SHIFT;
    __trim_leading_zeros(res, lx);
    return lx;
}

void __div_unit_core(const unit_t* x, slen_t lx, unit_t y, unit_t* q, slen_t* lq, unit_t* r, slen_t* lr)
{
    assert(y != 0);

    dunit_t rem = 0;
    const unit_t* px = x + lx - 1;
    unit_t* pq = q + lx - 1;
    while (pq >= q)
    {
        rem = __make_dunit((unit_t)rem, *px--);
        *pq-- = unit_t(rem / y);
        rem %= y;
    }
    *r = (unit_t)rem;
    *lq = lx;
    *lr = 1;
    __trim_leading_zeros(q, *lq);
    __trim_leading_zeros(r, *lr);
}

unit_t __guess_quotient(unit_t x1, unit_t x2, unit_t x3, unit_t y1, unit_t y2)
{
    dunit_t t, r;
    dunit_t x1x2 = __make_dunit(x1, x2);

    t = x1x2 / y1;
    if (t >= BASE)
    {
        t = MASK;
    }
    r = x1x2 - t * y1;
    while (r < BASE && t * y2 > __make_dunit(r & MASK, x3))
    {
        t--;
        r += y1;
    }
    return t & MASK;
}

unit_t __truing_quotient(unit_t* x, const unit_t* y, slen_t len, unit_t trial)
{
    dunit_t borrow = 0, carry = 0;
    for (slen_t i = 0; i < len; i++)
    {
        carry += (dunit_t)trial * y[i];
        borrow = x[i] - (carry & MASK) - borrow;
        x[i] = borrow & MASK;
        carry >>= SHIFT;
        borrow >>= SHIFT;
        borrow &= 1;
    }
    borrow = (dunit_t)x[len] - carry - borrow;
    x[len] = borrow & MASK;
    borrow >>= SHIFT;

    if (borrow)
    {
        carry = 0;
        for (slen_t i = 0; i < len; i++)
        {
            carry += (dunit_t)x[i] + y[i];
            x[i] = carry & MASK;
            carry >>= SHIFT;
        }
        --trial;
    }
    return trial;
}

slen_t __div_core(unit_t* x, slen_t lx, const unit_t* y, slen_t ly, unit_t* q)
{
    assert(ly >= 2);
    assert(lx >= ly);
    assert(*(x + lx - 1) < *(y + ly - 1));

    slen_t k = lx - ly;
    unit_t y1 = *(y + ly - 1);
    unit_t y2 = *(y + ly - 2);
    unit_t *xk = x + k - 1;
    unit_t *qk = q + k - 1;
    while (xk >= x)
    {
        unit_t trial = __guess_quotient(xk[ly], xk[ly - 1], xk[ly - 2], y1, y2);
        *qk-- = __truing_quotient(xk, y, ly, trial);
        xk--;
    }
    __trim_leading_zeros(q, k);
    return k;
}

slen_t __shl_core(unit_t* x, slen_t lx, int d)
{
    slen_t i = 0;
    unit_t carry = 0;
    for (; i != lx; i++)
    {
        dunit_t acc = (dunit_t)x[i] << d | carry;
        x[i] = acc & MASK;
        carry = acc >> SHIFT;
    }
    if (carry)
    {
        x[i++] = carry;
    }
    return i;
}

unit_t __shr_core(unit_t* x, slen_t lx, int d)
{
    slen_t i = lx;
    unit_t mask = ((unit_t)1 << d) - 1, carry = 0;
    while (i-- > 0)
    {
        dunit_t acc = (dunit_t)carry << SHIFT | x[i];
        carry = acc & mask;
        x[i] = (unit_t)(acc >> d);
    }
    return carry;
}

slen_t __bit_and_core(const unit_t* x, const unit_t* y, slen_t lx, unit_t* res)
{
    unit_t* p = res;
    const unit_t* e = x + lx;
    while (x != e)
    {
        *p++ = *x++ & *y++;
    }
    __trim_leading_zeros(res, lx);
    return lx;
}

slen_t __bit_or_core(const unit_t* x, slen_t lx, const unit_t* y, slen_t ly, unit_t* res)
{
    assert(lx >= ly);

    unit_t* p = res;
    const unit_t* e1 = x + ly;
    const unit_t* e2 = x + lx;
    while (x != e1)
    {
        *p++ = *x++ | *y++;
    }
    while (x != e2)
    {
        *p++ = *x++;
    }
    __trim_leading_zeros(res, lx);
    return lx;
}

slen_t __bit_xor_core(const unit_t* x, slen_t lx, const unit_t* y, slen_t ly, unit_t* res)
{
    assert(lx >= ly);

    unit_t* p = res;
    const unit_t* e1 = x + ly;
    const unit_t* e2 = x + lx;
    while (x != e1)
    {
        *p++ = *x++ ^ *y++;
    }
    while (x != e2)
    {
        *p++ = *x++;
    }
    __trim_leading_zeros(res, lx);
    return lx;
}

slen_t __bit_not_core(const unit_t* x, slen_t lx, unit_t* res)
{
    assert(lx > 0);

#if UNITBITS == 16
    typedef signed short sunit_t;
#elif UNITBITS == 32
    typedef signed int sunit_t;
#endif

    unit_t* p = res;
    const unit_t* e = x + lx;
    sunit_t s = sunit_t(unit_t(1) << (UNITBITS - 1));
    s >>= (UNITBITS - __vbits_count(*(e - 1)));
    while (x != e)
    {
        *p++ = ~*x++;
    }
    *(p - 1) &= ~s;
    __trim_leading_zeros(res, lx);
    return lx;
}

unsigned char __CHAR_DIGIT[256] =
{
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
     0,  1,  2,  3,  4,  5,  6,  7,  8,  9, -1, -1, -1, -1, -1, -1,
    -1, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
    25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 44, 45, 46, -1, -1,
    -1, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
    25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
};

int __char_digit(char c)
{
    return __CHAR_DIGIT[(unsigned char)c];
}

bool __char_digit_valid(char c, int base)
{
    return __CHAR_DIGIT[(unsigned char)c] < base;
}

#ifdef __GNUC__
slen_t __vbits_count(unit_t x)
{
    assert(x != 0);

    return UNITBITS - __builtin_clz((unsigned int)x << (32 - UNITBITS));
}
#elif defined(_MSC_VER)
#include <intrin.h>
slen_t __vbits_count(unit_t x)
{
    assert(x != 0);

    unsigned long b;
    _BitScanReverse(&b, x);
    return b + 1;
}
#else
static const char __msb_256_table[256] =
{
    0, 0, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3,
    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
    5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
};

static __always_inline(int) __bsr32(unsigned int bb)
{
    int result = 0;
    if (bb > 0xFFFF)
    {
		bb >>= 16;
		result += 16;
    }
    if (bb > 0xFF)
    {
		bb >>= 8;
		result += 8;
    }
    return (result + __msb_256_table[bb]);
}

slen_t __vbits_count(unit_t x)
{
    assert(x != 0);

    return __bsr32((unsigned int)x) + 1;
}
#endif

} // namespace end
