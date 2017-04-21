/* MYNUM LIBARAY HEADER */
#pragma once

#include "mynum.h"

namespace mynum {

inline bool operator == (const number_t& a, const number_t& b) { return eq(a, b); }
inline bool operator == (const number_t& a, bool b)  { return eq(a, int(b)); }
inline bool operator == (bool a, const number_t& b)  { return eq(int(a), b); }
inline bool operator == (const number_t& a, char b)  { return eq(a, int(b)); }
inline bool operator == (char a, const number_t& b)  { return eq(int(a), b); }
inline bool operator == (const number_t& a, short b) { return eq(a, int(b)); }
inline bool operator == (short a, const number_t& b) { return eq(int(a), b); }
inline bool operator == (const number_t& a, int b)  { return eq(a, b); }
inline bool operator == (int a, const number_t& b)  { return eq(a, b); }
inline bool operator == (const number_t& a, long b) { return eq(a, b); }
inline bool operator == (long a, const number_t& b) { return eq(a, b); }
inline bool operator == (const number_t& a, long long b) { return eq(a, b); }
inline bool operator == (long long a, const number_t& b) { return eq(a, b); }
inline bool operator == (const number_t& a, unsigned char b)  { return eq(a, (unsigned int)b); }
inline bool operator == (unsigned char a, const number_t& b)  { return eq((unsigned int)a, b); }
inline bool operator == (const number_t& a, unsigned short b)  { return eq(a, (unsigned int)b); }
inline bool operator == (unsigned short a, const number_t& b)  { return eq((unsigned int)a, b); }
inline bool operator == (const number_t& a, unsigned int b)  { return eq(a, b); }
inline bool operator == (unsigned int a, const number_t& b)  { return eq(a, b); }
inline bool operator == (const number_t& a, unsigned long b) { return eq(a, b); }
inline bool operator == (unsigned long a, const number_t& b) { return eq(a, b); }
inline bool operator == (const number_t& a, unsigned long long b) { return eq(a, b); }
inline bool operator == (unsigned long long a, const number_t& b) { return eq(a, b); }

inline bool operator != (const number_t& a, const number_t& b) { return neq(a, b); }
inline bool operator != (const number_t& a, bool b)  { return neq(a, int(b)); }
inline bool operator != (bool a, const number_t& b)  { return neq(int(a), b); }
inline bool operator != (const number_t& a, char b)  { return neq(a, int(b)); }
inline bool operator != (char a, const number_t& b)  { return neq(int(a), b); }
inline bool operator != (const number_t& a, short b) { return neq(a, int(b)); }
inline bool operator != (short a, const number_t& b) { return neq(int(a), b); }
inline bool operator != (const number_t& a, int b)  { return neq(a, b); }
inline bool operator != (int a, const number_t& b)  { return neq(a, b); }
inline bool operator != (const number_t& a, long b) { return neq(a, b); }
inline bool operator != (long a, const number_t& b) { return neq(a, b); }
inline bool operator != (const number_t& a, long long b) { return neq(a, b); }
inline bool operator != (long long a, const number_t& b) { return neq(a, b); }

inline bool operator != (const number_t& a, unsigned char b)  { return neq(a, (unsigned int)b); }
inline bool operator != (unsigned char a, const number_t& b)  { return neq((unsigned int)a, b); }
inline bool operator != (const number_t& a, unsigned short b)  { return neq(a, (unsigned int)b); }
inline bool operator != (unsigned short a, const number_t& b)  { return neq((unsigned int)a, b); }
inline bool operator != (const number_t& a, unsigned int b)  { return neq(a, b); }
inline bool operator != (unsigned int a, const number_t& b)  { return neq(a, b); }
inline bool operator != (const number_t& a, unsigned long b) { return neq(a, b); }
inline bool operator != (unsigned long a, const number_t& b) { return neq(a, b); }
inline bool operator != (const number_t& a, unsigned long long b) { return neq(a, b); }
inline bool operator != (unsigned long long a, const number_t& b) { return neq(a, b); }

inline bool operator > (const number_t& a, const number_t& b) { return cmp(a, b) > 0; }
inline bool operator > (const number_t& a, bool b)  { return cmp(a, int(b)) > 0; }
inline bool operator > (bool a, const number_t& b)  { return cmp(int(a), b) > 0; }
inline bool operator > (const number_t& a, char b)  { return cmp(a, int(b)) > 0; }
inline bool operator > (char a, const number_t& b)  { return cmp(int(a), b) > 0; }
inline bool operator > (const number_t& a, short b) { return cmp(a, int(b)) > 0; }
inline bool operator > (short a, const number_t& b) { return cmp(int(a), b) > 0; }
inline bool operator > (const number_t& a, int b)  { return cmp(a, b) > 0; }
inline bool operator > (int a, const number_t& b)  { return cmp(a, b) > 0; }
inline bool operator > (const number_t& a, long b) { return cmp(a, b) > 0; }
inline bool operator > (long a, const number_t& b) { return cmp(a, b) > 0; }
inline bool operator > (const number_t& a, long long b) { return cmp(a, b) > 0; }
inline bool operator > (long long a, const number_t& b) { return cmp(a, b) > 0; }
inline bool operator > (const number_t& a, unsigned char b)  { return cmp(a, (unsigned int)b) > 0; }
inline bool operator > (unsigned char a, const number_t& b)  { return cmp((unsigned int)a, b) > 0; }
inline bool operator > (const number_t& a, unsigned short b)  { return cmp(a, (unsigned int)b) > 0; }
inline bool operator > (unsigned short a, const number_t& b)  { return cmp((unsigned int)a, b) > 0; }
inline bool operator > (const number_t& a, unsigned int b)  { return cmp(a, b) > 0; }
inline bool operator > (unsigned int a, const number_t& b)  { return cmp(a, b) > 0; }
inline bool operator > (const number_t& a, unsigned long b) { return cmp(a, b) > 0; }
inline bool operator > (unsigned long a, const number_t& b) { return cmp(a, b) > 0; }
inline bool operator > (const number_t& a, unsigned long long b) { return cmp(a, b) > 0; }
inline bool operator > (unsigned long long a, const number_t& b) { return cmp(a, b) > 0; }

inline bool operator < (const number_t& a, const number_t& b) { return cmp(a, b) < 0; }
inline bool operator < (const number_t& a, bool b)  { return cmp(a, int(b)) < 0; }
inline bool operator < (bool a, const number_t& b)  { return cmp(int(a), b) < 0; }
inline bool operator < (const number_t& a, char b)  { return cmp(a, int(b)) < 0; }
inline bool operator < (char a, const number_t& b)  { return cmp(int(a), b) < 0; }
inline bool operator < (const number_t& a, short b) { return cmp(a, int(b)) < 0; }
inline bool operator < (short a, const number_t& b) { return cmp(int(a), b) < 0; }
inline bool operator < (const number_t& a, int b)  { return cmp(a, b) < 0; }
inline bool operator < (int a, const number_t& b)  { return cmp(a, b) < 0; }
inline bool operator < (const number_t& a, long b) { return cmp(a, b) < 0; }
inline bool operator < (long a, const number_t& b) { return cmp(a, b) < 0; }
inline bool operator < (const number_t& a, long long b) { return cmp(a, b) < 0; }
inline bool operator < (long long a, const number_t& b) { return cmp(a, b) < 0; }
inline bool operator < (const number_t& a, unsigned char b)  { return cmp(a, (unsigned int)b) < 0; }
inline bool operator < (unsigned char a, const number_t& b)  { return cmp((unsigned int)a, b) < 0; }
inline bool operator < (const number_t& a, unsigned short b)  { return cmp(a, (unsigned int)b) < 0; }
inline bool operator < (unsigned short a, const number_t& b)  { return cmp((unsigned int)a, b) < 0; }
inline bool operator < (const number_t& a, unsigned int b)  { return cmp(a, b) < 0; }
inline bool operator < (unsigned int a, const number_t& b)  { return cmp(a, b) < 0; }
inline bool operator < (const number_t& a, unsigned long b) { return cmp(a, b) < 0; }
inline bool operator < (unsigned long a, const number_t& b) { return cmp(a, b) < 0; }
inline bool operator < (const number_t& a, unsigned long long b) { return cmp(a, b) < 0; }
inline bool operator < (unsigned long long a, const number_t& b) { return cmp(a, b) < 0; }

inline bool operator >= (const number_t& a, const number_t& b) { return cmp(a, b) >= 0; }
inline bool operator >= (const number_t& a, bool b)  { return cmp(a, int(b)) >= 0; }
inline bool operator >= (bool a, const number_t& b)  { return cmp(int(a), b) >= 0; }
inline bool operator >= (const number_t& a, char b)  { return cmp(a, int(b)) >= 0; }
inline bool operator >= (char a, const number_t& b)  { return cmp(int(a), b) >= 0; }
inline bool operator >= (const number_t& a, short b) { return cmp(a, int(b)) >= 0; }
inline bool operator >= (short a, const number_t& b) { return cmp(int(a), b) >= 0; }
inline bool operator >= (const number_t& a, int b)  { return cmp(a, b) >= 0; }
inline bool operator >= (int a, const number_t& b)  { return cmp(a, b) >= 0; }
inline bool operator >= (const number_t& a, long b) { return cmp(a, b) >= 0; }
inline bool operator >= (long a, const number_t& b) { return cmp(a, b) >= 0; }
inline bool operator >= (const number_t& a, long long b) { return cmp(a, b) >= 0; }
inline bool operator >= (long long a, const number_t& b) { return cmp(a, b) >= 0; }
inline bool operator >= (const number_t& a, unsigned char b)  { return cmp(a, (unsigned int)b) >= 0; }
inline bool operator >= (unsigned char a, const number_t& b)  { return cmp((unsigned int)a, b) >= 0; }
inline bool operator >= (const number_t& a, unsigned short b)  { return cmp(a, (unsigned int)b) >= 0; }
inline bool operator >= (unsigned short a, const number_t& b)  { return cmp((unsigned int)a, b) >= 0; }
inline bool operator >= (const number_t& a, unsigned int b)  { return cmp(a, b) >= 0; }
inline bool operator >= (unsigned int a, const number_t& b)  { return cmp(a, b) >= 0; }
inline bool operator >= (const number_t& a, unsigned long b) { return cmp(a, b) >= 0; }
inline bool operator >= (unsigned long a, const number_t& b) { return cmp(a, b) >= 0; }
inline bool operator >= (const number_t& a, unsigned long long b) { return cmp(a, b) >= 0; }
inline bool operator >= (unsigned long long a, const number_t& b) { return cmp(a, b) >= 0; }

inline bool operator <= (const number_t& a, const number_t& b) { return cmp(a, b) <= 0; }
inline bool operator <= (const number_t& a, bool b)  { return cmp(a, int(b)) <= 0; }
inline bool operator <= (bool a, const number_t& b)  { return cmp(int(a), b) <= 0; }
inline bool operator <= (const number_t& a, char b)  { return cmp(a, int(b)) <= 0; }
inline bool operator <= (char a, const number_t& b)  { return cmp(int(a), b) <= 0; }
inline bool operator <= (const number_t& a, short b) { return cmp(a, int(b)) <= 0; }
inline bool operator <= (short a, const number_t& b) { return cmp(int(a), b) <= 0; }
inline bool operator <= (const number_t& a, int b)  { return cmp(a, b) <= 0; }
inline bool operator <= (int a, const number_t& b)  { return cmp(a, b) <= 0; }
inline bool operator <= (const number_t& a, long b) { return cmp(a, b) <= 0; }
inline bool operator <= (long a, const number_t& b) { return cmp(a, b) <= 0; }
inline bool operator <= (const number_t& a, long long b) { return cmp(a, b) <= 0; }
inline bool operator <= (long long a, const number_t& b) { return cmp(a, b) <= 0; }
inline bool operator <= (const number_t& a, unsigned char b)  { return cmp(a, (unsigned int)b) <= 0; }
inline bool operator <= (unsigned char a, const number_t& b)  { return cmp((unsigned int)a, b) <= 0; }
inline bool operator <= (const number_t& a, unsigned short b)  { return cmp(a, (unsigned int)b) <= 0; }
inline bool operator <= (unsigned short a, const number_t& b)  { return cmp((unsigned int)a, b) <= 0; }
inline bool operator <= (const number_t& a, unsigned int b)  { return cmp(a, b) <= 0; }
inline bool operator <= (unsigned int a, const number_t& b)  { return cmp(a, b) <= 0; }
inline bool operator <= (const number_t& a, unsigned long b) { return cmp(a, b) <= 0; }
inline bool operator <= (unsigned long a, const number_t& b) { return cmp(a, b) <= 0; }
inline bool operator <= (const number_t& a, unsigned long long b) { return cmp(a, b) <= 0; }
inline bool operator <= (unsigned long long a, const number_t& b) { return cmp(a, b) <= 0; }

inline number_t operator + (const number_t& a, const number_t& b) { number_t c; add(a, b, c); return c; }
inline number_t operator + (const number_t& a, bool b) { number_t c; add(a, int(b), c); return c; }
inline number_t operator + (bool a, const number_t& b) { number_t c; add(int(a), b, c); return c; }
inline number_t operator + (const number_t& a, char b) { number_t c; add(a, int(b), c); return c; }
inline number_t operator + (char a, const number_t& b) { number_t c; add(int(a), b, c); return c; }
inline number_t operator + (const number_t& a, short b) { number_t c; add(a, int(b), c); return c; }
inline number_t operator + (short a, const number_t& b) { number_t c; add(int(a), b, c); return c; }
inline number_t operator + (const number_t& a, int b) { number_t c; add(a, b, c); return c; }
inline number_t operator + (int a, const number_t& b) { number_t c; add(a, b, c); return c; }
inline number_t operator + (const number_t& a, long b) { number_t c; add(a, b, c); return c; }
inline number_t operator + (long a, const number_t& b) { number_t c; add(a, b, c); return c; }
inline number_t operator + (const number_t& a, long long b) { number_t c; add(a, b, c); return c; }
inline number_t operator + (long long a, const number_t& b) { number_t c; add(a, b, c); return c; }
inline number_t operator + (const number_t& a, unsigned char b) { number_t c; add(a, (unsigned int)b, c); return c; }
inline number_t operator + (unsigned char a, const number_t& b) { number_t c; add((unsigned int)a, b, c); return c; }
inline number_t operator + (const number_t& a, unsigned short b) { number_t c; add(a, (unsigned int)b, c); return c; }
inline number_t operator + (unsigned short a, const number_t& b) { number_t c; add((unsigned int)a, b, c); return c; }
inline number_t operator + (const number_t& a, unsigned int b) { number_t c; add(a, b, c); return c; }
inline number_t operator + (unsigned int a, const number_t& b) { number_t c; add(a, b, c); return c; }
inline number_t operator + (const number_t& a, unsigned long b) { number_t c; add(a, b, c); return c; }
inline number_t operator + (unsigned long a, const number_t& b) { number_t c; add(a, b, c); return c; }
inline number_t operator + (const number_t& a, unsigned long long b) { number_t c; add(a, b, c); return c; }
inline number_t operator + (unsigned long long a, const number_t& b) { number_t c; add(a, b, c); return c; }

inline number_t operator - (const number_t& a, const number_t& b) { number_t c; sub(a, b, c); return c; }
inline number_t operator - (const number_t& a, bool b) { number_t c; sub(a, int(b), c); return c; }
inline number_t operator - (bool a, const number_t& b) { number_t c; sub(int(a), b, c); return c; }
inline number_t operator - (const number_t& a, char b) { number_t c; sub(a, int(b), c); return c; }
inline number_t operator - (char a, const number_t& b) { number_t c; sub(int(a), b, c); return c; }
inline number_t operator - (const number_t& a, short b) { number_t c; sub(a, int(b), c); return c; }
inline number_t operator - (short a, const number_t& b) { number_t c; sub(int(a), b, c); return c; }
inline number_t operator - (const number_t& a, int b) { number_t c; sub(a, b, c); return c; }
inline number_t operator - (int a, const number_t& b) { number_t c; sub(a, b, c); return c; }
inline number_t operator - (const number_t& a, long b) { number_t c; sub(a, b, c); return c; }
inline number_t operator - (long a, const number_t& b) { number_t c; sub(a, b, c); return c; }
inline number_t operator - (const number_t& a, long long b) { number_t c; sub(a, b, c); return c; }
inline number_t operator - (long long a, const number_t& b) { number_t c; sub(a, b, c); return c; }
inline number_t operator - (const number_t& a, unsigned char b) { number_t c; sub(a, (unsigned int)b, c); return c; }
inline number_t operator - (unsigned char a, const number_t& b) { number_t c; sub((unsigned int)a, b, c); return c; }
inline number_t operator - (const number_t& a, unsigned short b) { number_t c; sub(a, (unsigned int)b, c); return c; }
inline number_t operator - (unsigned short a, const number_t& b) { number_t c; sub((unsigned int)a, b, c); return c; }
inline number_t operator - (const number_t& a, unsigned int b) { number_t c; sub(a, b, c); return c; }
inline number_t operator - (unsigned int a, const number_t& b) { number_t c; sub(a, b, c); return c; }
inline number_t operator - (const number_t& a, unsigned long b) { number_t c; sub(a, b, c); return c; }
inline number_t operator - (unsigned long a, const number_t& b) { number_t c; sub(a, b, c); return c; }
inline number_t operator - (const number_t& a, unsigned long long b) { number_t c; sub(a, b, c); return c; }
inline number_t operator - (unsigned long long a, const number_t& b) { number_t c; sub(a, b, c); return c; }

inline number_t operator * (const number_t& a, const number_t& b) { number_t c; kmul(a, b, c); return c; }
inline number_t operator * (const number_t& a, bool b) { number_t c; kmul(a, int(b), c); return c; }
inline number_t operator * (bool a, const number_t& b) { number_t c; kmul(int(a), b, c); return c; }
inline number_t operator * (const number_t& a, char b) { number_t c; kmul(a, int(b), c); return c; }
inline number_t operator * (char a, const number_t& b) { number_t c; kmul(int(a), b, c); return c; }
inline number_t operator * (const number_t& a, short b) { number_t c; kmul(a, int(b), c); return c; }
inline number_t operator * (short a, const number_t& b) { number_t c; kmul(int(a), b, c); return c; }
inline number_t operator * (const number_t& a, int b) { number_t c; kmul(a, b, c); return c; }
inline number_t operator * (int a, const number_t& b) { number_t c; kmul(a, b, c); return c; }
inline number_t operator * (const number_t& a, long b) { number_t c; kmul(a, b, c); return c; }
inline number_t operator * (long a, const number_t& b) { number_t c; kmul(a, b, c); return c; }
inline number_t operator * (const number_t& a, long long b) { number_t c; kmul(a, b, c); return c; }
inline number_t operator * (long long a, const number_t& b) { number_t c; kmul(a, b, c); return c; }
inline number_t operator * (const number_t& a, unsigned char b) { number_t c; kmul(a, (unsigned int)b, c); return c; }
inline number_t operator * (unsigned char a, const number_t& b) { number_t c; kmul((unsigned int)a, b, c); return c; }
inline number_t operator * (const number_t& a, unsigned short b) { number_t c; kmul(a, (unsigned int)b, c); return c; }
inline number_t operator * (unsigned short a, const number_t& b) { number_t c; kmul((unsigned int)a, b, c); return c; }
inline number_t operator * (const number_t& a, unsigned int b) { number_t c; kmul(a, b, c); return c; }
inline number_t operator * (unsigned int a, const number_t& b) { number_t c; kmul(a, b, c); return c; }
inline number_t operator * (const number_t& a, unsigned long b) { number_t c; kmul(a, b, c); return c; }
inline number_t operator * (unsigned long a, const number_t& b) { number_t c; kmul(a, b, c); return c; }
inline number_t operator * (const number_t& a, unsigned long long b) { number_t c; kmul(a, b, c); return c; }
inline number_t operator * (unsigned long long a, const number_t& b) { number_t c; kmul(a, b, c); return c; }

inline number_t operator / (const number_t& a, const number_t& b) { number_t c; div(a, b, c); return c; }
inline number_t operator / (const number_t& a, bool b) { number_t c; div(a, int(b), c); return c; }
inline number_t operator / (bool a, const number_t& b) { number_t c; div(int(a), b, c); return c; }
inline number_t operator / (const number_t& a, char b) { number_t c; div(a, int(b), c); return c; }
inline number_t operator / (char a, const number_t& b) { number_t c; div(int(a), b, c); return c; }
inline number_t operator / (const number_t& a, short b) { number_t c; div(a, int(b), c); return c; }
inline number_t operator / (short a, const number_t& b) { number_t c; div(int(a), b, c); return c; }
inline number_t operator / (const number_t& a, int b) { number_t c; div(a, b, c); return c; }
inline number_t operator / (int a, const number_t& b) { number_t c; div(a, b, c); return c; }
inline number_t operator / (const number_t& a, long b) { number_t c; div(a, b, c); return c; }
inline number_t operator / (long a, const number_t& b) { number_t c; div(a, b, c); return c; }
inline number_t operator / (const number_t& a, long long b) { number_t c; div(a, b, c); return c; }
inline number_t operator / (long long a, const number_t& b) { number_t c; div(a, b, c); return c; }
inline number_t operator / (const number_t& a, unsigned char b) { number_t c; div(a, (unsigned int)b, c); return c; }
inline number_t operator / (unsigned char a, const number_t& b) { number_t c; div((unsigned int)a, b, c); return c; }
inline number_t operator / (const number_t& a, unsigned short b) { number_t c; div(a, (unsigned int)b, c); return c; }
inline number_t operator / (unsigned short a, const number_t& b) { number_t c; div((unsigned int)a, b, c); return c; }
inline number_t operator / (const number_t& a, unsigned int b) { number_t c; div(a, b, c); return c; }
inline number_t operator / (unsigned int a, const number_t& b) { number_t c; div(a, b, c); return c; }
inline number_t operator / (const number_t& a, unsigned long b) { number_t c; div(a, b, c); return c; }
inline number_t operator / (unsigned long a, const number_t& b) { number_t c; div(a, b, c); return c; }
inline number_t operator / (const number_t& a, unsigned long long b) { number_t c; div(a, b, c); return c; }
inline number_t operator / (unsigned long long a, const number_t& b) { number_t c; div(a, b, c); return c; }

inline number_t operator % (const number_t& a, const number_t& b) { number_t c; mod(a, b, c); return c; }
inline number_t operator % (const number_t& a, bool b) { number_t c; mod(a, int(b), c); return c; }
inline number_t operator % (bool a, const number_t& b) { number_t c; mod(int(a), b, c); return c; }
inline number_t operator % (const number_t& a, char b) { number_t c; mod(a, int(b), c); return c; }
inline number_t operator % (char a, const number_t& b) { number_t c; mod(int(a), b, c); return c; }
inline number_t operator % (const number_t& a, short b) { number_t c; mod(a, int(b), c); return c; }
inline number_t operator % (short a, const number_t& b) { number_t c; mod(int(a), b, c); return c; }
inline number_t operator % (const number_t& a, int b) { number_t c; mod(a, b, c); return c; }
inline number_t operator % (int a, const number_t& b) { number_t c; mod(a, b, c); return c; }
inline number_t operator % (const number_t& a, long b) { number_t c; mod(a, b, c); return c; }
inline number_t operator % (long a, const number_t& b) { number_t c; mod(a, b, c); return c; }
inline number_t operator % (const number_t& a, long long b) { number_t c; mod(a, b, c); return c; }
inline number_t operator % (long long a, const number_t& b) { number_t c; mod(a, b, c); return c; }
inline number_t operator % (const number_t& a, unsigned char b) { number_t c; mod(a, (unsigned int)b, c); return c; }
inline number_t operator % (unsigned char a, const number_t& b) { number_t c; mod((unsigned int)a, b, c); return c; }
inline number_t operator % (const number_t& a, unsigned short b) { number_t c; mod(a, (unsigned int)b, c); return c; }
inline number_t operator % (unsigned short a, const number_t& b) { number_t c; mod((unsigned int)a, b, c); return c; }
inline number_t operator % (const number_t& a, unsigned int b) { number_t c; mod(a, b, c); return c; }
inline number_t operator % (unsigned int a, const number_t& b) { number_t c; mod(a, b, c); return c; }
inline number_t operator % (const number_t& a, unsigned long b) { number_t c; mod(a, b, c); return c; }
inline number_t operator % (unsigned long a, const number_t& b) { number_t c; mod(a, b, c); return c; }
inline number_t operator % (const number_t& a, unsigned long long b) { number_t c; mod(a, b, c); return c; }
inline number_t operator % (unsigned long long a, const number_t& b) { number_t c; mod(a, b, c); return c; }

inline number_t operator & (const number_t& a, const number_t& b) { number_t c; bit_and(a, b, c); return c; }
inline number_t operator & (const number_t& a, bool b) { number_t c; bit_and(a, int(b), c); return c; }
inline number_t operator & (bool a, const number_t& b) { number_t c; bit_and(int(a), b, c); return c; }
inline number_t operator & (const number_t& a, char b) { number_t c; bit_and(a, int(b), c); return c; }
inline number_t operator & (char a, const number_t& b) { number_t c; bit_and(int(a), b, c); return c; }
inline number_t operator & (const number_t& a, short b) { number_t c; bit_and(a, int(b), c); return c; }
inline number_t operator & (short a, const number_t& b) { number_t c; bit_and(int(a), b, c); return c; }
inline number_t operator & (const number_t& a, int b) { number_t c; bit_and(a, b, c); return c; }
inline number_t operator & (int a, const number_t& b) { number_t c; bit_and(a, b, c); return c; }
inline number_t operator & (const number_t& a, long b) { number_t c; bit_and(a, b, c); return c; }
inline number_t operator & (long a, const number_t& b) { number_t c; bit_and(a, b, c); return c; }
inline number_t operator & (const number_t& a, long long b) { number_t c; bit_and(a, b, c); return c; }
inline number_t operator & (long long a, const number_t& b) { number_t c; bit_and(a, b, c); return c; }
inline number_t operator & (const number_t& a, unsigned char b) { number_t c; bit_and(a, (unsigned int)b, c); return c; }
inline number_t operator & (unsigned char a, const number_t& b) { number_t c; bit_and((unsigned int)a, b, c); return c; }
inline number_t operator & (const number_t& a, unsigned short b) { number_t c; bit_and(a, (unsigned int)b, c); return c; }
inline number_t operator & (unsigned short a, const number_t& b) { number_t c; bit_and((unsigned int)a, b, c); return c; }
inline number_t operator & (const number_t& a, unsigned int b) { number_t c; bit_and(a, b, c); return c; }
inline number_t operator & (unsigned int a, const number_t& b) { number_t c; bit_and(a, b, c); return c; }
inline number_t operator & (const number_t& a, unsigned long b) { number_t c; bit_and(a, b, c); return c; }
inline number_t operator & (unsigned long a, const number_t& b) { number_t c; bit_and(a, b, c); return c; }
inline number_t operator & (const number_t& a, unsigned long long b) { number_t c; bit_and(a, b, c); return c; }
inline number_t operator & (unsigned long long a, const number_t& b) { number_t c; bit_and(a, b, c); return c; }

inline number_t operator | (const number_t& a, const number_t& b) { number_t c; bit_or(a, b, c); return c; }
inline number_t operator | (const number_t& a, bool b) { number_t c; bit_or(a, int(b), c); return c; }
inline number_t operator | (bool a, const number_t& b) { number_t c; bit_or(int(a), b, c); return c; }
inline number_t operator | (const number_t& a, char b) { number_t c; bit_or(a, int(b), c); return c; }
inline number_t operator | (char a, const number_t& b) { number_t c; bit_or(int(a), b, c); return c; }
inline number_t operator | (const number_t& a, short b) { number_t c; bit_or(a, int(b), c); return c; }
inline number_t operator | (short a, const number_t& b) { number_t c; bit_or(int(a), b, c); return c; }
inline number_t operator | (const number_t& a, int b) { number_t c; bit_or(a, b, c); return c; }
inline number_t operator | (int a, const number_t& b) { number_t c; bit_or(a, b, c); return c; }
inline number_t operator | (const number_t& a, long b) { number_t c; bit_or(a, b, c); return c; }
inline number_t operator | (long a, const number_t& b) { number_t c; bit_or(a, b, c); return c; }
inline number_t operator | (const number_t& a, long long b) { number_t c; bit_or(a, b, c); return c; }
inline number_t operator | (long long a, const number_t& b) { number_t c; bit_or(a, b, c); return c; }
inline number_t operator | (const number_t& a, unsigned char b) { number_t c; bit_or(a, (unsigned int)b, c); return c; }
inline number_t operator | (unsigned char a, const number_t& b) { number_t c; bit_or((unsigned int)a, b, c); return c; }
inline number_t operator | (const number_t& a, unsigned short b) { number_t c; bit_or(a, (unsigned int)b, c); return c; }
inline number_t operator | (unsigned short a, const number_t& b) { number_t c; bit_or((unsigned int)a, b, c); return c; }
inline number_t operator | (const number_t& a, unsigned int b) { number_t c; bit_or(a, b, c); return c; }
inline number_t operator | (unsigned int a, const number_t& b) { number_t c; bit_or(a, b, c); return c; }
inline number_t operator | (const number_t& a, unsigned long b) { number_t c; bit_or(a, b, c); return c; }
inline number_t operator | (unsigned long a, const number_t& b) { number_t c; bit_or(a, b, c); return c; }
inline number_t operator | (const number_t& a, unsigned long long b) { number_t c; bit_or(a, b, c); return c; }
inline number_t operator | (unsigned long long a, const number_t& b) { number_t c; bit_or(a, b, c); return c; }

inline number_t operator ^ (const number_t& a, const number_t& b) { number_t c; bit_xor(a, b, c); return c; }
inline number_t operator ^ (const number_t& a, bool b) { number_t c; bit_xor(a, int(b), c); return c; }
inline number_t operator ^ (bool a, const number_t& b) { number_t c; bit_xor(int(a), b, c); return c; }
inline number_t operator ^ (const number_t& a, char b) { number_t c; bit_xor(a, int(b), c); return c; }
inline number_t operator ^ (char a, const number_t& b) { number_t c; bit_xor(int(a), b, c); return c; }
inline number_t operator ^ (const number_t& a, short b) { number_t c; bit_xor(a, int(b), c); return c; }
inline number_t operator ^ (short a, const number_t& b) { number_t c; bit_xor(int(a), b, c); return c; }
inline number_t operator ^ (const number_t& a, int b) { number_t c; bit_xor(a, b, c); return c; }
inline number_t operator ^ (int a, const number_t& b) { number_t c; bit_xor(a, b, c); return c; }
inline number_t operator ^ (const number_t& a, long b) { number_t c; bit_xor(a, b, c); return c; }
inline number_t operator ^ (long a, const number_t& b) { number_t c; bit_xor(a, b, c); return c; }
inline number_t operator ^ (const number_t& a, long long b) { number_t c; bit_xor(a, b, c); return c; }
inline number_t operator ^ (long long a, const number_t& b) { number_t c; bit_xor(a, b, c); return c; }
inline number_t operator ^ (const number_t& a, unsigned char b) { number_t c; bit_xor(a, (unsigned int)b, c); return c; }
inline number_t operator ^ (unsigned char a, const number_t& b) { number_t c; bit_xor((unsigned int)a, b, c); return c; }
inline number_t operator ^ (const number_t& a, unsigned short b) { number_t c; bit_xor(a, (unsigned int)b, c); return c; }
inline number_t operator ^ (unsigned short a, const number_t& b) { number_t c; bit_xor((unsigned int)a, b, c); return c; }
inline number_t operator ^ (const number_t& a, unsigned int b) { number_t c; bit_xor(a, b, c); return c; }
inline number_t operator ^ (unsigned int a, const number_t& b) { number_t c; bit_xor(a, b, c); return c; }
inline number_t operator ^ (const number_t& a, unsigned long b) { number_t c; bit_xor(a, b, c); return c; }
inline number_t operator ^ (unsigned long a, const number_t& b) { number_t c; bit_xor(a, b, c); return c; }
inline number_t operator ^ (const number_t& a, unsigned long long b) { number_t c; bit_xor(a, b, c); return c; }
inline number_t operator ^ (unsigned long long a, const number_t& b) { number_t c; bit_xor(a, b, c); return c; }

inline number_t operator << (const number_t& a, int b) { number_t c; if (b >= 0) shl(a, b, c); else shr(a, -b, c); return c; }
inline number_t operator >> (const number_t& a, int b) { number_t c; if (b >= 0) shr(a, b, c); else shl(a, -b, c); return c; }
inline number_t operator << (const number_t& a, long b) { number_t c; if (b >= 0) shl(a, b, c); else shr(a, -b, c); return c; }
inline number_t operator >> (const number_t& a, long b) { number_t c; if (b >= 0) shr(a, b, c); else shl(a, -b, c); return c; }
inline number_t operator << (const number_t& a, unsigned int b) { number_t c; shl(a, b, c); return c; }
inline number_t operator >> (const number_t& a, unsigned int b) { number_t c; shr(a, b, c); return c; }
inline number_t operator << (const number_t& a, unsigned long b) { number_t c; shl(a, b, c); return c; }
inline number_t operator >> (const number_t& a, unsigned long b) { number_t c; shr(a, b, c); return c; }
#if UNITBITS == 32
inline number_t operator << (const number_t& a, long long b) { number_t c; if (b >= 0) shl(a, b, c); else shr(a, -b, c); return c; }
inline number_t operator >> (const number_t& a, long long b) { number_t c; if (b >= 0) shr(a, b, c); else shl(a, -b, c); return c; }
inline number_t operator << (const number_t& a, unsigned long long b) { number_t c; shl(a, b, c); return c; }
inline number_t operator >> (const number_t& a, unsigned long long b) { number_t c; shr(a, b, c); return c; }
#endif

inline bool operator == (const string_t& a, const string_t& b) { return cmp(a, b) == 0; }
inline bool operator != (const string_t& a, const string_t& b) { return cmp(a, b) != 0; }
inline bool operator >  (const string_t& a, const string_t& b) { return cmp(a, b) > 0;  }
inline bool operator <  (const string_t& a, const string_t& b) { return cmp(a, b) < 0;  }
inline bool operator >= (const string_t& a, const string_t& b) { return cmp(a, b) >= 0; }
inline bool operator <= (const string_t& a, const string_t& b) { return cmp(a, b) <= 0; }
inline bool operator == (const string_t& a, const char* b) { return cmp(a, b) == 0; }
inline bool operator != (const string_t& a, const char* b) { return cmp(a, b) != 0; }
inline bool operator >  (const string_t& a, const char* b) { return cmp(a, b) > 0;  }
inline bool operator <  (const string_t& a, const char* b) { return cmp(a, b) < 0;  }
inline bool operator >= (const string_t& a, const char* b) { return cmp(a, b) >= 0; }
inline bool operator <= (const string_t& a, const char* b) { return cmp(a, b) <= 0; }
inline bool operator == (const char* a, const string_t& b) { return cmp(a, b) == 0; }
inline bool operator != (const char* a, const string_t& b) { return cmp(a, b) != 0; }
inline bool operator >  (const char* a, const string_t& b) { return cmp(a, b) > 0;  }
inline bool operator <  (const char* a, const string_t& b) { return cmp(a, b) < 0;  }
inline bool operator >= (const char* a, const string_t& b) { return cmp(a, b) >= 0; }
inline bool operator <= (const char* a, const string_t& b) { return cmp(a, b) <= 0; }

inline bool operator == (const bitref_t& b, bool x) { return b.value() == x; }
inline bool operator != (const bitref_t& b, bool x) { return b.value() != x; }
inline bool operator == (const bitref_t& b, int x)  { return (int)b.value() == x; }
inline bool operator != (const bitref_t& b, int x)  { return (int)b.value() != x; }

inline bool operator == (bool x, const bitref_t& b) { return x == b.value(); }
inline bool operator != (bool x, const bitref_t& b) { return x != b.value(); }
inline bool operator == (int x, const bitref_t& b)  { return x == (int)b.value(); }
inline bool operator != (int x, const bitref_t& b)  { return x != (int)b.value(); }

} // namespace end

#ifndef NO_STL_SUPPORT

#include <string>
#include <iostream>


namespace mynum {

inline std::ostream& operator << (std::ostream& os, const number_t& a)
{
    string_t s;
    format_t fmt;
    int base = 10;

    std::ostream::fmtflags ff = os.flags();
    if (ff & std::ostream::oct)
    {
        base = 8;
    }
    else if (ff & std::ostream::hex)
    {
        base = 16;
    }
    if (ff & std::ostream::showbase)
    {
        fmt.set(SHOW_LEADING);
    }
    if (ff & std::ostream::uppercase)
    {
        fmt.set(UPPER_CASE);
    }
    if (ff & std::ostream::showpos)
    {
        fmt.set(SHOW_POS);
    }
    return os << fmt.dump(a, base, s).c_str();
}

inline std::istream& operator >> (std::istream& is, number_t& a)
{
    std::string tmp;
    is >> tmp;
    if (is.good())
    {
        int base = 10;
        if (is.flags() & std::istream::oct)
        {
            base = 8;
        }
        else if (is.flags() & std::istream::hex)
        {
            base = 16;
        }
        if (!load(a, tmp.c_str(), tmp.length(), base))
        {
            a.set_zero();
            is.setstate(std::ios::failbit);
        }
    }
    return is;
}

inline std::ostream& operator << (std::ostream& os, const string_t& s)
{
    if (os.flags() & std::ostream::uppercase)
    {
        for (size_t i = 0; i < s.length(); i++)
        {
            os.put(toupper(s[i]));
        }
    }
    else if (s.valid())
    {
        os << s.c_str();
    }
    return os;
}

inline std::ostream& operator << (std::ostream& os, const bitref_t& b)
{
    return os << (bool)b;
}

}  // namespace end

#endif