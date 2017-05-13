#pragma once

#include "mymem.h"
#include "myprime.h"
#include <intrin.h>
#include <iostream>
#include <cassert>

NAME_SPACE_START

const dunit_t P = 3221225473;   // P = 0xf600000000000001 for 64-bit proc
const dunit_t v = 1431655763;
const dunit_t g = 5;
const dunit_t W[] = // g**((P-1)/n) % P
{
    /*1, */ 3221225472, 1013946479, 1031213943, 2526611335, 66931328, 651814490, 1292405718, 1958494276,
    764652596, 1855261384, 1168849724, 211283056, 1734477367, 1445148504, 1405588668, 2519378301,
    1800384970, 1358427440, 511777184, 3009749949, 448192265, 3182672916, 140749519, 538601489,
    1838654099, 247425463, 229807484, 244140625, 15625, 125, 5, 1
};

// W[i] * RW[i] % P = 1
const dunit_t RW[] =
{
    /*1, */ 3221225472, 2207278994, 613406496, 741264833, 3098715598, 704887665, 764521865, 2935727869,
    2450347685, 532203874, 2829850173, 1588903488, 919321183, 1736059882, 3000663231, 2793181474,
    2702431710, 615116962, 2055537301, 618423934, 762996773, 670776757, 2925583959, 1181743895,
    210645833, 1294972776, 2980752801, 1869040087, 121221157, 2267742733, 1932735284, 1
};

const dunit_t N[] = 
{
    1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144, 
};

const dunit_t RN[] = // N[i] ** (P - 2) % P
{
    1, 1610612737, 2415919105, 2818572289, 3019898881, 3120562177, 3170893825, 3196059649, 3208642561,
    3214934017, 3218079745, 3219652609, 3220439041, 3220832257, 3221028865, 3221127169, 3221176321, 3221200897, 3221213185,
};

static const byte_t __bit_rev_table[] = 
{
    0x00, 0x80, 0x40, 0xC0, 0x20, 0xA0, 0x60, 0xE0,
    0x10, 0x90, 0x50, 0xD0, 0x30, 0xB0, 0x70, 0xF0, 
    0x08, 0x88, 0x48, 0xC8, 0x28, 0xA8, 0x68, 0xE8,
    0x18, 0x98, 0x58, 0xD8, 0x38, 0xB8, 0x78, 0xF8, 
    0x04, 0x84, 0x44, 0xC4, 0x24, 0xA4, 0x64, 0xE4,
    0x14, 0x94, 0x54, 0xD4, 0x34, 0xB4, 0x74, 0xF4, 
    0x0C, 0x8C, 0x4C, 0xCC, 0x2C, 0xAC, 0x6C, 0xEC,
    0x1C, 0x9C, 0x5C, 0xDC, 0x3C, 0xBC, 0x7C, 0xFC, 
    0x02, 0x82, 0x42, 0xC2, 0x22, 0xA2, 0x62, 0xE2,
    0x12, 0x92, 0x52, 0xD2, 0x32, 0xB2, 0x72, 0xF2, 
    0x0A, 0x8A, 0x4A, 0xCA, 0x2A, 0xAA, 0x6A, 0xEA,
    0x1A, 0x9A, 0x5A, 0xDA, 0x3A, 0xBA, 0x7A, 0xFA,
    0x06, 0x86, 0x46, 0xC6, 0x26, 0xA6, 0x66, 0xE6,
    0x16, 0x96, 0x56, 0xD6, 0x36, 0xB6, 0x76, 0xF6, 
    0x0E, 0x8E, 0x4E, 0xCE, 0x2E, 0xAE, 0x6E, 0xEE,
    0x1E, 0x9E, 0x5E, 0xDE, 0x3E, 0xBE, 0x7E, 0xFE,
    0x01, 0x81, 0x41, 0xC1, 0x21, 0xA1, 0x61, 0xE1,
    0x11, 0x91, 0x51, 0xD1, 0x31, 0xB1, 0x71, 0xF1,
    0x09, 0x89, 0x49, 0xC9, 0x29, 0xA9, 0x69, 0xE9,
    0x19, 0x99, 0x59, 0xD9, 0x39, 0xB9, 0x79, 0xF9, 
    0x05, 0x85, 0x45, 0xC5, 0x25, 0xA5, 0x65, 0xE5,
    0x15, 0x95, 0x55, 0xD5, 0x35, 0xB5, 0x75, 0xF5,
    0x0D, 0x8D, 0x4D, 0xCD, 0x2D, 0xAD, 0x6D, 0xED,
    0x1D, 0x9D, 0x5D, 0xDD, 0x3D, 0xBD, 0x7D, 0xFD,
    0x03, 0x83, 0x43, 0xC3, 0x23, 0xA3, 0x63, 0xE3,
    0x13, 0x93, 0x53, 0xD3, 0x33, 0xB3, 0x73, 0xF3, 
    0x0B, 0x8B, 0x4B, 0xCB, 0x2B, 0xAB, 0x6B, 0xEB,
    0x1B, 0x9B, 0x5B, 0xDB, 0x3B, 0xBB, 0x7B, 0xFB,
    0x07, 0x87, 0x47, 0xC7, 0x27, 0xA7, 0x67, 0xE7,
    0x17, 0x97, 0x57, 0xD7, 0x37, 0xB7, 0x77, 0xF7, 
    0x0F, 0x8F, 0x4F, 0xCF, 0x2F, 0xAF, 0x6F, 0xEF,
    0x1F, 0x9F, 0x5F, 0xDF, 0x3F, 0xBF, 0x7F, 0xFF,
};

//inline void fft(dunit_t* dat, int n, int lgn, const dunit_t W[])
//{
//    int i, k, s, h;
//    dunit_t wm, m, t;
//    qunit_t w, u;
//
//    for (s = 1; s <= lgn; s++)
//    {
//        m = 1 << s;
//        wm = W[s];
//        for (k = 0; k < n; k += m)
//        {
//            w = 1;
//            h = m >> 1;
//            for (i = k; i < k + h; i++)
//            {
//                t = w * dat[i + h] % P;
//                u = dat[i];
//                dat[i] = (u + t) % P;
//                dat[i + h] = (u + P - t) % P;
//                w = w * wm % P;
//            }
//        }
//    }
//}

static dunit_t __mul_mod_P(dunit_t x, dunit_t y)
{
    unsigned __int64 U, Q;
    dunit_t u1, u0, q1, q0, r;

    U = __emulu(x, y);
    u1 = U >> 32;
    u0 = U & 0xFFFFFFFF;

    Q = __emulu(v, u1) + U;   
    q1 = Q >> 32;
    q0 = Q & 0xFFFFFFFF;

    r = u0 - ++q1 * P;
    if (r > q0)
    {
        r += P;
    }
    if (r >= P)
    {
        r -= P;
    }
    return r;
}

__forceinline dunit_t __add_mod_P(dunit_t x, dunit_t y)
{
    dunit_t z = P - y;
    return x - z + P * (z > x);
}

//struct root_pool_t
//{
//    dunit_t* pool;
//
//    root_pool_t()
//    {
//        pool = new dunit_t[16 * 32768];
//    }
//
//    ~root_pool_t()
//    {
//        delete[] pool;
//    }
//
//    void init(const dunit_t W[])
//    {
//        dunit_t *p, *e, w, x;
//
//        for (int i = 0; i < 16; i++)
//        {
//            w = 1;
//            x = 1;
//            p = pool + i * 32768;
//            e = p + 32768;
//
//            for (; p != e; p++)
//            {
//                *p = x = __mul_mod_P(x, w);
//                w = W[i + 1];
//            }
//        }
//    }
//
//    const dunit_t* get(int i) const
//    {
//        return pool + i * 32768;
//    }
//};

struct root_pool_t
{
    dunit_t* pool;

    root_pool_t()
    {
        pool = new dunit_t[65535];
    }

    ~root_pool_t()
    {
        delete[] pool;
        pool = NULL;
    }

    void init(const dunit_t W[])
    {
        int s = 1, i = 0;
        dunit_t *p, *e, w, x;

        while (s < 65536)
        {
            x = 1;
            w = W[i++];
            p = pool + s - 1;
            e = p + s;   
            for (*p++ = 1; p != e; p++)
            {
                *p = x = __mul_mod_P(x, w);
            }
            s <<= 1;
        }
    }

    const dunit_t* get(int i) const
    {
        return pool + (1 << i) - 1;
    }
};



//inline void fft(dunit_t* dat, int n, int lgn, const dunit_t W[])
//{
//    int i, k, s, h;
//    dunit_t wm, m, t, w, u;
//
//    for (s = 1; s <= lgn; s++)
//    {
//        m = 1 << s;
//        wm = W[s];
//        for (k = 0; k < n; k += m)
//        {
//            w = 1;
//            h = m >> 1;
//            for (i = k; i < k + h; i++)
//            {
//                t = __mul_mod_P(w, dat[i + h]);
//                u = dat[i];
//                dat[i] = __add_mod_P(u, t);
//                dat[i + h] = __add_mod_P(u, P - t);
//                w = __mul_mod_P(w, wm);
//            }
//        }
//    }
//}

//inline void fft(dunit_t* dat, int n, int lgn, const dunit_t W[])
//{
//    int k, s, h;
//    dunit_t wm, m, t, w, u;
//    dunit_t *p, *q, *e;
//
//    for (s = 1; s <= lgn; s++)
//    {
//        wm = W[s];
//        m = 1 << s;
//        for (k = 0; k < n; k += m)
//        {
//            w = 1;
//            h = m >> 1;
//            p = dat + k;
//            e = p + h;
//            for (; p != e; p++)
//            {
//                u = *p;
//                q = p + h;
//
//                t = __mul_mod_P(w, *q);
//               *p = __add_mod_P(u, t);
//               *q = __add_mod_P(u, P - t);
//                w = __mul_mod_P(w, wm);
//            }
//        }
//    }
//}

inline void fft(dunit_t* dat, int n, int lgn, const root_pool_t& pool)
{
    int k, s, h;
    dunit_t m, t, w, u;
    dunit_t *p, *q, *e;
    const dunit_t *pp, *pw;

    p = dat;
    e = p + n;
    while (p != e)
    {
        q = p + 1;
        u = *p;
        t = *q;
       *p = __add_mod_P(u, t);
       *q = __add_mod_P(u, P - t);
        p += 2;
    }

    for (s = 1; s < lgn; s++)
    {
        pp = pool.get(s);

        h = 1 << s;
        m = h << 1;
        for (k = 0; k < n; k += m)
        {
            w = 1;
            pw = pp;
            p = dat + k;
            e = p + h;
            for (; p != e; p++)
            {
                u = *p;
                q = p + h;

                t = __mul_mod_P(w, *q);
               *p = __add_mod_P(u, t);
               *q = __add_mod_P(u, P - t);
                w = *++pw;
            }
        }
    }
}

__forceinline size_t __log2(size_t x)
{
    assert(x > 0);

    unsigned long b;
    _BitScanReverse(&b, x);
    return b;
}

inline void points_mul(dunit_t* dat0, dunit_t* dat1, int n)
{
    dunit_t* end = dat0 + n;
    for (; dat0 != end; dat0++, dat1++)
    {
        *dat0 = __mul_mod_P(*dat0, *dat1);
    }
}

inline void points_rmul(dunit_t* dat0, dunit_t R, int n)
{
    dunit_t* end = dat0 + n;
    for (; dat0 != end; dat0++)
    {
        *dat0 = __mul_mod_P(*dat0, R);
    }
}

//inline void fmul(const number_t& a, const number_t& b, number_t& res, const root_pool_t& pool0, const root_pool_t& pool1)
//{
//    int i;
//    int n = a.len * 2 * 2;
//    int lgn = __log2(n);
//
//    dunit_t* dat0 = (dunit_t*)calloc(n, sizeof(dunit_t));
//    dunit_t* dat1 = (dunit_t*)calloc(n, sizeof(dunit_t));
//
//    byte_t *p;
//    dunit_t *q, *e;
//
//    p = (byte_t*)a.dat;
//    q = dat0;
//    e = dat0 + a.len * 2;
//    while (q != e)
//    {
//        *q++ = *p++;
//    }
//
//    p = (byte_t*)b.dat;
//    q = dat1;
//    e = dat1 + b.len * 2;
//    while (q != e)
//    {
//        *q++ = *p++;
//    }
//
//    __rev_x86(dat0, n, lgn, __bit_rev_table);
//    __rev_x86(dat1, n, lgn, __bit_rev_table);
//
//    fft(dat0, n, lgn, pool0);
//    fft(dat1, n, lgn, pool0);
//    points_mul(dat0, dat1, n);
//
//    __rev_x86(dat0, n, lgn, __bit_rev_table);
//    fft(dat0, n, lgn, pool1);
//    points_rmul(dat0, RN[lgn], n);
//
//    dunit_t carry = 0;   // perform the carrying
//    for (i = 0; i < n; i++)
//    {
//        carry += dat0[i];
//        dat0[i] = carry % 256;
//        carry /= 256;
//    }
//
//    while (!dat0[n - 1])
//    {
//        n--;
//    }
//
//    int len = (n + 1) / 2;
//    unit_t* dat = number_t::allocate_units(len);
//    for (p = (byte_t*)dat, i = 0; i < n; i++, p++)
//    {
//        *p = (byte_t)dat0[i];
//    }
//    if (n & 1)
//    {
//        *p = 0;
//    }
//
//    mem::deallocate(dat0);
//    mem::deallocate(dat1);
//
//    res.__release();
//    res.len = len;
//    res.dat = dat;
//    res.__init_ref();
//}

inline void init(const number_t& a, int n, int lgn, dunit_t* dat)
{
    byte_t *p = (byte_t*)a.dat;
    byte_t *e = p + a.len * 2;
    unsigned short s = 16 - lgn;
    unsigned short i = 0, ih, il, k;
    
    for (; p != e; p++, i++)
    {
        ih = __bit_rev_table[i >> 8];
        il = __bit_rev_table[i & 0xff];
        k = ((il << 8) | ih) >> s;
        dat[k] = *p;
    }
}

inline void rinit(int n, int lgn, dunit_t* dat)
{
    dunit_t *p = dat;
    dunit_t *e = p + n, tmp;
    unsigned short i, ih, il, k;
    unsigned short s = 16 - lgn;

    for (i = 0; i < n; i++)
    {
        ih = __bit_rev_table[i >> 8];
        il = __bit_rev_table[i & 0xff];
        k = ((il << 8) | ih) >> s;
        if (i > k)
        {
            tmp = dat[i];
            dat[i] = dat[k];
            dat[k] = tmp;
        }
    }
}

inline void fmul(const number_t& a, const number_t& b, number_t& res, const root_pool_t& pool0, const root_pool_t& pool1)
{
    int n = a.len * 2 * 2;
    int lgn = __log2(n);

    dunit_t* dat0 = (dunit_t*)calloc(n, sizeof(dunit_t));
    dunit_t* dat1 = (dunit_t*)calloc(n, sizeof(dunit_t));

    init(a, n, lgn, dat0);
    init(b, n, lgn, dat1);
    fft(dat0, n, lgn, pool0);
    fft(dat1, n, lgn, pool0);
    points_mul(dat0, dat1, n);

    rinit(n, lgn, dat0);
    fft(dat0, n, lgn, pool1);
    points_rmul(dat0, RN[lgn], n);

    int i;
    dunit_t carry = 0;   // perform the carrying
    for (i = 0; i < n; i++)
    {
        carry += dat0[i];
        dat0[i] = carry % 256;
        carry /= 256;
    }

    while (!dat0[n - 1])
    {
        n--;
    }

    int len = (n + 1) / 2;
    byte_t* p;
    unit_t* dat = number_t::allocate_units(len);

    for (p = (byte_t*)dat, i = 0; i < n; i++, p++)
    {
        *p = (byte_t)dat0[i];
    }
    if (n & 1)
    {
        *p = 0;
    }

    mem::deallocate(dat0);
    mem::deallocate(dat1);

    res.__release();
    res.len = len;
    res.dat = dat;
    res.__init_ref();
}

//inline void fmul(const number_t& a, const number_t& b, number_t& res)
//{
//    int i;
//    int n = a.len * 2;
//    int lgn = 0;
//    while ((1 << (++lgn)) < n);
//
//    dunit_t* dat0 = (dunit_t*)mem::allocate(n, sizeof(dunit_t));
//    dunit_t* dat1 = (dunit_t*)mem::allocate(n, sizeof(dunit_t));
//
//    mem::set(dat0, 0, n * sizeof(dunit_t));
//    mem::set(dat1, 0, n * sizeof(dunit_t));
//
//    unit_t* p;
//    for (p = a.dat, i = 0; i < a.len; i++, p++)
//    {
//        dat0[i] = *p;
//    }
//    for (p = b.dat, i = 0; i < b.len; i++, p++)
//    {
//        dat1[i] = *p;
//    }
//
//    __rev_x86(dat0, n, lgn, __bit_rev_table);
//    __rev_x86(dat1, n, lgn, __bit_rev_table);
//    fft(dat0, n, lgn, W);
//    fft(dat1, n, lgn, W);
//    __points_mul_x86(dat0, dat1, n, P);
//
//    __rev_x86(dat0, n, lgn, __bit_rev_table);
//    fft(dat0, n, lgn, RW);
//    __points_rmul_x86(dat0, RN[lgn], n, P);
//
//    dunit_t g = 0;
//    for (i = 0; i < n; i++)
//    {
//        g += dat0[i];
//        dat0[i] = g % 65536;
//        g /= 65536;
//    }
//
//    while (!dat0[n - 1])    // ¸Ä½ø
//    {
//        n--;
//    }
//
//    int len = n;
//    unit_t* dat = number_t::allocate_units(n);
//    for (p = dat, i = 0; i < n; i++, p++)
//    {
//        *p = (unit_t)dat0[i];
//    }
//    if (n & 1)
//    {
//        *p = 0;
//    }
//
//    mem::deallocate(dat0);
//    mem::deallocate(dat1);
//
//    res.__release();
//    res.len = len;
//    res.dat = dat;
//    res.__init_ref();
//}

NAME_SPACE_END
