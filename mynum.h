/* MYNUM LIBARAY HEADER */
#pragma once

#include <cstddef>
#include <climits>
#include <cassert>


namespace mynum {

#if ULONG_MAX == 0xffffffffUL
#define UNITBITS 16
typedef unsigned short unit_t;
typedef unsigned int dunit_t;
typedef int slen_t;
#elif ULONG_MAX == 0xffffffffffffffffUL
#define UNITBITS 32
typedef unsigned int unit_t;
typedef unsigned long long dunit_t;
typedef long long slen_t;
#else
#error ULONG_MAX unknown
#endif


struct _base_number_t
{
    unit_t* dat;
    slen_t len;
    slen_t cap;

    _base_number_t(): dat(NULL), len(0), cap(0)
    {}
};

struct string_t;
struct number_t: public _base_number_t
{
    number_t() {}
    number_t(const char*);
    number_t(const char*, int base);
    number_t(const char* s, size_t length, int base);
    number_t(long x)                { __assign(x);  }
    number_t(long long x)           { __assign(x);  }
    number_t(int x)                 { __assign(x);  }
    number_t(unsigned long x)       { __uassign(x); }
    number_t(unsigned long long x)  { __uassign(x); } 
    number_t(unsigned int x)        { __uassign(x); }
    number_t(const number_t&);

    number_t& assign(const number_t&);
    number_t& assign(int);
    number_t& assign(long);
    number_t& assign(long long);
    number_t& assign(unsigned int);
    number_t& assign(unsigned long);
    number_t& assign(unsigned long long);
    number_t& assign(const char*, int base = 10);
    number_t& assign(const char*, size_t length, int base = 10);

    ~number_t();

    bool is_even() const;
    bool is_not(const number_t& another) const {return this != &another;}
    bool is_odd() const;
    bool is_one() const {return len == 1 && dat[0] == 1;}
    bool is_power2() const;
    bool is_zero() const {return len == 0;}

    string_t& to_bin_string(string_t&) const;
    string_t& to_oct_string(string_t&) const;
    string_t& to_dec_string(string_t&) const;
    string_t& to_hex_string(string_t&) const;
    string_t& to_string(string_t& res, int base = 10) const;

    string_t to_bin_string() const;
    string_t to_oct_string() const;
    string_t to_dec_string() const;
    string_t to_hex_string() const;
    string_t to_string(int base = 10) const;

    void copy(const number_t&);
    void set_one();
    void set_zero();
    void steal(number_t&);

    number_t abs();
    number_t neg();
    number_t& set_abs();
    number_t& set_neg();
    number_t& set_sign(int sign);
    number_t& add(const number_t&);
    number_t& sub(const number_t&);
    number_t& mul(const number_t&);
    number_t& kmul(const number_t&);
    number_t& div(const number_t&);
    number_t& div(const number_t&, number_t&);
    number_t& mod(const number_t&);
    number_t& shr(size_t);
    number_t& shl(size_t);
    number_t& bit_or(const number_t&);
    number_t& bit_and(const number_t&);
    number_t& bit_xor(const number_t&);
    number_t& bit_not();
    number_t& sqr();
    number_t& ksqr();
    number_t& sqrt();
    number_t& pow(const number_t&);
    number_t& pom(const number_t&, const number_t&);

    number_t& add_unit(unit_t);
    number_t& sub_unit(unit_t);
    number_t& mul_unit(unit_t);
    number_t& div_unit(unit_t);
    number_t& mod_unit(unit_t);

    int bit_at(size_t) const;
    void bit_set_one(size_t);
    void bit_set_zero(size_t);
    size_t bits_count() const;
    void bits_reserve(size_t);

    number_t& operator = (const number_t&);
    number_t& operator = (short);
    number_t& operator = (long);
    number_t& operator = (long long);
    number_t& operator = (char);
    number_t& operator = (int);
    number_t& operator = (bool);
    number_t& operator = (unsigned short);
    number_t& operator = (unsigned long);
    number_t& operator = (unsigned long long);
    number_t& operator = (unsigned char);
    number_t& operator = (unsigned int);
    number_t operator + () const;
    number_t operator - () const;
    number_t& operator ~ ();
    number_t& operator ++ ();
    number_t& operator -- ();
    number_t& operator ++ (int);
    number_t& operator -- (int);
    number_t& operator += (const number_t&);
    number_t& operator -= (const number_t&);
    number_t& operator *= (const number_t&);
    number_t& operator /= (const number_t&);
    number_t& operator %= (const number_t&);
    number_t& operator |= (const number_t&);
    number_t& operator &= (const number_t&);
    number_t& operator ^= (const number_t&);
    number_t& operator <<= (int);
    number_t& operator >>= (int);
    operator bool () const;
    bool operator ! () const;

    void __reserve(slen_t units);
    void __release();
    void __trim();
    void __add(unit_t);
    void __mul(unit_t);
    slen_t __abs_add_unit(unit_t);
    slen_t __abs_sub_unit(unit_t);
    slen_t __bits_reserve(slen_t);
    slen_t __vbits_count() const;

    void __copy(const number_t&);

    template <class T> void __uassign(T x)
    {
        __reserve((len = (sizeof(T) + sizeof(unit_t) - 1) / sizeof(unit_t)));
        *dat = 0;
        *(T*)dat = x;
        __trim();
    }

    template <class T> void __assign(T x)
    {
        slen_t sign;
        if (x > 0)
        {
            sign = 1;
        }
        else
        {
            sign = -1;
            x = -x;
        }
        __reserve((len = (sizeof(T) + sizeof(unit_t) - 1) / sizeof(unit_t)));
        *dat = 0;
        *(T*)dat = x;
        len *= sign;
        __trim();
    }

    void __construct_from_bin_string(const char*s, slen_t l);
    void __construct_from_hex_string(const char* s, slen_t l);
    void __construct_from_xbase_string(const char* s, slen_t l, unit_t base, float ln_base, unit_t inner_base, unit_t inner_base_digits);
    void __construct_from_string(const char* s, slen_t l, int base);

    string_t& __to_bin_string(string_t&) const;
    string_t& __to_hex_string(string_t&) const;
    string_t& __to_xbase_string(string_t& res, unit_t base, unit_t inner_base, unit_t inner_base_digits, float ln_inner_base) const;
};

int cmp(const number_t& a, const number_t& b);
int cmp_abs(const number_t& a, const number_t& b);
bool neq(const number_t& a, const number_t& b);
inline bool eq(const number_t& a, const number_t& b)  { return !neq(a, b); }
inline bool lt(const number_t& a, const number_t& b)  { return cmp(a, b) == -1; }
inline bool gt(const number_t& a, const number_t& b)  { return cmp(a, b) == 1; }
inline bool elt(const number_t& a, const number_t& b) { return cmp(a, b) <= 0; }
inline bool egt(const number_t& a, const number_t& b) { return cmp(a, b) >= 0; }

void abs(const number_t& a, number_t& res);
void neg(const number_t& a, number_t& res);
void add(const number_t& a, const number_t& b, number_t& res);
void sub(const number_t& a, const number_t& b, number_t& res);
void mul(const number_t& a, const number_t& b, number_t& res);
void sqr(const number_t& a, number_t& res);
void ksqr(const number_t& a, number_t& res);
void kmul(const number_t& a, const number_t& b, number_t& res);
void shr(const number_t& a, size_t b, number_t& res);
void shl(const number_t& a, size_t b, number_t& res);
void pow(const number_t& a, const number_t& b, number_t& res);
int  div(const number_t& a, const number_t& b, number_t& q, number_t& r);
int  div(const number_t& a, const number_t& b, number_t& q);
int  mod(const number_t& a, const number_t& b, number_t& r);
int  pom(const number_t& a, const number_t& b, const number_t& c, number_t& res);
int  floor_div(const number_t& a, const number_t& b, number_t& q, number_t& r);

unit_t sqrt(dunit_t x);
int sqrt(const number_t& a, number_t& res);
void bit_and(const number_t& a, const number_t& b, number_t& res);
void bit_or(const number_t& a, const number_t& b, number_t& res);
void bit_xor(const number_t& a, const number_t& b, number_t& res);
void bit_not(const number_t& a, number_t& res);

void swap(number_t& a, number_t& b);

bool is_power2(const number_t& a);
bool is_odd(const number_t& a);
bool is_even(const number_t& a);

number_t& set_abs(number_t&);
number_t& set_neg(number_t&);
number_t& set_sign(number_t&, int sign);

number_t abs(const number_t& a);
number_t neg(const number_t& a);
number_t add(const number_t& a, const number_t& b);
number_t sub(const number_t& a, const number_t& b);
number_t mul(const number_t& a, const number_t& b);
number_t sqr(const number_t& a);
number_t ksqr(const number_t& a);
number_t kmul(const number_t& a, const number_t& b);
number_t div(const number_t& a, const number_t& b);
number_t mod(const number_t& a, const number_t& b);
number_t shr(const number_t& a, size_t b);
number_t shl(const number_t& a, size_t b);
number_t pow(const number_t& a, const number_t& b);
number_t sqrt(const number_t& a);
number_t bit_and(const number_t& a, const number_t& b);
number_t bit_or(const number_t& a, const number_t& b);
number_t bit_xor(const number_t& a, const number_t& b);
number_t bit_not(const number_t& a);

inline int sign(const number_t& a)                          { return (a.len >> (sizeof(slen_t) * 8 - 1)) | 1; }
inline int sign(const number_t& a, const number_t& b)       { return ((a.len ^ b.len) >> (sizeof(slen_t) * 8 - 1)) | 1; }
inline bool same_sign(const number_t& a, const number_t& b) { return (a.len ^ b.len) >> (sizeof(slen_t) * 8 - 1) == 0; }

int max_base();

struct string_t
{
    char* dat;
    size_t len;

    string_t(): dat(NULL), len(0) {}
    string_t(const char*);
    string_t(const char*, size_t);
    string_t(const string_t&);

    ~string_t();

    int cmp(const string_t&) const;
    void take(char* p, size_t l) { dat = p; len = l; }
    bool valid() const { return dat != NULL; }
    void release();

    string_t& to_upper();
    string_t& to_lower();
    string_t& to_upper(string_t& res);
    string_t& to_lower(string_t& res);

    string_t& operator = (const char*);
    string_t& operator = (const string_t&);
    char operator [] (size_t x) const { return dat[x]; }
    const char* c_str() const { return dat; }

    void __copy(const char* p, size_t l);
};

int cmp(const string_t& a, const string_t& b);
int check(const char* p, int base);
int check(const char* p, const char* e, int base);

const char* find_valid(const char* p, const char* e, int base, size_t* size);

}  //namespace end

