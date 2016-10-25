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
#else
#define __always_inline(x)  inline x
#endif

namespace mynum {

const float LN_8 = 2.0794f;   // log(8)
const float LN_10 = 2.3026f;  // log(10)
const unit_t  SHIFT = sizeof(unit_t) << 3;
const dunit_t BASE = (dunit_t)1 << SHIFT;
const unit_t  MASK = ~unit_t(0);
const dunit_t DMASK = ~dunit_t(0);
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
    return (unit_t*)mem::allocate(s + (s & 1), sizeof(unit_t));
}

static __always_inline(unit_t*) __allocate_units(slen_t units, slen_t* pcap)
{
    *pcap = units + (units & 1);
    return (unit_t*)mem::allocate(*pcap, sizeof(unit_t));
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

static __always_inline(unit_t*) __move_units(unit_t* d, const unit_t* s, slen_t l)
{
    return (unit_t*)memmove(d, s, l * sizeof(unit_t));
}

static __always_inline(int) __char_digit(char c);
static __always_inline(bool) __char_digit_valid(char c, int base);
static __always_inline(slen_t) __vbits_count(unit_t x);
static __always_inline(dunit_t) __mul_add_dunit(dunit_t x, dunit_t y, dunit_t z, dunit_t* low);
static __always_inline(dunit_t) __qunit_mod_by_dunit(dunit_t h, dunit_t l, dunit_t d);
static __always_inline(dunit_t) __qunit_div_by_dunit(dunit_t h, dunit_t l, dunit_t d, dunit_t* r);

#define __sign_shift(x) ((x) >> ((sizeof(slen_t) << 3) - 1))

static __always_inline(slen_t) __abs(slen_t x)
{
    slen_t y = __sign_shift(x);
    return (x + y) ^ y;
}

static __always_inline(int) __sign(slen_t x)
{
    return __sign_shift(x) | 1;
}

static __always_inline(int) __sign(slen_t x, slen_t y)
{
    return __sign_shift(x ^ y) | 1;
}

static __always_inline(bool) __same_sign(slen_t x, slen_t y)
{
    return (x ^ y) >= 0;
}

static __always_inline(dunit_t) __make_dunit(unit_t high, unit_t low)
{
    return (dunit_t)high << SHIFT | low;
}

static __always_inline(dunit_t) __make_dunit(dunit_t high, unit_t low)
{
    return high << SHIFT | low;
}

/** class number_t implementation */

#define __trim_leading_zeros(dat, len) do\
    {\
        assert(len >= 0); \
        const unit_t *e = dat - 1, *p = e + len; \
        while (p != e && !*p) {p--;} \
        len = slen_t(p - e); \
    } while (0)

#define __pad_word(dat, len) do\
    { \
        slen_t l = __abs(len); \
        if ((l) & 1) *((dat) + l) = 0; \
    } while (0)


number_t::number_t(const char* s)
{
    if (s)
    {
        slen_t l = (slen_t)strlen(s);
        if (l > 0)
        {
            __construct_from_xbase_string(s, l, 10, LN_10, INNERDEC_BASE, INNERDEC_BASE_DIGITS);
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
        }
    }
}

number_t::number_t(const char* s, size_t l, int base)
{
    if (s && slen_t(l) > 0 && base <= __max_base())
    {
        __construct_from_string(s, l, base);
    }
}

number_t::number_t(const number_t& another)
{
    __copy(another);
}

number_t::number_t(long x)
{
    slen_t sign = 1;
    if (x < 0)
    {
        sign = -1;
        x = -x;
    }
    len = sizeof(long) / sizeof(unit_t);
    __reserve(len);
    *(long*)dat = x;
    __trim_leading_zeros(dat, len);
    len *= sign;
}

number_t::number_t(long long x)
{
    slen_t sign = 1;
    if (x < 0)
    {
        sign = -1;
        x = -x;
    }
    len = sizeof(long long) / sizeof(unit_t);
    __reserve(len);
    *(long long*)dat = x;
    __trim_leading_zeros(dat, len);
    len *= sign;
}

number_t::number_t(int x)
{
    slen_t sign = 1;
    if (x < 0)
    {
        sign = -1;
        x = -x;
    }
    len = sizeof(int) / sizeof(unit_t);
    __reserve(len);
    *(int*)dat = x;
    __trim_leading_zeros(dat, len);
    len *= sign;
}

number_t::number_t(unsigned long x)
{
    len = sizeof(unsigned long) / sizeof(unit_t);
    __reserve(len);
    *(unsigned long*)dat = x;
    __trim_leading_zeros(dat, len);
}

number_t::number_t(unsigned long long x)
{
    len = sizeof(unsigned long long) / sizeof(unit_t);
    __reserve(len);
    *(unsigned long long*)dat = x;
    __trim_leading_zeros(dat, len);
}

number_t::number_t(unsigned int x)
{
    len = sizeof(unsigned int) / sizeof(unit_t);
    __reserve(len);
    *(unsigned int*)dat = x;
    __trim_leading_zeros(dat, len);
}

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

number_t& number_t::assign(int x)
{
    len = sizeof(int) / sizeof(unit_t);
    if (cap < len)
    {
        __deallocate_units(dat);
        __reserve(len);
    }
	int s = 1;
	if (x > 0)
	{
		*(int*)dat = x;
	}
	else
	{
		*(int*)dat = -x;
		s = -1;
	}
    __trim_leading_zeros(dat, len);
    len *= s;
    return *this;
}

number_t& number_t::assign(long x)
{
    len = sizeof(long) / sizeof(unit_t);
    if (cap < len)
    {
        __deallocate_units(dat);
        __reserve(len);
    }
	long s = 1;
	if (x > 0)
	{
		*(long*)dat = x;
	}
	else
	{
		*(long*)dat = -x;
		s = -1;
	}
    __trim_leading_zeros(dat, len);
    len *= s;
    return *this;
}

number_t& number_t::assign(long long x)
{
    len = sizeof(long long) / sizeof(unit_t);
    if (cap < len)
    {
        __deallocate_units(dat);
        __reserve(len);
    }

    int sign = 1;
    if (x < 0)
    {
        sign = -1;
        x = -x;
    }
    *(long long*)dat = x;
    __trim_leading_zeros(dat, len);
    len *= sign;
    return *this;
}

number_t& number_t::assign(unsigned int x)
{
    len = sizeof(unsigned int) / sizeof(unit_t);
    if (cap < len)
    {
        __deallocate_units(dat);
        __reserve(len);
    }
    *(unsigned int*)dat = x;
    __trim_leading_zeros(dat, len);
    return *this;
}

number_t& number_t::assign(unsigned long x)
{
    len = sizeof(unsigned long) / sizeof(unit_t);
    if (cap < len)
    {
        __deallocate_units(dat);
        __reserve(len);
    }
    *(unsigned long*)dat = x;
    __trim_leading_zeros(dat, len);
    return *this;
}

number_t& number_t::assign(unsigned long long x)
{
    len = sizeof(unsigned long long) / sizeof(unit_t);
    if (cap < len)
    {
        __deallocate_units(dat);
        __reserve(len);
    }
    *(unsigned long long*)dat = x;
    __trim_leading_zeros(dat, len);
    return *this;
}

number_t& number_t::assign(const char* s)
{
    len = 0;
    if (s)
    {
        slen_t l = (slen_t)strlen(s);
        if (l > 0)
        {
            __construct_from_xbase_string(s, l, 10, LN_10, INNERDEC_BASE, INNERDEC_BASE_DIGITS);
        }
    }
    return *this;
}

number_t& number_t::assign(const char* s, int base)
{
    len = 0;
    if (s && base <= __max_base())
    {
        slen_t l = (slen_t)strlen(s);
        if (l > 0)
        {
            __construct_from_string(s, l, base);
        }
    }
    return *this;
}

number_t& number_t::assign(const char* s, size_t l, int base)
{
    len = 0;
    if (s && slen_t(l) > 0 && base <= __max_base())
    {
        __construct_from_string(s, l, base);
    }
    return *this;
}

number_t::~number_t()
{
    __deallocate_units(dat);
    dat = NULL;
    len = 0;
    cap = 0;
}

void number_t::bits_reserve(size_t n)
{
    n = (n + (sizeof(unit_t) * 8) - 1) / (sizeof(unit_t) * 8);
    if ((slen_t)n > cap)
    {
        slen_t l, s, newcap;
        unit_t* tmp = __allocate_units(n, &newcap);
        l = __abs(len);
        s = __sign(len);
        __copy_units(tmp, dat, l);
        __deallocate_units(dat);
        dat = tmp;
        len = l * s;
        cap = newcap;
    }
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

number_t number_t::abs()
{
    return mynum::abs(*this);
}

number_t number_t::neg()
{
    return mynum::neg(*this);
}

number_t& number_t::set_abs()
{
    mynum::set_abs(*this);
    return *this;
}

number_t& number_t::set_neg()
{
    mynum::set_neg(*this);
    return *this;
}

number_t& number_t::set_sign(int sign)
{
    mynum::set_sign(*this, sign);
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
    number_t tmp;
    mynum::sqrt(*this, tmp);
    steal(tmp);
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

number_t& number_t::add_unit(unit_t x)
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
    return *this;
}

number_t& number_t::add_word(word_t x)
{
    __pad_word(dat, len);

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
            len = 1 + (x > MASK);
        }
    }
    else
    {
        if (*(word_t*)dat >= x || len < -2)
        {
            len = 0 - __abs_sub_word(x);
        }
        else
        {
            *(word_t*)dat = x - *(word_t*)dat;
            len = 1 + (dat[1] != 0);
        }
    }
    return *this;
}

number_t& number_t::sub_unit(unit_t x)
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
    return *this;
}

number_t& number_t::sub_word(word_t x)
{
    __pad_word(dat, len);

    if (len > 0)
    {
        if (*(word_t*)dat >= x || len > 2)
        {
            len = __abs_sub_word(x);
        }
        else
        {
            *(word_t*)dat = x - *(word_t*)dat;
            len = -2 + (dat[1] != 0);
        }
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
            len = -2 + (dat[1] != 0);
        }
    }
    else
    {
        len = 0 - __abs_add_word(x);
    }
    return *this;
}

number_t& number_t::mul_unit(unit_t x)
{
    if (len)
    {
        dunit_t carry = 0;
        slen_t l = __abs(len);
        unit_t* p = dat;
        unit_t* e = dat + l;

        while (p != e)
        {
            carry += (dunit_t)*p * x;
            *p++ = carry & MASK;
            carry >>= SHIFT;
        }
        if (carry)
        {
            if (cap > l)
            {
                *e = carry & MASK;
            }
            else
            {
                unit_t* tmp = __allocate_units((cap = l << 1));
                __copy_units(tmp, dat, l);
                tmp[l] = carry & MASK;
                __deallocate_units(dat);
                dat = tmp;
            }
            l++;
        }
        __trim_leading_zeros(dat, l);
        len = l * __sign(len);
    }
    return *this;
}

number_t& number_t::mul_word(word_t x)
{
    if (len)
    {
        __pad_word(dat, len);

        word_t *p, *e, carry = 0;
        slen_t l = __abs(len), m = (l + (l & 1)) / 2;

        p = (word_t*)dat;
        e = p + m;
        for (; p != e; p++)
        {
            // multiply *p and x, and add carry to the result
            // see issue #9
            carry = __mul_add_dunit(*p, x, carry, p);
        }
        if (!carry)
        {
            if (p == e && *((unit_t*)p - 1))
            {
                l = (unit_t*)p - dat;
            }
        }
        else
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
            l += 2;
        }
        __trim_leading_zeros(dat, l);
        len = l * __sign(len);
    }
    return *this;
}

number_t& number_t::div_unit(unit_t x)
{
    assert(x != 0);

    if (len)
    {
        dunit_t rem = 0;
        slen_t l = __abs(len);
        unit_t* q = dat + l;
        while (--q >= dat)
        {
            rem = __make_dunit(rem, *q);
            *q = unit_t(rem / x);
            rem %= x;
        }
        __trim_leading_zeros(dat, l);
        len = l * __sign(len);
    }
    return *this;
}

number_t& number_t::div_word(word_t x)
{
    assert(x != 0);

    if (len)
    {
        __pad_word(dat, len);

        word_t *q, *e, rem = 0;
        slen_t l = __abs(len), m = (l + (l & 1)) / 2;       

        e = (word_t*)dat;
        q = e + m;
        while (--q >= e)
        {
            *q = __qunit_div_by_dunit(rem, *q, x, &rem);
        }
        __trim_leading_zeros(dat, l);
        len = l * __sign(len);
    }
    return *this;
}

number_t& number_t::mod_unit(unit_t x)
{
    assert(x != 0);

    if (len)
    {
        dunit_t rem = 0;
        unit_t* q = dat + __abs(len);
        while (--q >= dat)
        {
            rem = __make_dunit(rem, *q) % x;
        }
        if (rem)
        {
            *dat = (unit_t)rem;
            len = __sign(len);
        }
        else
        {
            set_zero();
        }
    }
    return *this;
}

number_t& number_t::mod_word(word_t x)
{
    assert(x != 0);

    if (len)
    {
        __pad_word(dat, len);

        word_t *q, *e, rem = 0;
        slen_t l = __abs(len), m = (l + (l & 1)) / 2;       

        e = (word_t*)dat;
        q = e + m;
        while (--q >= e)
        {
            rem = __qunit_mod_by_dunit(rem, *q, x);
        }
        if (rem)
        {
            *(word_t*)dat = rem;
            len = 2;
        }
        else
        {
            set_zero();
        }
        __trim_leading_zeros(dat, l);
        len = l * __sign(len);
    }
    return *this;
}

bool number_t::bit_at(size_t x) const
{
    if (slen_t(x / SHIFT) < len)
    {
        return (dat[x / SHIFT] & ((unit_t)1 << x % SHIFT)) != 0;
    }
    return 0;
}

void number_t::bit_set_one(size_t x)
{
    slen_t u = slen_t(x / SHIFT);
    if (u < len)
    {
        dat[u] |= (unit_t)1 << x % SHIFT;
    }
    else
    {
        if (u >= cap)
        {
            slen_t newcap;
            unit_t* tmp = __allocate_units(u + 1, &newcap);
            __copy_units(tmp, dat, len);
            __deallocate_units(dat);
            dat = tmp;
            cap = newcap;
        }
        dat[u] = (unit_t)1 << x % SHIFT;
        __set_units_zero(dat + len, u - len);
        len = u + 1;
    }
}

void number_t::bit_set_zero(size_t x)
{
    slen_t u = slen_t(x / SHIFT);
    if (u < len)
    {
        dat[u] &= ~((unit_t)1 << x % SHIFT);
    }
}

void number_t::bit_set(size_t x, bool v)
{
    if (v)
    {
        bit_set_one(x);
    }
    else
    {
        bit_set_zero(x);
    }
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
        x.len = 0;
        x.cap = 0;
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
    return this->operator ++ (0);
}

number_t& number_t::operator -- ()
{
    return this->operator -- (0);
}

number_t& number_t::operator ++ (int)
{
    return this->add(unit_t(1));
}

number_t& number_t::operator -- (int)
{
    return this->sub(unit_t(1));
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

string_t number_t::operator () (int base) const
{
    return this->to_string(base);
}

bool number_t::operator [] (size_t x) const
{
    return this->bit_at(x);
}

bitref_t number_t::operator [] (size_t x)
{
    return bitref_t(*this, x);
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
        dat = NULL;
        len = another.len;
        cap = another.cap;
    }
}

void number_t::__reserve(slen_t units)
{
    dat = __allocate_units(units, &cap);
}

slen_t number_t::__vbits_count() const
{
    return mynum::__vbits_count(dat[len - 1]) + (len - 1) * SHIFT;
}

/**
 *  Only can be used in the process of constructing object
 */
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

/**
 *  Only can be used in the process of constructing object
 */
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

slen_t number_t::__abs_add_unit(unit_t x)
{
    assert(len != 0);

    slen_t l = __abs(len);
    dunit_t carry = 0;
    unit_t* p = dat;
    unit_t* e = dat + l;

    carry = (dunit_t)*p + x;
    *p = carry & MASK;
    carry >>= SHIFT;
    while (++p != e && carry)
    {
        carry += *p;
        *p = carry & MASK;
        carry >>= SHIFT;
    }
    if (carry)
    {
        if (cap > l)
        {
            *e = carry & MASK;
        }
        else
        {
            unit_t* tmp = __allocate_units((cap = l * 2));
            __copy_units(tmp, dat, l);
            tmp[l] = carry & MASK;
            __deallocate_units(dat);
            dat = tmp;
        }
        l++;
    }
    return l;
}

slen_t number_t::__abs_add_word(word_t x)
{
    assert(len != 0);

    word_t *p, *e, tmp;
    slen_t l = __abs(len), m = (l + (l & 1)) / 2;
 
    p = (dunit_t*)dat;
    e = p + m;
    *p += x;
    bool carry = *p < x;
    while (++p != e && carry)
    {
        tmp = *p;
        (*p)++;
        carry = *p < tmp;
    }
    if (!carry)
    {
        if (p == e && *((unit_t*)p - 1))
        {
            l = (unit_t*)p - dat;
        }
    }
    else
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
        l = 2 * m + 1;
    }
    return l;
}

slen_t number_t::__abs_sub_unit(unit_t x)
{
    assert(len != 0);

    dunit_t borrow = 0;
    unit_t* p = dat;
    unit_t* e = dat + __abs(len);

    borrow = (dunit_t)*p - x;
    *p = borrow & MASK;
    borrow >>= SHIFT;
    borrow &= 1;
    while (++p != e && borrow)
    {
        borrow = (dunit_t)*p - borrow;
        *p = borrow & MASK;
        borrow >>= SHIFT;
        borrow &= 1;
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

    word_t *p, *e;
    slen_t l = __abs(len), m = (l + (l & 1)) / 2;

    p = (word_t*)dat;
    e = p + m;
    bool borrow = *p < x;
    *p -= x;
    while (++p != e && borrow)
    {
        borrow = *p < 1;
        (*p)--;
    }
    __trim_leading_zeros(dat, l);
    return l;
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
    assert(len == 0);

    int sign = 1;
    if (*s == '-' || *s == '+')
    {
        sign = *s == '-'? -1: 1;
        s++;
        l--;
    }

    const char *p0 = s + l - UNITBITS;
    const char *p1 = s + l % UNITBITS;

    slen_t n = (l + UNITBITS - 1) / UNITBITS;
    if (n > cap)
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
    assert(len == 0);

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

    slen_t n = (l + k - 1) / k;
    if (n > cap)
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

void number_t::__construct_from_xbase_string(const char* s, slen_t l, int base, float ln_base, unit_t inner_base, unit_t inner_base_digits)
{
    assert(len == 0 && l >= 0 && base <= __max_base());

    unit_t u;
    int i = 0, sign = 1, d;

    slen_t n = slen_t(ln_base * l / LN_BASE + 1);
    if (n > cap)
    {
        __deallocate_units(dat);
        __reserve(n);
    }
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
    __trim_leading_zeros(dat, len);
    len *= sign;
}

void number_t::__construct_from_string(const char* s, slen_t l, int base)
{
    assert(len == 0 && l >= 0 && base <= __max_base());

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

number_t& set_abs(number_t& a)                          { a.len = __abs(a.len); return a; }
number_t& set_neg(number_t& a)                          { a.len = 0 - __abs(a.len); return a; }
number_t& set_sign(number_t& a, int sign)               { a.len = __abs(a.len) * __sign(sign); return a; }
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
    while (*p != '\0')
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

int check(const char* p, const char* e, int base)
{
    const char* q = p;
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
    return int(p - q);
}

/** algorithms implementation */

static void __sub(const number_t& a, const number_t& b, number_t& res);
static void __mul(const unit_t* x, slen_t lx, const unit_t* y, slen_t ly, number_t& res);
static void __kmul(const unit_t* x, slen_t lx, const unit_t* y, slen_t ly, number_t& res);
static void __sqr(const unit_t* x, slen_t lx, number_t& res);
static void __ksqr(const unit_t* x, slen_t lx, number_t& res);
static void __div(const unit_t* a, slen_t la, const unit_t* b, slen_t lb, number_t& q, number_t& r);
static void __div(const unit_t* a, slen_t la, const unit_t* b, slen_t lb, number_t& q);
static bool __neq_core(const unit_t* x, const unit_t* y, slen_t l);
static int __cmp_same_len_core(const unit_t* x, const unit_t* y, slen_t l);
static int __cmp_core(const unit_t* x, slen_t lx, const unit_t* y, slen_t ly);
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
static slen_t __shl_core(unit_t* x, slen_t lx, slen_t d);
static unit_t __shr_core(unit_t* x, slen_t lx, slen_t d);
static slen_t __bit_and_core(const unit_t* x, const unit_t* y, slen_t lx, unit_t* res);
static slen_t __bit_or_core(const unit_t* x, slen_t lx, const unit_t* y, slen_t ly, unit_t* res);
static slen_t __bit_xor_core(const unit_t* x, slen_t lx, const unit_t* y, slen_t ly, unit_t* res);
static slen_t __bit_not_core(const unit_t* x, slen_t lx, unit_t* res);

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
    slen_t lr, la, lb, sa, sb, newcap;
    unit_t *tmp, *pa = a.dat, *pb = b.dat;

    la = __abs(a.len);
    lb = __abs(b.len);
    sa = __sign(a.len);
    sb = __sign(b.len);
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
    if (tmp != res.dat)
    {
        __deallocate_units(res.dat);
        res.dat = tmp;
        res.cap = newcap;
    }
    res.len = lr * sa;
}

void sub(const number_t& a, const number_t& b, number_t& res)
{
    slen_t lr, la, lb, sa, sb, sign = 1, newcap;
    unit_t *tmp, *pa = a.dat, *pb = b.dat;

    la = __abs(a.len);
    lb = __abs(b.len);
    sa = __sign(a.len);
    sb = __sign(b.len);
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
    if (tmp != res.dat)
    {
        __deallocate_units(res.dat);
        res.dat = tmp;
        res.cap = newcap;
    }
    res.len = lr * sign;
}

void mul(const number_t& a, const number_t& b, number_t& res)
{
    __mul(a.dat, __abs(a.len), b.dat, __abs(b.len), res);
    res.len *= __sign(a.len, b.len);
}

void kmul(const number_t& u, const number_t& v, number_t& res)
{
    const unit_t *a, *b, *c, *d, *x, *y;
    slen_t n, lx, ly, la, lb, lc, ld, newcap;

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

    if (tmp != res.dat)
    {
        __deallocate_units(res.dat);
        res.dat = tmp;
        res.cap = newcap;
    }
    res.len = (lx + ly + 1);
    __trim_leading_zeros(res.dat, res.len);
    res.len *= __sign(u.len, v.len);
}

void sqr(const number_t& a, number_t& res)
{
    __sqr(a.dat, __abs(a.len), res);
}

void ksqr(const number_t& u, number_t& res)
{
    const unit_t *a, *b, *x = u.dat;
    slen_t n, la, lb, lx = __abs(u.len), newcap;

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

    if (tmp != res.dat)
    {
        __deallocate_units(res.dat);
        res.dat = tmp;
        res.cap = newcap;
    }
    res.len = 2 * lx;
    __trim_leading_zeros(res.dat, res.len);
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

int mod(const number_t& a, const number_t& b, number_t& r)
{
    if (!b.is_zero())
    {
        slen_t la = __abs(a.len);
        slen_t lb = __abs(b.len);
        if (la >= lb)
        {
            number_t dummy;
            __div(a.dat, la, b.dat, lb, dummy, r);
            r.len *= __sign(a.len);
        }
        else
        {
            r.assign(a);
        }
        return 1;
    }
    return 0;
}

int floor_div(const number_t& a, const number_t& b, number_t& q, number_t& r)    // XXX
{
    if (!b.is_zero())
    {
        slen_t la, lb, lr, sign, rnewcap;

        la = __abs(a.len);
        lb = __abs(b.len);
        if (la >= lb)
        {
            __div(a.dat, la, b.dat, lb, q, r);
        }
        else
        {
            q.set_zero();
            abs(a, r);
        }

        sign = __sign(a.len);
        if (__same_sign(a.len, b.len))
        {
            r.len *= sign;
        }
        else
        {
            if (!r.is_zero())
            {
                q.add_unit(1);
                unit_t* tmp = __allocate_units(lb, &rnewcap);
                lr = __sub_core(b.dat, lb, r.dat, r.len, tmp);
                __deallocate_units(r.dat);
                r.dat = tmp;
                r.len = lr;
                r.cap = rnewcap;
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
    slen_t n, m, l, newcap;

    n = b / SHIFT;
    m = b % SHIFT;
    l = __abs(a.len) - n;

    if (l > 0)
    {
        unit_t* tmp = res.dat;
        if (res.cap < l)
        {
            tmp = __allocate_units(l, &newcap);
        }
        __move_units(tmp, a.dat + n, l);
        __shr_core(tmp, l, m);
        if (tmp != res.dat)
        {
            __deallocate_units(res.dat);
            res.dat = tmp;
            res.cap = newcap;
        }
        res.len = l;
        __trim_leading_zeros(res.dat, res.len);
        res.len *= __sign(a.len);
    }
    else
    {
        res.set_zero();
    }
}

void shl(const number_t& a, size_t b, number_t& res)
{
    slen_t n, m, l, newcap;
    n = b / SHIFT;
    m = b % SHIFT;
    l = __abs(a.len) + n;

    unit_t* tmp = res.dat;
    if (res.cap < l + 1)
    {
        tmp = __allocate_units(l + 1, &newcap);
    }
    __move_units(tmp + n, a.dat, a.len);
    if (m && __shl_core(tmp + n, a.len, m) > a.len)
    {
        l++;
    }
    __set_units_zero(tmp, n);
    if (tmp != res.dat)
    {
        __deallocate_units(res.dat);
        res.dat = tmp;
        res.cap = newcap;
    }
    res.len = l;
    __trim_leading_zeros(res.dat, res.len);
    res.len *= __sign(a.len);
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
    static byte_t __sqrt_table[] =
    {
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
    if (a.is_not(res))
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
    else
    {
        int r;
        number_t tmp;
        if ((r = sqrt(a, tmp)))
        {
            res.steal(tmp);
        }
        return r;
    }
}

void bit_and(const number_t& a, const number_t& b, number_t& res)
{
    unit_t* tmp = res.dat;
    slen_t la = __abs(a.len), lb = __abs(b.len), lr = la, newcap;

    if (la > lb)
    {
        lr = lb;
    }
    if (res.cap < lr)
    {
        tmp = __allocate_units(lr, &newcap);
    }
    lr = __bit_and_core(a.dat, b.dat, lr, tmp);
    if (tmp != res.dat)
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
    slen_t la = __abs(a.len), lb = __abs(b.len), lr, newcap;
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
    if (tmp != res.dat)
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
    slen_t la = __abs(a.len), lb = __abs(b.len), lr, newcap;
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
    if (tmp != res.dat)
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
        slen_t la = __abs(a.len), lr, newcap;
        if (res.cap < la)
        {
            tmp = __allocate_units(la, &newcap);
        }
        lr = __bit_not_core(a.dat, la, tmp);
        if (tmp != res.dat)
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

void __mul(const unit_t* x, slen_t lx, const unit_t* y, slen_t ly, number_t& res)
{
    assert (lx >= 0 && ly >= 0);

    if (lx && ly)
    {
        unit_t* tmp = res.dat;
        slen_t lr, newcap, lxy = lx + ly;

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
        if (res.dat != tmp)
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
        slen_t lr, newcap;
        unit_t* tmp = res.dat;

        if (res.cap < 2 * lx)
        {
            tmp = __allocate_units(2 * lx, &newcap);
        }
        else if (res.dat == x)
        {
            tmp = __allocate_units(res.cap, &newcap);
        }
        lr = __sqr_core(x, lx, tmp);
        if (tmp != res.dat)
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

    if (lb > 1)
    {
        unit_t *x, *y = r.dat, *tmp = q.dat;
        slen_t n = 0, lx, ly, lr, qnewcap, rnewcap;

        lx = la;
        ly = lb;
        x = __allocate_units(lx + 1);
        if (r.cap < ly)
        {
            y = __allocate_units(ly, &rnewcap);
        }
        else if (r.dat == a || r.dat == b)
        {
            y = __allocate_units(r.cap, &rnewcap);
        }
        __copy_units(x, a, lx);
        __copy_units(y, b, ly);

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

        if (q.cap < lx - ly)
        {
            tmp = __allocate_units(lx - ly, &qnewcap);
        }
        lr = __div_core(x, lx, y, ly, tmp);
        if (tmp != q.dat)
        {
            __deallocate_units(q.dat);
            q.dat = tmp;
            q.cap = qnewcap;
        }
        q.len = lr;

        if (n)
        {
            __shr_core(x, ly, n);
        }
        __copy_units(y, x, ly);
        if (y != r.dat)
        {
            __deallocate_units(r.dat);
            r.dat = y;
            r.cap = rnewcap;
        }
        r.len = ly;
        __trim_leading_zeros(r.dat, r.len);

        __deallocate_units(x);
    }
    else
    {
        slen_t lq, lr, qnewcap, rnewcap;
        unit_t *tmpq = q.dat, *tmpr = r.dat;

        if (q.cap < la)
        {
            tmpq = __allocate_units(la, &qnewcap);
        }
        else if (q.dat == a || q.dat == b)
        {
            tmpq = __allocate_units(q.cap, &qnewcap);
        }
        if (r.cap < 1)
        {
            tmpr = __allocate_units(1, &rnewcap);
        }
        else if (r.dat == a || r.dat == b)
        {
            tmpr = __allocate_units(r.cap, &rnewcap);
        }
        __div_unit_core(a, la, *b, tmpq, &lq, tmpr, &lr);
        if (tmpq != q.dat)
        {
            __deallocate_units(q.dat);
            q.dat = tmpq;
            q.cap = qnewcap;
        }
        if (tmpr != r.dat)
        {
            __deallocate_units(r.dat);
            r.dat = tmpr;
            r.cap = rnewcap;
        }
        q.len = lq;
        r.len = lr;
    }
}

void __div(const unit_t* a, slen_t la, const unit_t* b, slen_t lb, number_t& q)
{
    assert(la >= lb && lb > 0);

    if (lb > 1)
    {
        unit_t *x, *y, *tmp = q.dat;
        slen_t n = 0, lx, ly, lr, newcap;

        lx = la;
        ly = lb;
        x = __allocate_units(lx + 1);
        y = __allocate_units(ly);
        __copy_units(x, a, lx);
        __copy_units(y, b, ly);

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

        if (q.cap < lx - ly)
        {
            tmp = __allocate_units(lx - ly, &newcap);
        }
        lr = __div_core(x, lx, y, ly, tmp);
        if (tmp != q.dat)
        {
            __deallocate_units(q.dat);
            q.dat = tmp;
            q.cap = newcap;
        }
        q.len = lr;
        __deallocate_units(x);
        __deallocate_units(y);
    }
    else
    {
        unit_t *tmp = q.dat, r;
        slen_t lq, lr, newcap;

        if (q.cap < la)
        {
            tmp = __allocate_units(la, &newcap);
        }
        else if (q.dat == a || q.dat == b)
        {
            tmp = __allocate_units(q.cap, &newcap);
        }
        __div_unit_core(a, la, *b, tmp, &lq, &r, &lr);
        if (tmp != q.dat)
        {
            __deallocate_units(q.dat);
            q.dat = tmp;
            q.cap = newcap;
        }
        q.len = lq;
    }
}

void __shl_units(number_t& a, int b)
{
    assert(a.len >= 0);

    unit_t* tmp = __allocate_units(a.len + b);
    __set_units_zero(tmp, b);
    __copy_units(tmp + b, a.dat, a.len);
    __deallocate_units(a.dat);
    a.dat = tmp;
    a.len = a.len + b;
    __trim_leading_zeros(a.dat, a.len);
}

void __sub(const number_t& a, const number_t& b, number_t& res)
{
    assert(cmp(a, b) >= 0);

    unit_t* tmp = __allocate_units(a.len);
    slen_t lr = __sub_core(a.dat, a.len, b.dat, b.len, tmp);
    __deallocate_units(res.dat);
    res.dat = tmp;
    res.len = lr;
}

void __guess_sqrt(const number_t& a, number_t& res)
{
    assert(a.len > 2 && a.is_not(res));

    slen_t n = a.__vbits_count();
    n = n & 1? (n + 1) >> 1 : n >> 1;
    __deallocate_units(res.dat);
    res.len = (n + SHIFT - 1) / SHIFT;
    res.__reserve(res.len);
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
    __trim_leading_zeros(rem.dat, rem.len);

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
    } while (--times > 0 && p != e);

    unit_t* pe = est.dat + est.len - 1;
    while (pe >= est.dat)
    {
        *pr-- = *pe--;
    }

    __trim_leading_zeros(res.dat, res.len);
}

bool __neq_core(const unit_t* x, const unit_t* y, slen_t l)
{
    assert (l >= 0);

    if (l)
    {
        if (x[l - 1] != y[l - 1] || x[0] != y[0])
        {
            return true;
        }
        else
        {
            slen_t i = (x[0] * 19937 + 1) % l;
            if (x[i] != y[i])
            {
                return true;
            }        
        }
    }
    const dunit_t* p1 = (dunit_t*)x;
    const dunit_t* p2 = (dunit_t*)y;
    const dunit_t* pe = l & 1? (dunit_t*)(x + l - 1): (dunit_t*)(x + l);
    while (p1 != pe)
    {  
        if (*p1++ != *p2++)
        {
            return true;
        }
    }
    return false;
}

__always_inline(int) __cmp_same_len_core(const unit_t* x, const unit_t* y, slen_t l)
{
    assert(l >= 0);

    if (l & 1)
    {
        l--;
        if (x[l] > y[l])
        {
            return 1;
        }
        else if (x[l] < y[l])
        {
            return -1;
        }
    }
    const dunit_t* p1 = (dunit_t*)(x + l - 2);
    const dunit_t* p2 = (dunit_t*)(y + l - 2);
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

int __cmp_core(const unit_t* x, slen_t lx, const unit_t* y, slen_t ly)
{
    assert(lx >= 0 && ly >= 0);

    if (lx - ly)
    {
        return __sign(lx - ly);
    }
    return __cmp_same_len_core(x, y, lx);
}

/*
 * x, y, res can be the same, this is an inplace algorithm
 */
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

/*
 * x, y, res can be the same, this is an inplace algorithm
 */
slen_t __sub_core(const unit_t* x, slen_t lx, const unit_t* y, slen_t ly, unit_t* res)
{
    assert(lx >= ly);

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

/*
 * not inplace
 */
slen_t __mul_core(const unit_t* x, slen_t lx, const unit_t* y, slen_t ly, unit_t* res)
{
    assert(res != x && res != y);

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

/*
 * not inplace
 */
slen_t __sqr_core(const unit_t* x, slen_t lx, unit_t* res)
{
    assert(res != x);

    unit_t cflag, *r;
    dunit_t m, carry;
    const unit_t* p, *q;
    const unit_t* pend = x + lx - 1;
    const unit_t* qend = x + lx;

    slen_t i = 0;
    __set_units_zero(res, 2 * lx);
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
        rem = __make_dunit(rem, *px--);
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

slen_t __shl_core(unit_t* x, slen_t lx, slen_t d)
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

unit_t __shr_core(unit_t* x, slen_t lx, slen_t d)
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

static __always_inline(dunit_t) __original_div_4by2(dunit_t h, dunit_t l, dunit_t d, dunit_t* r)
{
    assert(h < d && d != 0);

    dunit_t q1, q2, rx, u, v;
    unit_t d1 = d >> SHIFT, d2 = d & MASK;
    unit_t l1 = l >> SHIFT, l2 = l & MASK;

    if (d1)
    {
        // compute:
        // q1 = (h * BASE + l1) / d
        // r  = (h * BASE + l1) % d
        // q2 = (r * BASE + l2) / d
        // r  = (r * BASE + l2) % d
        // q1 * BASE + q2 is the result of this division

        // first step, use h / d1 to evaluate q1
        q1 = h / d1;
        rx = h % d1;
        // if h < d, q1 can only be less than BASE
        if (q1 >= BASE)
        {
            q1 = MASK;
            rx = h - MASK * d1;
        }
        // evaluated q1 may be greater than the true value, the proof is issue #10 */
        // second step, adjust q1 to the true value, see issue #12
        u = q1 * d2;
        v = __make_dunit(rx, l1);
        while (rx < BASE && v < u)
        {
            u = --q1 * d2;
            v = __make_dunit(rx += d1, l1);
        }
        *r = v - u;

        q2 = *r / d1;
        rx = *r % d1;
        if (q2 >= BASE)
        {
            q2 = MASK;
            rx = *r - MASK * d1;
        }
        u = q2 * d2;
        v = __make_dunit(rx, l2);
        while (rx < BASE && v < u)
        {
            u = --q2 * d2;
            v = __make_dunit(rx += d1, l2);
        }
        *r = v - u;
    }
    else
    {
        u = __make_dunit(h, l1);
        q1 = u / d2;
        *r = u % d2;
        v = __make_dunit(*r, l2);
        q2 = v / d2;
        *r = v % d2;
    }
    return __make_dunit(q1, q2);  // Oh~ It's too slowwwww
}

#ifdef __GNUC__

slen_t __vbits_count(unit_t x)
{
    assert(x != 0);

    return UNITBITS - __builtin_clz((unsigned int)x << (32 - UNITBITS));
}

#if UNITBITS == 16
typedef unsigned long long __qunit_t;
#elif UNITBITS == 32
typedef __uint128_t __qunit_t;
#endif

dunit_t __mul_add_dunit(dunit_t x, dunit_t y, dunit_t z, dunit_t* l)
{
    __qunit_t m = __qunit_t(x) * y + z;
    *l = m & DMASK;
    return m >> (UNITBITS * 2);
}

dunit_t __qunit_div_by_dunit(dunit_t h, dunit_t l, dunit_t d, dunit_t* r)
{
    assert(h < d);

    __qunit_t qunit = __qunit_t(h) << (UNITBITS * 2) | l;
    dunit_t q = dunit_t(qunit / d);
    *r = dunit_t(qunit % d);
    return q;
}

dunit_t __qunit_mod_by_dunit(dunit_t h, dunit_t l, dunit_t d)
{
    assert(h < d);

    __qunit_t qunit = __qunit_t(h) << (UNITBITS * 2) | l;
    return dunit_t(qunit % d);
}

#elif defined(_MSC_VER)

#include <intrin.h>

#pragma intrinsic(_BitScanReverse)

slen_t __vbits_count(unit_t x)
{
    assert(x != 0);

    unsigned long b;
    _BitScanReverse(&b, x);
    return b + 1;
}

#if UNITBITS == 16

#pragma intrinsic(__emulu)
dunit_t __mul_add_dunit(dunit_t x, dunit_t y, dunit_t z, dunit_t* l)
{
    unsigned __int64 r = __emulu(x, y) + z;
    *l = r & DMASK;
    return r >> (UNITBITS * 2);
}

dunit_t __qunit_div_by_dunit(dunit_t h, dunit_t l, dunit_t d, dunit_t* r)
{
    return __original_div_4by2(h, l, d, r);

    //assert(h < d);

    //unsigned __int64 qunit = (unsigned __int64)h << (UNITBITS * 2) | l;
    //dunit_t q = dunit_t(qunit / d);
    //*r = dunit_t(qunit % d);
    //return q;
}

dunit_t __qunit_mod_by_dunit(dunit_t h, dunit_t l, dunit_t d)
{
    assert(h < d);

    unsigned __int64 qunit = (unsigned __int64)h << (UNITBITS * 2) | l;
    return dunit_t(qunit % d);
}

#elif UNITBITS == 32

#pragma intrinsic(_umul128)
dunit_t __mul_add_dunit(dunit_t x, dunit_t y, dunit_t z, dunit_t* l)
{
    unsigned __int64 h, t;
    t = *l = _umul128(x, y, &h);
    *l += z;
    return h + (*l < t);
}

dunit_t __qunit_div_by_dunit(dunit_t h, dunit_t l, dunit_t d, dunit_t* r)
{
    return __original_div_4by2(h, l, d, r);
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

dunit_t __mul_add_dunit(dunit_t x, dunit_t y, dunit_t z, dunit_t* l)
{
    dunit_t a, b, c, d;
    dunit_t m, n, o, h;

    a = x >> UNITBITS; b = x & MASK;
    c = y >> UNITBITS; d = y & MASK;
    m = d * a + (d * b >> UNITBITS);    // never overflow
    n = c * b;
    o = m + n;   // maybe overflow, if overflow o < m

    h = a * c + (o >> UNITBITS) + (dunit_t(o < m) << UNITBITS);
    *l = x * y;

    a = *l;
    *l += z;
    h += *l < a;
    return h;
}

dunit_t __qunit_div_by_dunit(dunit_t h, dunit_t l, dunit_t d, dunit_t* r)
{
    return __original_div_4by2(h, l, d, r);
}

dunit_t __qunit_mod_by_dunit(dunit_t h, dunit_t l, dunit_t d)
{
    dunit_t r;
    __original_div_4by2(h, l, d, &r);
    return r;
}

#endif

} // namespace end
