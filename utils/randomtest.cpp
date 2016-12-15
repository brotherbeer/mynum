#include "myoperators.h"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>

using namespace std;
using namespace mynum;

string oper, i1, i2, i3;
int b1, b2;

static void __check(const number_t& res, const number_t& exp)
{
    if (res != exp)
    {
        cout << "UNEXPECTED!!" << endl;
        cout << oper << endl;
        cout << i1 << ", " << b1 << endl;
        cout << i2 << ", " << b2 << endl;
        cout << exp(10).c_str() << endl;
        cout << res(10).c_str() << endl;
        abort();
    }
}

#define check_basic(fun, in_range_fun, to_basic_fun) do\
{ \
    if (!b.in_range_fun()) \
    { \
        cout << b(10).c_str() << " "#in_range_fun" error" << endl; \
        abort(); \
    } \
    number_t aa(a), r; \
    __check(aa.fun(b.to_basic_fun()), c); \
    mynum::fun(a, b.to_basic_fun(), r); \
    __check(r, c); \
} while(0)

size_t __random_test()
{
    ifstream in("randomtest.dat");
    if (!in)
    {
        cout << "no randomtest.dat" << endl;
        return 0;
    }

    size_t n = 0;
    number_t a, b, c;
    number_t aa, bb, res;
    while (in.good() && !in.eof())
    {
        n++;
        in >> oper >>  i1 >>  b1 >>  i2 >>  b2 >>  i3;

        a.assign(i1.c_str(), b1);
        b.assign(i2.c_str(), b2);
        c.assign(i3.c_str());

        if (oper == "+")
        {
            aa.assign(a);
            bb.assign(b);

            add(a, b, res);
            __check(res, c);
            
            aa.add(b);
            __check(aa, c);
            
            add(a, bb, bb);
            __check(bb, c);

            __check(a + b, c);
        }
        else if (oper == "-")
        {
            aa.assign(a);
            bb.assign(b);

            sub(a, b, res);
            __check(res, c);

            aa.sub(b);
            __check(aa, c);
           
            sub(a, bb, bb);
            __check(bb, c);

            __check(a - b, c);
        }
        else if (oper == "*")
        {
            aa.assign(a);
            bb.assign(b);

            mul(a, b, res);
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
            aa.assign(a);
            bb.assign(b);

            mynum::bit_and(a, b, res);
            __check(res, c);

            aa.bit_and(b);
            __check(aa, c);

            mynum::bit_and(a, bb, bb);
            __check(bb, c);

            __check(a & b, c);
        }
        else if (oper == "|")
        {
            aa.assign(a);
            bb.assign(b);

            mynum::bit_or(a, b, res);
            __check(res, c);

            aa.bit_or(b);
            __check(aa, c);

            mynum::bit_or(a, bb, bb);
            __check(bb, c);

            __check(a | b, c);
        }
        else if (oper == "^")
        {
            aa.assign(a);
            bb.assign(b);

            mynum::bit_xor(a, b, res);
            __check(res, c);

            aa.bit_xor(b);
            __check(aa, c);

            mynum::bit_xor(a, bb, bb);
            __check(bb, c);

            __check(a ^ b, c);
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
            a.sqr();
            __check(a, c);
        }
        else if (oper == ">>")
        {
            aa.assign(a);
            __check(aa >> b.to_ulong(), c);
            shr(a, b.to_ulong(), res);
            __check(res, c);
            a.shr(b.to_ulong());
            __check(a, c);
        }
        else if (oper == "<<")
        {
            aa.assign(a);
            __check(aa << b.to_ulong(), c);
            shl(a, b.to_ulong(), res);
            __check(res, c);
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
        }
        else if (oper == "-u32")
        {
            check_basic(sub, in_range_uint, to_uint);
            if (sizeof(long) == sizeof(int)) check_basic(sub, in_range_ulong, to_ulong);
        }
        else if (oper == "*u32")
        {
            check_basic(mul, in_range_uint, to_uint);
            if (sizeof(long) == sizeof(int)) check_basic(mul, in_range_ulong, to_ulong);
        }
        else if (oper == "/u32")
        {
            check_basic(div, in_range_uint, to_uint);
            if (sizeof(long) == sizeof(int)) check_basic(div, in_range_ulong, to_ulong);
        }
        else if (oper == "%u32")
        {
            check_basic(mod, in_range_uint, to_uint);
            if (sizeof(long) == sizeof(int)) check_basic(mod, in_range_ulong, to_ulong);
        }
        else if (oper == "&u32")
        {
            check_basic(bit_and, in_range_uint, to_uint);
            if (sizeof(long) == sizeof(int)) check_basic(bit_and, in_range_ulong, to_ulong);
        }
        else if (oper == "|u32")
        {
            check_basic(bit_or, in_range_uint, to_uint);
            if (sizeof(long) == sizeof(int)) check_basic(bit_or, in_range_ulong, to_ulong);
        }
        else if (oper == "^u32")
        {
            check_basic(bit_xor, in_range_uint, to_uint);
            if (sizeof(long) == sizeof(int)) check_basic(bit_xor, in_range_ulong, to_ulong);
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
    }
    in.close();
    return n;
}

void random(number_t& a);
void create_big(number_t& x, int size);

void __random_test_kmul()
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
}

void __random_test_ksqr()
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
}

void __test_sqr_and_mul_performace()
{
    clock_t t0, t1, t2;
    clock_t T0 = 0, T1 = 0;
    size_t n = 200;
    srand((unsigned int)time(NULL));

    number_t a, r1, r2;
    while (n--)
    {
        create_big(a, rand() % 6000);
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
        cout << "warning: sqr is not faster than mul" << endl;
    }
    cout << "sqr time: " << double(T0) / CLOCKS_PER_SEC
         << " mul time: " << double(T1) / CLOCKS_PER_SEC << endl;
}

#define test_with_time(title, fun) do\
{\
    cout << title << endl; \
    clock_t t0 = clock(); \
    fun(); \
    cout << "OK!" << endl; \
    cout << "time: " << double(clock() - t0) / CLOCKS_PER_SEC << endl; \
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
        fac.mul_ui(i);
    }
    t1 = clock();
    cout << "The factorial of 100000, time: " << double(t1 - t0) / CLOCKS_PER_SEC << " " << fac.bits_count() << "bits" <<endl;
    /* 100000! has 1516688 bits, 1000000! has 18488865 bits */

    __test_sqr_and_mul_performace();
}

void random_test()
{
    test_with_time("testing random", __random_test);
    test_with_time("testing kmul", __random_test_kmul);
    test_with_time("testing ksqr", __random_test_ksqr);
}