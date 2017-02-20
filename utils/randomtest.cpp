#include "myoperators.h"

#include <ctime>
#include <cstdlib>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


using namespace std;
using namespace mynum;

const number_t ZERO(0);
const number_t ONE(1);

string oper, i1, i2, i3, i4;
int b1, b2;

static void __check(const number_t& res, const number_t& exp)
{
    if (res != exp)
    {
        cout << "UNEXPECTED!!" << endl;
        cout << oper << endl;
        cout << i1 << ", " << b1 << endl;
        cout << i2 << ", " << b2 << endl;
        cout << exp(10) << endl;
        cout << res(10) << endl;
        abort();
    }
}

#define check_basic(fun, in_range_fun, to_basic_fun) do\
{ \
    if (!b.in_range_fun()) \
    { \
        cout << b(10) << " "#in_range_fun" error" << endl; \
        abort(); \
    } \
    number_t aa(a), r; \
    aa.fun(b.to_basic_fun());\
    __check(aa, c); \
    mynum::fun(a, b.to_basic_fun(), r); \
    __check(r, c); \
    aa.assign(a); \
    mynum::fun(aa, b.to_basic_fun(), aa); \
    __check(aa, c); \
} while(0)

size_t __random_test_div(ifstream& in)
{
    size_t n = 0;
    number_t a, b, c, d;
    number_t aa, bb, q, r;
    b1 = b2 = 16;
    while (in.good() && !in.eof())
    {
        n++;
        in >> i1 >> i2 >> i3 >> i4;

        a.assign(i1.c_str(), b1);
        b.assign(i2.c_str(), b2);
        c.assign(i3.c_str(), 16);
        d.assign(i4.c_str(), 16);

        div(a, b, q, r);
        __check(q, c);
        __check(r, d);

        div(a, b, q);
        __check(q, c);

        mod(a, b, r);
        __check(r, d);

        __check(a / b, c);
        __check(a % b, d);

        aa.assign(a);
        bb.assign(b);

        div(a, bb, bb);
        __check(bb, c);

        div(aa, b, aa);
        __check(aa, c);

        aa.assign(a);
        bb.assign(b);

        mod(a, bb, bb);
        __check(bb, d);

        mod(aa, b, aa);
        __check(aa, d);

        aa.assign(a);
        mod(aa, aa, aa);
        __check(aa, ZERO);

        aa.assign(a);
        bb.assign(b);

        div(a, bb, bb, r);
        __check(bb, c);
        __check(r, d);

        div(aa, b, aa, r);
        __check(aa, c);
        __check(r, d);

        aa.assign(a);
        bb.assign(b);

        div(a, bb, q, bb);
        __check(q, c);
        __check(bb, d);

        div(aa, b, q, aa);
        __check(q, c);
        __check(aa, d);

        div(b, b, q, r);
        __check(q, ONE);
        __check(r, ZERO);

        div(b, b, q);
        __check(q, ONE);

        bb.assign(b);

        div(bb, bb, bb);
        __check(bb, ONE);

        aa.assign(a);
        bb.assign(b);

        div(aa, aa, q, aa);
        __check(q, ONE);
        __check(aa, ZERO);

        div(bb, bb, bb, r);
        __check(bb, ONE);
        __check(r, ZERO);

        aa.assign(a);
        bb.assign(b);
        div(aa, bb, aa, bb);
        __check(aa, c);
        __check(bb, d);
    }
    in.close();
    return n;
}

size_t __random_test_mul(ifstream& in)
{
    size_t n = 0;
    number_t a, b, c, d;
    number_t aa, bb, p;
    b1 = b2 = 16;
    while (in.good() && !in.eof())
    {
        n++;
        in >> i1 >> i2 >> i3;

        a.assign(i1.c_str(), b1);
        b.assign(i2.c_str(), b2);
        c.assign(i3.c_str(), 16);

        mul(a, b, p);
        __check(p, c);

        kmul(a, b, p);
        __check(p, c);

        aa.assign(a);
        bb.assign(b);

        mul(a, bb, bb);
        __check(bb, c);

        mul(aa, b, aa);
        __check(aa, c);

        aa.assign(a);
        bb.assign(b);

        kmul(a, bb, bb);
        __check(bb, c);

        kmul(aa, b, aa);
        __check(aa, c);

        if (a == b)
        {
            sqr(a, p);
            __check(p, c);

            ksqr(a, p);
            __check(p, c);

            aa.assign(a);
            sqr(aa, aa);
            __check(aa, c);

            aa.assign(a);
            ksqr(aa, aa);
            __check(aa, c);
        }

        __check(a * b, c);
    }
    in.close();
    return n;
}

size_t __random_test_pom(ifstream& in)
{
    size_t n = 0;
    number_t a, b, c, d;
    number_t aa, bb, cc, r;
    b1 = b2 = 16;
    while (in.good() && !in.eof())
    {
        n++;
        in >> i1 >> i2 >> i3 >> i4;

        a.assign(i1.c_str(), b1);
        b.assign(i2.c_str(), b2);
        c.assign(i3.c_str(), 16);
        d.assign(i4.c_str(), 16);

        pom(a, b, c, r);
        __check(r, d);
    }
    in.close();
    return n;
}

size_t __random_test_all(ifstream& in)
{
    size_t n = 0;
    number_t a, b, c;
    number_t aa, bb, cc, res, q, r;
    while (in.good() && !in.eof())
    {
        n++;
        in >> oper >>  i1 >>  b1 >>  i2 >>  b2 >>  i3;

        a.assign(i1.c_str(), b1);
        b.assign(i2.c_str(), b2);
        c.assign(i3.c_str());

        if (oper == "+")
        {
            add(a, b, res);
            __check(res, c);

            aa.assign(a);
            bb.assign(b);

            add(a, bb, bb);
            __check(bb, c);
            add(aa, b, aa);
            __check(aa, c);

            aa.assign(a);
            aa.add(b);
            __check(aa, c);

            __check(a + b, c);
            __check(a += b, c);
        }
        else if (oper == "-")
        {
            sub(a, b, res);
            __check(res, c);

            aa.assign(a);
            bb.assign(b);

            sub(a, bb, bb);
            __check(bb, c);
            sub(aa, b, aa);
            __check(aa, c);

            aa.assign(a);
            aa.sub(b);
            __check(aa, c);

            __check(a - b, c);
            __check(a -= b, c);
        }
        else if (oper == "*")
        {
            aa.assign(a);
            bb.assign(b);

            mul(a, b, res);
            __check(res, c);

            kmul(a, b, res);
            __check(res, c);

            aa.mul(b);
            __check(aa, c);

            mul(a, bb, bb);
            __check(bb, c);

            __check(a * b, c);
        }
        else if (oper == "/")
        {
            aa.assign(a);
            bb.assign(b);

            div(a, b, res);
            __check(res, c);

            div(a, b, q, r);
            __check(q, c);

            aa.div(b);
            __check(aa, c);

            div(a, bb, bb);
            __check(bb, c);

            __check(a / b, c);
        }
        else if (oper == "%")
        {
            aa.assign(a);
            bb.assign(b);

            mod(a, b, res);
            __check(res, c);

            aa.mod(b);
            __check(aa, c);

            mod(a, bb, bb);
            __check(bb, c);

            __check(a % b, c);

            div(a, b, aa, res);
            __check(res, c);
            
            bb.assign(b);
            div(a, bb, aa, bb);
            __check(bb, c);

            aa.assign(a);
            div(aa, b, bb, aa);
            __check(aa, c);
        }
        else if (oper == "&")
        {
            mynum::bit_and(a, b, res);
            __check(res, c);

            aa.assign(a);
            bb.assign(b);
            mynum::bit_and(a, bb, bb);
            __check(bb, c);
            mynum::bit_and(aa, b, aa);
            __check(aa, c);

            //aa.assign(a);
            //mynum::bit_and(aa, aa, aa);
            //__check(aa, a);

            aa.assign(a);
            aa.bit_and(b);
            __check(aa, c);

            __check(a & b, c);
            __check(a &= b, c);
        }
        else if (oper == "|")
        {
            mynum::bit_or(a, b, res);
            __check(res, c);

            aa.assign(a);
            bb.assign(b);
            mynum::bit_or(a, bb, bb);
            __check(bb, c);
            mynum::bit_or(aa, b, aa);
            __check(aa, c);

            //aa.assign(a);
            //mynum::bit_or(aa, aa, aa);
            //__check(aa, a);

            aa.assign(a);
            aa.bit_or(b);
            __check(aa, c);

            __check(a | b, c);
            __check(a |= b, c);
        }
        else if (oper == "^")
        {
            mynum::bit_xor(a, b, res);
            __check(res, c);

            aa.assign(a);
            bb.assign(b);
            mynum::bit_xor(a, bb, bb);
            __check(bb, c);
            mynum::bit_xor(aa, b, aa);
            __check(aa, c);

            aa.assign(a);
            mynum::bit_xor(aa, aa, aa);
            __check(aa, ZERO);

            aa.assign(a);
            aa.bit_xor(b);
            __check(aa, c);

            __check(a ^ b, c);
            __check(a ^= b, c);
        }
        else if (oper == "<")
        {
            __check(lt(a, b), c);
            __check(a < b, c);
        }
        else if (oper == ">")
        {
            __check(gt(a, b), c);
            __check(a > b, c);
        }
        else if (oper == "<=")
        {
            __check(elt(a, b), c);
            __check(a <= b, c);
        }
        else if (oper == ">=")
        {
            __check(egt(a, b), c);
            __check(a >= b, c);
        }
        else if (oper == "==")
        {
            __check(eq(a, b), c);
            __check(a == b, c);
        }
        else if (oper == "!=")
        {
            __check(neq(a, b), c);
            __check(a != b, c);
        }
        else if (oper == "**")
        {
            sqr(a, res);
            __check(res, c);
            ksqr(a, res);
            __check(res, c);
            aa.assign(a);
            aa.ksqr();
            __check(aa, c);
            a.sqr();
            __check(a, c);
        }
        else if (oper == ">>")
        {
            aa.assign(a);
            __check(aa >> b.to_ulong(), c);

            shr(a, b.to_ulong(), res);
            __check(res, c);

            shr(aa, b.to_ulong(), aa);
            __check(aa, c);

            a.shr(b.to_ulong());
            __check(a, c);
        }
        else if (oper == "<<")
        {
            aa.assign(a);
            __check(aa << b.to_ulong(), c);

            shl(a, b.to_ulong(), res);
            __check(res, c);

            shl(aa, b.to_ulong(), aa);
            __check(aa, c);

            a.shl(b.to_ulong());
            __check(a, c);
        }
        else if (oper == "+s32")
        {
            check_basic(add, in_range_int, to_int);
            if (sizeof(long) == sizeof(int)) check_basic(add, in_range_long, to_long);
        }
        else if (oper == "-s32")
        {
            check_basic(sub, in_range_int, to_int);
            if (sizeof(long) == sizeof(int)) check_basic(sub, in_range_long, to_long);
        }
        else if (oper == "*s32")
        {
            check_basic(mul, in_range_int, to_int);
            if (sizeof(long) == sizeof(int)) check_basic(mul, in_range_long, to_long);
        }
        else if (oper == "/s32")
        {
            check_basic(div, in_range_int, to_int);
            if (sizeof(long) == sizeof(int)) check_basic(div, in_range_long, to_long);
        }
        else if (oper == "%s32")
        {
            check_basic(mod, in_range_int, to_int);
            if (sizeof(long) == sizeof(int)) check_basic(mod, in_range_long, to_long);
        }
        else if (oper == "&s32")
        {
            check_basic(bit_and, in_range_int, to_int);
            if (sizeof(long) == sizeof(int)) check_basic(bit_and, in_range_long, to_long);
        }
        else if (oper == "|s32")
        {
            check_basic(bit_or, in_range_int, to_int);
            if (sizeof(long) == sizeof(int)) check_basic(bit_or, in_range_long, to_long);
        }
        else if (oper == "^s32")
        {
            check_basic(bit_xor, in_range_int, to_int);
            if (sizeof(long) == sizeof(int)) check_basic(bit_xor, in_range_long, to_long);
        }
        else if (oper == "+u32")
        {
            check_basic(add, in_range_uint, to_uint);
            if (sizeof(long) == sizeof(int)) check_basic(add, in_range_ulong, to_ulong);
            if (sizeof(word_t) == 8) check_basic(add_unit, in_range_uint, to_uint);
        }
        else if (oper == "-u32")
        {
            check_basic(sub, in_range_uint, to_uint);
            if (sizeof(long) == sizeof(int)) check_basic(sub, in_range_ulong, to_ulong);
            if (sizeof(word_t) == 8) check_basic(sub_unit, in_range_uint, to_uint);
        }
        else if (oper == "*u32")
        {
            check_basic(mul, in_range_uint, to_uint);
            if (sizeof(long) == sizeof(int)) check_basic(mul, in_range_ulong, to_ulong);
            if (sizeof(word_t) == 8) check_basic(mul_unit, in_range_uint, to_uint);
        }
        else if (oper == "/u32")
        {
            check_basic(div, in_range_uint, to_uint);
            if (sizeof(long) == sizeof(int)) check_basic(div, in_range_ulong, to_ulong);
            if (sizeof(word_t) == 8)
            {
                check_basic(div_unit, in_range_uint, to_uint);
                aa.assign(a);
                UDM udm(b.to_uint());
                aa.div_unit(udm); __check(aa, c);
                div_unit(a, udm, cc); __check(cc, c);
            }
        }
        else if (oper == "%u32")
        {
            check_basic(mod, in_range_uint, to_uint);
            if (sizeof(long) == sizeof(int)) check_basic(mod, in_range_ulong, to_ulong);
            if (sizeof(word_t) == 8)
            {
                check_basic(mod_unit, in_range_uint, to_uint);
                aa.assign(a);
                UDM udm(b.to_uint());
                aa.mod_unit(udm); __check(aa, c);
                mod_unit(a, udm, cc); __check(cc, c);
            }
        }
        else if (oper == "&u32")
        {
            check_basic(bit_and, in_range_uint, to_uint);
            if (sizeof(long) == sizeof(int)) check_basic(bit_and, in_range_ulong, to_ulong);
            if (sizeof(word_t) == 8) check_basic(bit_and_unit, in_range_uint, to_uint);
        }
        else if (oper == "|u32")
        {
            check_basic(bit_or, in_range_uint, to_uint);
            if (sizeof(long) == sizeof(int)) check_basic(bit_or, in_range_ulong, to_ulong);
            if (sizeof(word_t) == 8) check_basic(bit_or_unit, in_range_uint, to_uint);
        }
        else if (oper == "^u32")
        {
            check_basic(bit_xor, in_range_uint, to_uint);
            if (sizeof(long) == sizeof(int)) check_basic(bit_xor, in_range_ulong, to_ulong);
            if (sizeof(word_t) == 8) check_basic(bit_xor_unit, in_range_uint, to_uint);
        }
        else if (oper == "+s64")
        {
            check_basic(add, in_range_longlong, to_longlong);
            if (sizeof(long) == sizeof(long long)) check_basic(add, in_range_long, to_long);
        }
        else if (oper == "-s64")
        {
            check_basic(sub, in_range_longlong, to_longlong);
            if (sizeof(long) == sizeof(long long)) check_basic(sub, in_range_long, to_long);
        }
        else if (oper == "*s64")
        {
            check_basic(mul, in_range_longlong, to_longlong);
            if (sizeof(long) == sizeof(long long)) check_basic(mul, in_range_long, to_long);
        }
        else if (oper == "/s64")
        {
            check_basic(div, in_range_longlong, to_longlong);
            if (sizeof(long) == sizeof(long long)) check_basic(div, in_range_long, to_long);
        }
        else if (oper == "%s64")
        {
            check_basic(mod, in_range_longlong, to_longlong);
            if (sizeof(long) == sizeof(long long)) check_basic(mod, in_range_long, to_long);
        }
        else if (oper == "&s64")
        {
            check_basic(bit_and, in_range_longlong, to_longlong);
            if (sizeof(long) == sizeof(long long)) check_basic(bit_and, in_range_long, to_long);
        }
        else if (oper == "|s64")
        {
            check_basic(bit_or, in_range_longlong, to_longlong);
            if (sizeof(long) == sizeof(long long)) check_basic(bit_or, in_range_long, to_long);
        }
        else if (oper == "^s64")
        {
            check_basic(bit_xor, in_range_longlong, to_longlong);
            if (sizeof(long) == sizeof(long long)) check_basic(bit_xor, in_range_long, to_long);
        }
        else if (oper == "+u64")
        {
            check_basic(add, in_range_ulonglong, to_ulonglong);
            if (sizeof(long) == sizeof(long long)) check_basic(add, in_range_ulong, to_ulong);
        }
        else if (oper == "-u64")
        {
            check_basic(sub, in_range_ulonglong, to_ulonglong);
            if (sizeof(long) == sizeof(long long)) check_basic(sub, in_range_ulong, to_ulong);
        }
        else if (oper == "*u64")
        {
            check_basic(mul, in_range_ulonglong, to_ulonglong);
            if (sizeof(long) == sizeof(long long)) check_basic(mul, in_range_ulong, to_ulong);
        }
        else if (oper == "/u64")
        {
            check_basic(div, in_range_ulonglong, to_ulonglong);
            if (sizeof(long) == sizeof(long long)) check_basic(div, in_range_ulong, to_ulong);
        }
        else if (oper == "%u64")
        {
            check_basic(mod, in_range_ulonglong, to_ulonglong);
            if (sizeof(long) == sizeof(long long)) check_basic(mod, in_range_ulong, to_ulong);
        }
        else if (oper == "&u64")
        {
            check_basic(bit_and, in_range_ulonglong, to_ulonglong);
            if (sizeof(long) == sizeof(long long)) check_basic(bit_and, in_range_ulong, to_ulong);
        }
        else if (oper == "|u64")
        {
            check_basic(bit_or, in_range_ulonglong, to_ulonglong);
            if (sizeof(long) == sizeof(long long)) check_basic(bit_or, in_range_ulong, to_ulong);
        }
        else if (oper == "^u64")
        {
            check_basic(bit_xor, in_range_ulonglong, to_ulonglong);
            if (sizeof(long) == sizeof(long long)) check_basic(bit_xor, in_range_ulong, to_ulong);
        }
        else if (oper == "po")
        {
            pow(a, b.to_int(), res);
            __check(res, c);
            a.pow(b.to_int());
            __check(a, c);
        }
        else if (oper == "+u16")
        {
            check_basic(add_unit, in_range_ushort, to_ushort);
        }
        else if (oper == "-u16")
        {
            check_basic(sub_unit, in_range_ushort, to_ushort);
        }
        else if (oper == "*u16")
        {
            check_basic(mul_unit, in_range_ushort, to_ushort);
        }
        else if (oper == "/u16")
        {
            check_basic(div_unit, in_range_ushort, to_ushort);

            aa.assign(a);
            UDM udm(b.to_ushort());
            aa.div_unit(udm); __check(aa, c);
            div_unit(a, udm, cc); __check(cc, c);
        }
        else if (oper == "%u16")
        {
            check_basic(mod_unit, in_range_ushort, to_ushort);

            aa.assign(a);
            UDM udm(b.to_ushort());
            aa.mod_unit(udm); __check(aa, c);
            mod_unit(a, udm, cc); __check(cc, c);
        }
        else if (oper == "&u16")
        {
            check_basic(bit_and_unit, in_range_ushort, to_ushort);
        }
        else if (oper == "|u16")
        {
            check_basic(bit_or_unit, in_range_ushort, to_ushort);
        }
        else if (oper == "^u16")
        {
            check_basic(bit_xor_unit, in_range_ushort, to_ushort);
        }
    }
    in.close();
    return n;
}

void random(number_t& a);
void create_big(number_t& x, int size);

size_t __random_test_kmul()
{
    size_t n = 100;
    srand((unsigned int)time(NULL));

    number_t a, b, r1, r2;
    int i1, i2;
    while (n--)
    {
        create_big(a, i1 = rand() % 16000);
        create_big(b, i2 = rand() % 16000);
        kmul(a, b, r1);
        mul(a, b, r2);
        if (r1 != r2)
        {
            cout << "KMUL ERROR!!" << endl;
            abort();
        }
    }
    return n;
}

size_t __random_test_ksqr()
{
    size_t n = 100;
    srand((unsigned int)time(NULL));

    number_t a, r1, r2;
    while (n--)
    {
        create_big(a, rand() % 16000);
        ksqr(a, r1);
        sqr(a, r2);
        if (r1 != r2)
        {
            cout << "KSQR ERROR!!" << endl;
            abort();
        }
    }
    return n;
}

size_t __test_sqr_and_mul_performace()
{
    clock_t t0, t1, t2;
    clock_t T0 = 0, T1 = 0;
    size_t n = 3000;
    srand((unsigned int)time(NULL));

    number_t a, r1, r2;
    while (n--)
    {
        create_big(a, 1 + rand() % 2000);
        t0 = clock();
        sqr(a, r1);
        t1 = clock();
        mul(a, a, r2);
        t2 = clock();
        T0 += t1 - t0;
        T1 += t2 - t1;
        if (r1 != r2)
        {
            cout << "SQR ERROR!!" << endl;
            abort();
        }
    }
    if (T0 >= T1)
    {
        cout << "WARNING: sqr is not faster than mul" << endl;
    }
    cout << "sqr time: " << double(T0) / CLOCKS_PER_SEC
         << " mul time: " << double(T1) / CLOCKS_PER_SEC << endl;
    return n;
}

#define test_with_time(title, fun) do\
{\
    cout << title << endl; \
    clock_t t0 = clock(); \
    size_t n = fun; \
    cout << "OK!" << endl; \
    cout << "time: " << double(clock() - t0) / CLOCKS_PER_SEC << endl; \
    cout << n << " items tested" << endl << endl; \
} while (0)

void test_performance()
{
    clock_t t0, t1, t2;
    number_t a(1), b(1), r1, r2;
    a.shl(1257787);
    b.shl(1398269);
    a--;
    b--;
    t0 = clock();
    kmul(a, b, r1);
    t1 = clock();
    mul(a, b, r2);
    t2 = clock();
    if (r1 != r2)
    {
        std::cout << "M1257787 * M1398269 kmul result not equal to mul result" << std::endl;
        abort();
    }
    cout << "M1257787 * M1398269 kmul time: " << double(t1 - t0) / CLOCKS_PER_SEC << "s mul time: " << double(t2 - t1) / CLOCKS_PER_SEC << "s" << endl;

    number_t fac(1);
    t0 = clock();
    for (int i = 1; i < 100000; i++)
    {
        fac.mul_word(i);
    }
    t1 = clock();
    cout << "The factorial of 100000, time: " << double(t1 - t0) / CLOCKS_PER_SEC << " " << fac.bits_count() << "bits" << endl;
    /* 100000! has 1516688 bits, 1000000! has 18488865 bits */

    __test_sqr_and_mul_performace();
}

void random_test()
{
    ifstream randomtest_dat("randomtest.dat");
    if (randomtest_dat)
    {
        test_with_time("Testing all operations", __random_test_all(randomtest_dat));
    }

    ifstream divrandomtest_dat("divrandomtest.dat");
    if (divrandomtest_dat)
    {
        test_with_time("Testing division", __random_test_div(divrandomtest_dat));
    }

    ifstream mulrandomtest_dat("mulrandomtest.dat");
    if (mulrandomtest_dat)
    {
        test_with_time("Testing multiplication", __random_test_mul(mulrandomtest_dat));
    }

    ifstream pomrandomtest_dat("pomrandomtest.dat");
    if (mulrandomtest_dat)
    {
        test_with_time("Testing multiplication", __random_test_pom(pomrandomtest_dat));
    }

    test_with_time("Testing sqr and mul", __test_sqr_and_mul_performace());
    test_with_time("Testing kmul", __random_test_kmul());
    test_with_time("Testing ksqr", __random_test_ksqr());
}