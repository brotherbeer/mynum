/* MYNUM LIBARAY HEADER */
#pragma once

#include <cstddef>
#include <climits>


namespace mynum {

#ifndef UNITBITS
#if defined(_MSC_VER)
#define WORDMAX SIZE_MAX
#else
#define WORDMAX ULONG_MAX
#endif
#if WORDMAX == 4294967295UL
#define UNITBITS 16
#elif WORDMAX == 18446744073709551615ULL
#define UNITBITS 32
#else
#error WORDMAX unknown
#endif
#endif

#if UNITBITS == 16
typedef short sunit_t;
typedef unsigned short unit_t;
typedef int sdunit_t;
typedef unsigned int dunit_t;
typedef int slen_t;
#elif UNITBITS == 32
typedef int sunit_t;
typedef unsigned int unit_t;
typedef long long sdunit_t;
typedef unsigned long long dunit_t;
typedef long long slen_t;
#else
#error UNITBITS error
#endif

typedef dunit_t word_t;
typedef sdunit_t sword_t;
typedef unsigned char byte_t;

#define __trim_leading_zeros(dat, len) \
{\
    const unit_t *e = dat - 1, *p = e + len; \
    while (p != e && !*p) {p--;} \
    len = slen_t(p - e); \
}

struct _base_number_t
{
    unit_t* dat;
    slen_t len;
    slen_t cap;

    _base_number_t():
        dat(NULL), len(0), cap(0)
    {}

    _base_number_t(unit_t* a, slen_t b, slen_t c):
        dat(a), len(b), cap(c)
    {}
};

struct UDM   // Unit Divisor to Multiplier
{
    dunit_t multiplier;
    unit_t divisor;
    unsigned char shift;
    bool notpo2;
    bool nooverflow;

    UDM(unit_t d);
};

struct string_t;
struct bitref_t;
struct number_t: public _base_number_t
{
    number_t() {}
    number_t(int);
    number_t(long);
    number_t(long long);
    number_t(unsigned int);
    number_t(unsigned long);
    number_t(unsigned long long);
    number_t(const char*);
    number_t(const char*, int base);
    number_t(const char*, size_t length, int base);
    number_t(const string_t&);
    number_t(const string_t&, int base);
    number_t(const string_t&, size_t bpos, size_t epos, int base);
    number_t(const number_t&);

    ~number_t();

    number_t& assign(int);
    number_t& assign(long);
    number_t& assign(long long);
    number_t& assign(unsigned int);
    number_t& assign(unsigned long);
    number_t& assign(unsigned long long);
    number_t& assign(const char*);
    number_t& assign(const char*, int base);
    number_t& assign(const char*, size_t length, int base);
    number_t& assign(const string_t&);
    number_t& assign(const string_t&, int base);
    number_t& assign(const string_t&, size_t bpos, size_t epos, int base);
    number_t& assign(const number_t&);

    void copy(const number_t&);

    void clear();
    void release();
    void reserve(size_t units);
    void clear_and_reserve(size_t units);

    void set_one();
    void set_zero();
    void steal(number_t&);

    number_t abs() const;
    number_t neg() const;
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
    number_t& pow(size_t);
    number_t& pom(const number_t&, const number_t&);

    void add_unit(unit_t);
    void sub_unit(unit_t);
    void mul_unit(unit_t);
    unit_t div_unit(unit_t);
    unit_t div_unit(const UDM&);
    void mod_unit(unit_t);
    void mod_unit(const UDM&);
    void bit_and_unit(unit_t);
    void bit_or_unit(unit_t);
    void bit_xor_unit(unit_t);

    unit_t absrem_unit(unit_t) const;
    unit_t absrem_unit(const UDM&) const;

    number_t& add_ui(word_t);
    number_t& sub_ui(word_t);
    number_t& mul_ui(word_t);
    number_t& div_ui(word_t);
    number_t& mod_ui(word_t);
    number_t& bit_and_ui(word_t);
    number_t& bit_or_ui(word_t);
    number_t& bit_xor_ui(word_t);

    number_t& add_si(sword_t);
    number_t& sub_si(sword_t);
    number_t& mul_si(sword_t);
    number_t& div_si(sword_t);
    number_t& mod_si(sword_t);
    number_t& bit_and_si(sword_t);
    number_t& bit_or_si(sword_t);
    number_t& bit_xor_si(sword_t);

    number_t& add(int x);
    number_t& add(unsigned int x);
    number_t& add(long x);
    number_t& add(unsigned long x);
    number_t& add(long long x);
    number_t& add(unsigned long long x);
    number_t& sub(int x);
    number_t& sub(unsigned int x);
    number_t& sub(long x);
    number_t& sub(unsigned long x);
    number_t& sub(long long x);
    number_t& sub(unsigned long long x);
    number_t& mul(int x);
    number_t& mul(unsigned int x);
    number_t& mul(long x);
    number_t& mul(unsigned long x);
    number_t& mul(long long x);
    number_t& mul(unsigned long long x);
    number_t& div(int x);
    number_t& div(unsigned int x);
    number_t& div(long x);
    number_t& div(unsigned long x);
    number_t& div(long long x);
    number_t& div(unsigned long long x);
    number_t& mod(int x);
    number_t& mod(unsigned int x);
    number_t& mod(long x);
    number_t& mod(unsigned long x);
    number_t& mod(long long x);
    number_t& mod(unsigned long long x);
    number_t& bit_and(int x);
    number_t& bit_and(unsigned int x);
    number_t& bit_and(long x);
    number_t& bit_and(unsigned long x);
    number_t& bit_and(long long x);
    number_t& bit_and(unsigned long long x);
    number_t& bit_or(int x);
    number_t& bit_or(unsigned int x);
    number_t& bit_or(long x);
    number_t& bit_or(unsigned long x);
    number_t& bit_or(long long x);
    number_t& bit_or(unsigned long long x);
    number_t& bit_xor(int x);
    number_t& bit_xor(unsigned int x);
    number_t& bit_xor(long x);
    number_t& bit_xor(unsigned long x);
    number_t& bit_xor(long long x);
    number_t& bit_xor(unsigned long long x);

    bool bit_at(size_t) const;
    void bit_set(size_t, bool v = 1);
    void bit_set_one(size_t);
    void bit_set_zero(size_t);
    size_t bits_count() const;
    size_t tzbits_count() const;
    void bits_reserve(size_t);

    bool is_even() const;
    bool is_not(const number_t& another) const { return this != &another; }
    bool is_neg() const  { return len < 0; }
    bool is_neg_one() const  { return len == -1 && dat[0] == 1; }
    bool is_odd() const;
    bool is_one() const  { return len == 1 && dat[0] == 1; }
    bool is_po2() const;
    bool is_pos() const  { return len > 0; }
    bool is_zero() const { return len == 0; }

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

    bool in_range_char() const;
    bool in_range_short() const;
    bool in_range_int() const;
    bool in_range_long() const;
    bool in_range_longlong() const;
    bool in_range_uchar() const;
    bool in_range_ushort() const;
    bool in_range_uint() const;
    bool in_range_ulong() const;
    bool in_range_ulonglong() const;
    bool in_range_word() const;
    bool in_range_sword() const;

    char to_char() const;
    short to_short() const;
    int to_int() const;
    long to_long() const;
    long long to_longlong() const;
    unsigned char to_uchar() const;
    unsigned short to_ushort() const;
    unsigned int to_uint() const;
    unsigned long to_ulong() const;
    unsigned long long to_ulonglong() const;

    number_t& operator = (const number_t& x)     { return assign(x); }
    number_t& operator = (bool x)                { return assign(x); }
    number_t& operator = (char x)                { return assign((int)x); }
    number_t& operator = (short x)               { return assign((int)x); }
    number_t& operator = (int x)                 { return assign(x); }
    number_t& operator = (long x)                { return assign(x); }
    number_t& operator = (long long x)           { return assign(x); }   
    number_t& operator = (unsigned char x)       { return assign((unsigned int)x); }
    number_t& operator = (unsigned short x)      { return assign((unsigned int)x); }
    number_t& operator = (unsigned int x)        { return assign(x); }
    number_t& operator = (unsigned long x)       { return assign(x); }
    number_t& operator = (unsigned long long x)  { return assign(x); }

    number_t  operator + () const                { return *this; }
    number_t  operator - () const                { return neg(); }
    number_t  operator ~ () const                { number_t x(*this); return x.bit_not(); }
    number_t& operator ++ ()                     { add_unit(1); return *this; }
    number_t& operator -- ()                     { sub_unit(1); return *this; }
    number_t& operator ++ (int)                  { add_unit(1); return *this; }
    number_t& operator -- (int)                  { sub_unit(1); return *this; }
    number_t& operator += (const number_t& x)    { return add(x); }
    number_t& operator -= (const number_t& x)    { return sub(x); }
    number_t& operator *= (const number_t& x)    { return mul(x); }
    number_t& operator /= (const number_t& x)    { return div(x); }
    number_t& operator %= (const number_t& x)    { return mod(x); }
    number_t& operator &= (const number_t& x)    { return bit_and(x); }
    number_t& operator |= (const number_t& x)    { return bit_or(x); }
    number_t& operator ^= (const number_t& x)    { return bit_xor(x); }
    number_t& operator += (int x)                { return add(x); }
    number_t& operator -= (int x)                { return sub(x); }
    number_t& operator *= (int x)                { return mul(x); }
    number_t& operator /= (int x)                { return div(x); }
    number_t& operator %= (int x)                { return mod(x); }
    number_t& operator &= (int x)                { return bit_and(x); }
    number_t& operator |= (int x)                { return bit_or(x); }
    number_t& operator ^= (int x)                { return bit_xor(x); }
    number_t& operator += (unsigned int x)       { return add(x); }
    number_t& operator -= (unsigned int x)       { return sub(x); }
    number_t& operator *= (unsigned int x)       { return mul(x); }
    number_t& operator /= (unsigned int x)       { return div(x); }
    number_t& operator %= (unsigned int x)       { return mod(x); }
    number_t& operator &= (unsigned int x)       { return bit_and(x); }
    number_t& operator |= (unsigned int x)       { return bit_or(x); }
    number_t& operator ^= (unsigned int x)       { return bit_xor(x); }
    number_t& operator += (long x)               { return add(x); }
    number_t& operator -= (long x)               { return sub(x); }
    number_t& operator *= (long x)               { return mul(x); }
    number_t& operator /= (long x)               { return div(x); }
    number_t& operator %= (long x)               { return mod(x); }
    number_t& operator &= (long x)               { return bit_and(x); }
    number_t& operator |= (long x)               { return bit_or(x); }
    number_t& operator ^= (long x)               { return bit_xor(x); }
    number_t& operator += (unsigned long x)      { return add(x); }
    number_t& operator -= (unsigned long x)      { return sub(x); }
    number_t& operator *= (unsigned long x)      { return mul(x); }
    number_t& operator /= (unsigned long x)      { return div(x); }
    number_t& operator %= (unsigned long x)      { return mod(x); }
    number_t& operator &= (unsigned long x)      { return bit_and(x); }
    number_t& operator |= (unsigned long x)      { return bit_or(x); }
    number_t& operator ^= (unsigned long x)      { return bit_xor(x); }
    number_t& operator += (long long x)          { return add(x); }
    number_t& operator -= (long long x)          { return sub(x); }
    number_t& operator *= (long long x)          { return mul(x); }
    number_t& operator /= (long long x)          { return div(x); }
    number_t& operator %= (long long x)          { return mod(x); }
    number_t& operator &= (long long x)          { return bit_and(x); }
    number_t& operator |= (long long x)          { return bit_or(x); }
    number_t& operator ^= (long long x)          { return bit_xor(x); }
    number_t& operator += (unsigned long long x) { return add(x); }
    number_t& operator -= (unsigned long long x) { return sub(x); }
    number_t& operator *= (unsigned long long x) { return mul(x); }
    number_t& operator /= (unsigned long long x) { return div(x); }
    number_t& operator %= (unsigned long long x) { return mod(x); }
    number_t& operator &= (unsigned long long x) { return bit_and(x); }
    number_t& operator |= (unsigned long long x) { return bit_or(x); }
    number_t& operator ^= (unsigned long long x) { return bit_xor(x); }
    number_t& operator <<= (size_t x)            { return shl(x); }
    number_t& operator >>= (size_t x)            { return shr(x); }

    operator bool () const   { return !is_zero(); }
    bool operator ! () const { return  is_zero(); }
    operator char () const   { return to_char(); }
    operator short () const  { return to_short(); }
    operator int () const    { return to_int(); }
    operator long () const   { return to_long(); }
    operator long long () const      { return to_longlong(); }
    operator unsigned char () const  { return to_uchar(); }
    operator unsigned short () const { return to_ushort(); }
    operator unsigned int () const   { return to_int(); }
    operator unsigned long () const  { return to_long(); }
    operator unsigned long long () const  { return to_longlong(); }

    string_t operator () (int) const;
    bool operator [] (size_t) const;
    bitref_t operator [] (size_t);

protected:
    number_t(unit_t* a, slen_t b, slen_t c):
         _base_number_t(a, b, c) {}

    void __reserve(slen_t units);
    slen_t __abs_add_unit(unit_t);
    slen_t __abs_sub_unit(unit_t);
    slen_t __abs_add_word(word_t);
    slen_t __abs_sub_word(word_t);
    slen_t __bits_reserve(slen_t);
    slen_t __vbits_count() const;
    void __copy(const number_t&);
    void __construct_add(unit_t);
    void __construct_mul(unit_t);
    void __construct_from_bin_string(const char*, slen_t);
    void __construct_from_hex_string(const char*, slen_t);
    void __construct_from_xbase_string(const char*, slen_t l, int base, float ln_base, unit_t power_base, unit_t power_base_digits);
    void __construct_from_string(const char*, slen_t, int base);

    string_t& __to_bin_string(string_t&) const;
    string_t& __to_hex_string(string_t&) const;
    string_t& __to_xbase_string(string_t& res, unit_t base, unit_t power_base, unit_t power_base_digits, float ln_power_base) const;
};

#define REFCAP (sizeof(T) / sizeof(unit_t))
template <class T> struct _stype_ref_t: public number_t
{
    T value;

    _stype_ref_t(T x):
        number_t((unit_t*)&value, REFCAP, REFCAP)
    {
        T y = x >> ((sizeof(T) << 3) - 1);
        value = (x + y) ^ y;
        __trim_leading_zeros(dat, len);
        len *= slen_t(y | 1);
    }

    ~_stype_ref_t()
    {
        dat = NULL;
    }
};

template <class T> struct _utype_ref_t: public number_t
{
    T value;

    _utype_ref_t(T x):
        number_t((unit_t*)&value, REFCAP, REFCAP), value(x)
    {
        __trim_leading_zeros(dat, len);
    }

    ~_utype_ref_t()
    {
        dat = NULL;
    }
};

int cmp(const number_t& a, const number_t& b);
int cmp_abs(const number_t& a, const number_t& b);
inline int cmp(const number_t& a, int b)                 { _stype_ref_t<int> r(b); return cmp(a, r); }
inline int cmp(const number_t& a, unsigned int b)        { _utype_ref_t<unsigned int> r(b); return cmp(a, r); }
inline int cmp(const number_t& a, long b)                { _stype_ref_t<long> r(b); return cmp(a, r); }
inline int cmp(const number_t& a, unsigned long b)       { _utype_ref_t<unsigned long> r(b); return cmp(a, r); }
inline int cmp(const number_t& a, long long b)           { _stype_ref_t<long long> r(b); return cmp(a, r); }
inline int cmp(const number_t& a, unsigned long long b)  { _utype_ref_t<unsigned long long> r(b); return cmp(a, r); }
inline int cmp(int a, const number_t& b)                 { _stype_ref_t<int> r(a); return cmp(r, b); }
inline int cmp(unsigned int a, const number_t& b)        { _utype_ref_t<unsigned int> r(a); return cmp(r, b); }
inline int cmp(long a, const number_t& b)                { _stype_ref_t<long> r(a); return cmp(r, b); }
inline int cmp(unsigned long a, const number_t& b)       { _utype_ref_t<unsigned long> r(a); return cmp(r, b); }
inline int cmp(long long a, const number_t& b)           { _stype_ref_t<long long> r(a); return cmp(r, b); }
inline int cmp(unsigned long long a, const number_t& b)  { _utype_ref_t<unsigned long long> r(a); return cmp(r, b); }
bool neq(const number_t& a, const number_t& b);
inline bool neq(const number_t& a, int b)                { _stype_ref_t<int> r(b); return neq(a, r); }
inline bool neq(const number_t& a, unsigned int b)       { _utype_ref_t<unsigned int> r(b); return neq(a, r); }
inline bool neq(const number_t& a, long b)               { _stype_ref_t<long> r(b); return neq(a, r); }
inline bool neq(const number_t& a, unsigned long b)      { _utype_ref_t<unsigned long> r(b); return neq(a, r); }
inline bool neq(const number_t& a, long long b)          { _stype_ref_t<long long> r(b); return neq(a, r); }
inline bool neq(const number_t& a, unsigned long long b) { _utype_ref_t<unsigned long long> r(b); return neq(a, r); }
inline bool neq(int a, const number_t& b)                { _stype_ref_t<int> r(a); return neq(r, b); }
inline bool neq(unsigned int a, const number_t& b)       { _utype_ref_t<unsigned int> r(a); return neq(r, b); }
inline bool neq(long a, const number_t& b)               { _stype_ref_t<long> r(a); return neq(r, b); }
inline bool neq(unsigned long a, const number_t& b)      { _utype_ref_t<unsigned long> r(a); return neq(r, b); }
inline bool neq(long long a, const number_t& b)          { _stype_ref_t<long long> r(a); return neq(r, b); }
inline bool neq(unsigned long long a, const number_t& b) { _utype_ref_t<unsigned long long> r(a); return neq(r, b); }
inline bool eq(const number_t& a, const number_t& b)     { return !neq(a, b); }
inline bool eq(const number_t& a, int b)                 { return !neq(a, b); }
inline bool eq(const number_t& a, unsigned int b)        { return !neq(a, b); }
inline bool eq(const number_t& a, long b)                { return !neq(a, b); }
inline bool eq(const number_t& a, unsigned long b)       { return !neq(a, b); }
inline bool eq(const number_t& a, long long b)           { return !neq(a, b); }
inline bool eq(const number_t& a, unsigned long long b)  { return !neq(a, b); }
inline bool eq(int a, const number_t& b)                 { return !neq(a, b); }
inline bool eq(unsigned int a, const number_t& b)        { return !neq(a, b); }
inline bool eq(long a, const number_t& b)                { return !neq(a, b); }
inline bool eq(unsigned long a, const number_t& b)       { return !neq(a, b); }
inline bool eq(long long a, const number_t& b)           { return !neq(a, b); }
inline bool eq(unsigned long long a, const number_t& b)  { return !neq(a, b); }
inline bool lt(const number_t& a, const number_t& b)     { return cmp(a, b) == -1; }
inline bool lt(const number_t& a, int b)                 { return cmp(a, b) == -1; }
inline bool lt(const number_t& a, unsigned int b)        { return cmp(a, b) == -1; }
inline bool lt(const number_t& a, long b)                { return cmp(a, b) == -1; }
inline bool lt(const number_t& a, unsigned long b)       { return cmp(a, b) == -1; }
inline bool lt(const number_t& a, long long b)           { return cmp(a, b) == -1; }
inline bool lt(const number_t& a, unsigned long long b)  { return cmp(a, b) == -1; }
inline bool lt(int a, const number_t& b)                 { return cmp(a, b) == -1; }
inline bool lt(unsigned int a, const number_t& b)        { return cmp(a, b) == -1; }
inline bool lt(long a, const number_t& b)                { return cmp(a, b) == -1; }
inline bool lt(unsigned long a, const number_t& b)       { return cmp(a, b) == -1; }
inline bool lt(long long a, const number_t& b)           { return cmp(a, b) == -1; }
inline bool lt(unsigned long long a, const number_t& b)  { return cmp(a, b) == -1; }
inline bool gt(const number_t& a, const number_t& b)     { return cmp(a, b) == 1; }
inline bool gt(const number_t& a, int b)                 { return cmp(a, b) == 1; }
inline bool gt(const number_t& a, unsigned int b)        { return cmp(a, b) == 1; }
inline bool gt(const number_t& a, long b)                { return cmp(a, b) == 1; }
inline bool gt(const number_t& a, unsigned long b)       { return cmp(a, b) == 1; }
inline bool gt(const number_t& a, long long b)           { return cmp(a, b) == 1; }
inline bool gt(const number_t& a, unsigned long long b)  { return cmp(a, b) == 1; }
inline bool gt(int a, const number_t& b)                 { return cmp(a, b) == 1; }
inline bool gt(unsigned int a, const number_t& b)        { return cmp(a, b) == 1; }
inline bool gt(long a, const number_t& b)                { return cmp(a, b) == 1; }
inline bool gt(unsigned long a, const number_t& b)       { return cmp(a, b) == 1; }
inline bool gt(long long a, const number_t& b)           { return cmp(a, b) == 1; }
inline bool gt(unsigned long long a, const number_t& b)  { return cmp(a, b) == 1; }
inline bool elt(const number_t& a, const number_t& b)    { return cmp(a, b) <= 0; }
inline bool elt(const number_t& a, int b)                { return cmp(a, b) <= 0; }
inline bool elt(const number_t& a, unsigned int b)       { return cmp(a, b) <= 0; }
inline bool elt(const number_t& a, long b)               { return cmp(a, b) <= 0; }
inline bool elt(const number_t& a, unsigned long b)      { return cmp(a, b) <= 0; }
inline bool elt(const number_t& a, long long b)          { return cmp(a, b) <= 0; }
inline bool elt(const number_t& a, unsigned long long b) { return cmp(a, b) <= 0; }
inline bool elt(int a, const number_t& b)                { return cmp(a, b) <= 0; }
inline bool elt(unsigned int a, const number_t& b)       { return cmp(a, b) <= 0; }
inline bool elt(long a, const number_t& b)               { return cmp(a, b) <= 0; }
inline bool elt(unsigned long a, const number_t& b)      { return cmp(a, b) <= 0; }
inline bool elt(long long a, const number_t& b)          { return cmp(a, b) <= 0; }
inline bool elt(unsigned long long a, const number_t& b) { return cmp(a, b) <= 0; }
inline bool egt(const number_t& a, const number_t& b)    { return cmp(a, b) >= 0; }
inline bool egt(const number_t& a, int b)                { return cmp(a, b) >= 0; }
inline bool egt(const number_t& a, unsigned int b)       { return cmp(a, b) >= 0; }
inline bool egt(const number_t& a, long b)               { return cmp(a, b) >= 0; }
inline bool egt(const number_t& a, unsigned long b)      { return cmp(a, b) >= 0; }
inline bool egt(const number_t& a, long long b)          { return cmp(a, b) >= 0; }
inline bool egt(const number_t& a, unsigned long long b) { return cmp(a, b) >= 0; }
inline bool egt(int a, const number_t& b)                { return cmp(a, b) >= 0; }
inline bool egt(unsigned int a, const number_t& b)       { return cmp(a, b) >= 0; }
inline bool egt(long a, const number_t& b)               { return cmp(a, b) >= 0; }
inline bool egt(unsigned long a, const number_t& b)      { return cmp(a, b) >= 0; }
inline bool egt(long long a, const number_t& b)          { return cmp(a, b) >= 0; }
inline bool egt(unsigned long long a, const number_t& b) { return cmp(a, b) >= 0; }

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
void pow(const number_t& a, size_t b, number_t& res);
int  div(const number_t& a, const number_t& b, number_t& q, number_t& r);
int  div(const number_t& a, const number_t& b, number_t& q);
int  mod(const number_t& a, const number_t& b, number_t& r);
int  pom(const number_t& a, const number_t& b, const number_t& c, number_t& res);
void bit_and(const number_t& a, const number_t& b, number_t& res);
void bit_or(const number_t& a, const number_t& b, number_t& res);
void bit_xor(const number_t& a, const number_t& b, number_t& res);
void bit_not(const number_t& a, number_t& res);

void add_unit(const number_t& a, unit_t x, number_t& res);
void sub_unit(const number_t& a, unit_t x, number_t& res);
void mul_unit(const number_t& a, unit_t x, number_t& res);
unit_t div_unit(const number_t& a, unit_t x, number_t& res);
unit_t div_unit(const number_t& a, const UDM&, number_t& res);
void mod_unit(const number_t& a, unit_t x, number_t& res);
void mod_unit(const number_t& a, const UDM&, number_t& res);
void bit_and_unit(const number_t& a, unit_t x, number_t& res);
void bit_or_unit(const number_t& a, unit_t x, number_t& res);
void bit_xor_unit(const number_t& a, unit_t x, number_t& res);

inline void add(const number_t& a, int x, number_t& res)                 { _stype_ref_t<int> ref(x); add(a, ref, res);}
inline void add(const number_t& a, unsigned int x, number_t& res)        { _utype_ref_t<unsigned int> ref(x); add(a, ref, res);}
inline void add(const number_t& a, long x, number_t& res)                { _stype_ref_t<long> ref(x); add(a, ref, res);}
inline void add(const number_t& a, unsigned long x, number_t& res)       { _utype_ref_t<unsigned long> ref(x); add(a, ref, res);}
inline void add(const number_t& a, long long x, number_t& res)           { _stype_ref_t<long long> ref(x); add(a, ref, res);}
inline void add(const number_t& a, unsigned long long x, number_t& res)  { _utype_ref_t<unsigned long long> ref(x); add(a, ref, res);}
inline void add(int x, const number_t& b, number_t& res)                 { _stype_ref_t<int> ref(x); add(ref, b, res);}
inline void add(unsigned int x, const number_t& b, number_t& res)        { _utype_ref_t<unsigned int> ref(x); add(ref, b, res);}
inline void add(long x, const number_t& b, number_t& res)                { _stype_ref_t<long> ref(x); add(ref, b, res);}
inline void add(unsigned long x, const number_t& b, number_t& res)       { _utype_ref_t<unsigned long> ref(x); add(ref, b, res);}
inline void add(long long x, const number_t& b, number_t& res)           { _stype_ref_t<long long> ref(x); add(ref, b, res);}
inline void add(unsigned long long x, const number_t& b, number_t& res)  { _utype_ref_t<unsigned long long> ref(x); add(ref, b, res);}

inline void sub(const number_t& a, int x, number_t& res)                 { _stype_ref_t<int> ref(x); sub(a, ref, res);}
inline void sub(const number_t& a, unsigned int x, number_t& res)        { _utype_ref_t<unsigned int> ref(x); sub(a, ref, res);}
inline void sub(const number_t& a, long x, number_t& res)                { _stype_ref_t<long> ref(x); sub(a, ref, res);}
inline void sub(const number_t& a, unsigned long x, number_t& res)       { _utype_ref_t<unsigned long> ref(x); sub(a, ref, res);}
inline void sub(const number_t& a, long long x, number_t& res)           { _stype_ref_t<long long> ref(x); sub(a, ref, res);}
inline void sub(const number_t& a, unsigned long long x, number_t& res)  { _utype_ref_t<unsigned long long> ref(x); sub(a, ref, res);}
inline void sub(int x, const number_t& b, number_t& res)                 { _stype_ref_t<int> ref(x); sub(ref, b, res);}
inline void sub(unsigned int x, const number_t& b, number_t& res)        { _utype_ref_t<unsigned int> ref(x); sub(ref, b, res);}
inline void sub(long x, const number_t& b, number_t& res)                { _stype_ref_t<long> ref(x); sub(ref, b, res);}
inline void sub(unsigned long x, const number_t& b, number_t& res)       { _utype_ref_t<unsigned long> ref(x); sub(ref, b, res);}
inline void sub(long long x, const number_t& b, number_t& res)           { _stype_ref_t<long long> ref(x); sub(ref, b, res);}
inline void sub(unsigned long long x, const number_t& b, number_t& res)  { _utype_ref_t<unsigned long long> ref(x); sub(ref, b, res);}

inline void mul(const number_t& a, int x, number_t& res)                 { _stype_ref_t<int> ref(x); mul(a, ref, res);}
inline void mul(const number_t& a, unsigned int x, number_t& res)        { _utype_ref_t<unsigned int> ref(x); mul(a, ref, res);}
inline void mul(const number_t& a, long x, number_t& res)                { _stype_ref_t<long> ref(x); mul(a, ref, res);}
inline void mul(const number_t& a, unsigned long x, number_t& res)       { _utype_ref_t<unsigned long> ref(x); mul(a, ref, res);}
inline void mul(const number_t& a, long long x, number_t& res)           { _stype_ref_t<long long> ref(x); mul(a, ref, res);}
inline void mul(const number_t& a, unsigned long long x, number_t& res)  { _utype_ref_t<unsigned long long> ref(x); mul(a, ref, res);}
inline void mul(int x, const number_t& b, number_t& res)                 { _stype_ref_t<int> ref(x); mul(ref, b, res);}
inline void mul(unsigned int x, const number_t& b, number_t& res)        { _utype_ref_t<unsigned int> ref(x); mul(ref, b, res);}
inline void mul(long x, const number_t& b, number_t& res)                { _stype_ref_t<long> ref(x); mul(ref, b, res);}
inline void mul(unsigned long x, const number_t& b, number_t& res)       { _utype_ref_t<unsigned long> ref(x); mul(ref, b, res);}
inline void mul(long long x, const number_t& b, number_t& res)           { _stype_ref_t<long long> ref(x); mul(ref, b, res);}
inline void mul(unsigned long long x, const number_t& b, number_t& res)  { _utype_ref_t<unsigned long long> ref(x); mul(ref, b, res);}

inline void div(const number_t& a, int x, number_t& res)                 { _stype_ref_t<int> ref(x); div(a, ref, res);}
inline void div(const number_t& a, unsigned int x, number_t& res)        { _utype_ref_t<unsigned int> ref(x); div(a, ref, res);}
inline void div(const number_t& a, long x, number_t& res)                { _stype_ref_t<long> ref(x); div(a, ref, res);}
inline void div(const number_t& a, unsigned long x, number_t& res)       { _utype_ref_t<unsigned long> ref(x); div(a, ref, res);}
inline void div(const number_t& a, long long x, number_t& res)           { _stype_ref_t<long long> ref(x); div(a, ref, res);}
inline void div(const number_t& a, unsigned long long x, number_t& res)  { _utype_ref_t<unsigned long long> ref(x); div(a, ref, res);}
inline void div(int x, const number_t& b, number_t& res)                 { _stype_ref_t<int> ref(x); div(ref, b, res);}
inline void div(unsigned int x, const number_t& b, number_t& res)        { _utype_ref_t<unsigned int> ref(x); div(ref, b, res);}
inline void div(long x, const number_t& b, number_t& res)                { _stype_ref_t<long> ref(x); div(ref, b, res);}
inline void div(unsigned long x, const number_t& b, number_t& res)       { _utype_ref_t<unsigned long> ref(x); div(ref, b, res);}
inline void div(long long x, const number_t& b, number_t& res)           { _stype_ref_t<long long> ref(x); div(ref, b, res);}
inline void div(unsigned long long x, const number_t& b, number_t& res)  { _utype_ref_t<unsigned long long> ref(x); div(ref, b, res);}

inline void mod(const number_t& a, int x, number_t& res)                 { _stype_ref_t<int> ref(x); mod(a, ref, res);}
inline void mod(const number_t& a, unsigned int x, number_t& res)        { _utype_ref_t<unsigned int> ref(x); mod(a, ref, res);}
inline void mod(const number_t& a, long x, number_t& res)                { _stype_ref_t<long> ref(x); mod(a, ref, res);}
inline void mod(const number_t& a, unsigned long x, number_t& res)       { _utype_ref_t<unsigned long> ref(x); mod(a, ref, res);}
inline void mod(const number_t& a, long long x, number_t& res)           { _stype_ref_t<long long> ref(x); mod(a, ref, res);}
inline void mod(const number_t& a, unsigned long long x, number_t& res)  { _utype_ref_t<unsigned long long> ref(x); mod(a, ref, res);}
inline void mod(int x, const number_t& b, number_t& res)                 { _stype_ref_t<int> ref(x); mod(ref, b, res);}
inline void mod(unsigned int x, const number_t& b, number_t& res)        { _utype_ref_t<unsigned int> ref(x); mod(ref, b, res);}
inline void mod(long x, const number_t& b, number_t& res)                { _stype_ref_t<long> ref(x); mod(ref, b, res);}
inline void mod(unsigned long x, const number_t& b, number_t& res)       { _utype_ref_t<unsigned long> ref(x); mod(ref, b, res);}
inline void mod(long long x, const number_t& b, number_t& res)           { _stype_ref_t<long long> ref(x); mod(ref, b, res);}
inline void mod(unsigned long long x, const number_t& b, number_t& res)  { _utype_ref_t<unsigned long long> ref(x); mod(ref, b, res);}

inline void bit_and(const number_t& a, int x, number_t& res)                 { _stype_ref_t<int> ref(x); bit_and(a, ref, res);}
inline void bit_and(const number_t& a, unsigned int x, number_t& res)        { _utype_ref_t<unsigned int> ref(x); bit_and(a, ref, res);}
inline void bit_and(const number_t& a, long x, number_t& res)                { _stype_ref_t<long> ref(x); bit_and(a, ref, res);}
inline void bit_and(const number_t& a, unsigned long x, number_t& res)       { _utype_ref_t<unsigned long> ref(x); bit_and(a, ref, res);}
inline void bit_and(const number_t& a, long long x, number_t& res)           { _stype_ref_t<long long> ref(x); bit_and(a, ref, res);}
inline void bit_and(const number_t& a, unsigned long long x, number_t& res)  { _utype_ref_t<unsigned long long> ref(x); bit_and(a, ref, res);}
inline void bit_and(int x, const number_t& b, number_t& res)                 { _stype_ref_t<int> ref(x); bit_and(ref, b, res);}
inline void bit_and(unsigned int x, const number_t& b, number_t& res)        { _utype_ref_t<unsigned int> ref(x); bit_and(ref, b, res);}
inline void bit_and(long x, const number_t& b, number_t& res)                { _stype_ref_t<long> ref(x); bit_and(ref, b, res);}
inline void bit_and(unsigned long x, const number_t& b, number_t& res)       { _utype_ref_t<unsigned long> ref(x); bit_and(ref, b, res);}
inline void bit_and(long long x, const number_t& b, number_t& res)           { _stype_ref_t<long long> ref(x); bit_and(ref, b, res);}
inline void bit_and(unsigned long long x, const number_t& b, number_t& res)  { _utype_ref_t<unsigned long long> ref(x); bit_and(ref, b, res);}

inline void bit_or(const number_t& a, int x, number_t& res)                 { _stype_ref_t<int> ref(x); bit_or(a, ref, res);}
inline void bit_or(const number_t& a, unsigned int x, number_t& res)        { _utype_ref_t<unsigned int> ref(x); bit_or(a, ref, res);}
inline void bit_or(const number_t& a, long x, number_t& res)                { _stype_ref_t<long> ref(x); bit_or(a, ref, res);}
inline void bit_or(const number_t& a, unsigned long x, number_t& res)       { _utype_ref_t<unsigned long> ref(x); bit_or(a, ref, res);}
inline void bit_or(const number_t& a, long long x, number_t& res)           { _stype_ref_t<long long> ref(x); bit_or(a, ref, res);}
inline void bit_or(const number_t& a, unsigned long long x, number_t& res)  { _utype_ref_t<unsigned long long> ref(x); bit_or(a, ref, res);}
inline void bit_or(int x, const number_t& b, number_t& res)                 { _stype_ref_t<int> ref(x); bit_or(ref, b, res);}
inline void bit_or(unsigned int x, const number_t& b, number_t& res)        { _utype_ref_t<unsigned int> ref(x); bit_or(ref, b, res);}
inline void bit_or(long x, const number_t& b, number_t& res)                { _stype_ref_t<long> ref(x); bit_or(ref, b, res);}
inline void bit_or(unsigned long x, const number_t& b, number_t& res)       { _utype_ref_t<unsigned long> ref(x); bit_or(ref, b, res);}
inline void bit_or(long long x, const number_t& b, number_t& res)           { _stype_ref_t<long long> ref(x); bit_or(ref, b, res);}
inline void bit_or(unsigned long long x, const number_t& b, number_t& res)  { _utype_ref_t<unsigned long long> ref(x); bit_or(ref, b, res);}

inline void bit_xor(const number_t& a, int x, number_t& res)                 { _stype_ref_t<int> ref(x); bit_xor(a, ref, res);}
inline void bit_xor(const number_t& a, unsigned int x, number_t& res)        { _utype_ref_t<unsigned int> ref(x); bit_xor(a, ref, res);}
inline void bit_xor(const number_t& a, long x, number_t& res)                { _stype_ref_t<long> ref(x); bit_xor(a, ref, res);}
inline void bit_xor(const number_t& a, unsigned long x, number_t& res)       { _utype_ref_t<unsigned long> ref(x); bit_xor(a, ref, res);}
inline void bit_xor(const number_t& a, long long x, number_t& res)           { _stype_ref_t<long long> ref(x); bit_xor(a, ref, res);}
inline void bit_xor(const number_t& a, unsigned long long x, number_t& res)  { _utype_ref_t<unsigned long long> ref(x); bit_xor(a, ref, res);}
inline void bit_xor(int x, const number_t& b, number_t& res)                 { _stype_ref_t<int> ref(x); bit_xor(ref, b, res);}
inline void bit_xor(unsigned int x, const number_t& b, number_t& res)        { _utype_ref_t<unsigned int> ref(x); bit_xor(ref, b, res);}
inline void bit_xor(long x, const number_t& b, number_t& res)                { _stype_ref_t<long> ref(x); bit_xor(ref, b, res);}
inline void bit_xor(unsigned long x, const number_t& b, number_t& res)       { _utype_ref_t<unsigned long> ref(x); bit_xor(ref, b, res);}
inline void bit_xor(long long x, const number_t& b, number_t& res)           { _stype_ref_t<long long> ref(x); bit_xor(ref, b, res);}
inline void bit_xor(unsigned long long x, const number_t& b, number_t& res)  { _utype_ref_t<unsigned long long> ref(x); bit_xor(ref, b, res);}

inline number_t& set_abs(number_t& a)                          { a.len = a.len >= 0? a.len: -a.len; return a; }
inline number_t& set_neg(number_t& a)                          { a.len = -a.len; return a; }
inline number_t& set_sign(number_t& a, int sign)               { set_abs(a); if (sign < 0) a.len = -a.len; return a; }
inline number_t abs(const number_t& a)                         { number_t res; abs(a, res); return res; }
inline number_t neg(const number_t& a)                         { number_t res; neg(a, res); return res; }
inline number_t add(const number_t& a, const number_t& b)      { number_t res; add(a, b, res); return res; }
inline number_t sub(const number_t& a, const number_t& b)      { number_t res; sub(a, b, res); return res; }
inline number_t mul(const number_t& a, const number_t& b)      { number_t res; mul(a, b, res); return res; }
inline number_t sqr(const number_t& a)                         { number_t res; sqr(a, res); return res; }
inline number_t ksqr(const number_t& a)                        { number_t res; ksqr(a, res); return res; }
inline number_t kmul(const number_t& a, const number_t& b)     { number_t res; kmul(a, b, res); return res; }
inline number_t div(const number_t& a, const number_t& b)      { number_t res, dummy; div(a, b, res, dummy); return res; }
inline number_t mod(const number_t& a, const number_t& b)      { number_t res, dummy; div(a, b, dummy, res); return res; }
inline number_t shr(const number_t& a, size_t b)               { number_t res; shr(a, b, res); return res; }
inline number_t shl(const number_t& a, size_t b)               { number_t res; shl(a, b, res); return res; }
inline number_t pow(const number_t& a, size_t b)               { number_t res; pow(a, b, res); return res; }
inline number_t bit_and(const number_t& a, const number_t& b)  { number_t res; bit_and(a, b, res); return res; }
inline number_t bit_or(const number_t& a, const number_t& b)   { number_t res; bit_or(a, b, res); return res; }
inline number_t bit_xor(const number_t& a, const number_t& b)  { number_t res; bit_xor(a, b, res); return res; }
inline number_t bit_not(const number_t& a)                     { number_t res; bit_not(a, res); return res; }

inline int sign(const number_t& a)                             { return (a.len >> (sizeof(slen_t) * 8 - 1)) | 1; }
inline int sign(const number_t& a, const number_t& b)          { return ((a.len ^ b.len) >> (sizeof(slen_t) * 8 - 1)) | 1; }
inline bool same_sign(const number_t& a, const number_t& b)    { return (a.len ^ b.len) >> (sizeof(slen_t) * 8 - 1) == 0; }
inline number_t pom(const number_t& a, const number_t& b, const number_t& c) { number_t res; pom(a, b, c, res); return res; }
void swap(number_t& a, number_t& b);
inline int min_base() { return 2; }
int max_base();

struct string_t
{
    static const size_t npos = -1;

    char* dat;
    size_t len;
    size_t cap;

    string_t(): dat(NULL), len(0), cap(0) {}
    template<class T> string_t(T n): dat(NULL), len(0), cap(0)
    {
        reserve(n);
    }
    template<class T> string_t(T n, char c): dat(NULL), len(0), cap(0)
    {
        if (n)
        {
            reserve(n);
            for (size_t i = 0; i != n; i++) dat[i] = c;
            dat[len = n] = '\0';
        }
    }
    string_t(char);
    string_t(const char*);
    string_t(const char*, size_t);
    string_t(const string_t&);
    string_t(const string_t&, size_t bpos, size_t epos);

    ~string_t();

    const char* c_str() const { return dat; }
    size_t length() const { return len; }
    size_t capacity() const { return cap; }
    bool empty() const { return len == 0; }

    void clear();
    int cmp(const string_t&) const;

    void take(char* p, size_t l);
    void take(char* p, size_t l, size_t c);

    bool valid() const { return dat != NULL; }
    void release();
    void reserve(size_t);

    string_t& append(char, size_t);
    string_t& append(const char*);
    string_t& append(const char*, size_t);
    string_t& append(const string_t&);
    string_t& append(const string_t&, size_t bpos, size_t epos);

    string_t& prepend(char, size_t);
    string_t& prepend(const char*);
    string_t& prepend(const char*, size_t);
    string_t& prepend(const string_t&);
    string_t& prepend(const string_t&, size_t bpos, size_t epos);

    string_t& insert(size_t pos, char, size_t);
    string_t& insert(size_t pos, const char*);
    string_t& insert(size_t pos, const char*, size_t);
    string_t& insert(size_t pos, const string_t&);
    string_t& insert(size_t pos, const string_t&, size_t bpos, size_t epos);

    string_t& remove(size_t pos);
    string_t& remove(size_t bpos, size_t epos);
    string_t& remove_to_begin(size_t pos);
    string_t& remove_to_end(size_t pos);

    string_t& to_upper();
    string_t& to_lower();
    string_t& to_upper(string_t& res) const;
    string_t& to_lower(string_t& res) const;

    string_t& assign(char);
    string_t& assign(const char*);
    string_t& assign(const char*, size_t);
    string_t& assign(const string_t&);
    string_t& assign(const string_t&, size_t bpos, size_t epos);

    size_t pos_not_chars(size_t pos, const char*) const;
    size_t pos_not_blank(size_t pos) const;
    size_t pos_not_chars(const char*) const;
    size_t pos_not_blank() const;
    size_t pos_not_chars(const string_t&) const;

    size_t rpos_not_chars(size_t pos, const char*) const;
    size_t rpos_not_blank(size_t pos) const;
    size_t rpos_not_chars(const char*) const;
    size_t rpos_not_blank() const;
    size_t rpos_not_chars(const string_t&) const;

    string_t& strip_left(const char*);
    string_t& strip_left(const string_t&);
    string_t& strip_right(const char*);
    string_t& strip_right(const string_t&);
    string_t& strip_left();
    string_t& strip_right();
    string_t& strip();
    string_t& strip(const char*);
    string_t& strip(const string_t&);

    bool starts_with(size_t pos, const char*, bool ignorecase = false) const;
    bool starts_with(size_t pos, const char*, size_t, bool ignorecase = false) const;
    bool starts_with(size_t pos, const string_t&, bool ignorecase = false) const;
    bool starts_with(const char*, bool ignorecase = false) const;
    bool starts_with(const char*, size_t, bool ignorecase = false) const;
    bool starts_with(const string_t&, bool ignorecase = false) const;

    bool ends_with(size_t pos, const char*, bool ignorecase = false) const;
    bool ends_with(size_t pos, const char*, size_t, bool ignorecase = false) const;
    bool ends_with(size_t pos, const string_t&, bool ignorecase = false) const;
    bool ends_with(const char*, bool ignorecase = false) const;
    bool ends_with(const char*, size_t, bool ignorecase = false) const;
    bool ends_with(const string_t&, bool ignorecase = false) const;

    bool has(char c) const;
    void cut(size_t l);

    string_t& operator = (const char*);
    string_t& operator = (const string_t&);
    char operator [] (size_t x) const { return dat[x]; }
    char& operator [] (size_t x) { return dat[x]; }

    bool overlap(const char* p, size_t l);
};

int cmp(const string_t& a, const string_t& b);
int cmp(const string_t& a, const char* b);
int cmp(const char* a, const string_t& b);

int check(const char* p, int base);
int check(const char* p, size_t l, int base);
int check(const string_t& str, int base);
int check(const string_t& str, size_t bpos, size_t epos, int base);

typedef unsigned int format_flags_t;
const format_flags_t NO_FLAGS = 0;
const format_flags_t UPPER_CASE = 1 << 0;
const format_flags_t UPPER_LEADING = 1 << 1;
const format_flags_t SHOW_POS = 1 << 2;
const format_flags_t SHOW_LEADING = 1 << 3;
const format_flags_t SIGN_RIGHT_LEADING = 1 << 4;
const format_flags_t GROUP_COMPLETE = 1 << 5;
const format_flags_t GROUP_FROM_MSB = 1 << 6;
const format_flags_t ZERO_NO_LEADING = 1 << 7;
const format_flags_t ZERO_POS = 1 << 8;
const format_flags_t ZERO_NEG = 1 << 9;
const format_flags_t EMPTY_AS_ERROR = 1 << 16;
const format_flags_t MULTISIGN_AS_ERROR = 1 << 17;

struct _leadref_t
{
    const string_t* pstr;
    int base;
};

struct _leadings_t
{
    string_t* strs;
    _leadref_t* refs;

    _leadings_t();
    ~_leadings_t();

    const string_t& get(int base) const;
    void set(int base, const char* leading);
};

struct format_t
{
    static _leadings_t leadings;

    format_t(format_flags_t ff = 0):
        _flags(ff), _group(0), _groupinline(0), _filler('0')
    {}

    void set(format_flags_t ff);
    void clear(format_flags_t ff) { _flags &= ~ff; }

    bool has(format_flags_t ff) const { return (_flags & ff) > 0; }
    format_flags_t flags() const { return _flags; }

    size_t group_size() const { return _group; }
    size_t line_group_count() const { return _groupinline; }
    const string_t& group_separator() const { return _groupsep; }
    const string_t& line_separator() const { return _linesep; }
    char group_filler() const { return _filler; }

    void set_group_separator(const char* p) { _groupsep.assign(p); }
    void set_group_filler(char c) { _filler = c; }
    void set_group_size(size_t size) { _group = size; }

    void set_line_group_count(size_t cnt);
    void set_line_separator(const char* p);

    string_t& dump(const number_t& a, int _base, string_t& str) const;

protected:
    format_flags_t _flags;
    size_t _group;
    size_t _groupinline;
    string_t _groupsep;
    string_t _linesep;
    char _filler;

    const string_t* __append_group(string_t& str, const char* p, size_t len, size_t n) const;
};

const string_t NO_LEADING;
void reset_leading();
inline void set_leading(int base, const char* chars) { format_t::leadings.set(base, chars); }
inline const string_t& get_leading(int base) { return format_t::leadings.get(base); }

int load(number_t& a, const char* p, int base = 0, const format_t* format = NULL);
int load(number_t& a, const char* p, size_t l, int base = 0, const format_t* format = NULL);
int load(number_t& a, const string_t& str, int base = 0, const format_t* format = NULL);

struct bitref_t
{
    number_t& _ref;
    size_t _x;

    bitref_t(number_t& ref, size_t x): _ref(ref), _x(x) {}

    bool value() const
    {
        return _ref.bit_at(_x);
    }

    operator bool () const
    {
        return value();
    }

    bool operator ~ () const
    {
        return !value();
    }

    bool operator = (bool v)
    {
        _ref.bit_set(_x, v);
        return v;
    }

    bool operator = (const bitref_t& b)
    {
        bool v = b.value();
        _ref.bit_set(_x, v);
        return v;
    }

    bool operator |= (bool v)
    {
        bool u = value();
        _ref.bit_set(_x, u | v);
        return v;
    }

    bool operator |= (const bitref_t& b)
    {
        bool u = value();
        bool v = b.value();
        _ref.bit_set(_x, u | v);
        return v;
    }

    bool operator &= (bool v)
    {
        bool u = value();
        _ref.bit_set(_x, u & v);
        return v;
    }

    bool operator &= (const bitref_t& b)
    {
        bool u = value();
        bool v = b.value();
        _ref.bit_set(_x, u & v);
        return v;
    }

    bool operator ^= (bool v)
    {
        bool u = value();
        _ref.bit_set(_x, u ^ v);
        return v;
    }

    bool operator ^= (const bitref_t& b)
    {
        bool u = value();
        bool v = b.value();
        _ref.bit_set(_x, u ^ v);
        return v;
    }
};

/** inner functions */
void __mul(const unit_t* x, slen_t lx, const unit_t* y, slen_t ly, number_t& res);
void __kmul(const unit_t* x, slen_t lx, const unit_t* y, slen_t ly, number_t& res);
void __sqr(const unit_t* x, slen_t lx, number_t& res);
void __ksqr(const unit_t* x, slen_t lx, number_t& res);
void __div(const unit_t* a, slen_t la, const unit_t* b, slen_t lb, number_t& q, number_t& r);
void __div(const unit_t* a, slen_t la, const unit_t* b, slen_t lb, number_t& q);
bool __neq_core(const unit_t* x, const unit_t* y, slen_t l);
int __cmp_same_len_core(const unit_t* x, const unit_t* y, slen_t l);
int __cmp_core(const unit_t* x, slen_t lx, const unit_t* y, slen_t ly);
slen_t __add_core(const unit_t* x, slen_t lx, const unit_t* y, slen_t ly, unit_t* res);
slen_t __sub_core(const unit_t* x, slen_t lx, const unit_t* y, slen_t ly, unit_t* res);
slen_t __mul_core(const unit_t* x, slen_t lx, const unit_t* y, slen_t ly, unit_t* res);
slen_t __sqr_core(const unit_t* x, slen_t lx, unit_t* res);
unit_t __mul_unit_core(const unit_t* x, slen_t lx, unit_t y, unit_t* z);
unit_t __div_unit_core(const unit_t* x, slen_t lx, unit_t y, unit_t* q, slen_t* lq);
unit_t __div_unit_core(const unit_t* x, slen_t lx, const UDM& udm, unit_t* q, slen_t* lq);
unit_t __mod_unit_core(const unit_t* x, slen_t lx, unit_t y);
unit_t __mod_unit_core(const unit_t* x, slen_t lx, const UDM& udm);
unit_t __guess_quotient(unit_t x1, unit_t x2, unit_t x3, unit_t y1, unit_t y2);
unit_t __truing_quotient(unit_t* x, const unit_t* y, slen_t len, unit_t trial);
slen_t __div_core(unit_t* x, slen_t lx, const unit_t* y, slen_t ly, unit_t* q);
slen_t __shl_core(unit_t* x, slen_t lx, slen_t d);
unit_t __shr_core(unit_t* x, slen_t lx, slen_t d);
slen_t __bit_and_core(const unit_t* x, const unit_t* y, slen_t lx, unit_t* res);
slen_t __bit_or_core(const unit_t* x, slen_t lx, const unit_t* y, slen_t ly, unit_t* res);
slen_t __bit_xor_core(const unit_t* x, slen_t lx, const unit_t* y, slen_t ly, unit_t* res);
slen_t __bit_not_core(const unit_t* x, slen_t lx, unit_t* res);

}  //namespace end
