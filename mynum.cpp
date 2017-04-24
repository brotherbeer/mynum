/* MYNUM LIBARAY SOURCE CODE
 *
 * This library provides addition, subtraction, multiplication, division,
 * modulo, exponentiation and bitwise operations for big integer arithmetic.
 * The author hopes mynum will be useful, but dose not make any warranty.
 *
 * If you have any questions, please contact <brotherbeer@163.com>
 */

#include <cmath>
#include <cctype>
#include <cstring>
#include <cassert>
#include "mynum.h"


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

#if INT_MAX != 2147483647 || UINT_MAX != 0xffffffff
#error int is not 32-bit, not supported
#endif
#if LLONG_MAX != 9223372036854775807LL || ULLONG_MAX != 0xffffffffffffffffULL
#error long long is not 64-bit, not supported
#endif
#if LONG_MAX != INT_MAX && LONG_MAX != LLONG_MAX
#error long is neither 32-bit nor 64-bit, not supported
#endif

namespace mynum {

const float LN_8 = 2.0794f;   // log(8)
const float LN_10 = 2.3026f;  // log(10)
const int KMUL_THRESHOLD = 80;
const int KSQR_THRESHOLD = 120;

#if UNITBITS == 16
const int POWERDEC_BASE_DIGITS = 4;
const int POWEROCT_BASE_DIGITS = 5;
const unit_t POWERDEC_BASE = 10000;
const unit_t POWEROCT_BASE = 0100000;
const float LN_BASE = 11.0903f;            // log(BASE)
const float LN_POWERDEC_BASE = 9.2103f;    // log(POWERDEC_BASE)
const float LN_POWEROCT_BASE = 10.3972f;   // log(POWEROCT_BASE)

#elif UNITBITS == 32
const int POWERDEC_BASE_DIGITS = 9;
const int POWEROCT_BASE_DIGITS = 10;
const unit_t POWERDEC_BASE = 1000000000;
const unit_t POWEROCT_BASE = 010000000000;
const float LN_BASE = 22.1807f;
const float LN_POWERDEC_BASE = 20.7233f;
const float LN_POWEROCT_BASE = 20.7944f;

#endif

__force_inline(int) __max_base();
__force_inline(int) __is_valid(int b);

struct _radix_t
{
    unit_t power_base_digits;
    unit_t power_base;
    float ln_base;
    float ln_power_base;

    _radix_t(int base): power_base_digits(1), power_base(base)
    {
        assert(__is_valid(base));

        while ((dunit_t)power_base * base < (dunit_t)UNITMAX)
        {
            power_base_digits++;
            power_base *= base;
        }
        ln_base = log((float)base);
        ln_power_base = log((float)power_base);
    }
};

static __force_inline(unit_t*) __allocate_units(slen_t s)
{
    return (unit_t*)mem::allocate(s + (s & 1), sizeof(unit_t));
}

static __force_inline(unit_t*) __allocate_units(slen_t units, slen_t* pcap)
{
    *pcap = units + (units & 1);
    return (unit_t*)mem::allocate(*pcap, sizeof(unit_t));
}

static __force_inline(void) __deallocate_units(unit_t* p)
{
    return mem::deallocate(p);
}

static __force_inline(unit_t*) __set_units_zero(unit_t* p, slen_t l)
{
    return (unit_t*)memset(p, 0, l * sizeof(unit_t));
}

static __force_inline(unit_t*) __copy_units(unit_t* d, const unit_t* s, slen_t l)
{
    return (unit_t*)memcpy(d, s, l * sizeof(unit_t));
}

static __force_inline(unit_t*) __move_units(unit_t* d, const unit_t* s, slen_t l)
{
    return (unit_t*)memmove(d, s, l * sizeof(unit_t));
}

static __force_inline(int) __char_digit(char c);
static __force_inline(bool) __char_digit_valid(char c, int base);
static __force_inline(slen_t) __vbits_count(unit_t x);
static __force_inline(slen_t) __tzbits_count(unit_t x);
static __force_inline(slen_t) __pop_count(unit_t x);
static __force_inline(dunit_t) __mul_dunit_high(dunit_t x, dunit_t y);
static __force_inline(dunit_t) __mul_add_dunit(dunit_t x, dunit_t y, dunit_t z, dunit_t* low);
static __force_inline(dunit_t) __qunit_by_dunit(dunit_t h, dunit_t l, dunit_t d, dunit_t* r);
static __force_inline(dunit_t) __qunit_mod_dunit(dunit_t h, dunit_t l, dunit_t d);

#define __sign_shift(x) ((x) >> ((sizeof(slen_t) << 3) - 1))

static __force_inline(slen_t) __abs(slen_t x)
{
    slen_t y = __sign_shift(x);
    return (x + y) ^ y;
}

static __force_inline(int) __sign(slen_t x)
{
    return __sign_shift(x) | 1;
}

static __force_inline(int) __sign(slen_t x, slen_t y)
{
    return __sign_shift(x ^ y) | 1;
}

static __force_inline(dunit_t) __make_dunit(unit_t high, unit_t low)
{
    return (dunit_t)high << UNITBITS | low;
}

static __force_inline(dunit_t) __make_dunit(dunit_t high, unit_t low)
{
    return high << UNITBITS | low;
}

/** class number_t implementation */

number_t::number_t(const char* s)
{
    if (s)
    {
        __construct_from_xbase_string(s, (slen_t)strlen(s), 10, LN_10, POWERDEC_BASE, POWERDEC_BASE_DIGITS);
    }
}

number_t::number_t(const char* s, int base)
{
    if (s && __is_valid(base))
    {
        __construct_from_string(s, (slen_t)strlen(s), base);
    }
}

number_t::number_t(const char* s, size_t l, int base)
{
    if (s && __is_valid(base))
    {
        __construct_from_string(s, l, base);
    }
}

number_t::number_t(const string_t& s)
{
    if (s.dat)
    {
        __construct_from_string(s.dat, s.len, 10);
    }
}

number_t::number_t(const string_t& s, int base)
{
    if (s.dat && __is_valid(base))
    {
        __construct_from_string(s.dat, s.len, base);
    }
}

number_t::number_t(const string_t& s, size_t bpos, size_t epos, int base)
{
    if (s.dat && __is_valid(base))
    {
        if (epos > s.len)
        {
            epos = s.len;
        }
        if (bpos <= epos)
        {
            __construct_from_string(s.dat + bpos, epos - bpos, base);
        }
    }
}

number_t::number_t(const number_t& another)
{
    __copy(another);
}

number_t::number_t(const number_t& another, size_t bbit, size_t ebit)
{
    if (ebit > bbit)
    {
        __reserve((ebit - bbit) / UNITBITS + 2);
        __construct_from_bit_range(another, bbit, ebit);
    }
}

#define __construct_from_signed_ordinary(type, x) { \
    slen_t sign = 1; \
    if (x < 0) {x = -x; sign = -1;} \
    __reserve(len = sizeof(type) / sizeof(unit_t)); \
    *(type*)dat = x; \
    __trim_leading_zeros(dat, len); \
    len *= sign; \
}

#define __construct_from_unsigned_ordinary(type, x) { \
    __reserve(len = sizeof(type) / sizeof(unit_t)); \
    *(type*)dat = x; \
    __trim_leading_zeros(dat, len); \
}

number_t::number_t(int x)                { __construct_from_signed_ordinary(int, x); }
number_t::number_t(long x)               { __construct_from_signed_ordinary(long, x); }
number_t::number_t(long long x)          { __construct_from_signed_ordinary(long long, x); }
number_t::number_t(unsigned int x)       { __construct_from_unsigned_ordinary(unsigned int, x); }
number_t::number_t(unsigned long x)      { __construct_from_unsigned_ordinary(unsigned long, x); }
number_t::number_t(unsigned long long x) { __construct_from_unsigned_ordinary(unsigned long long, x); }

#define __assign_from_signed_ordinary(type, x) { \
    len = sizeof(type) / sizeof(unit_t); \
    if (cap < len) { __deallocate_units(dat); __reserve(len); } \
    int s = 1; \
    if (x > 0) *(type*)dat = x; else { s = -1; *(type*)dat = -x; } \
    __trim_leading_zeros(dat, len); \
    len *= s; \
    return *this; \
}

#define __assign_from_unsigned_ordinary(type, x) { \
    len = sizeof(type) / sizeof(unit_t); \
    if (cap < len) { __deallocate_units(dat); __reserve(len); } \
    *(type*)dat = x; \
    __trim_leading_zeros(dat, len); \
    return *this; \
}

number_t& number_t::assign(int x)                { __assign_from_signed_ordinary(int, x); }
number_t& number_t::assign(long x)               { __assign_from_signed_ordinary(long, x); }
number_t& number_t::assign(long long x)          { __assign_from_signed_ordinary(long long, x); }
number_t& number_t::assign(unsigned int x)       { __assign_from_unsigned_ordinary(unsigned int, x); }
number_t& number_t::assign(unsigned long x)      { __assign_from_unsigned_ordinary(unsigned long, x); }
number_t& number_t::assign(unsigned long long x) { __assign_from_unsigned_ordinary(unsigned long long, x); }

number_t& number_t::assign(const number_t& x)
{
    if (this->is_not(x))
    {
        slen_t l = __abs(x.len);
        if (l > cap)
        {
            __deallocate_units(dat);
            __reserve(l);
        }
        __copy_units(dat, x.dat, l);
        len = x.len;
    }
    return *this;
}

number_t& number_t::assign(const number_t& x, size_t bbit, size_t ebit)
{
    if (ebit > bbit)
    {
        if (this->is_not(x))
        {
            slen_t newcap = (ebit - bbit) / UNITBITS + 2;
            if (newcap > cap)
            {
                __deallocate_units(dat);
                __reserve(newcap);
            }
            __construct_from_bit_range(x, bbit, ebit);
        }
        else
        {
            shr(bbit);
            ebit -= bbit;
            len = ebit / UNITBITS;
            if ((ebit %= UNITBITS))
            {
                *(dat + ++len - 1) &= UNITMAX >> (UNITBITS - ebit);
            }
            __trim_leading_zeros(dat, len);
        }
    }
    else
    {
        set_zero();
    }
    return *this;
}

number_t& number_t::assign(const char* s)
{
    set_zero();
    if (s)
    {
        __construct_from_xbase_string(s, (slen_t)strlen(s), 10, LN_10, POWERDEC_BASE, POWERDEC_BASE_DIGITS);
    }
    return *this;
}

number_t& number_t::assign(const char* s, int base)
{
    set_zero();
    if (s && __is_valid(base))
    {
        __construct_from_string(s, (slen_t)strlen(s), base);
    }
    return *this;
}

number_t& number_t::assign(const char* s, size_t l, int base)
{
    set_zero();
    if (s && __is_valid(base))
    {
        __construct_from_string(s, l, base);
    }
    return *this;
}

number_t& number_t::assign(const string_t& s)
{
    return assign(s.dat, s.len, 10);
}

number_t& number_t::assign(const string_t& s, int base)
{
    return assign(s.dat, s.len, base);
}

number_t& number_t::assign(const string_t& s, size_t bpos, size_t epos, int base)
{
    if (epos > s.len)
    {
        epos = s.len;
    }
    if (bpos < epos)
    {
        return assign(s.dat + bpos, epos - bpos, base);
    }
    return *this;
}

number_t::~number_t()
{
    release();
}

void number_t::bits_reserve(size_t n)
{
    reserve((n + UNITBITS - 1) / UNITBITS);
}

void number_t::fill_unused_capacity(unit_t v)
{
    unit_t *p = dat + __abs(len), *e = dat + cap;
    while (p != e)
    {
        *p++ = v;
    }
}

number_t& number_t::halve()
{
    return shr(1);
}

number_t& number_t::twice()
{
    return shl(1);
}

string_t& number_t::to_bin_string(string_t& res) const
{
    return __to_bin_string(res);
}

string_t& number_t::to_oct_string(string_t& res) const
{
    return __to_xbase_string(res, 8, POWEROCT_BASE, POWEROCT_BASE_DIGITS, LN_POWEROCT_BASE);
}

string_t& number_t::to_dec_string(string_t& res) const
{
    return __to_xbase_string(res, 10, POWERDEC_BASE, POWERDEC_BASE_DIGITS, LN_POWERDEC_BASE);
}

string_t& number_t::to_hex_string(string_t& res) const
{
    return __to_hex_string(res);
}

string_t& number_t::to_string(string_t& res, int base) const
{
    if (__is_valid(base)) switch (base)
    {
        case  2: return __to_bin_string(res);
        case  8: return __to_xbase_string(res, 8,  POWEROCT_BASE, POWEROCT_BASE_DIGITS, LN_POWEROCT_BASE);
        case 10: return __to_xbase_string(res, 10, POWERDEC_BASE, POWERDEC_BASE_DIGITS, LN_POWERDEC_BASE);
        case 16: return __to_hex_string(res);
        default:
        {
            _radix_t r(base);
            return __to_xbase_string(res, base, r.power_base, r.power_base_digits, r.ln_power_base);
        }
    }
    res.release();
    return res;
}

unit_t number_t::absrem_unit(unit_t x) const
{
    if (len && x)
    {
        return __mod_unit_core(dat, __abs(len), x);
    }
    return 0;
}

unit_t number_t::absrem_unit(const UDM& udm) const
{
    if (len && udm.divisor)
    {
        return __mod_unit_core(dat, __abs(len), udm);
    }
    return 0;
}

void number_t::add_unit(unit_t x)
{
    if (len > 0)
    {
        len = __abs_add_unit(x);
    }
    else if (len == 0)
    {
        if (x)
        {
            if (!cap)
            {
                __reserve(1);
            }
            *dat = x;
            len = 1;
        }
    }
    else
    {
        if (*dat >= x || len < -1)
        {
            len = 0 - __abs_sub_unit(x);
        }
        else
        {
            *dat = x - *dat;
            len = 1;
        }
    }
}

void number_t::sub_unit(unit_t x)
{
    if (len > 0)
    {
        if (*dat >= x || len > 1)
        {
            len = __abs_sub_unit(x);
        }
        else
        {
            *dat = x - *dat;
            len = -1;
        }
    }
    else if (len == 0)
    {
        if (x)
        {
            if (!cap)
            {
                __reserve(1);
            }
            *dat = x;
            len = -1;
        }
    }
    else
    {
        len = 0 - __abs_add_unit(x);
    }
}

void number_t::mul_unit(unit_t x)
{
    unit_t carry;
    slen_t l = __abs(len);

    if ((carry = __mul_unit_core(dat, l, x, dat)))
    {
        if (cap > l)
        {
            dat[l] = carry & UNITMAX;
        }
        else
        {
            unit_t* tmp = __allocate_units((cap = l << 1));
            __copy_units(tmp, dat, l);
            tmp[l] = carry & UNITMAX;
            __deallocate_units(dat);
            dat = tmp;
        }
        l++;
    }
    __trim_leading_zeros(dat, l);
    len = l * __sign(len);
}

unit_t number_t::div_unit(unit_t x)
{
    unit_t r = 0;
    slen_t l = __abs(len);

    if (l && x)
    {
        r = __div_unit_core(dat, l, x, dat, &l);
        len = l * __sign(len);
    }
    return r;
}

unit_t number_t::div_unit(const UDM& udm)
{
    dunit_t r = 0, h;
    slen_t l = __abs(len);
    unsigned char shift = udm.shift;

    if (udm.notpo2)
    {
        unit_t* p = dat + l;
        if (udm.nooverflow) while (--p >= dat)
        {
            r = __make_dunit(r, *p);
            h = __mul_dunit_high(r, udm.multiplier) >> shift;
            r -= h * udm.divisor;
            *p = unit_t(h);
        }
        else while (--p >= dat)
        {
            r = __make_dunit(r, *p);
            h = __mul_dunit_high(r, udm.multiplier);
            h = (((r - h) >> 1) + h) >> shift;
            r -= h * udm.divisor;
            *p = unit_t(h);
        }
    }
    else
    {
        r = __shr_core(dat, l, shift);
    }
    __trim_leading_zeros(dat, l);
    len = l * __sign(len);
    return r & UNITMAX;
}

void number_t::mod_unit(unit_t x)
{
    if (len && x)
    {
        if ((*dat = __mod_unit_core(dat, __abs(len), x)))
        {
            len = len < 0? -1: 1;
        }
        else
        {
            len = 0;
        }
    }
}

void number_t::mod_unit(const UDM& udm)
{
    if (len && udm.divisor)
    {
        if ((*dat = __mod_unit_core(dat, __abs(len), udm)))
        {
            len = len < 0? -1: 1;
        }
        else
        {
            len = 0;
        }
    }
}

void number_t::bit_and_unit(unit_t x)
{
    if (len)
    {
        if ((dat[0] &= x))
        {
            len = __sign(len);
        }
        else
        {
            len = 0;
        }
    }
}

void number_t::bit_or_unit(unit_t x)
{
    if (len)
    {
        dat[0] |= x;
    }
    else
    {
        assign(x);
    }
}

void number_t::bit_xor_unit(unit_t x)
{
    if (len)
    {
        if (!(dat[0] ^= x) && __abs(len) == 1)
        {
            len = 0;
        }
    }
    else
    {
        assign(x);
    }
}

void number_t::add_word(word_t x)
{
    if (len > 0)
    {
        len = __abs_add_word(x);
    }
    else if (len == 0)
    {
        if (x)
        {
            if (!cap)
            {
                __reserve(2);
            }
            *(word_t*)dat = x;
            len = 1 + (x > UNITMAX);
        }
    }
    else
    {
        len = 0 - __abs_sub_word(x);
    }
}

void number_t::sub_word(word_t x)
{
    if (len > 0)
    {
        len = __abs_sub_word(x);
    }
    else if (len == 0)
    {
        if (x)
        {
            if (!cap)
            {
                __reserve(2);
            }
            *(word_t*)dat = x;
            len = -2 + (dat[1] == 0);
        }
    }
    else
    {
        len = 0 - __abs_add_word(x);
    }
}

void number_t::mul_word(word_t x)
{
    slen_t l = __abs(len), m = (l + (l & 1)) / 2;
    word_t *w = (word_t*)dat, *e = w + m, carry = 0;

    if (l & 1)
    {
        *(e - 1) &= UNITMAX;
    }
    for (; w != e; w++)
    {
        // multiply *w and x, and add carry to the result
        // see issue #9
        carry = __mul_add_dunit(*w, x, carry, w);
    }
    if (carry)
    {
        if (cap / 2 > m)
        {
            *e = carry;
        }
        else
        {
            unit_t* tmp = __allocate_units((cap = m * 4));
            __copy_units(tmp, dat, m * 2);
            *((word_t*)tmp + m) = carry;
            __deallocate_units(dat);
            dat = tmp;
        }
        m++;
    }
    l = m * 2;
    __trim_leading_zeros(dat, l);
    len = l * __sign(len);
}

word_t number_t::div_word(word_t x)
{
    if (len && x)
    {
        slen_t l = __abs(len), m = (l + (l & 1)) / 2;
        word_t *e = (word_t*)dat, *q = e + m, rem = 0;

        if (l & 1)
        {
            *(q - 1) &= UNITMAX;
        }
        while (--q >= e)
        {
            *q = __qunit_by_dunit(rem, *q, x, &rem);
        }
        __trim_leading_zeros(dat, l);
        len = l * __sign(len);
        return rem;
    }
    return 0;
}

void number_t::mod_word(word_t x)
{
    if (len && x)
    {
        slen_t l = __abs(len), m = (l + (l & 1)) / 2;
        word_t *e = (word_t*)dat, *q = e + m, rem = 0;

        if (l & 1)
        {
            *(q - 1) &= UNITMAX;
        }
        while (--q >= e)
        {
            rem = __qunit_mod_dunit(rem, *q, x);
        }
        if (rem)
        {
            *e = rem;
            len = (rem >= BASE? 2: 1) * __sign(len);
        }
        else
        {
            set_zero();
        }
    }
}

void number_t::bit_and_word(word_t x)
{
    if (len)
    {
        slen_t l = __abs(len);
        if (l == 1)
        {
            *dat &= x & UNITMAX;
        }
        else
        {
            l = 2;
            *(word_t*)dat &= x;
        }
        __trim_leading_zeros(dat, l);
        len = l * __sign(len);
    }
}

void number_t::bit_or_word(word_t x)
{
    if (len)
    {
        word_t *w = (word_t*)dat;
        slen_t l = __abs(len), m = (l + (l & 1)) / 2;

        if (l & 1)
        {
            *(w + m - 1) &= UNITMAX;
        }
        if (l == 1)
        {
            l++;
        }
        *(word_t*)dat |= x;
        __trim_leading_zeros(dat, l);
        len = l * __sign(len);
    }
    else
    {
        assign(x);
    }
}

void number_t::bit_xor_word(word_t x)
{
    if (len)
    {
        word_t *w = (word_t*)dat;
        slen_t l = __abs(len), m = (l + (l & 1)) / 2;

        if (l & 1)
        {
            *(w + m - 1) &= UNITMAX;
        }
        if (l == 1)
        {
            l++;
        }
        *w ^= x;
        __trim_leading_zeros(dat, l);
        len = l * __sign(len);
    }
    else
    {
        assign(x);
    }
}

void number_t::add_sword(sword_t x)
{
    if (x > 0)
    {
        add_word(x);
    }
    else
    {
        sub_word(-x);
    }
}

void number_t::sub_sword(sword_t x)
{
    if (x > 0)
    {
        sub_word(x);
    }
    else
    {
        add_word(-x);
    }
}

void number_t::mul_sword(sword_t x)
{
    if (x >= 0)
    {
        mul_word(x);
    }
    else
    {
        mul_word(-x);
        set_neg();
    }
}

word_t number_t::div_sword(sword_t x)
{
    word_t r;
    if (x > 0)
    {
        r = div_word(x);
    }
    else
    {
        r = div_word(-x);
        set_neg();
    }
    return r;
}

void number_t::mod_sword(sword_t x)
{
    if (x > 0)
    {
        mod_word(x);
    }
    else
    {
        mod_word(-x);
    }
}

void number_t::bit_and_sword(sword_t x)
{
    if (x >= 0)
    {
        bit_and_word(x);
    }
    else
    {
        bit_and_word(-x);
        set_neg();
    }
}

void number_t::bit_or_sword(sword_t x)
{
    if (x >= 0)
    {
        bit_or_word(x);
    }
    else
    {
        bit_or_word(-x);
        set_neg();
    }
}

void number_t::bit_xor_sword(sword_t x)
{
    if (x >= 0)
    {
        bit_xor_word(x);
    }
    else
    {
        bit_xor_word(-x);
        set_neg();
    }
}

number_t& number_t::bit_remove(size_t bpos, size_t epos)
{
    if (dat)
    {
        slen_t l = __abs(len);
        unit_t *p, *q, *e = dat + l, m1, m2, over;

        p = dat + bpos / UNITBITS;
        q = dat + epos / UNITBITS;
        m1 = bpos % UNITBITS;
        m2 = epos % UNITBITS;
        if (q >= e && p < e)
        {
            l = p - dat + 1;
            *p &= ~(UNITMAX << m1);
            __trim_leading_zeros(dat, l);
            len = l * __sign(len);
        }
        else if (q < e)
        {
            if (p < q)
            {
                if (m1)
                {
                    int d1 = UNITBITS - m1;
                    int d2 = UNITBITS - m2;
                    *p &= UNITMAX >> d1;
                    if (d1 >= d2)
                    {
                        __shr_core(q, e - q, d1 - d2);
                        *q &= UNITMAX << (UNITBITS - d1);
                        *p++ |= *q++;
                    }
                    else
                    {
                        over = __shr_core(q, e - q, d1 + UNITBITS - d2);
                        over >>= UNITBITS - d2;
                        *p++ |= over << (UNITBITS - d1);
                    }
                }
                else
                {
                    __shr_core(q, e - q, m2);
                }
                while (q != e)
                {
                    *p++ = *q++;
                }
                l = p - dat;
            }
            else if (p == q)
            {
                unit_t t0, t1;
                slen_t d = epos - bpos;

                over = 0;
                if (p != dat + l)
                {
                    over = __shr_core(p + 1, e - p - 1, d);
                }
                t0 = *p & (UNITMAX << m2);
                t1 = m1? *p & (UNITMAX >> (UNITBITS - m1)): 0;
                over <<= UNITBITS - d;
                *p = over | (t0 >> d | t1);
            }
            __trim_leading_zeros(dat, l);
            len = l * __sign(len);
        }
    }
    return *this;
}

number_t& number_t::bit_insert(size_t pos, size_t size, bool v)
{
    slen_t l = __abs(len), newcap, newlen;
    size_t bcnt = bits_count();

    if (pos < bcnt)
    {
        newlen = (bcnt + size + UNITBITS - 1) / UNITBITS;
        if (newlen > cap)
        {
            unit_t* tmp = __allocate_units(newlen, &newcap);
            __copy_units(tmp, dat, l);
            __deallocate_units(dat);
            dat = tmp;
            cap = newcap;
        }
        fill_unused_capacity(0);
        unit_t* p = dat + pos / UNITBITS;
        if (size >= UNITBITS)
        {
            unit_t* q = p + (size - UNITBITS) / UNITBITS + 1;
            __move_units(q, p, dat + l - p);
            __shl_core(q, dat + l - p, size % UNITBITS);
        }
        else
        {
            unit_t tmp = *p;
            __shl_core(p, dat + l - p, size);
            *p |= tmp & ~(UNITMAX << (pos % UNITBITS));
        }
        __trim_leading_zeros(dat, newlen);
        len = newlen * __sign(len);
    }
    return bit_set(pos, pos + size, v);
}

bool number_t::bit_at(size_t x) const
{
    slen_t i = x / UNITBITS;
    slen_t r = x % UNITBITS;

    if (i < __abs(len))
    {
        return (dat[i] & ((unit_t)1 << r)) != 0;
    }
    return false;
}

number_t& number_t::bit_set_one(size_t x)
{
    slen_t i = x / UNITBITS;
    slen_t r = x % UNITBITS, l = __abs(len);

    if (i < l)
    {
        dat[i] |= (unit_t)1 << r;
    }
    else
    {
        if (i >= cap)
        {
            unit_t* tmp = __allocate_units(i + 1, &cap);
            __copy_units(tmp, dat, l);
            __deallocate_units(dat);
            dat = tmp;
        }
        dat[i] = (unit_t)1 << r;
        __set_units_zero(dat + l, i - l);
        len = (i + 1) * __sign(len);
    }
    return *this;
}

number_t& number_t::bit_set_zero(size_t x)
{
    slen_t l = __abs(len);
    slen_t i = x / UNITBITS;
    slen_t r = x % UNITBITS;

    if (i < l)
    {
        dat[i] &= ~((unit_t)1 << r);
        __trim_leading_zeros(dat, l);
        len = l * __sign(len);
    }
    return *this;
}

number_t& number_t::bit_set_flip(size_t x)
{
    slen_t l = __abs(len);
    slen_t i = x / UNITBITS;
    slen_t r = x % UNITBITS;

    if (i < l)
    {
        dat[i] ^= (unit_t)1 << r;
        __trim_leading_zeros(dat, l);
        len = l * __sign(len);
    }
    else
    {
        bit_set_one(x);
    }
    return *this;
}

number_t& number_t::bit_set(size_t x, int v)
{
    if (v > 0)
    {
        bit_set_one(x);
    }
    else if (v == 0)
    {
        bit_set_zero(x);
    }
    else
    {
        bit_set_flip(x);
    }
    return *this;
}

number_t& number_t::bit_set(size_t bpos, size_t epos, int v)
{
    if (bpos < epos)
    {
        slen_t l = __abs(len);
        slen_t ub = bpos / UNITBITS, rb = bpos % UNITBITS;
        slen_t ue = epos / UNITBITS, re = epos % UNITBITS;

        if (ue >= cap)
        {
            unit_t* tmp = __allocate_units(ue + 1, &cap);
            __copy_units(tmp, dat, l);
            __deallocate_units(dat);
            dat = tmp;
        }
        __set_units_zero(dat + l, cap - l);
        unit_t* pb = dat + ub;
        unit_t* pe = dat + ue;

        if (pb != pe)
        {
            if (v > 0)
            {
                *pb++ |= UNITMAX << rb;
                while (pb != pe)
                {
                    *pb++ = UNITMAX;
                }
                *pe |= re? UNITMAX >> (UNITBITS - re): 0;
            }
            else if (v == 0)
            {
                *pb++ &= ~(UNITMAX << rb);
                while (pb != pe)
                {
                    *pb++ = 0;
                }
                *pe &= ~(re? UNITMAX >> (UNITBITS - re): 0);
            }
            else
            {
                *pb++ ^= UNITMAX << rb;
                for (; pb != pe; pb++)
                {
                    *pb = ~*pb;
                }
                *pe ^= re? UNITMAX >> (UNITBITS - re): 0;
            }
        }
        else
        {
            unit_t mask = UNITMAX << rb;
            mask &= re? UNITMAX >> (UNITBITS - re): 0;
            if (v > 0)
            {
                *pb |= mask;
            }
            else if (v == 0)
            {
                *pb &= ~mask;
            }
            else
            {
                *pb ^= mask;
            }
        }

        if (ue + 1 > l)
        {
            l = ue + 1;
        }
        __trim_leading_zeros(dat, l);
        len = l * __sign(len);
    }
    return *this;
}

size_t number_t::bits_count() const
{
    if (len)
    {
        slen_t l = __abs(len);
        return __vbits_count(dat[l - 1]) + (l - 1) * UNITBITS;
    }
    return 0;
}

size_t number_t::tz_count() const
{
    if (len)
    {
        unit_t* p = dat, *e = p + __abs(len);
        while (p != e && !*p)
        {
            p++;
        }
        return __tzbits_count(*p) + (p - dat) * UNITBITS;
    }
    return 0;
}

size_t number_t::pop_count() const
{
    size_t cnt = 0;
    unit_t* p = dat, *e = p + __abs(len);

    while (p != e)
    {
        cnt += __pop_count(*p++);
    }  
    return cnt;
}

bool number_t::is_po2() const
{
    if (!this->is_zero() && len > 0)
    {
        const unit_t* p = dat;
        const unit_t* e = dat + len - 1;

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

bool number_t::is_odd() const
{
    if (!this->is_zero())
    {
        return dat[0] & 1;
    }
    return false;
}

void number_t::release()
{
    __deallocate_units(dat);
    dat = NULL;
    len = cap = 0;
}

void number_t::reserve(size_t units)
{
    if (units > (size_t)cap)
    {
        slen_t l = __abs(len), newcap;
        unit_t* tmp = __allocate_units(units, &newcap);
        __copy_units(tmp, dat, l);
        __deallocate_units(dat);
        dat = tmp;
        cap = newcap;
    }
}

void number_t::clear_and_reserve(size_t units)
{
    release();
    __reserve(units);
}

void number_t::copy(const number_t& another)
{
    if (this->is_not(another))
    {
        __copy(another);
    }
}

void number_t::steal(number_t& x)
{
    if (this->is_not(x))
    {
        __deallocate_units(dat);
        dat = x.dat;
        len = x.len;
        cap = x.cap;
        x.dat = NULL;
        x.len = x.cap = 0;
    }
}

void number_t::set_zero()
{
    len = 0;
}

void number_t::set_one()
{
    if (!cap)
    {
        __reserve(1);
    }
    len = *dat = 1;
}

string_t number_t::operator () (int base) const
{
    return to_string(base);
}

bitref_t number_t::operator [] (size_t x)
{
    return bitref_t(*this, x);
}

bool number_t::in_range_char() const
{
    if (len == 0)
    {
        return true;
    }
    else if (len == 1)
    {
        return *dat <= CHAR_MAX;
    }
    else if (len == -1)
    {
        return *dat <= CHAR_MAX + 1;
    }
    return false;
}

bool number_t::in_range_short() const
{
    if (len == 0)
    {
        return true;
    }
    else if (len == 1)
    {
        return *dat <= SHRT_MAX;
    }
    else if (len == -1)
    {
        return *dat <= SHRT_MAX + 1;
    }
    return false;
}

#define __judge_signed_range(type, typemax, typemin) { \
    slen_t m = sizeof(type) / sizeof(unit_t); \
    if (__abs(len) < m) return true; \
    else if (len ==  m) return *(unsigned type*)dat <=  typemax; \
    else if (len == -m) return *(unsigned type*)dat <= -typemin; \
    return false; \
}

#define __judge_unsigned_range(type) { \
    return len == 0 || (len > 0 && len <= slen_t(sizeof(type) / sizeof(unit_t))); \
}

bool number_t::in_range_int() const
{
    __judge_signed_range(int, INT_MAX, INT_MIN);
}

bool number_t::in_range_long() const
{
    __judge_signed_range(long, LONG_MAX, LONG_MIN);
}

bool number_t::in_range_longlong() const
{
    __judge_signed_range(long long, LLONG_MAX, LLONG_MIN);
}

bool number_t::in_range_uchar() const
{
    if (len == 0)
    {
        return true;
    }
    else if (len == 1)
    {
        return *dat <= UCHAR_MAX;
    }
    return false;
}

bool number_t::in_range_ushort() const
{
    if (len == 0)
    {
        return true;
    }
    else if (len == 1)
    {
        return *dat <= USHRT_MAX;
    }
    return false;
}

bool number_t::in_range_uint() const
{
    __judge_unsigned_range(unsigned int);
}

bool number_t::in_range_ulong() const
{
    __judge_unsigned_range(unsigned long);
}

bool number_t::in_range_ulonglong() const
{
    __judge_unsigned_range(unsigned long long);
}

bool number_t::in_range_word() const
{
#if UNITBITS == 32
    return in_range_ulonglong();
#else
    return in_range_uint();
#endif
}

bool number_t::in_range_sword() const
{
#if UNITBITS == 32
    return in_range_longlong();
#else
    return in_range_int();
#endif
}

char number_t::to_char() const
{
    char v = char(len? *dat: 0);
    return len >= 0? v: -v;
}

short number_t::to_short() const
{
    short v = short(len? *dat: 0);
    return len >= 0? v: -v;
}

unsigned char number_t::to_uchar() const
{
    return (unsigned char)(len? *dat: 0);
}

unsigned short number_t::to_ushort() const
{
    return (unsigned short)(len? *dat: 0);
}

#if UNITBITS == 32

int number_t::to_int() const
{
    if (len)
    {
        return len > 0? *(int*)dat: -*(int*)dat;
    }
    return 0;
}

long long number_t::to_longlong() const
{
    long long v;
    switch (__abs(len))
    {
        case 0:
            v = 0;
            break;
        case 1:
            v = dat[0];
            break;
        case 2:
        default:
            v = *(long long*)dat;
            break;
    }
    return len > 0? v: -v;
}

unsigned int number_t::to_uint() const
{
    if (len)
    {
        return *(unsigned int*)dat;
    }
    return 0;
}

unsigned long long number_t::to_ulonglong() const
{
    unsigned long long v;
    switch (__abs(len))
    {
        case 0:
            v = 0;
            break;
        case 1:
            v = dat[0];
            break;
        case 2:
        default:
            v = *(unsigned long long*)dat;
            break;
    }
    return v;
}

#elif UNITBITS == 16

int number_t::to_int() const
{
    int v = 0;
    if (len)
    {
        v = *(int*)dat;
        if (__abs(len) < 2)
        {
            v &= UNITMAX;
        }
    }
    return len >= 0? v: -v;
}

long long number_t::to_longlong() const
{
    long long v;
    switch (__abs(len))
    {
        case 0:
            v = 0;
            break;
        case 1:
            v = dat[0];
            break;
        case 2:
            v = *(dunit_t*)dat;
            break;
        case 3:
            v = dat[2];
            v <<= 32;
            v |= *(dunit_t*)dat;
            break;
        case 4:
        default:
            v = *(long long*)dat;
            break;
    }
    return len > 0? v: -v;
}

unsigned int number_t::to_uint() const
{
    unsigned int v = 0;
    if (len)
    {
        v = *(unsigned int*)dat;
        if (__abs(len) < 2)
        {
            v &= UNITMAX;
        }
    }
    return v;
}

unsigned long long number_t::to_ulonglong() const
{
    unsigned long long v;
    switch (__abs(len))
    {
        case 0:
            v = 0;
            break;
        case 1:
            v = dat[0];
            break;
        case 2:
            v = *(dunit_t*)dat;
            break;
        case 3:
            v = dat[2];
            v <<= 32;
            v |= *(dunit_t*)dat;
            break;
        case 4:
        default:
            v = *(unsigned long long*)dat;
            break;
    }
    return v;
}

#endif

long number_t::to_long() const
{
#if LONG_MAX == INT_MAX
    return (long)to_int();
#elif LONG_MAX == LLONG_MAX
    return (long)to_longlong();
#endif
}

unsigned long number_t::to_ulong() const
{
#if ULONG_MAX == UINT_MAX
    return (unsigned long)to_uint();
#elif ULONG_MAX == ULLONG_MAX
    return (unsigned long)to_ulonglong();
#endif
}

void number_t::__copy(const number_t& another)
{
    assert(this->is_not(another));

    if (another.dat)
    {
        if (cap != another.cap)
        {
            __deallocate_units(dat);
            __reserve(another.cap);
        }
        len = another.len;
        __copy_units(dat, another.dat, __abs(len));
    }
    else
    {
        release();
    }
}

void number_t::__reserve(slen_t units)
{
    dat = __allocate_units(units, &cap);
}

void number_t::__construct_add(unit_t x)
{
    if (len)
    {
        unit_t* p = dat;
        unit_t *e = dat + len;

        *p += x;
        while (*p < x && ++p != e)
        {
            x = (*p)++;
        }
        if (p == e && *(p - 1) < x)
        {
            *p = 1; len++;
        }
    }
    else
    {
        dat[len++] = x;
    }
}

void number_t::__construct_mul(unit_t x)
{
    unit_t* p = dat;
    dunit_t carry = 0;

    for (slen_t i = 0; i < len; i++)
    {
        carry += (dunit_t)dat[i] * x;
        *p++ = carry & UNITMAX;
        carry >>= UNITBITS;
    }
    if (carry)
    {
        *p++ = carry & UNITMAX;
        len++;
    }
}

slen_t number_t::__abs_add_unit(unit_t x)
{
    assert(len != 0);

    slen_t l = __abs(len);
    unit_t* p = dat;
    unit_t* e = dat + l;

    *p += x;
    while (*p < x && ++p != e)
    {
        x = (*p)++;
    }
    if (p == e && *(p - 1) < x)
    {
        if (cap > l)
        {
            *e = 1;
        }
        else
        {
            unit_t* tmp = __allocate_units((cap = l * 2));
            __copy_units(tmp, dat, l);
            tmp[l] = 1;
            __deallocate_units(dat);
            dat = tmp;
        }
        l++;
    }
    return l; // donot need trimming
}

slen_t number_t::__abs_add_word(word_t x)
{
    assert(len != 0);

    slen_t l = __abs(len);
    slen_t m = (l + (l & 1)) / 2;
    word_t* w = (word_t*)dat, *e = w + m;

    if (l & 1)
    {
        *(e - 1) &= UNITMAX;
    }
    *w += x;
    while (*w < x && ++w != e)
    {
        x = (*w)++;
    }
    if (w == e && *(w - 1) < x)
    {
        if (cap / 2 > m)
        {
            *e = 1;
        }
        else
        {
            unit_t* tmp = __allocate_units((cap = m * 4));
            __copy_units(tmp, dat, m * 2);
            *((word_t*)tmp + m) = 1;
            __deallocate_units(dat);
            dat = tmp;
        }
        m++;
    }
    l = m * 2;
    __trim_leading_zeros(dat, l);
    return l;
}

slen_t number_t::__abs_sub_unit(unit_t x)
{
    assert(len != 0);

    unit_t* p = dat;
    unit_t* e = p + __abs(len), t = *p;

    *p -= x;
    while (*p > t && ++p != e)
    {
        t = (*p)--;
    }
    while (e != dat && !*(e - 1))
    {
        e--;
    }
    return e - dat;
}

slen_t number_t::__abs_sub_word(word_t x)
{
    assert(len != 0);

    slen_t l = __abs(len), s = 1;
    slen_t m = (l + (l & 1)) / 2;
    word_t* w = (word_t*)dat, *e = w + m, t;

    if (l & 1)
    {
        *(e - 1) &= UNITMAX;
    }
    t = *w;
    if (t >= x || l > 2)
    {
        *w -= x;
        while (*w > t && ++w != e)
        {
            t = (*w)--;
        }
    }
    else
    {
        l = 2;
        s = -1;
        *w = x - t;
    }
    __trim_leading_zeros(dat, l);
    return l * s;
}

static __force_inline(unit_t) __strbin_to_unit(const char* p, int l)
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
    assert(len == 0);

    int sign = 1;
    const char *p0, *p1;
    slen_t n;

    if (*s == '-')
    {
        s++;
        l--;
        sign = -1;
    }
    p0 = s + l - UNITBITS;
    p1 = s + l % UNITBITS;
    if ((n = (l + UNITBITS - 1) / UNITBITS) > cap)
    {
        __deallocate_units(dat);
        __reserve(n);
    }
    for (; p0 >= p1; p0 -= UNITBITS)
    {
        dat[len++] = __strbin_to_unit(p0, UNITBITS);
    }
    if (l % UNITBITS)
    {
        dat[len++] = __strbin_to_unit(s, l % UNITBITS);
    }
    __trim_leading_zeros(dat, len);
    len *= sign;
}

static __force_inline(unit_t) __strhex_to_unit(const char* p, int l)
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
    assert(len == 0);

    int sign = 1, k = sizeof(unit_t) << 1;
    const char *p0, *p1;
    slen_t n;

    if (*s == '-')
    {
        s++;
        l--;
        sign = -1;
    }
    p0 = s + l - k;
    p1 = s + l % k;
    if ((n = (l + k - 1) / k) > cap)
    {
        __deallocate_units(dat);
        __reserve(n);
    }
    for (; p0 >= p1; p0 -= k)
    {
        dat[len++] = __strhex_to_unit(p0, k);
    }
    if (l % k)
    {
        dat[len++] = __strhex_to_unit(s, l % k);
    }
    __trim_leading_zeros(dat, len);
    len *= sign;
}

static __force_inline(unit_t) __str_to_unit(const char* p, int base, slen_t l)
{
    unit_t x = 0;
    while (l--)
    {
        x *= base;
        x += __char_digit(*p++);
    }
    return x;
}

void number_t::__construct_from_xbase_string(const char* s, slen_t l, int base, float ln_base, unit_t power_base, unit_t power_base_digits)
{
    assert(len == 0);

    unit_t u, rbase;
    slen_t n, i = 0, sign = 1, k, r;

    if ((n = slen_t(ln_base * l / LN_BASE + 1)) > cap)
    {
        __deallocate_units(dat);
        __reserve(n);
    }
    if (*s == '-')
    {
        s++;
        l--;
        sign = -1;
    }
    r = l % power_base_digits;
    for (k = l - r; i < k; i += power_base_digits)
    {
        if ((u = __str_to_unit(s + i, base, power_base_digits)) < power_base)
        {
            __construct_mul(power_base);
            __construct_add(u);
        }
    }
    if (r && (u = __str_to_unit(s + i, base, r)) < power_base)
    {
        for (rbase = 1; r--;)
        {
            rbase *= base;
        }
        __construct_mul(rbase);
        __construct_add(u);
    }
    __trim_leading_zeros(dat, len);
    len *= sign;
}

void number_t::__construct_from_string(const char* s, slen_t l, int base)
{
    assert(len == 0 && __is_valid(base));

    switch (base)
    {
        case 2:
            __construct_from_bin_string(s, l);
            break;
        case 8:
            __construct_from_xbase_string(s, l, 8,  LN_8, POWEROCT_BASE, POWEROCT_BASE_DIGITS);
            break;
        case 10:
            __construct_from_xbase_string(s, l, 10, LN_10, POWERDEC_BASE, POWERDEC_BASE_DIGITS);
            break;
        case 16:
            __construct_from_hex_string(s, l);
            break;
        default:
        {
            _radix_t r(base);
            __construct_from_xbase_string(s, l, base, r.ln_base, r.power_base, r.power_base_digits);
            break;
        }
    }
}

void number_t::__construct_from_bit_range(const number_t& x, size_t bbit, size_t ebit)
{
    assert (ebit > bbit && x.dat != dat);

    unit_t* p = dat;
    const unit_t* b = x.dat + bbit / UNITBITS;
    const unit_t* e = x.dat + (ebit - 1) / UNITBITS;
    while (b <= e)
    {
        *p++ = *b++;
    }
    len = p - dat;
    if ((ebit %= UNITBITS))
    {
        *(--p) &= UNITMAX >> (UNITBITS - ebit);
    }
    __shr_core(dat, len, bbit % UNITBITS);
    __trim_leading_zeros(dat, len);
}

static const char* __B[16] =
{
    "0000", "0001", "0010", "0011", "0100", "0101",
    "0110", "0111", "1000", "1001", "1010", "1011",
    "1100", "1101", "1110", "1111",
};

string_t& number_t::__to_bin_string(string_t& res) const
{
    slen_t l = __abs(len);

    if (l)
    {
        char* str, *ps;
        byte_t* p = (byte_t*)(dat + l - 1) + sizeof(unit_t) - 1;
        byte_t* e = (byte_t*)dat - 1;

        while (!*p)
        {
            p--;
        }
        size_t chars = l * UNITBITS + (len < 0);
        if (res.cap >= chars)
        {
            ps = str = res.dat;
        }
        else
        {
            ps = str = (char*)mem::allocate(chars + 1, sizeof(char));
        }
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
        if (str != res.dat)
        {
            res.take(str, ps - str);
        }
        else
        {
            res.len = ps - str;
        }
        return res;
    }
    return res.assign('0');
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

int __is_valid(int b)
{
    return b >= 2 && b <= __max_base();
}

string_t& number_t::__to_hex_string(string_t& res) const
{
    slen_t l = __abs(len);

    if (l)
    {
        char* str, *ps;
        byte_t* p = (byte_t*)(dat + l - 1) + sizeof(unit_t) - 1;
        byte_t* e = (byte_t*)dat - 1;

        while (!*p)
        {
            p--;
        }
        size_t chars = l * sizeof(unit_t) * 2 + (len < 0);
        if (res.cap >= chars)
        {
            ps = str = res.dat;
        }
        else
        {
            ps = str = (char*)mem::allocate(chars + 1, sizeof(char));
        }
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
        if (str != res.dat)
        {
            res.take(str, ps - str);
        }
        else
        {
            res.len = ps - str;
        }
        return res;
    }
    return res.assign('0');
}

static __force_inline(void) __unit_to_str(unit_t x, char* str, int base, int width)
{
    assert(__is_valid(base));

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

string_t& number_t::__to_xbase_string(string_t& res, unit_t base, unit_t power_base, unit_t power_base_digits, float ln_power_base) const
{
    slen_t l = __abs(len);

    if (l)
    {
        slen_t size = 0;
        char* str = NULL, *ps = NULL;
        unit_t* tmp = __allocate_units(slen_t(LN_BASE * l / ln_power_base + 1));

        for (slen_t i = l - 1; i >= 0; i--)
        {
            unit_t unit = dat[i];
            for (slen_t j = 0; j < size; j++)
            {
                dunit_t dunit = __make_dunit(tmp[j], unit);
                tmp[j] = unit_t(dunit % power_base);
                unit = unit_t(dunit / power_base);
            }
            while (unit)
            {
                tmp[size++] = unit % power_base;
                unit /= power_base;
            }
        }
        if (size)
        {
            char buf[32];
            unit_t* p = tmp + size - 1;
            unit_t* e = tmp - 1;

            __unit_to_str(*p--, buf, base, power_base_digits);
            size_t chars = size * power_base_digits + (len < 0);
            if (res.cap >= chars)
            {
                ps = str = res.dat;
            }
            else
            {
                ps = str = (char*)mem::allocate(chars + 1, sizeof(char));
            }
            if (len < 0)
            {
                *ps++ = '-';
            }
            slen_t i = 0;
            while (buf[i] == '0')
            {
                i++;
            }
            for (; i < power_base_digits; i++)
            {
                *ps++ = buf[i];
            }
            while (p != e)
            {
                __unit_to_str(*p--, ps, base, power_base_digits);
                ps += power_base_digits;
            }
        }
        __deallocate_units(tmp);
        if (str)
        {
            *ps = '\0';
            if (str != res.dat)
            {
                res.take(str, ps - str);
            }
            else
            {
                res.len = ps - str;
            }
            return res;
        }
    }
    return res.assign('0');
}

/** class UDM implementation */

UDM::UDM(unit_t d): divisor(d), shift(0), notpo2((d & (d - 1)) != 0), nooverflow(true)
{
    if (divisor)
    {
        dunit_t q, r, b, h;
        b = __vbits_count(d);
        shift = (unsigned char)(b - 1);
        if (notpo2)
        {
            h = dunit_t(1) << shift;
            q = __qunit_by_dunit(h, 0, d, &r);
            multiplier = q + 1;
            if (d > r + h)
            {
                multiplier = (q << 1) | 1;
                nooverflow = false;
            }
        }
    }
}

void add_unit(const number_t& a, unit_t x, number_t& res)
{
    res.assign(a).add_unit(x);
}

void sub_unit(const number_t& a, unit_t x, number_t& res)
{
    res.assign(a).sub_unit(x);
}

void mul_unit(const number_t& a, unit_t x, number_t& res)
{
    unit_t carry;
    slen_t la = __abs(a.len);

    if (res.cap < la + 1)
    {
        res.clear_and_reserve(la + 1);
    }
    if ((carry = __mul_unit_core(a.dat, la, x, res.dat)))
    {
        res.dat[la++] = carry & UNITMAX;
    }
    __trim_leading_zeros(res.dat, la);
    res.len = la * __sign(a.len);
}

unit_t div_unit(const number_t& a, unit_t x, number_t& res)
{
    if (a.len && x)
    {
        slen_t la = __abs(a.len), lr;
        if (res.cap < la)
        {
            res.clear_and_reserve(la);
        }
        unit_t r = __div_unit_core(a.dat, la, x, res.dat, &lr);
        res.len = lr * __sign(a.len);
        return r;
    }
    return 0;
}

unit_t div_unit(const number_t& a, const UDM& udm, number_t& q)
{
    if (udm.divisor)
    {
        slen_t la = __abs(a.len), lq;
        if (q.cap < la)
        {
            q.clear_and_reserve(la);
        }
        unit_t r = __div_unit_core(a.dat, la, udm, q.dat, &lq);
        q.len = lq * __sign(a.len);
        return r;
    }
    return 0;
}

void mod_unit(const number_t& a, unit_t x, number_t& res)
{
    if (x)
    {
        slen_t s = __sign(a.len);
        res.assign(__mod_unit_core(a.dat, __abs(a.len), x));
        res.len *= s;
    }
}

void mod_unit(const number_t& a, const UDM& udm, number_t& res)
{
    if (udm.divisor)
    {
        slen_t s = __sign(a.len);
        res.assign(__mod_unit_core(a.dat, __abs(a.len), udm));
        res.len *= s;
    }
}

void bit_and_unit(const number_t& a, unit_t x, number_t& res)
{
    if (a.len)
    {
        slen_t s = __sign(a.len);
        res.assign(a.dat[0] & x);
        res.len *= s;
    }
    else
    {
        res.set_zero();
    }
}

void bit_or_unit(const number_t& a, unit_t x, number_t& res)
{
    res.assign(a).bit_or_unit(x);
}

void bit_xor_unit(const number_t& a, unit_t x, number_t& res)
{
    res.assign(a).bit_xor_unit(x);
}

size_t _try_strlen(const char* p) { return p? strlen(p): 0; }
const char* _try_strchr(const char* p, int c) { return p? strchr(p, c): NULL; }
const char* _try_strstr(const char* p, const char* q) { return p && q? strstr(p, q): NULL; }

/** class string_t implementation */

string_t::string_t(char c): cap(sizeof(dunit_t))
{
    dat = (char*)mem::allocate(cap + 1, sizeof(char));
    dat[0] = c;
    dat[len = 1] = '\0';
}

string_t::string_t(const char* p): dat(NULL), len(0), cap(0)
{
    if (p)
    {
        cap = len = strlen(p);
        dat = (char*)mem::allocate(cap + 1, sizeof(char));
        memcpy(dat, p, len);
        dat[len] = '\0';
    }
}

string_t::string_t(const char* p, size_t l): dat(NULL), len(0), cap(0)
{
    if (p)
    {
        cap = len = l;
        dat = (char*)mem::allocate(cap + 1, sizeof(char));
        memcpy(dat, p, len);
        dat[len] = '\0';
    }
}

string_t::string_t(const string_t& another): dat(NULL), len(0), cap(0)
{
    if (another.dat)
    {
        cap = another.cap;
        len = another.len;
        dat = (char*)mem::allocate(cap + 1, sizeof(char));
        memcpy(dat, another.dat, len);
        dat[len] = '\0';
    }
}

string_t::string_t(const string_t& another, size_t bpos, size_t epos): dat(NULL), len(0), cap(0)
{
    if (epos > another.len)
    {
        epos = another.len;
    }
    if (another.dat && bpos < epos)
    {
        assign(another.dat + bpos, epos - bpos);
    }
}

string_t::~string_t()
{
    release();
}

string_t& string_t::assign(const char* p, size_t l)
{
    len = 0;
    if (p && l)
    {
        if (l > cap)
        {
            mem::deallocate(dat);
            dat = (char*)mem::allocate((cap = l) + 1, sizeof(char));
        }
        memmove(dat, p, len = l);
    }
    if (dat)
    {
        dat[len] = '\0';
    }
    return *this;
}

string_t& string_t::assign(char c)
{
    if (!dat)
    {
        cap = sizeof(word_t);
        dat = (char*)mem::allocate(cap + 1, sizeof(char));
    }
    dat[0] = c;
    dat[len = 1] = '\0';
    return *this;
}

string_t& string_t::assign(const string_t& another, size_t bpos, size_t epos)
{
    if (epos > another.len)
    {
        epos = another.len;
    }
    if (bpos < epos && another.dat)
    {
        return assign(another.dat + bpos, epos - bpos);
    }
    else if (dat)
    {
        dat[len = 0] = '\0';
    }
    return *this;
}

size_t string_t::pos_not_chars(size_t pos, const char* chars) const
{
    if (dat && chars && pos < len)
    {
        char* p = dat + pos;
        for (; *p != '\0'; p++)
        {
            if (!strchr(chars, *p))
            {
                return p - dat;
            }
        }
    }
    return npos;
}

size_t string_t::rpos_not_chars(size_t pos, const char* chars) const
{
    if (dat && chars && pos < len)
    {
        for (const char* p = dat + pos; p != dat - 1; p--)
        {
            if (!strchr(chars, *p))
            {
                return p - dat;
            }
        }
    }
    return npos;
}

string_t& string_t::strip_left(const char* chars)
{
    return assign(*this, pos_not_chars(0, chars), len);
}

string_t& string_t::strip_right(const char* chars)
{
    if (len)
    {
        return remove_to_end(rpos_not_chars(len - 1, chars) + 1);
    }
    return *this;
}

string_t& string_t::strip(const char* chars)
{
    if (len)
    {
        size_t bpos = pos_not_chars(0, chars);
        size_t epos = rpos_not_chars(len - 1, chars);
        return assign(*this, bpos, epos + 1);
    }
    return *this;
}

size_t string_t::find(size_t pos, char c) const
{
    if (pos < len && dat)
    {
        const char* pc = strchr(dat + pos, c);

        if (pc)
        {
            return pc - dat;
        }
    }
    return npos;
}

size_t string_t::find(size_t pos, const char* p) const
{
    if (pos < len && dat && p)
    {
        const char* pp = strstr(dat + pos, p);

        if (pp)
        {
            return pp - dat;
        }
    }
    return npos;
}

bool string_t::starts_with(size_t pos, const char* p, size_t l, bool ic) const
{
    const char* e = p + l;
    const char* q = dat + pos;

    if (dat && p && pos < len && l)
    {
        if (len >= l)
        {
            if (ic)
            {
                while (p != e && tolower(*p) == tolower(*q))
                {
                     p++; q++;
                }
            }
            else
            {
                while (p != e && *p == *q)
                {
                    p++; q++;
                }
            }
        }
    }
    return p == e && l != 0;
}

bool string_t::ends_with(size_t pos, const char* p, size_t l, bool ic) const
{
    const char* q;
    if (dat && p && pos < len && l && len >= l)
    {
        p = p + l - 1;
        q = dat + pos;
        if (ic)
        {
            while (l && tolower(*p) == tolower(*q))
            {
                p--, q--, l--;
            }
        }
        else
        {
            while (l && *p == *q)
            {
                p--, q--, l--;
            }
        }
        return l == 0;
    }
    return false;
}

string_t& string_t::reverse(size_t bpos, size_t epos)
{
    if (epos > len)
    {
        epos = len;
    }
    char* p = dat + bpos;
    char* e = dat + epos - 1, tmp;
    if (dat) while (e > p)
    {
        tmp = *p;
        *p++ = *e;
        *e-- = tmp;
    }
    return *this;
}

void string_t::take(char* p, size_t l)
{
    release();
    dat = p;
    len = cap = l;
}

void string_t::take(char* p, size_t l, size_t c)
{
    release();
    dat = p;
    len = l;
    cap = c;
}

void string_t::release()
{
    mem::deallocate(dat);
    dat = NULL;
    cap = len = 0;
}

void string_t::reserve(size_t newcap)
{
    if (newcap > cap)
    {
        char* newdat = (char*)mem::allocate(newcap + 1, sizeof(char));
        memcpy(newdat, dat, len);
        mem::deallocate(dat);
        dat = newdat;
        cap = newcap;
        dat[len] = '\0';
    }
}

string_t& string_t::append(char c, size_t n)
{
    if (len + n > cap)
    {
        reserve(len + n);
    }
    for (size_t i = 0; i < n; i++)
    {
        *(dat + len + i) = c;
    }
    dat[len += n] = '\0';
    return *this;
}

bool string_t::overlap(const char* p, size_t l)
{
    return (p >= dat && p <= dat + len) || (p + l >= dat && p + l <= dat + len);
}

string_t& string_t::append(const char* p, size_t l)
{
    if (p)
    {
        string_t tmp;
        if (len + l > cap)
        {
            if (overlap(p, l))
            {
                tmp.assign(p, l);
                p = tmp.dat;
            }
            reserve(len + l);
        }
        if (dat)
        {
            memcpy(dat + len, p, l);
            dat[len += l] = '\0';
        }
    }
    return *this;
}

string_t& string_t::append(const string_t& another, size_t bpos, size_t epos)
{
    if (another.dat && bpos < epos)
    {
        if (epos > another.len)
        {
            epos = another.len;
        }
        return append(another.dat + bpos, epos - bpos);
    }
    return *this;
}

string_t& string_t::insert(size_t pos, char c, size_t n)
{
    if (len + n > cap)
    {
        reserve(len + n);
    }
    pos = pos > len? len: pos;
    memmove(dat + pos + n, dat + pos, len - pos);
    for (char* p = dat + pos, *e = p + n; p != e; p++)
    {
        *p = c;
    }
    dat[len += n] = '\0';
    return *this;
}

string_t& string_t::insert(size_t pos, const char* p, size_t l)
{
    if (p)
    {
        string_t tmp;
        if (overlap(p, l))
        {
            tmp.assign(p, l);
            p = tmp.dat;
        }
        if (len + l > cap)
        {
            reserve(len + l);
        }
        if (dat)
        {
            pos = pos > len? len: pos;
            memmove(dat + pos + l, dat + pos, len - pos);
            memmove(dat + pos, p, l);
            dat[len += l] = '\0';
        }
    }
    return *this;
}

string_t& string_t::insert(size_t pos, const string_t& another, size_t bpos, size_t epos)
{
    if (bpos < epos)
    {
        if (epos > another.len)
        {
            epos = another.len;
        }
        return insert(pos, another.dat + bpos, epos - bpos);
    }
    return *this;
}

string_t& string_t::remove(size_t pos)
{
    if (pos < len)
    {
        memmove(dat + pos, dat + pos + 1, len-- - pos);
    }
    return *this;
}

string_t& string_t::remove(size_t bpos, size_t epos)
{
    if (dat)
    {
        if (epos > len)
        {
            epos = len;
        }
        if (bpos < epos)
        {
            memmove(dat + bpos, dat + epos, len - epos + 1);
            len -= epos - bpos;
        }
    }
    return *this;
}

string_t& string_t::remove_to_begin(size_t pos)
{
    if (dat)
    {
        if (pos >= len)
        {
            pos = len - 1;
        }
        memmove(dat, dat + pos + 1, len = len - pos - 1);
        dat[len] = '\0';
    }
    return *this;
}

string_t& string_t::remove_to_end(size_t pos)
{
    if (dat && pos < len)
    {
        dat[len = pos] = '\0';
    }
    return *this;
}

void string_t::clear()
{
    len = 0;
    if (dat)
    {
        dat[len] = '\0';
    }
}

__force_inline(void) __to_lower(char* p, char* e)
{
    for (; p != e; p++)
    {
        *p = tolower(*p);
    }
}

__force_inline(void) __to_upper(char* p, char* e)
{
    for (; p != e; p++)
    {
        *p = toupper(*p);
    }
}

string_t& string_t::to_lower()
{
    __to_lower(dat, dat + len);
    return *this;
}

string_t& string_t::to_lower(string_t& res) const
{
    res.assign(*this);
    __to_lower(res.dat, res.dat + res.len);
    return res;
}

string_t& string_t::to_upper()
{
    __to_upper(dat, dat + len);
    return *this;
}

string_t& string_t::to_upper(string_t& res) const
{
    res.assign(*this);
    __to_upper(res.dat, res.dat + res.len);
    return res;
}

_leadings_t format_t::leadings;

_leadings_t::_leadings_t()
{
    size_t size = __max_base() + 1;
    strs = (string_t*)mem::allocate(size, sizeof(string_t));
    refs = (_leadref_t*)mem::allocate(size, sizeof(_leadref_t));
    memset(strs, 0, size * sizeof(string_t));
    memset(refs, 0, size * sizeof(_leadref_t));
    strs[2].assign("0b");
    strs[8].assign("0");
    strs[16].assign("0x");
    refs[0].pstr = &strs[2];
    refs[0].base = 2;
    refs[1].pstr = &strs[8];
    refs[1].base = 8;
    refs[2].pstr = &strs[16];
    refs[2].base = 16;
}

_leadings_t::~_leadings_t()
{
    for (int i = 0; i <= __max_base(); i++)
    {
        strs[i].release();
    }
    mem::deallocate(strs);
    mem::deallocate(refs);
}

const string_t& _leadings_t::get(int base) const
{
    return __is_valid(base)? strs[base]: NO_LEADING;
}

void _leadings_t::set(int base, const char* leading)
{
    if (__is_valid(base))
    {
        int l, i;
        string_t& str = strs[base];
        if ((l = leading? (int)strlen(leading): 0))
        {
            str.assign(leading, l);
            for (i = 0; i <= __max_base(); i++)
            {
                if (refs[i].base == base || refs[i].base == 0)
                {
                    refs[i].pstr = &str;
                    refs[i].base = base;
                    break;
                }
            }
        }
        else
        {
            str.release();
            for (i = 0; i <= __max_base(); i++)
            {
                if (!refs[i].base && refs[i].base == base)
                {
                    break;
                }
            }
            for (; i <= __max_base() && refs[i].base; i++)
            {
                refs[i] = refs[i + 1];
            }
        }
    }
}

void reset_leading()
{
    for (int i = 2; i <= __max_base(); i++)
    {
        set_leading(i, NULL);
    }
    set_leading(2, "0b");
    set_leading(8, "0");
    set_leading(16, "0x");
}

void format_t::set(format_flags_t ff)
{
    if (ff != NO_FLAGS)
    {
        _flags |= ff;
    }
    else
    {
        _flags = NO_FLAGS;
    }
}

void format_t::set_line_group_count(size_t cnt)
{
    _groupinline = cnt;
    if (_linesep.empty())
    {
        _linesep.assign('\n');
    }
}

void format_t::set_line_separator(const char* p)
{
    _linesep.assign(p);
}

const string_t* format_t::__append_group(string_t& str, const char* p, size_t l, size_t n) const
{
    const string_t* sep = _groupinline && !(n % _groupinline)? &_linesep: &_groupsep;
    str.append(p, l).append(*sep);
    return sep;
}

string_t& format_t::dump(const number_t& a, int b, string_t& str) const
{
    const char *p, *e;
    string_t tmp, sign(16), leading(16);
    size_t l, r, space, g;

    a.to_string(tmp, b);
    p = tmp.dat;
    e = p + tmp.len;
    l = tmp.len;
    if (*p == '-')
    {
        p++;
        l--;
        sign.assign('-');
    }
    else if (a.is_zero() && has(ZERO_NEG))
    {
        sign.assign('-');
    }
    else if ((has(SHOW_POS) && a.is_pos()) || (has(ZERO_POS) && a.is_zero()))
    {
        sign.assign('+');
    }
    if (has(UPPER_CASE))
    {
        tmp.to_upper();
    }
    str.clear();
    space = tmp.len + 16;
    if (_group)
    {
        g = l / _group + 1;
        if (_groupinline)
        {
            space += (g - g / _groupinline) * _groupsep.len + g / _groupinline * _linesep.len;
        }
        else
        {
            space += g * _groupsep.len;
        }
    }
    str.reserve(space);
    if (has(SHOW_LEADING) && !(a.is_zero() && has(ZERO_NO_LEADING)))
    {
        leading.assign(get_leading(b));
        if (has(UPPER_LEADING))
        {
            leading.to_upper();
        }
        if (has(SIGN_RIGHT_LEADING))
        {
            str.append(leading);
            str.append(sign);
        }
        else
        {
            str.append(sign);
            str.append(leading);
        }
    }
    else
    {
        str.append(sign);
    }
    if (_group)
    {
        r = l % _group; g = 0;
        const string_t* sep = NULL;
        if (has(GROUP_FROM_MSB))
        {
            e -= r;
            while (p != e)
            {
                sep = __append_group(str, p, _group, ++g);
                p += _group;
            }
            if (r)
            {
                sep = __append_group(str, p, r, ++g);
            }
        }
        else
        {
            if (r)
            {
                if (has(GROUP_COMPLETE))
                {
                    str.append(_filler, _group - r);
                }
                sep = __append_group(str, p, r, ++g);
                p += r;
            }
            while (p != e)
            {
                sep = __append_group(str, p, _group, ++g);
                p += _group;
            }
        }
        if (sep)
        {
            str.remove_to_end(str.len - sep->len);
        }
    }
    else
    {
        str.append(p, l);
    }
    return str;
}

size_t __sign_count(const char* b, const char* e, size_t* nn, size_t* np)
{
    const char* p = b;
    for (; p != e && (*p == '-' || *p == '+'); p++)
    {
        if (*p == '-')
        {
            (*nn)++;
        }
        else
        {
            (*np)++;
        }
    }
    return p - b;
}

int load(number_t& a, const char* str, size_t len, int base, const format_t* fmt)
{
    bool emptyerror = fmt && fmt->has(EMPTY_AS_ERROR);
    bool mulsignerror = fmt && fmt->has(MULTISIGN_AS_ERROR);

    if (!(str && len))
    {
        if (emptyerror)
        {
            return 0;
        }
        a.set_zero();
        return 1;
    }
    if (base > __max_base())
    {
        return 0;
    }

    string_t tmp(len);
    size_t i = 0, nn = 0, np = 0, matchmax = 0;
    char* q = tmp.dat;

    for (const char* p = str, *e = str + len; p != e; p++)
    {
        if (*p != ' ' && (*p < '\t' || *p > '\r') && !(fmt && fmt->group_separator().contains(*p)))
        {
            *q++ = *p;
        }
    }
    tmp.len = q - tmp.dat;
    tmp.dat[tmp.len] = '\0';

    i = __sign_count(tmp.dat, tmp.dat + tmp.len, &nn, &np);
    if (base <= 1)
    {
        _leadref_t* ref = format_t::leadings.refs;
        for (; ref->pstr != NULL; ref++)
        {
            const string_t& leading = *ref->pstr;
            if (tmp.starts_with(i, leading, true) && matchmax < leading.len)
            {
                base = ref->base;
                matchmax = leading.len;
            }
        }
        i += matchmax;
        base = base <= 1? 10: base;
    }
    else
    {
        const string_t& leading = get_leading(base);
        if (tmp.starts_with(i, leading, true))
        {
            i += leading.len;
        }
    }
    i += __sign_count(tmp.dat + i, tmp.dat + tmp.len, &nn, &np);
    if (mulsignerror && (nn + np > 1))
    {
        return 0;
    }
    else if (i < tmp.len && check(tmp, i, tmp.len, base))
    {
        a.assign(tmp, i, tmp.len, base);
        if (nn & 1)
        {
            a.set_neg();
        }
        return 1;
    }
    else if (i == tmp.len)
    {
        if (emptyerror)
        {
            return 0;
        }
        a.set_zero();
        return 1;
    }
    return 0;
}

int __cmp(const char* a, const char* b)
{
    if (a && b)
    {
        return strcmp(a, b);
    }
    else if (a && *a)
    {
        return 1;
    }
    else if (b && *b)
    {
        return -1;
    }
    return 0;
}

int cmp(const string_t& a, const string_t& b)
{
    return __cmp(a.dat, b.dat);
}

int cmp(const string_t& a, const char* b)
{
    return __cmp(a.dat, b);
}

int cmp(const char* a, const string_t& b)
{
    return __cmp(a, b.dat);
}

int check(const char* p, int base)
{
    const char* q = p;
    if (*p == '-')
    {
        p++;
    }
    while (*p)
    {
        if (__char_digit_valid(*p, base))
        {
            p++;
        }
        else
        {
            return 0;
        }
    }
    return int(p - q);
}

int check(const char* p, size_t l, int base)
{
    const char* e = p + l;
    if (*p == '-')
    {
        p++;
    }
    while (p != e)
    {
        if (__char_digit_valid(*p, base))
        {
            p++;
        }
        else
        {
            return 0;
        }
    }
    return int(l);
}

int check(const string_t& str, size_t bpos, size_t epos, int base)
{
    if (epos > str.len)
    {
        epos = str.len;
    }
    if (bpos < epos)
    {
        return check(str.c_str() + bpos, epos - bpos, base);
    }
    return 0;
}

/** algorithms implementation */

int max_base()
{
    return __max_base();
}

int cmp(const number_t& a, const number_t& b)
{
    if (a.len - b.len)
    {
        return a.len - b.len > 0? 1: -1;
    }
    return __cmp_same_len_core(a.dat, b.dat, __abs(a.len)) * __sign(a.len);
}

int cmp_abs(const number_t& a, const number_t& b)
{
    slen_t la = __abs(a.len);
    slen_t lb = __abs(b.len);
    if (la - lb)
    {
        return la - lb > 0? 1: -1;
    }
    return __cmp_same_len_core(a.dat, b.dat, la);
}

bool neq(const number_t& a, const number_t& b)
{
    if (a.len - b.len)
    {
        return true;
    }
    return __neq_core(a.dat, b.dat, __abs(a.len));
}

void abs(const number_t& a, number_t& res)
{
    res.assign(a);
    res.len = __abs(res.len);
}

void neg(const number_t& a, number_t& res)
{
    res.assign(a);
    res.len = 0 - res.len;
}

void add(const number_t& a, const number_t& b, number_t& res)
{
    unit_t *tmp, *pa = a.dat, *pb = b.dat;
    slen_t lr, la, lb, sa, sb, newcap = 0;

    sa = __sign(a.len);
    la = __abs(a.len);
    sb = __sign(b.len);
    lb = __abs(b.len);
    if (la < lb || (la == lb && __cmp_same_len_core(pa, pb, la) == -1))
    {
        tmp = pa; pa = pb;  pb = tmp;
        la ^= lb; lb ^= la; la ^= lb;
        sa ^= sb; sb ^= sa; sa ^= sb;
    }
    tmp = res.dat;
    if (sa == sb)
    {
        if (res.cap < la + 1)
        {
            tmp = __allocate_units(la + 1, &newcap);
        }
        lr = __add_core(pa, la, pb, lb, tmp);
    }
    else
    {
        if (res.cap < la)
        {
            tmp = __allocate_units(la, &newcap);
        }
        lr = __sub_core(pa, la, pb, lb, tmp);
    }
    if (newcap)
    {
        __deallocate_units(res.dat);
        res.dat = tmp;
        res.cap = newcap;
    }
    res.len = lr * sa;
}

void sub(const number_t& a, const number_t& b, number_t& res)
{
    unit_t *tmp, *pa = a.dat, *pb = b.dat;
    slen_t lr, la, lb, sa, sb, sign = 1, newcap = 0;

    sa = __sign(a.len);
    la = __abs(a.len);
    sb = __sign(b.len);
    lb = __abs(b.len);
    if (la < lb || (la == lb && __cmp_same_len_core(pa, pb, la) == -1))
    {
        tmp = pa; pa = pb;  pb = tmp;
        la ^= lb; lb ^= la; la ^= lb;
        sa ^= sb; sb ^= sa; sa ^= sb;
        sign = -1;
    }
    tmp = res.dat;
    if (sa == sb)
    {
        if (res.cap < la)
        {
            tmp = __allocate_units(la, &newcap);
        }
        lr = __sub_core(pa, la, pb, lb, tmp);
        sign *= sa;
    }
    else
    {
        if (res.cap < la + 1)
        {
            tmp = __allocate_units(la + 1, &newcap);
        }
        lr = __add_core(pa, la, pb, lb, tmp);
        sign = __sign(a.len);
    }
    if (newcap)
    {
        __deallocate_units(res.dat);
        res.dat = tmp;
        res.cap = newcap;
    }
    res.len = lr * sign;
}

void mul(const number_t& a, const number_t& b, number_t& res)
{
    slen_t sa = __sign(a.len);
    slen_t sb = __sign(b.len);
    __mul(a.dat, __abs(a.len), b.dat, __abs(b.len), res);
    res.len *= __sign(sa, sb);
}

void kmul(const number_t& u, const number_t& v, number_t& res)
{
    const unit_t *a, *b, *c, *d, *x, *y;
    slen_t n, lx, ly, la, lb, lc, ld, newcap = 0;

    lx = __abs(u.len);
    ly = __abs(v.len);
    if (lx <= KMUL_THRESHOLD || ly <= KMUL_THRESHOLD)
    {
        mul(u, v, res);
        return;
    }

    x = u.dat;
    y = v.dat;
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

    unit_t* tmp = res.dat;
    if (res.cap < lx + ly + 1)
    {
        tmp = __allocate_units(lx + ly + 1, &newcap);
    }
    else if (res.dat == x || res.dat == y)
    {
        tmp = __allocate_units(res.cap, &newcap);
    }

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

    if (newcap)
    {
        __deallocate_units(res.dat);
        res.dat = tmp;
        res.cap = newcap;
    }
    lx += ly + 1;
    __trim_leading_zeros(res.dat, lx);
    res.len = lx * __sign(u.len, v.len);
}

void sqr(const number_t& a, number_t& res)
{
    __sqr(a.dat, __abs(a.len), res);
}

void ksqr(const number_t& u, number_t& res)
{
    const unit_t *a, *b, *x = u.dat;
    slen_t n, la, lb, lx = __abs(u.len), newcap = 0;

    if (lx <= KSQR_THRESHOLD)
    {
        sqr(u, res);
        return;
    }

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

    unit_t* tmp = res.dat;
    if (res.cap < 2 * lx)
    {
        tmp = __allocate_units(2 * lx, &newcap);
    }
    else if (res.dat == x)
    {
        tmp = __allocate_units(res.cap, &newcap);
    }
    __set_units_zero(tmp, 2 * lx);
    __copy_units(tmp, bb.dat, bb.len);
    __copy_units(tmp + 2 * n, aa.dat, aa.len);
    __add_core(tmp + n, 2 * lx - n, ab.dat, ab.len, tmp + n);
    __add_core(tmp + n, 2 * lx - n, ab.dat, ab.len, tmp + n);

    if (newcap)
    {
        __deallocate_units(res.dat);
        res.dat = tmp;
        res.cap = newcap;
    }
    res.len = 2 * lx;
    __trim_leading_zeros(res.dat, res.len);
}

void pow(const number_t& a, size_t b, number_t& res)
{
    number_t A(a);
    res.set_one();
    while (b != 0)
    {
        if (b & 1)
        {
            res.kmul(A);
        }
        A.ksqr();
        b >>= 1;
    }
}

int div(const number_t& a, const number_t& b, number_t& q, number_t& r)
{
    if (!b.is_zero())
    {
        slen_t la = __abs(a.len), lb = __abs(b.len);
        slen_t sa = __sign(a.len), sb = __sign(b.len);

        if (la >= lb)
        {
            __div(a.dat, la, b.dat, lb, q, r);
            r.len *= __sign(sa);
            q.len *= __sign(sa, sb);
        }
        else
        {
            r.assign(a);
            q.set_zero();  // r must be assigned before setting q to 0
        }
        return 1;
    }
    return 0;
}

int div(const number_t& a, const number_t& b, number_t& q)
{
    if (!b.is_zero())
    {
        slen_t la = __abs(a.len), lb = __abs(b.len);
        slen_t sa = __sign(a.len), sb = __sign(b.len);

        if (la >= lb)
        {
            __div(a.dat, la, b.dat, lb, q);
            q.len *= __sign(sa, sb);
        }
        else
        {
            q.set_zero();
        }
        return 1;
    }
    return 0;
}

int mod(const number_t& a, const number_t& b, number_t& r)
{
    if (!b.is_zero())
    {
        slen_t la = __abs(a.len);
        slen_t sa = __sign(a.len);
        slen_t lb = __abs(b.len);

        if (la >= lb)
        {
            if (lb > 1)
            {
                __mod(a.dat, la, b.dat, lb, r);
            }
            else
            {
                r.assign(a.absrem_unit(b.dat[0]));
            }
            r.len *= sa;
        }
        else
        {
            r.assign(a);
        }
        return 1;
    }
    return 0;
}

void shr(const number_t& a, size_t b, number_t& res)
{
    slen_t n = b / UNITBITS, m = b % UNITBITS;
    slen_t l = __abs(a.len) - n, newcap = 0;

    if (l > 0)
    {
        unit_t* tmp = res.dat;
        if (res.cap < l)
        {
            tmp = __allocate_units(l, &newcap);
        }
        __move_units(tmp, a.dat + n, l);
        __shr_core(tmp, l, m);
        if (newcap)
        {
            __deallocate_units(res.dat);
            res.dat = tmp;
            res.cap = newcap;
        }
        __trim_leading_zeros(res.dat, l);
        res.len = l * __sign(a.len);
    }
    else
    {
        res.set_zero();
    }
}

void shl(const number_t& a, size_t b, number_t& res)
{
    slen_t n = b / UNITBITS, m = b % UNITBITS;
    slen_t la = __abs(a.len), l = la + n, newcap = 0;
    unit_t* tmp = res.dat;

    if (res.cap < l + 1)
    {
        tmp = __allocate_units(l + 1, &newcap);
    }
    __move_units(tmp + n, a.dat, la);
    if (m && __shl_core(tmp + n, la, m) > la)
    {
        l++;
    }
    __set_units_zero(tmp, n);
    if (newcap)
    {
        __deallocate_units(res.dat);
        res.dat = tmp;
        res.cap = newcap;
    }
    __trim_leading_zeros(res.dat, l);
    res.len = l * __sign(a.len);
}

void bit_and(const number_t& a, const number_t& b, number_t& res)
{
    unit_t* tmp = res.dat;
    slen_t la = __abs(a.len), lb = __abs(b.len), lr = la, newcap = 0;

    if (la > lb)
    {
        lr = lb;
    }
    if (res.cap < lr)
    {
        tmp = __allocate_units(lr, &newcap);
    }
    lr = __bit_and_core(a.dat, b.dat, lr, tmp);
    if (newcap)
    {
        __deallocate_units(res.dat);
        res.dat = tmp;
        res.cap = newcap;
    }
    res.len = lr * __sign(a.len, b.len);
}

void bit_or(const number_t& a, const number_t& b, number_t& res)
{
    unit_t* tmp = res.dat;
    slen_t la = __abs(a.len), lb = __abs(b.len), lr, newcap = 0;

    if (la > lb)
    {
        if (res.cap < la)
        {
            tmp = __allocate_units(la, &newcap);
        }
        lr = __bit_or_core(a.dat, la, b.dat, lb, tmp);
    }
    else
    {
        if (res.cap < lb)
        {
            tmp = __allocate_units(lb, &newcap);
        }
        lr = __bit_or_core(b.dat, lb, a.dat, la, tmp);
    }
    if (newcap)
    {
        __deallocate_units(res.dat);
        res.dat = tmp;
        res.cap = newcap;
    }
    res.len = lr * __sign(a.len, b.len);
}

void bit_xor(const number_t& a, const number_t& b, number_t& res)
{
    unit_t* tmp = res.dat;
    slen_t la = __abs(a.len), lb = __abs(b.len), lr, newcap = 0;

    if (la > lb)
    {
        if (res.cap < la)
        {
            tmp = __allocate_units(la, &newcap);
        }
        lr = __bit_xor_core(a.dat, la, b.dat, lb, tmp);
    }
    else
    {
        if (res.cap < lb)
        {
            tmp = __allocate_units(lb, &newcap);
        }
        lr = __bit_xor_core(b.dat, lb, a.dat, la, tmp);
    }
    if (newcap)
    {
        __deallocate_units(res.dat);
        res.dat = tmp;
        res.cap = newcap;
    }
    res.len = lr * __sign(a.len, b.len);
}

void bit_not(const number_t& a, number_t& res)
{
    if (a.len)
    {
        unit_t* tmp = res.dat;
        slen_t la = __abs(a.len), lr, newcap = 0;

        if (res.cap < la)
        {
            tmp = __allocate_units(la, &newcap);
        }
        lr = __bit_not_core(a.dat, la, tmp);
        if (newcap)
        {
            __deallocate_units(res.dat);
            res.dat = tmp;
            res.cap = newcap;
        }
        res.len = lr * __sign(a.len);
    }
}

void swap(number_t& a, number_t& b)
{
    unit_t* tmp = a.dat;
    a.dat = b.dat;
    b.dat = tmp;
    a.len ^= b.len; a.cap ^= b.cap;
    b.len ^= a.len; b.cap ^= a.cap;
    a.len ^= b.len; a.cap ^= b.cap;
}

void __mul(const unit_t* x, slen_t lx, const unit_t* y, slen_t ly, number_t& res)
{
    assert (lx >= 0 && ly >= 0);

    if (lx && ly)
    {
        unit_t* tmp = res.dat;
        slen_t lr, newcap = 0, lxy = lx + ly;

        if (res.cap < lxy)
        {
            tmp = __allocate_units(lxy, &newcap);
        }
        else if (res.dat == x || res.dat == y)
        {
            tmp = __allocate_units(res.cap, &newcap);
        }
        __set_units_zero(tmp, lxy);
        lr = __mul_core(x, lx, y, ly, tmp);
        if (newcap)
        {
            __deallocate_units(res.dat);
            res.dat = tmp;
            res.cap = newcap;
        }
        res.len = lr;
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
    __deallocate_units(res.dat);
    res.len = lx + ly + 1;
    res.dat = tmp;
    __trim_leading_zeros(res.dat, res.len);
}

void __sqr(const unit_t* x, slen_t lx, number_t& res)
{
    if (lx)
    {
        unit_t* tmp = res.dat;
        slen_t lr, newcap = 0;

        if (res.cap < 2 * lx)
        {
            tmp = __allocate_units(2 * lx, &newcap);
        }
        else if (res.dat == x)
        {
            tmp = __allocate_units(res.cap, &newcap);
        }
        lr = __sqr_core(x, lx, tmp);
        if (newcap)
        {
            __deallocate_units(res.dat);
            res.dat = tmp;
            res.cap = newcap;
        }
        res.len = lr;
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
    __deallocate_units(res.dat);
    res.len = 2 * lx;
    res.dat = tmp;
    __trim_leading_zeros(res.dat, res.len);
}

void __div(const unit_t* a, slen_t la, const unit_t* b, slen_t lb, number_t& q, number_t& r)
{
    assert(la >= lb && lb > 0);

    slen_t qnewcap = 0, rnewcap = 0;
    unit_t *x = r.dat, *z = q.dat, ns;

    if (lb > 1)
    {
        ns = *(a + la - 1) >= *(b + lb - 1);
        if (r.cap < la + ns)
        {
            x = __allocate_units(la + ns, &rnewcap);
        }
        else if (r.dat == b)
        {
            x = __allocate_units(r.cap, &rnewcap);
        }
        if (x != a)
        {
            __copy_units(x, a, la);
        }
        if (ns)
        {
            x[la++] = 0;
        }

        if (q.cap < la - lb)
        {
            z = __allocate_units(la - lb, &qnewcap);
        }
        else if (q.dat == a || q.dat == b)
        {
            z = __allocate_units(q.cap, &qnewcap);
        }
        q.len = __div_core(x, la, b, lb, z);
        if (qnewcap)
        {
            __deallocate_units(q.dat);
            q.dat = z;
            q.cap = qnewcap;
        }
        if (rnewcap)
        {
            __deallocate_units(r.dat);
            r.dat = x;
            r.cap = rnewcap;
        }
        r.len = lb;
        __trim_leading_zeros(r.dat, r.len);
    }
    else
    {
        if (q.cap < la)
        {
            z = __allocate_units(la, &qnewcap);
        }
        else if (q.dat == a || q.dat == b)
        {
            z = __allocate_units(q.cap, &qnewcap);
        }
        r.assign(__div_unit_core(a, la, *b, z, &q.len));
        if (qnewcap)
        {
            __deallocate_units(q.dat);
            q.dat = z;
            q.cap = qnewcap;
        }
    }
}

void __div(const unit_t* a, slen_t la, const unit_t* b, slen_t lb, number_t& q)
{
    assert(la >= lb && lb > 0);

    slen_t newcap = 0;
    unit_t *x, *z = q.dat, ns;

    if (lb > 1)
    {
        ns = *(a + la - 1) >= *(b + lb - 1);
        x = __allocate_units(la + ns);
        __copy_units(x, a, la);
        if (ns)
        {
            x[la++] = 0;
        }
        if (q.cap < la - lb)
        {
            z = __allocate_units(la - lb, &newcap);
        }
        else if (q.dat == a || q.dat == b)
        {
            z = __allocate_units(q.cap, &newcap);
        }
        q.len = __div_core(x, la, b, lb, z);
        if (newcap)
        {
            __deallocate_units(q.dat);
            q.dat = z;
            q.cap = newcap;
        }
        __deallocate_units(x);
    }
    else
    {
        if (q.cap < la)
        {
            z = __allocate_units(la, &newcap);
        }
        else if (q.dat == a || q.dat == b)
        {
            z = __allocate_units(q.cap, &newcap);
        }
        __div_unit_core(a, la, *b, z, &q.len);
        if (newcap)
        {
            __deallocate_units(q.dat);
            q.dat = z;
            q.cap = newcap;
        }
    }
}

void __mod(const unit_t* a, slen_t la, const unit_t* b, slen_t lb, number_t& r) // may be inplace
{
    assert(la >= lb && lb > 1);

    slen_t newcap = 0;
    unit_t *x = r.dat;
    unit_t ns = *(a + la - 1) >= *(b + lb - 1);

    if (r.cap < la + ns)
    {
        x = __allocate_units(la + ns, &newcap);
    }
    else if (r.dat == b)
    {
        x = __allocate_units(r.cap, &newcap);
    }
    if (x != a)
    {
        __copy_units(x, a, la);
    }
    if (ns)
    {
        x[la++] = 0;
    }
    __mod_core(x, la, b, lb);
    if (newcap)
    {
        __deallocate_units(r.dat);
        r.dat = x;
        r.cap = newcap;
    }
    __trim_leading_zeros(r.dat, lb);
    r.len = lb;
}

bool __neq_core(const unit_t* x, const unit_t* y, slen_t l)
{
    if (l)
    {
        const dunit_t *p1, *p2, *pe;
        if (*x != *y || x[l - 1] != y[l - 1] || x[l >> 1] != y[l >> 1])
        {
            return true;
        }
        p1 = (dunit_t*)x;
        p2 = (dunit_t*)y;
        pe = (dunit_t*)(x + l - (l & 1));
        while (p1 != pe) if (*p1++ != *p2++)
        {
            return true;
        }
    }
    return false;
}

__force_inline(int) __cmp_same_len_core(const unit_t* x, const unit_t* y, slen_t l)
{
    if (l)
    {
        const dunit_t *p1, *p2, *pe;
        const unit_t *hx = x + l - 1, *hy = y + l - 1;

        if (*hx != *hy)
        {
            return *hx > *hy? 1: -1;
        }
        p1 = (dunit_t*)(hx - 1 - (l & 1));
        p2 = (dunit_t*)(hy - 1 - (l & 1));
        pe = (dunit_t*)(x - 2);
        for (; p1 != pe; p1--, p2--)
        {
            if (*p1 != *p2)
            {
                return *p1 > *p2? 1: -1;
            }
        }
    }
    return 0;
}

int __cmp_core(const unit_t* x, slen_t lx, const unit_t* y, slen_t ly)
{
    if (lx - ly)
    {
        return __sign(lx - ly);
    }
    return __cmp_same_len_core(x, y, lx);
}

slen_t __add_core(const unit_t* x, slen_t lx, const unit_t* y, slen_t ly, unit_t* res) // inplace
{
    assert(lx >= ly);

    unit_t *r = res;
    dunit_t carry = 0;
    const unit_t *e1 = x + ly;
    const unit_t *e2 = x + lx;

    while (x != e1)
    {
        carry += (dunit_t)*x++ + *y++;
        *r++ = carry & UNITMAX;
        carry >>= UNITBITS;
    }
    while (x != e2)
    {
        carry += *x++;
        *r++ = carry & UNITMAX;
        carry >>= UNITBITS;
    }
    if (carry)
    {
        lx++;
        *r = (unit_t)carry;
    }
    return lx;
}

slen_t __sub_core(const unit_t* x, slen_t lx, const unit_t* y, slen_t ly, unit_t* res)  // inplace
{
    assert(lx >= ly);

    unit_t *r = res;
    dunit_t borrow = 0;
    const unit_t *e1 = x + ly;
    const unit_t *e2 = x + lx;

    while (x != e1)
    {
        borrow = (dunit_t)*x++ - *y++ - borrow;
        *r++ = borrow & UNITMAX;
        borrow >>= DUNITBITS - 1;
    }
    while (x != e2)
    {
        borrow = (dunit_t)*x++ - borrow;
        *r++ = borrow & UNITMAX;
        borrow >>= DUNITBITS - 1;
    }
    __trim_leading_zeros(res, lx);
    return lx;
}

slen_t __mul_core(const unit_t* x, slen_t lx, const unit_t* y, slen_t ly, unit_t* res)  // not inplace
{
    assert(res != x && res != y);

    dunit_t carry = 0, xi;
    unit_t *pr, *rb = res;
    const unit_t *ex = x + lx;
    const unit_t *ey = y + ly, *py;

    for (; x != ex; x++, rb++)
    {
        for (xi = *x, py = y, pr = rb; py < ey; py++, pr++)
        {
            carry += xi * *py + *pr;
            *pr = carry & UNITMAX;
            carry >>= UNITBITS;
        }
        if (carry)
        {
            *pr = (unit_t)carry;
            carry = 0;
        }
    }
    lx += ly;
    __trim_leading_zeros(res, lx);
    return lx;
}

slen_t __sqr_core(const unit_t* x, slen_t lx, unit_t* res)  // not inplace
{
    assert(res != x);

    dunit_t xi, carry;
    unit_t *r, *rb = res;
    const unit_t *q, *p = x;
    const unit_t *eq = x + lx, *ep = eq - 1;

    __set_units_zero(res, lx <<= 1);
    while (p != ep)
    {
        r = rb;
        xi = *p;
        carry = xi * *p + *r;
        *r++ = carry & UNITMAX;
        carry >>= UNITBITS;
        q = ++p;
        rb += 2;
        xi <<= 1;
        if (xi >= BASE) for (xi &= UNITMAX; q != eq;)
        {
            carry += xi * *q + *r; // never overflow, xi <= UNITMAX - 1
            *r++ = carry & UNITMAX;
            carry = (carry >> UNITBITS) + *q++;
        }
        else while (q != eq)
        {
            carry += xi * *q++ + *r;
            *r++ = carry & UNITMAX;
            carry >>= UNITBITS;
        }
        if (carry)
        {
            *(dunit_t*)r += carry;
        }
    }
    *(dunit_t*)rb += (dunit_t)*p * *p;
    __trim_leading_zeros(res, lx);
    return lx;
}

unit_t __mul_unit_core(const unit_t* x, slen_t lx, unit_t y, unit_t* z)  // inplace
{
    dunit_t carry = 0;
    const unit_t* e = x + lx;

    while (x != e)
    {
        carry += dunit_t(*x++) * y;
        *z++ = carry & UNITMAX;
        carry >>= UNITBITS;
    }
    return carry & UNITMAX;
}

unit_t __div_unit_core(const unit_t* x, slen_t lx, unit_t y, unit_t* q, slen_t* lq)  // inplace
{
    assert(lx && y != 0);

    dunit_t r = 0;
    const unit_t* px = x + lx;
    unit_t* pq = q + lx;

    while (--px >= x)
    {
        r = __make_dunit(r, *px);
        *--pq = unit_t(r / y);
        r %= y;
    }
    *lq = lx;
    __trim_leading_zeros(q, *lq);
    return r & UNITMAX;
}

unit_t __mod_unit_core(const unit_t* x, slen_t lx, unit_t y)  // inplace
{
    assert(x && y != 0);

    dunit_t r = 0;
    const unit_t* px = x + lx;

    while (--px >= x)
    {
        r = __make_dunit(r, *px) % y;
    }
    return r & UNITMAX;
}

unit_t __div_unit_core(const unit_t* x, slen_t lx, const UDM& udm, unit_t* q, slen_t* lq)  // inplace
{
    dunit_t r = 0, h;
    if (udm.notpo2)
    {
        unit_t* qt = q + lx;
        const unit_t* xt = x + lx;

        if (udm.nooverflow)
        {
            while (--xt >= x)
            {
                r = __make_dunit(r, *xt);
                h = __mul_dunit_high(r, udm.multiplier) >> udm.shift;
                r -= h * udm.divisor;
                *--qt = unit_t(h);
            }
        }
        else
        {
            while (--xt >= x)
            {
                r = __make_dunit(r, *xt);
                h = __mul_dunit_high(r, udm.multiplier);
                h = (((r - h) >> 1) + h) >> udm.shift;
                r -= h * udm.divisor;
                *--qt = unit_t(h);
            }
        }
    }
    else
    {
        if (q != x)
        {
            __copy_units(q, x, lx);
        }
        r = __shr_core(q, lx, udm.shift);
    }
    __trim_leading_zeros(q, lx);
    *lq = lx;
    return r & UNITMAX;
}

unit_t __mod_unit_core(const unit_t* x, slen_t lx, const UDM& udm)  // inplace
{
    dunit_t r = 0, h;
    if (udm.notpo2)
    {
        const unit_t* xt = x + lx;
        if (udm.nooverflow) while (--xt >= x)
        {
            r = __make_dunit(r, *xt);
            h = __mul_dunit_high(r, udm.multiplier) >> udm.shift;
            r -= h * udm.divisor;
        }
        else while (--xt >= x)
        {
            r = __make_dunit(r, *xt);
            h = __mul_dunit_high(r, udm.multiplier);
            h = (((r - h) >> 1) + h) >> udm.shift;
            r -= h * udm.divisor;
        }
    }
    else if (lx)
    {
        r = *x & (((unit_t)1 << udm.shift) - 1);
    }
    return r & UNITMAX;
}

unit_t __guess_quotient(unit_t x0, unit_t x1, unit_t x2, unit_t y0, unit_t y1)
{
    dunit_t t, r, u, v;
    dunit_t x0x1 = __make_dunit(x0, x1);
    dunit_t y0y1 = __make_dunit(y0, y1);

    t = x0x1 / y0;
    r = x0x1 % y0;
    if (t >= BASE)
    {
        t = UNITMAX;
        r = x0x1 - t * y0;
    }
    if (r < BASE && (u = t * y1) > (v = __make_dunit(r, x2)))
    {
        u -= v;
        t -= u / y0y1 + (u % y0y1 != 0);
    }
    return t & UNITMAX;
}

unit_t __truing_quotient(unit_t* x, const unit_t* y, slen_t ly, unit_t trial)  // inplace
{
    unit_t *x0 = x;
    const unit_t *y0 = y, *ey = y + ly;
    dunit_t borrow = 0, carry = 0;

    for (; y != ey; x++, y++)
    {
        carry += (dunit_t)trial * *y;
        borrow = *x - (carry & UNITMAX) - borrow;
        *x = borrow & UNITMAX;
        carry >>= UNITBITS;
        borrow >>= DUNITBITS - 1;
    }
    borrow = (dunit_t)*x - carry - borrow;
    *x = borrow & UNITMAX;
    if ((borrow >>= UNITBITS))
    {
        for (y = y0, x= x0, carry = 0; y != ey; x++, y++)
        {
            carry += (dunit_t)*x + *y;
            *x = carry & UNITMAX;
            carry >>= UNITBITS;
        }
        --trial;
    }
    return trial;
}

slen_t __div_core(unit_t* x, slen_t lx, const unit_t* y, slen_t ly, unit_t* q)  // not inplace
{
    assert(ly >= 2 && lx >= ly);
    assert(*(x + lx - 1) < *(y + ly - 1));

    slen_t k = lx - ly;
    unit_t y0 = *(y + ly - 1);
    unit_t y1 = *(y + ly - 2);
    unit_t *xk = x + k - 1;
    unit_t *qk = q + k - 1, t;

    for (; xk >= x; xk--)
    {
        t = __guess_quotient(xk[ly], xk[ly - 1], xk[ly - 2], y0, y1);
        *qk-- = __truing_quotient(xk, y, ly, t);
    }
    __trim_leading_zeros(q, k);
    return k;
}

void __mod_core(unit_t* x, slen_t lx, const unit_t* y, slen_t ly)   // inplace
{
    assert(ly >= 2 && lx >= ly);
    assert(*(x + lx - 1) < *(y + ly - 1));

    unit_t y0 = *(y + ly - 1);
    unit_t y1 = *(y + ly - 2);
    unit_t *xk = x + lx - ly - 1, t;

    for (; xk >= x; xk--)
    {
        t = __guess_quotient(xk[ly], xk[ly - 1], xk[ly - 2], y0, y1);
        __truing_quotient(xk, y, ly, t);
    }
}

slen_t __shl_core(unit_t* x, slen_t lx, slen_t d)  // inplace
{
    assert(d < UNITBITS);

    dunit_t carry = 0;
    unit_t* e = x + lx;

    for (; x != e; x++)
    {
        carry = dunit_t(*x) << d | carry;
        *x = carry & UNITMAX;
        carry >>= UNITBITS;
    }
    if (carry)
    {
        lx++;
        *x = carry & UNITMAX;
    }
    return lx;
}

unit_t __shr_core(unit_t* x, slen_t lx, slen_t d)  // inplace
{
    assert(d < UNITBITS);

    dunit_t over = 0;
    unit_t *e = x - 1;
    unit_t mask = ((unit_t)1 << d) - 1;

    for (x += lx; --x != e;)
    {
        over = over << UNITBITS | *x;
        *x = (over >> d) & UNITMAX;
        over &= mask;
    }
    return (unit_t)over;
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
    const unit_t* e1 = x + ly, *e2 = x + lx;

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
    const unit_t* e1 = x + ly, *e2 = x + lx;

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
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
      0,   1,   2,   3,   4,   5,   6,   7,   8,   9, 255, 255,
    255, 255, 255, 255, 255,  10,  11,  12,  13,  14,  15,  16,
     17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,
     29,  30,  31,  32,  33,  34,  35,  44,  45,  46, 255, 255,
    255,  10,  11,  12,  13,  14,  15,  16,  17,  18,  19,  20,
     21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,  32,
     33,  34,  35, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255,
};

int __char_digit(char c)
{
    return __CHAR_DIGIT[(unsigned char)c];
}

bool __char_digit_valid(char c, int base)
{
    return __CHAR_DIGIT[(unsigned char)c] < base;
}

__force_inline(dunit_t) __original_div_3by2(dunit_t x0x1, unit_t x2, dunit_t y0y1, dunit_t* pr)
{
    dunit_t t, r, u, v, w;
    unit_t y0 = y0y1 >> UNITBITS, y1 = y0y1 & UNITMAX;

    t = x0x1 / y0;
    r = x0x1 % y0;
    if (t >= BASE)
    {
        t = UNITMAX;
        r = x0x1 - t * y0;
    }
    u = t * y1;
    v = __make_dunit(r, x2);
    if (r < BASE && u > v)
    {
        u -= v;
        w = u / y0y1 + (u % y0y1 != 0);
        t -= w;
        u = t * y1;
        v = __make_dunit(r + w * y0, x2);
    }
    *pr = v - u;
    return t;
}

__force_inline(dunit_t) __original_div_4by2(dunit_t h, dunit_t l, dunit_t d, dunit_t* r)
{
    assert(h < d && d != 0);

    dunit_t q0, q1, u, v;
    unit_t l0 = l >> UNITBITS, l1 = l & UNITMAX;

    if (d >= BASE)
    {
        q0 = __original_div_3by2(h, l0, d, r);
        q1 = __original_div_3by2(*r, l1, d, r);
    }
    else
    {
        u = __make_dunit(h, l0);
        q0 = u / d;
        *r = u % d;
        v = __make_dunit(*r, l1);
        q1 = v / d;
        *r = v % d;
    }
    return q0 << UNITBITS | q1;
}

#if defined(__GNUC__) && !defined(NO_INTRINSIC)

#if UNITBITS == 16
typedef unsigned long long __qunit_t;
#elif UNITBITS == 32
typedef __uint128_t __qunit_t;
#endif

slen_t __vbits_count(unit_t x)
{
    assert(x != 0);

    return 32 - __builtin_clz(x);
}

slen_t __tzbits_count(unit_t x)
{
    assert(x != 0);

    return __builtin_ctz(x);
}

dunit_t __mul_dunit_high(dunit_t x, dunit_t y)
{
    __qunit_t m = __qunit_t(x) * y;
    return m >> DUNITBITS;
}

dunit_t __mul_add_dunit(dunit_t x, dunit_t y, dunit_t z, dunit_t* l)
{
    __qunit_t m = __qunit_t(x) * y + z;
    *l = m & DUNITMAX;
    return m >> DUNITBITS;
}

dunit_t __qunit_by_dunit(dunit_t h, dunit_t l, dunit_t d, dunit_t* r)
{
    assert(h < d);

    __qunit_t qunit = __qunit_t(h) << DUNITBITS | l;
    dunit_t q = dunit_t(qunit / d);
    *r = dunit_t(qunit % d);
    return q;
}

dunit_t __qunit_mod_dunit(dunit_t h, dunit_t l, dunit_t d)
{
    assert(h < d);

    __qunit_t qunit = __qunit_t(h) << DUNITBITS | l;
    return dunit_t(qunit % d);
}

slen_t __pop_count(unit_t x)
{
    return __builtin_popcount(x);
}

#elif defined(_MSC_VER) && !defined(NO_INTRINSIC)

slen_t __vbits_count(unit_t x)
{
    assert(x != 0);

    unsigned long b;
    _BitScanReverse(&b, x);
    return b + 1;
}

slen_t __tzbits_count(unit_t x)
{
    assert(x != 0);

    unsigned long b;
    _BitScanForward(&b, x);
    return b;
}

#if UNITBITS == 16

dunit_t __mul_dunit_high(dunit_t x, dunit_t y)
{
    return __emulu(x, y) >> DUNITBITS;
}

dunit_t __mul_add_dunit(dunit_t x, dunit_t y, dunit_t z, dunit_t* l)
{
    unsigned __int64 r;
    r = __emulu(x, y) + z;
    *l = r & DUNITMAX;
    return r >> DUNITBITS;
}

#elif UNITBITS == 32

dunit_t __mul_dunit_high(dunit_t x, dunit_t y)
{
    unsigned __int64 h;
    _umul128(x, y, &h);
    return h;
}

dunit_t __mul_add_dunit(dunit_t x, dunit_t y, dunit_t z, dunit_t* l)
{
    unsigned __int64 h, t;
    t = *l = _umul128(x, y, &h);
    *l += z;
    return h + (*l < t);
}

#endif

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

static __force_inline(int) __bsr32(unsigned int bb)
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

    return __bsr32(x) + 1;
}

slen_t __tzbits_count(unit_t x)
{
    assert(x != 0);

    int result = 0;
    for (; !(x & 0xf); result += 4, x >>= 4);
    for (; !(x & 1); result++, x >>= 1);
    return result;
}

dunit_t __mul_dunit_high(dunit_t x, dunit_t y)
{
    dunit_t a, b, c, d;
    dunit_t m, n, o;

    a = x >> UNITBITS; b = x & UNITMAX;
    c = y >> UNITBITS; d = y & UNITMAX;
    m = d * a + (d * b >> UNITBITS);    // never overflow
    n = c * b;
    o = m + n;   // maybe overflow, if overflow o < m
    return a * c + (o >> UNITBITS) + (dunit_t(o < m) << UNITBITS);
}

dunit_t __mul_add_dunit(dunit_t x, dunit_t y, dunit_t z, dunit_t* l)
{
    dunit_t t, h;
    h = __mul_dunit_high(x, y);
    t = *l = x * y;
    *l += z;
    return h + (*l < t);
}

#endif

#if !defined(__GNUC__) || defined(NO_INTRINSIC)

dunit_t __qunit_by_dunit(dunit_t h, dunit_t l, dunit_t d, dunit_t* r)
{
    assert(h < d);

    return __original_div_4by2(h, l, d, r);
}

dunit_t __qunit_mod_dunit(dunit_t h, dunit_t l, dunit_t d)
{
    assert(h < d);

    dunit_t r;
    __original_div_4by2(h, l, d, &r);
    return r;
}

#if UNITBITS == 32

slen_t __pop_count(unit_t x)
{
    x = ((x >> 1) & 0x55555555) + (x & 0x55555555);
    x = ((x >> 2) & 0x33333333) + (x & 0x33333333);
    x = ((x >> 4) & 0x0f0f0f0f) + (x & 0x0f0f0f0f);
    x = ((x >> 8) & 0x00ff00ff) + (x & 0x00ff00ff);
    x = (x >> 16) + (x & 0x0000ffff);
    return x;
}

#elif UNITBITS == 16

slen_t __pop_count(unit_t x)
{
    x = ((x >> 1) & 0x5555) + (x & 0x5555);
    x = ((x >> 2) & 0x3333) + (x & 0x3333);
    x = ((x >> 4) & 0x0f0f) + (x & 0x0f0f);
    x = (x >> 8) + (x & 0x00ff);
    return x;
}

#endif

#endif

} // namespace end
