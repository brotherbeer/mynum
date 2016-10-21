#pragma once

#include "mynum.h"

namespace mynum {

// TODO: Use following technique to optimize div_unit
// https://en.wikipedia.org/wiki/Division_algorithm#Division_by_a_constant
// https://gmplib.org/~tege/divcnst-pldi94.pdf
// https://gmplib.org/~tege/division-paper.pdf
// http://ridiculousfish.com/files/faster_unsigned_division_by_constants.pdf
inline template <unit_t U> void div_const_unit(number_t& a)
{
    if (a.len)
    {
        dunit_t rem = 0;
        slen_t l = a.len, s = 1;
        if (l < 0)
        {
            s = -1;
            l = -l;
        }
        unit_t* q = a.dat + l;
        while (--q >= a.dat)
        {
            rem = rem << UNITBITS | *q;
            *q = unit_t(rem / U);   // the compiler will optimize here
            rem %= U;
        }
        const unit_t *e = a.dat - 1, *p = e + l;
        while (p != e && !*p) {p--;}
        a.len = slen_t(p - e) * s;
    }
}

inline template <unit_t U> void div_const_unit(const number_t& a, number_t& res)
{
    if (a.len)
    {
        dunit_t rem = 0;
        slen_t l = a.len, s = 1;
        if (l < 0)
        {
            s = -1;
            l = -l;
        }
        if (res.cap < l)
        {
            res.__reserve(l);
        }
        unit_t* q = res.dat + l;
        while (--q >= a.dat)
        {
            rem = rem << UNITBITS | *q;
            *q = unit_t(rem / U);   // the compiler will optimize here
            rem %= U;
        }
        const unit_t *e = res.dat - 1, *p = e + l;
        while (p != e && !*p) {p--;}
        res.len = slen_t(p - e) * s;
    }
}

// TODO: add test cases about exact division
inline template <unit_t U> void mod_const_unit(number_t& a)
{
    if (a.len)
    {
        dunit_t rem = 0;
        slen_t l = a.len, s = 1;
        if (l < 0)
        {
            s = -1;
            l = -l;
        }
        unit_t* q = a.dat + l;
        while (--q >= a.dat)
        {
            rem = (rem << UNITBITS | *q) % U;
        }
        if (rem)
        {
            *a.dat = (unit_t)rem;
            a.len = s;
        }
        else
        {
            a.set_zero();
        }
    }
}

inline template <unit_t U> void mod_const_unit(const number_t& a, number_t& res)
{
    if (a.len)
    {
        dunit_t rem = 0;
        slen_t l = a.len, s = 1;
        if (l < 0)
        {
            s = -1;
            l = -l;
        }
        unit_t* q = a.dat + l;
        while (--q >= a.dat)
        {
            rem = (rem << UNITBITS | *q) % U;
        }
        if (rem)
        {
            if (res.cap < 1)
            {
                res.__reserve(1);
            }
            *res.dat = (unit_t)rem;
            res.len = s;
        }
        else
        {
            res.set_zero();
        }
    }
}

}
