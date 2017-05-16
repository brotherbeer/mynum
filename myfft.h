#pragma once

#include "mymem.h"
#include <intrin.h>
#include <iostream>
#include <cassert>


namespace mynum {

const dunit_t P = 0xc0000001; // primitive root 5
const dunit_t v = 0x55555553;

const dunit_t W[] = {
     0xc0000000, 0x3c6f986f, 0x3d771377, 0x96990787,
     0x03fd4a80, 0x26d9e65a, 0x4d088bd6, 0x74bc4044,
     0x2d93ac34, 0x6e950ac8, 0x45ab3b3c, 0xc97ec70,
     0x67620637, 0x56233758, 0x53c794bc, 0x962aa97d,
     0x6b4fb1ca, 0x50f7f530, 0x1e8119a0, 0xb36523bd,
     0x1ab6df09, 0xbdb3bc14, 0x0863aacf, 0x201a6811,
     0x6d97a293, 0x0ebf69b7, 0x0db2957c, 0x0e8d4a51,
};

const dunit_t RW[] = {
     0xc0000000, 0x83906792, 0x248fd720, 0x2c2ecdc1,
     0xb8b2a5ce, 0x2a03bb71, 0x2d91ad89, 0xaefba6fd,
     0x920d56a5, 0x1fb8c962, 0xa8ac163d, 0x5eb4be40,
     0x36cbba5f, 0x677a2bea, 0xb2da7cbf, 0xa67c9122,
     0xa113d5de, 0x24a9f0a2, 0x7a850295, 0x24dc667e,
     0x2d7a6825, 0x27fb3db5, 0xae60de57, 0x466ffb17,
     0x0c8e3349, 0x4d2fb768, 0xb1aaada1, 0x6f6749d7,
};

const dunit_t RN[] = {
     1, 0x60000001, 0x90000001, 0xa8000001, 0xb4000001,
     0xba000001, 0xbd000001, 0xbe800001, 0xbf400001,
     0xbfa00001, 0xbfd00001, 0xbfe80001, 0xbff40001,
     0xbffa0001, 0xbffd0001, 0xbffe8001, 0xbfff4001,
     0xbfffa001, 0xbfffd001, 0xbfffe801, 0xbffff401,
     0xbffffa01, 0xbffffd01, 0xbffffe81, 0xbfffff41,
     0xbfffffa1, 0xbfffffd1, 0xbfffffe9, 0xbffffff5,
     0xbffffffb, 0xbffffffe, 0x5fffffff, 0x90000000,
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
    int lgn = __log2(n), len;

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

    unit_t* dat = number_t::allocate_units(len = n / 2);
    byte_t* p = (byte_t*)dat;

    dunit_t* q = dat0;
    dunit_t* qe = dat0 + n;
    dunit_t carry = 0;   // perform the carrying
    while (q != qe)
    {
        carry += *q++;
        *p++ = carry % 256;
        carry /= 256;
    }
    mem::deallocate(dat0);
    mem::deallocate(dat1);

    __trim_leading_zeros(dat, len);
    
    res.__release();
    res.len = len;
    res.dat = dat;
    res.__init_ref();
}

} // namespace end
