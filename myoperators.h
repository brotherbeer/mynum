/* MYNUM LIBARAY HEADER FOR OPERATORS*/
#pragma once

#include "mynum.h"

namespace mynum {

inline bool operator == (const number_t& a, const number_t& b) { return cmp(a, b) == 0; }
inline bool operator == (const number_t& a, bool b)  { return cmp(a, number_t(b)) == 0; }
inline bool operator == (bool a, const number_t& b)  { return cmp(number_t(a), b) == 0; }
inline bool operator == (const number_t& a, char b)  { return cmp(a, number_t(b)) == 0; }
inline bool operator == (char a, const number_t& b)  { return cmp(number_t(a), b) == 0; }
inline bool operator == (const number_t& a, short b) { return cmp(a, number_t(b)) == 0; }
inline bool operator == (short a, const number_t& b) { return cmp(number_t(a), b) == 0; }
inline bool operator == (const number_t& a, int b)  { return cmp(a, number_t(b)) == 0; }
inline bool operator == (int a, const number_t& b)  { return cmp(number_t(a), b) == 0; }
inline bool operator == (const number_t& a, long b) { return cmp(a, number_t(b)) == 0; }
inline bool operator == (long a, const number_t& b) { return cmp(number_t(a), b) == 0; }
inline bool operator == (const number_t& a, long long b) { return cmp(a, number_t(b)) == 0; }
inline bool operator == (long long a, const number_t& b) { return cmp(number_t(a), b) == 0; }
inline bool operator == (const number_t& a, unsigned char b)  { return cmp(a, number_t(b)) == 0; }
inline bool operator == (unsigned char a, const number_t& b)  { return cmp(number_t(a), b) == 0; }
inline bool operator == (const number_t& a, unsigned short b)  { return cmp(a, number_t(b)) == 0; }
inline bool operator == (unsigned short a, const number_t& b)  { return cmp(number_t(a), b) == 0; }
inline bool operator == (const number_t& a, unsigned int b)  { return cmp(a, number_t(b)) == 0; }
inline bool operator == (unsigned int a, const number_t& b)  { return cmp(number_t(a), b) == 0; }
inline bool operator == (const number_t& a, unsigned long b) { return cmp(a, number_t(b)) == 0; }
inline bool operator == (unsigned long a, const number_t& b) { return cmp(number_t(a), b) == 0; }
inline bool operator == (const number_t& a, unsigned long long b) { return cmp(a, number_t(b)) == 0; }
inline bool operator == (unsigned long long a, const number_t& b) { return cmp(number_t(a), b) == 0; }

inline bool operator != (const number_t& a, const number_t& b) { return cmp(a, b) != 0; }
inline bool operator != (const number_t& a, bool b)  { return cmp(a, number_t(b)) != 0; }
inline bool operator != (bool a, const number_t& b)  { return cmp(number_t(a), b) != 0; }
inline bool operator != (const number_t& a, char b)  { return cmp(a, number_t(b)) != 0; }
inline bool operator != (char a, const number_t& b)  { return cmp(number_t(a), b) != 0; }
inline bool operator != (const number_t& a, short b) { return cmp(a, number_t(b)) != 0; }
inline bool operator != (short a, const number_t& b) { return cmp(number_t(a), b) != 0; }
inline bool operator != (const number_t& a, int b)  { return cmp(a, number_t(b)) != 0; }
inline bool operator != (int a, const number_t& b)  { return cmp(number_t(a), b) != 0; }
inline bool operator != (const number_t& a, long b) { return cmp(a, number_t(b)) != 0; }
inline bool operator != (long a, const number_t& b) { return cmp(number_t(a), b) != 0; }
inline bool operator != (const number_t& a, long long b) { return cmp(a, number_t(b)) != 0; }
inline bool operator != (long long a, const number_t& b) { return cmp(number_t(a), b) != 0; }
inline bool operator != (const number_t& a, unsigned char b)  { return cmp(a, number_t(b)) != 0; }
inline bool operator != (unsigned char a, const number_t& b)  { return cmp(number_t(a), b) != 0; }
inline bool operator != (const number_t& a, unsigned short b)  { return cmp(a, number_t(b)) != 0; }
inline bool operator != (unsigned short a, const number_t& b)  { return cmp(number_t(a), b) != 0; }
inline bool operator != (const number_t& a, unsigned int b)  { return cmp(a, number_t(b)) != 0; }
inline bool operator != (unsigned int a, const number_t& b)  { return cmp(number_t(a), b) != 0; }
inline bool operator != (const number_t& a, unsigned long b) { return cmp(a, number_t(b)) != 0; }
inline bool operator != (unsigned long a, const number_t& b) { return cmp(number_t(a), b) != 0; }
inline bool operator != (const number_t& a, unsigned long long b) { return cmp(a, number_t(b)) != 0; }
inline bool operator != (unsigned long long a, const number_t& b) { return cmp(number_t(a), b) != 0; }

inline bool operator > (const number_t& a, const number_t& b) { return cmp(a, b) > 0; }
inline bool operator > (const number_t& a, bool b)  { return cmp(a, number_t(b)) > 0; }
inline bool operator > (bool a, const number_t& b)  { return cmp(number_t(a), b) > 0; }
inline bool operator > (const number_t& a, char b)  { return cmp(a, number_t(b)) > 0; }
inline bool operator > (char a, const number_t& b)  { return cmp(number_t(a), b) > 0; }
inline bool operator > (const number_t& a, short b) { return cmp(a, number_t(b)) > 0; }
inline bool operator > (short a, const number_t& b) { return cmp(number_t(a), b) > 0; }
inline bool operator > (const number_t& a, int b)  { return cmp(a, number_t(b)) > 0; }
inline bool operator > (int a, const number_t& b)  { return cmp(number_t(a), b) > 0; }
inline bool operator > (const number_t& a, long b) { return cmp(a, number_t(b)) > 0; }
inline bool operator > (long a, const number_t& b) { return cmp(number_t(a), b) > 0; }
inline bool operator > (const number_t& a, long long b) { return cmp(a, number_t(b)) > 0; }
inline bool operator > (long long a, const number_t& b) { return cmp(number_t(a), b) > 0; }
inline bool operator > (const number_t& a, unsigned char b)  { return cmp(a, number_t(b)) > 0; }
inline bool operator > (unsigned char a, const number_t& b)  { return cmp(number_t(a), b) > 0; }
inline bool operator > (const number_t& a, unsigned short b)  { return cmp(a, number_t(b)) > 0; }
inline bool operator > (unsigned short a, const number_t& b)  { return cmp(number_t(a), b) > 0; }
inline bool operator > (const number_t& a, unsigned int b)  { return cmp(a, number_t(b)) > 0; }
inline bool operator > (unsigned int a, const number_t& b)  { return cmp(number_t(a), b) > 0; }
inline bool operator > (const number_t& a, unsigned long b) { return cmp(a, number_t(b)) > 0; }
inline bool operator > (unsigned long a, const number_t& b) { return cmp(number_t(a), b) > 0; }
inline bool operator > (const number_t& a, unsigned long long b) { return cmp(a, number_t(b)) > 0; }
inline bool operator > (unsigned long long a, const number_t& b) { return cmp(number_t(a), b) > 0; }

inline bool operator < (const number_t& a, const number_t& b) { return cmp(a, b) < 0; }
inline bool operator < (const number_t& a, bool b)  { return cmp(a, number_t(b)) < 0; }
inline bool operator < (bool a, const number_t& b)  { return cmp(number_t(a), b) < 0; }
inline bool operator < (const number_t& a, char b)  { return cmp(a, number_t(b)) < 0; }
inline bool operator < (char a, const number_t& b)  { return cmp(number_t(a), b) < 0; }
inline bool operator < (const number_t& a, short b) { return cmp(a, number_t(b)) < 0; }
inline bool operator < (short a, const number_t& b) { return cmp(number_t(a), b) < 0; }
inline bool operator < (const number_t& a, int b)  { return cmp(a, number_t(b)) < 0; }
inline bool operator < (int a, const number_t& b)  { return cmp(number_t(a), b) < 0; }
inline bool operator < (const number_t& a, long b) { return cmp(a, number_t(b)) < 0; }
inline bool operator < (long a, const number_t& b) { return cmp(number_t(a), b) < 0; }
inline bool operator < (const number_t& a, long long b) { return cmp(a, number_t(b)) < 0; }
inline bool operator < (long long a, const number_t& b) { return cmp(number_t(a), b) < 0; }
inline bool operator < (const number_t& a, unsigned char b)  { return cmp(a, number_t(b)) < 0; }
inline bool operator < (unsigned char a, const number_t& b)  { return cmp(number_t(a), b) < 0; }
inline bool operator < (const number_t& a, unsigned short b)  { return cmp(a, number_t(b)) < 0; }
inline bool operator < (unsigned short a, const number_t& b)  { return cmp(number_t(a), b) < 0; }
inline bool operator < (const number_t& a, unsigned int b)  { return cmp(a, number_t(b)) < 0; }
inline bool operator < (unsigned int a, const number_t& b)  { return cmp(number_t(a), b) < 0; }
inline bool operator < (const number_t& a, unsigned long b) { return cmp(a, number_t(b)) < 0; }
inline bool operator < (unsigned long a, const number_t& b) { return cmp(number_t(a), b) < 0; }
inline bool operator < (const number_t& a, unsigned long long b) { return cmp(a, number_t(b)) < 0; }
inline bool operator < (unsigned long long a, const number_t& b) { return cmp(number_t(a), b) < 0; }

inline bool operator >= (const number_t& a, const number_t& b) { return cmp(a, b) >= 0; }
inline bool operator >= (const number_t& a, bool b)  { return cmp(a, number_t(b)) >= 0; }
inline bool operator >= (bool a, const number_t& b)  { return cmp(number_t(a), b) >= 0; }
inline bool operator >= (const number_t& a, char b)  { return cmp(a, number_t(b)) >= 0; }
inline bool operator >= (char a, const number_t& b)  { return cmp(number_t(a), b) >= 0; }
inline bool operator >= (const number_t& a, short b) { return cmp(a, number_t(b)) >= 0; }
inline bool operator >= (short a, const number_t& b) { return cmp(number_t(a), b) >= 0; }
inline bool operator >= (const number_t& a, int b)  { return cmp(a, number_t(b)) >= 0; }
inline bool operator >= (int a, const number_t& b)  { return cmp(number_t(a), b) >= 0; }
inline bool operator >= (const number_t& a, long b) { return cmp(a, number_t(b)) >= 0; }
inline bool operator >= (long a, const number_t& b) { return cmp(number_t(a), b) >= 0; }
inline bool operator >= (const number_t& a, long long b) { return cmp(a, number_t(b)) >= 0; }
inline bool operator >= (long long a, const number_t& b) { return cmp(number_t(a), b) >= 0; }
inline bool operator >= (const number_t& a, unsigned char b)  { return cmp(a, number_t(b)) >= 0; }
inline bool operator >= (unsigned char a, const number_t& b)  { return cmp(number_t(a), b) >= 0; }
inline bool operator >= (const number_t& a, unsigned short b)  { return cmp(a, number_t(b)) >= 0; }
inline bool operator >= (unsigned short a, const number_t& b)  { return cmp(number_t(a), b) >= 0; }
inline bool operator >= (const number_t& a, unsigned int b)  { return cmp(a, number_t(b)) >= 0; }
inline bool operator >= (unsigned int a, const number_t& b)  { return cmp(number_t(a), b) >= 0; }
inline bool operator >= (const number_t& a, unsigned long b) { return cmp(a, number_t(b)) >= 0; }
inline bool operator >= (unsigned long a, const number_t& b) { return cmp(number_t(a), b) >= 0; }
inline bool operator >= (const number_t& a, unsigned long long b) { return cmp(a, number_t(b)) >= 0; }
inline bool operator >= (unsigned long long a, const number_t& b) { return cmp(number_t(a), b) >= 0; }

inline bool operator <= (const number_t& a, const number_t& b) { return cmp(a, b) <= 0; }
inline bool operator <= (const number_t& a, bool b)  { return cmp(a, number_t(b)) <= 0; }
inline bool operator <= (bool a, const number_t& b)  { return cmp(number_t(a), b) <= 0; }
inline bool operator <= (const number_t& a, char b)  { return cmp(a, number_t(b)) <= 0; }
inline bool operator <= (char a, const number_t& b)  { return cmp(number_t(a), b) <= 0; }
inline bool operator <= (const number_t& a, short b) { return cmp(a, number_t(b)) <= 0; }
inline bool operator <= (short a, const number_t& b) { return cmp(number_t(a), b) <= 0; }
inline bool operator <= (const number_t& a, int b)  { return cmp(a, number_t(b)) <= 0; }
inline bool operator <= (int a, const number_t& b)  { return cmp(number_t(a), b) <= 0; }
inline bool operator <= (const number_t& a, long b) { return cmp(a, number_t(b)) <= 0; }
inline bool operator <= (long a, const number_t& b) { return cmp(number_t(a), b) <= 0; }
inline bool operator <= (const number_t& a, long long b) { return cmp(a, number_t(b)) <= 0; }
inline bool operator <= (long long a, const number_t& b) { return cmp(number_t(a), b) <= 0; }
inline bool operator <= (const number_t& a, unsigned char b)  { return cmp(a, number_t(b)) <= 0; }
inline bool operator <= (unsigned char a, const number_t& b)  { return cmp(number_t(a), b) <= 0; }
inline bool operator <= (const number_t& a, unsigned short b)  { return cmp(a, number_t(b)) <= 0; }
inline bool operator <= (unsigned short a, const number_t& b)  { return cmp(number_t(a), b) <= 0; }
inline bool operator <= (const number_t& a, unsigned int b)  { return cmp(a, number_t(b)) <= 0; }
inline bool operator <= (unsigned int a, const number_t& b)  { return cmp(number_t(a), b) <= 0; }
inline bool operator <= (const number_t& a, unsigned long b) { return cmp(a, number_t(b)) <= 0; }
inline bool operator <= (unsigned long a, const number_t& b) { return cmp(number_t(a), b) <= 0; }
inline bool operator <= (const number_t& a, unsigned long long b) { return cmp(a, number_t(b)) <= 0; }
inline bool operator <= (unsigned long long a, const number_t& b) { return cmp(number_t(a), b) <= 0; }

inline number_t operator + (const number_t& a, const number_t& b) { number_t c; add(a, b, c); return c; }
inline number_t operator + (const number_t& a, bool b) { number_t c; add(a, number_t(b), c); return c; }
inline number_t operator + (bool a, const number_t& b) { number_t c; add(number_t(a), b, c); return c; }
inline number_t operator + (const number_t& a, char b) { number_t c; add(a, number_t(b), c); return c; }
inline number_t operator + (char a, const number_t& b) { number_t c; add(number_t(a), b, c); return c; }
inline number_t operator + (const number_t& a, short b) { number_t c; add(a, number_t(b), c); return c; }
inline number_t operator + (short a, const number_t& b) { number_t c; add(number_t(a), b, c); return c; }
inline number_t operator + (const number_t& a, int b) { number_t c; add(a, number_t(b), c); return c; }
inline number_t operator + (int a, const number_t& b) { number_t c; add(number_t(a), b, c); return c; }
inline number_t operator + (const number_t& a, long b) { number_t c; add(a, number_t(b), c); return c; }
inline number_t operator + (long a, const number_t& b) { number_t c; add(number_t(a), b, c); return c; }
inline number_t operator + (const number_t& a, long long b) { number_t c; add(a, number_t(b), c); return c; }
inline number_t operator + (long long a, const number_t& b) { number_t c; add(number_t(a), b, c); return c; }
inline number_t operator + (const number_t& a, unsigned char b) { number_t c; add(a, number_t(b), c); return c; }
inline number_t operator + (unsigned char a, const number_t& b) { number_t c; add(number_t(a), b, c); return c; }
inline number_t operator + (const number_t& a, unsigned short b) { number_t c; add(a, number_t(b), c); return c; }
inline number_t operator + (unsigned short a, const number_t& b) { number_t c; add(number_t(a), b, c); return c; }
inline number_t operator + (const number_t& a, unsigned int b) { number_t c; add(a, number_t(b), c); return c; }
inline number_t operator + (unsigned int a, const number_t& b) { number_t c; add(number_t(a), b, c); return c; }
inline number_t operator + (const number_t& a, unsigned long b) { number_t c; add(a, number_t(b), c); return c; }
inline number_t operator + (unsigned long a, const number_t& b) { number_t c; add(number_t(a), b, c); return c; }
inline number_t operator + (const number_t& a, unsigned long long b) { number_t c; add(a, number_t(b), c); return c; }
inline number_t operator + (unsigned long long a, const number_t& b) { number_t c; add(number_t(a), b, c); return c; }

inline number_t operator - (const number_t& a, const number_t& b) { number_t c; sub(a, b, c); return c; }
inline number_t operator - (const number_t& a, bool b) { number_t c; sub(a, number_t(b), c); return c; }
inline number_t operator - (bool a, const number_t& b) { number_t c; sub(number_t(a), b, c); return c; }
inline number_t operator - (const number_t& a, char b) { number_t c; sub(a, number_t(b), c); return c; }
inline number_t operator - (char a, const number_t& b) { number_t c; sub(number_t(a), b, c); return c; }
inline number_t operator - (const number_t& a, short b) { number_t c; sub(a, number_t(b), c); return c; }
inline number_t operator - (short a, const number_t& b) { number_t c; sub(number_t(a), b, c); return c; }
inline number_t operator - (const number_t& a, int b) { number_t c; sub(a, number_t(b), c); return c; }
inline number_t operator - (int a, const number_t& b) { number_t c; sub(number_t(a), b, c); return c; }
inline number_t operator - (const number_t& a, long b) { number_t c; sub(a, number_t(b), c); return c; }
inline number_t operator - (long a, const number_t& b) { number_t c; sub(number_t(a), b, c); return c; }
inline number_t operator - (const number_t& a, long long b) { number_t c; sub(a, number_t(b), c); return c; }
inline number_t operator - (long long a, const number_t& b) { number_t c; sub(number_t(a), b, c); return c; }
inline number_t operator - (const number_t& a, unsigned char b) { number_t c; sub(a, number_t(b), c); return c; }
inline number_t operator - (unsigned char a, const number_t& b) { number_t c; sub(number_t(a), b, c); return c; }
inline number_t operator - (const number_t& a, unsigned short b) { number_t c; sub(a, number_t(b), c); return c; }
inline number_t operator - (unsigned short a, const number_t& b) { number_t c; sub(number_t(a), b, c); return c; }
inline number_t operator - (const number_t& a, unsigned int b) { number_t c; sub(a, number_t(b), c); return c; }
inline number_t operator - (unsigned int a, const number_t& b) { number_t c; sub(number_t(a), b, c); return c; }
inline number_t operator - (const number_t& a, unsigned long b) { number_t c; sub(a, number_t(b), c); return c; }
inline number_t operator - (unsigned long a, const number_t& b) { number_t c; sub(number_t(a), b, c); return c; }
inline number_t operator - (const number_t& a, unsigned long long b) { number_t c; sub(a, number_t(b), c); return c; }
inline number_t operator - (unsigned long long a, const number_t& b) { number_t c; sub(number_t(a), b, c); return c; }

inline number_t operator * (const number_t& a, const number_t& b) { number_t c; kmul(a, b, c); return c; }
inline number_t operator * (const number_t& a, bool b) { number_t c; kmul(a, number_t(b), c); return c; }
inline number_t operator * (bool a, const number_t& b) { number_t c; kmul(number_t(a), b, c); return c; }
inline number_t operator * (const number_t& a, char b) { number_t c; kmul(a, number_t(b), c); return c; }
inline number_t operator * (char a, const number_t& b) { number_t c; kmul(number_t(a), b, c); return c; }
inline number_t operator * (const number_t& a, short b) { number_t c; kmul(a, number_t(b), c); return c; }
inline number_t operator * (short a, const number_t& b) { number_t c; kmul(number_t(a), b, c); return c; }
inline number_t operator * (const number_t& a, int b) { number_t c; kmul(a, number_t(b), c); return c; }
inline number_t operator * (int a, const number_t& b) { number_t c; kmul(number_t(a), b, c); return c; }
inline number_t operator * (const number_t& a, long b) { number_t c; kmul(a, number_t(b), c); return c; }
inline number_t operator * (long a, const number_t& b) { number_t c; kmul(number_t(a), b, c); return c; }
inline number_t operator * (const number_t& a, long long b) { number_t c; kmul(a, number_t(b), c); return c; }
inline number_t operator * (long long a, const number_t& b) { number_t c; kmul(number_t(a), b, c); return c; }
inline number_t operator * (const number_t& a, unsigned char b) { number_t c; kmul(a, number_t(b), c); return c; }
inline number_t operator * (unsigned char a, const number_t& b) { number_t c; kmul(number_t(a), b, c); return c; }
inline number_t operator * (const number_t& a, unsigned short b) { number_t c; kmul(a, number_t(b), c); return c; }
inline number_t operator * (unsigned short a, const number_t& b) { number_t c; kmul(number_t(a), b, c); return c; }
inline number_t operator * (const number_t& a, unsigned int b) { number_t c; kmul(a, number_t(b), c); return c; }
inline number_t operator * (unsigned int a, const number_t& b) { number_t c; kmul(number_t(a), b, c); return c; }
inline number_t operator * (const number_t& a, unsigned long b) { number_t c; kmul(a, number_t(b), c); return c; }
inline number_t operator * (unsigned long a, const number_t& b) { number_t c; kmul(number_t(a), b, c); return c; }
inline number_t operator * (const number_t& a, unsigned long long b) { number_t c; kmul(a, number_t(b), c); return c; }
inline number_t operator * (unsigned long long a, const number_t& b) { number_t c; kmul(number_t(a), b, c); return c; }

inline number_t operator / (const number_t& a, const number_t& b) { number_t q; div(a, b, q); return q; }
inline number_t operator / (const number_t& a, bool b) { number_t q; div(a, number_t(b), q); return q; }
inline number_t operator / (bool a, const number_t& b) { number_t q; div(number_t(a), b, q); return q; }
inline number_t operator / (const number_t& a, char b) { number_t q; div(a, number_t(b), q); return q; }
inline number_t operator / (char a, const number_t& b) { number_t q; div(number_t(a), b, q); return q; }
inline number_t operator / (const number_t& a, short b) { number_t q; div(a, number_t(b), q); return q; }
inline number_t operator / (short a, const number_t& b) { number_t q; div(number_t(a), b, q); return q; }
inline number_t operator / (const number_t& a, int b) { number_t q; div(a, number_t(b), q); return q; }
inline number_t operator / (int a, const number_t& b) { number_t q; div(number_t(a), b, q); return q; }
inline number_t operator / (const number_t& a, long b) { number_t q; div(a, number_t(b), q); return q; }
inline number_t operator / (long a, const number_t& b) { number_t q; div(number_t(a), b, q); return q; }
inline number_t operator / (const number_t& a, long long b) { number_t q; div(a, number_t(b), q); return q; }
inline number_t operator / (long long a, const number_t& b) { number_t q; div(number_t(a), b, q); return q; }
inline number_t operator / (const number_t& a, unsigned char b) { number_t q; div(a, number_t(b), q); return q; }
inline number_t operator / (unsigned char a, const number_t& b) { number_t q; div(number_t(a), b, q); return q; }
inline number_t operator / (const number_t& a, unsigned short b) { number_t q; div(a, number_t(b), q); return q; }
inline number_t operator / (unsigned short a, const number_t& b) { number_t q; div(number_t(a), b, q); return q; }
inline number_t operator / (const number_t& a, unsigned int b) { number_t q; div(a, number_t(b), q); return q; }
inline number_t operator / (unsigned int a, const number_t& b) { number_t q; div(number_t(a), b, q); return q; }
inline number_t operator / (const number_t& a, unsigned long b) { number_t q; div(a, number_t(b), q); return q; }
inline number_t operator / (unsigned long a, const number_t& b) { number_t q; div(number_t(a), b, q); return q; }
inline number_t operator / (const number_t& a, unsigned long long b) { number_t q; div(a, number_t(b), q); return q; }
inline number_t operator / (unsigned long long a, const number_t& b) { number_t q; div(number_t(a), b, q); return q; }

inline number_t operator % (const number_t& a, const number_t& b) { number_t q, r; div(a, b, q, r); return r; }
inline number_t operator % (const number_t& a, bool b) { number_t q, r; div(a, number_t(b), q, r); return r; }
inline number_t operator % (bool a, const number_t& b) { number_t q, r; div(number_t(a), b, q, r); return r; }
inline number_t operator % (const number_t& a, char b) { number_t q, r; div(a, number_t(b), q, r); return r; }
inline number_t operator % (char a, const number_t& b) { number_t q, r; div(number_t(a), b, q, r); return r; }
inline number_t operator % (const number_t& a, short b) { number_t q, r; div(a, number_t(b), q, r); return r; }
inline number_t operator % (short a, const number_t& b) { number_t q, r; div(number_t(a), b, q, r); return r; }
inline number_t operator % (const number_t& a, int b) { number_t q, r; div(a, number_t(b), q, r); return r; }
inline number_t operator % (int a, const number_t& b) { number_t q, r; div(number_t(a), b, q, r); return r; }
inline number_t operator % (const number_t& a, long b) { number_t q, r; div(a, number_t(b), q, r); return r; }
inline number_t operator % (long a, const number_t& b) { number_t q, r; div(number_t(a), b, q, r); return r; }
inline number_t operator % (const number_t& a, long long b) { number_t q, r; div(a, number_t(b), q, r); return r; }
inline number_t operator % (long long a, const number_t& b) { number_t q, r; div(number_t(a), b, q, r); return r; }
inline number_t operator % (const number_t& a, unsigned char b) { number_t q, r; div(a, number_t(b), q, r); return r; }
inline number_t operator % (unsigned char a, const number_t& b) { number_t q, r; div(number_t(a), b, q, r); return r; }
inline number_t operator % (const number_t& a, unsigned short b) { number_t q, r; div(a, number_t(b), q, r); return r; }
inline number_t operator % (unsigned short a, const number_t& b) { number_t q, r; div(number_t(a), b, q, r); return r; }
inline number_t operator % (const number_t& a, unsigned int b) { number_t q, r; div(a, number_t(b), q, r); return r; }
inline number_t operator % (unsigned int a, const number_t& b) { number_t q, r; div(number_t(a), b, q, r); return r; }
inline number_t operator % (const number_t& a, unsigned long b) { number_t q, r; div(a, number_t(b), q, r); return r; }
inline number_t operator % (unsigned long a, const number_t& b) { number_t q, r; div(number_t(a), b, q, r); return r; }
inline number_t operator % (const number_t& a, unsigned long long b) { number_t q, r; div(a, number_t(b), q, r); return r; }
inline number_t operator % (unsigned long long a, const number_t& b) { number_t q, r; div(number_t(a), b, q, r); return r; }

inline number_t operator & (const number_t& a, const number_t& b) { number_t c; bit_and(a, b, c); return c; }
inline number_t operator & (const number_t& a, bool b) { number_t c; bit_and(a, number_t(b), c); return c; }
inline number_t operator & (bool a, const number_t& b) { number_t c; bit_and(number_t(a), b, c); return c; }
inline number_t operator & (const number_t& a, char b) { number_t c; bit_and(a, number_t(b), c); return c; }
inline number_t operator & (char a, const number_t& b) { number_t c; bit_and(number_t(a), b, c); return c; }
inline number_t operator & (const number_t& a, short b) { number_t c; bit_and(a, number_t(b), c); return c; }
inline number_t operator & (short a, const number_t& b) { number_t c; bit_and(number_t(a), b, c); return c; }
inline number_t operator & (const number_t& a, int b) { number_t c; bit_and(a, number_t(b), c); return c; }
inline number_t operator & (int a, const number_t& b) { number_t c; bit_and(number_t(a), b, c); return c; }
inline number_t operator & (const number_t& a, long b) { number_t c; bit_and(a, number_t(b), c); return c; }
inline number_t operator & (long a, const number_t& b) { number_t c; bit_and(number_t(a), b, c); return c; }
inline number_t operator & (const number_t& a, long long b) { number_t c; bit_and(a, number_t(b), c); return c; }
inline number_t operator & (long long a, const number_t& b) { number_t c; bit_and(number_t(a), b, c); return c; }
inline number_t operator & (const number_t& a, unsigned char b) { number_t c; bit_and(a, number_t(b), c); return c; }
inline number_t operator & (unsigned char a, const number_t& b) { number_t c; bit_and(number_t(a), b, c); return c; }
inline number_t operator & (const number_t& a, unsigned short b) { number_t c; bit_and(a, number_t(b), c); return c; }
inline number_t operator & (unsigned short a, const number_t& b) { number_t c; bit_and(number_t(a), b, c); return c; }
inline number_t operator & (const number_t& a, unsigned int b) { number_t c; bit_and(a, number_t(b), c); return c; }
inline number_t operator & (unsigned int a, const number_t& b) { number_t c; bit_and(number_t(a), b, c); return c; }
inline number_t operator & (const number_t& a, unsigned long b) { number_t c; bit_and(a, number_t(b), c); return c; }
inline number_t operator & (unsigned long a, const number_t& b) { number_t c; bit_and(number_t(a), b, c); return c; }
inline number_t operator & (const number_t& a, unsigned long long b) { number_t c; bit_and(a, number_t(b), c); return c; }
inline number_t operator & (unsigned long long a, const number_t& b) { number_t c; bit_and(number_t(a), b, c); return c; }

inline number_t operator | (const number_t& a, const number_t& b) { number_t c; bit_or(a, b, c); return c; }
inline number_t operator | (const number_t& a, bool b) { number_t c; bit_or(a, number_t(b), c); return c; }
inline number_t operator | (bool a, const number_t& b) { number_t c; bit_or(number_t(a), b, c); return c; }
inline number_t operator | (const number_t& a, char b) { number_t c; bit_or(a, number_t(b), c); return c; }
inline number_t operator | (char a, const number_t& b) { number_t c; bit_or(number_t(a), b, c); return c; }
inline number_t operator | (const number_t& a, short b) { number_t c; bit_or(a, number_t(b), c); return c; }
inline number_t operator | (short a, const number_t& b) { number_t c; bit_or(number_t(a), b, c); return c; }
inline number_t operator | (const number_t& a, int b) { number_t c; bit_or(a, number_t(b), c); return c; }
inline number_t operator | (int a, const number_t& b) { number_t c; bit_or(number_t(a), b, c); return c; }
inline number_t operator | (const number_t& a, long b) { number_t c; bit_or(a, number_t(b), c); return c; }
inline number_t operator | (long a, const number_t& b) { number_t c; bit_or(number_t(a), b, c); return c; }
inline number_t operator | (const number_t& a, long long b) { number_t c; bit_or(a, number_t(b), c); return c; }
inline number_t operator | (long long a, const number_t& b) { number_t c; bit_or(number_t(a), b, c); return c; }
inline number_t operator | (const number_t& a, unsigned char b) { number_t c; bit_or(a, number_t(b), c); return c; }
inline number_t operator | (unsigned char a, const number_t& b) { number_t c; bit_or(number_t(a), b, c); return c; }
inline number_t operator | (const number_t& a, unsigned short b) { number_t c; bit_or(a, number_t(b), c); return c; }
inline number_t operator | (unsigned short a, const number_t& b) { number_t c; bit_or(number_t(a), b, c); return c; }
inline number_t operator | (const number_t& a, unsigned int b) { number_t c; bit_or(a, number_t(b), c); return c; }
inline number_t operator | (unsigned int a, const number_t& b) { number_t c; bit_or(number_t(a), b, c); return c; }
inline number_t operator | (const number_t& a, unsigned long b) { number_t c; bit_or(a, number_t(b), c); return c; }
inline number_t operator | (unsigned long a, const number_t& b) { number_t c; bit_or(number_t(a), b, c); return c; }
inline number_t operator | (const number_t& a, unsigned long long b) { number_t c; bit_or(a, number_t(b), c); return c; }
inline number_t operator | (unsigned long long a, const number_t& b) { number_t c; bit_or(number_t(a), b, c); return c; }

inline number_t operator ^ (const number_t& a, const number_t& b) { number_t c; bit_xor(a, b, c); return c; }
inline number_t operator ^ (const number_t& a, bool b) { number_t c; bit_xor(a, number_t(b), c); return c; }
inline number_t operator ^ (bool a, const number_t& b) { number_t c; bit_xor(number_t(a), b, c); return c; }
inline number_t operator ^ (const number_t& a, char b) { number_t c; bit_xor(a, number_t(b), c); return c; }
inline number_t operator ^ (char a, const number_t& b) { number_t c; bit_xor(number_t(a), b, c); return c; }
inline number_t operator ^ (const number_t& a, short b) { number_t c; bit_xor(a, number_t(b), c); return c; }
inline number_t operator ^ (short a, const number_t& b) { number_t c; bit_xor(number_t(a), b, c); return c; }
inline number_t operator ^ (const number_t& a, int b) { number_t c; bit_xor(a, number_t(b), c); return c; }
inline number_t operator ^ (int a, const number_t& b) { number_t c; bit_xor(number_t(a), b, c); return c; }
inline number_t operator ^ (const number_t& a, long b) { number_t c; bit_xor(a, number_t(b), c); return c; }
inline number_t operator ^ (long a, const number_t& b) { number_t c; bit_xor(number_t(a), b, c); return c; }
inline number_t operator ^ (const number_t& a, long long b) { number_t c; bit_xor(a, number_t(b), c); return c; }
inline number_t operator ^ (long long a, const number_t& b) { number_t c; bit_xor(number_t(a), b, c); return c; }
inline number_t operator ^ (const number_t& a, unsigned char b) { number_t c; bit_xor(a, number_t(b), c); return c; }
inline number_t operator ^ (unsigned char a, const number_t& b) { number_t c; bit_xor(number_t(a), b, c); return c; }
inline number_t operator ^ (const number_t& a, unsigned short b) { number_t c; bit_xor(a, number_t(b), c); return c; }
inline number_t operator ^ (unsigned short a, const number_t& b) { number_t c; bit_xor(number_t(a), b, c); return c; }
inline number_t operator ^ (const number_t& a, unsigned int b) { number_t c; bit_xor(a, number_t(b), c); return c; }
inline number_t operator ^ (unsigned int a, const number_t& b) { number_t c; bit_xor(number_t(a), b, c); return c; }
inline number_t operator ^ (const number_t& a, unsigned long b) { number_t c; bit_xor(a, number_t(b), c); return c; }
inline number_t operator ^ (unsigned long a, const number_t& b) { number_t c; bit_xor(number_t(a), b, c); return c; }
inline number_t operator ^ (const number_t& a, unsigned long long b) { number_t c; bit_xor(a, number_t(b), c); return c; }
inline number_t operator ^ (unsigned long long a, const number_t& b) { number_t c; bit_xor(number_t(a), b, c); return c; }

inline number_t operator << (const number_t& a, int b) { number_t c; shl(a, b, c); return c; }
inline number_t operator >> (const number_t& a, int b) { number_t c; shr(a, b, c); return c; }

inline bool operator && (const number_t& a, const number_t& b) { return !a.is_zero() && !b.is_zero(); }
inline bool operator && (const number_t& a, bool b) { return !a.is_zero() && b; }
inline bool operator && (bool a, const number_t& b) { return a && !b.is_zero(); }
inline bool operator && (const number_t& a, char b) { return !a.is_zero() && b; }
inline bool operator && (char a, const number_t& b) { return a && !b.is_zero(); }
inline bool operator && (const number_t& a, short b) { return !a.is_zero() && b; }
inline bool operator && (short a, const number_t& b) { return a && !b.is_zero(); }
inline bool operator && (const number_t& a, int b) { return !a.is_zero() && b; }
inline bool operator && (int a, const number_t& b) { return a && !b.is_zero(); }
inline bool operator && (const number_t& a, long b) { return !a.is_zero() && b; }
inline bool operator && (long a, const number_t& b) { return a && !b.is_zero(); }
inline bool operator && (const number_t& a, long long b) { return !a.is_zero() && b; }
inline bool operator && (long long a, const number_t& b) { return a && !b.is_zero(); }
inline bool operator && (const number_t& a, unsigned char b) { return !a.is_zero() && b; }
inline bool operator && (unsigned char a, const number_t& b) { return a && !b.is_zero(); }
inline bool operator && (const number_t& a, unsigned short b) { return !a.is_zero() && b; }
inline bool operator && (unsigned short a, const number_t& b) { return a && !b.is_zero(); }
inline bool operator && (const number_t& a, unsigned int b) { return !a.is_zero() && b; }
inline bool operator && (unsigned int a, const number_t& b) { return a && !b.is_zero(); }
inline bool operator && (const number_t& a, unsigned long b) { return !a.is_zero() && b; }
inline bool operator && (unsigned long a, const number_t& b) { return a && !b.is_zero(); }
inline bool operator && (const number_t& a, unsigned long long b) { return !a.is_zero() && b; }
inline bool operator && (unsigned long long a, const number_t& b) { return a && !b.is_zero(); }

inline bool operator || (const number_t& a, const number_t& b) { return !a.is_zero() || !b.is_zero(); }
inline bool operator || (const number_t& a, bool b) { return !a.is_zero() || b; }
inline bool operator || (bool a, const number_t& b) { return a || !b.is_zero(); }
inline bool operator || (const number_t& a, char b) { return !a.is_zero() || b; }
inline bool operator || (char a, const number_t& b) { return a || !b.is_zero(); }
inline bool operator || (const number_t& a, short b) { return !a.is_zero() || b; }
inline bool operator || (short a, const number_t& b) { return a || !b.is_zero(); }
inline bool operator || (const number_t& a, int b) { return !a.is_zero() || b; }
inline bool operator || (int a, const number_t& b) { return a || !b.is_zero(); }
inline bool operator || (const number_t& a, long b) { return !a.is_zero() || b; }
inline bool operator || (long a, const number_t& b) { return a || !b.is_zero(); }
inline bool operator || (const number_t& a, long long b) { return !a.is_zero() || b; }
inline bool operator || (long long a, const number_t& b) { return a || !b.is_zero(); }
inline bool operator || (const number_t& a, unsigned char b) { return !a.is_zero() || b; }
inline bool operator || (unsigned char a, const number_t& b) { return a || !b.is_zero(); }
inline bool operator || (const number_t& a, unsigned short b) { return !a.is_zero() || b; }
inline bool operator || (unsigned short a, const number_t& b) { return a || !b.is_zero(); }
inline bool operator || (const number_t& a, unsigned int b) { return !a.is_zero() || b; }
inline bool operator || (unsigned int a, const number_t& b) { return a || !b.is_zero(); }
inline bool operator || (const number_t& a, unsigned long b) { return !a.is_zero() || b; }
inline bool operator || (unsigned long a, const number_t& b) { return a || !b.is_zero(); }
inline bool operator || (const number_t& a, unsigned long long b) { return !a.is_zero() || b; }
inline bool operator || (unsigned long long a, const number_t& b) { return a || !b.is_zero(); }

inline bool operator == (const string_t& a, const string_t& b) { return cmp(a, b) == 0; }
inline bool operator != (const string_t& a, const string_t& b) { return cmp(a, b) != 0; }
inline bool operator >  (const string_t& a, const string_t& b) { return cmp(a, b) > 0;  }
inline bool operator <  (const string_t& a, const string_t& b) { return cmp(a, b) < 0;  }
inline bool operator >= (const string_t& a, const string_t& b) { return cmp(a, b) >= 0; }
inline bool operator <= (const string_t& a, const string_t& b) { return cmp(a, b) <= 0; }

inline bool operator == (const bitref_t& b, bool x) { return b.value() == x; }
inline bool operator != (const bitref_t& b, bool x) { return b.value() != x; }
inline bool operator == (const bitref_t& b, int x) { return (int)b.value() == x; }
inline bool operator != (const bitref_t& b, int x) { return (int)b.value() != x; }
inline bool operator >  (const bitref_t& b, int x) { return (int)b.value() >  x; }
inline bool operator <  (const bitref_t& b, int x) { return (int)b.value() <  x; }
inline bool operator >= (const bitref_t& b, int x) { return (int)b.value() >= x; }
inline bool operator <= (const bitref_t& b, int x) { return (int)b.value() <= x; }

inline bool operator == (bool x, const bitref_t& b) { return x == b.value(); }
inline bool operator != (bool x, const bitref_t& b) { return x != b.value(); }
inline bool operator == (int x, const bitref_t& b) { return x == (int)b.value(); }
inline bool operator != (int x, const bitref_t& b) { return x != (int)b.value(); }
inline bool operator >  (int x, const bitref_t& b) { return x >  (int)b.value(); }
inline bool operator <  (int x, const bitref_t& b) { return x <  (int)b.value(); }
inline bool operator >= (int x, const bitref_t& b) { return x >= (int)b.value(); }
inline bool operator <= (int x, const bitref_t& b) { return x <= (int)b.value(); }

} // namespace end
