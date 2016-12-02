#include "myoperators.h"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>

using namespace std;
using namespace mynum;

static void __check(const string& s, const number_t& r, const number_t& r1)
{
    if (r != r1)
    {
        cout << s << " " << r(10).c_str() << endl;
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
    __check(line, aa.fun(b.to_basic_fun()), c); \
    mynum::fun(a, b.to_basic_fun(), r); \
    __check(line, r, c); \
} while(0)

void __random_test()
{
    ifstream in("randomtest.dat");
    if (!in)
    {
        cout << "no randomtest.dat" << endl;
        return;
    }

    string line;
    istringstream linestream;  

    string oper, i1, i2, i3;
    int b1, b2;

    number_t a, b, c;
    number_t aa, bb, r1, r2;

    while (getline(in, line))
    {
        linestream.str(line);
        linestream >> oper >>  i1 >>  b1 >>  i2 >>  b2 >>  i3;

        a.assign(i1.c_str(), b1);
        b.assign(i2.c_str(), b2);
        c.assign(i3.c_str());

        if (oper == "+")
        {
            aa.assign(a);
            bb.assign(b);

            add(a, b, r1);
            __check(line, r1, c);

            aa.assign(a);
            aa.add(b);
            __check(line, aa, c);

            add(a, bb, bb);
            __check(line, bb, c);

            __check(line, a + b, c);
        }
        else if (oper == "-")
        {
            aa.assign(a);
            bb.assign(b);

            sub(a, b, r1);
            __check(line, r1, c);

            aa.sub(b);
            __check(line, aa, c);

            sub(a, bb, bb);
            __check(line, bb, c);

            __check(line, a - b, c);
        }
        else if (oper == "*")
        {
            aa.assign(a);
            bb.assign(b);

            mul(a, b, r1);
            __check(line, r1, c);

            aa.mul(b);
            __check(line, aa, c);

            mul(a, bb, bb);
            __check(line, bb, c);

            __check(line, a * b, c);
        }
        else if (oper == "/")
        {
            aa.assign(a);
            bb.assign(b);

            div(a, b, r1);
            __check(line, r1, c);

            aa.div(b);
            __check(line, aa, c);

            div(a, bb, bb);
            __check(line, bb, c);

            __check(line, a / b, c);
        }
        else if (oper == "%")
        {
            aa.assign(a);
            bb.assign(b);

            mod(a, b, r1);
            __check(line, r1, c);

            aa.mod(b);
            __check(line, aa, c);

            mod(a, bb, bb);
            __check(line, bb, c);

            __check(line, a % b, c);

            div(a, b, aa, r1);
            __check(line, r1, c);
            
            bb.assign(b);
            div(a, bb, aa, bb);
            __check(line, bb, c);

            aa.assign(a);
            div(aa, b, bb, aa);
            __check(line, aa, c);
        }
        else if (oper == "&")
        {
            aa.assign(a);
            bb.assign(b);

            mynum::bit_and(a, b, r1);
            __check(line, r1, c);

            aa.bit_and(b);
            __check(line, aa, c);

            mynum::bit_and(a, bb, bb);
            __check(line, bb, c);

            __check(line, a & b, c);
        }
        else if (oper == "|")
        {
            aa.assign(a);
            bb.assign(b);

            mynum::bit_or(a, b, r1);
            __check(line, r1, c);

            aa.bit_or(b);
            __check(line, aa, c);

            mynum::bit_or(a, bb, bb);
            __check(line, bb, c);

            __check(line, a | b, c);
        }
        else if (oper == "^")
        {
            aa.assign(a);
            bb.assign(b);

            mynum::bit_xor(a, b, r1);
            __check(line, r1, c);

            aa.bit_xor(b);
            __check(line, aa, c);

            mynum::bit_xor(a, bb, bb);
            __check(line, bb, c);

            __check(line, a ^ b, c);
        }
        else if (oper == "<")
        {
            __check(line, lt(a, b), c);
            __check(line, a < b, c);
        }
        else if (oper == ">")
        {
            __check(line, gt(a, b), c);
            __check(line, a > b, c);
        }
        else if (oper == "<=")
        {
            __check(line, elt(a, b), c);
            __check(line, a <= b, c);
        }
        else if (oper == ">=")
        {
            __check(line, egt(a, b), c);
            __check(line, a >= b, c);
        }
        else if (oper == "==")
        {
            __check(line, eq(a, b), c);
            __check(line, a == b, c);
        }
        else if (oper == "!=")
        {
            __check(line, neq(a, b), c);
            __check(line, a != b, c);
        }
        else if (oper == "**")
        {
            sqr(a, r1);
            __check(line, r1, c);
            a.sqr();
            __check(line, a, c);
        }
        else if (oper == ">>")
        {
            shr(a, b.to_ulong(), r1);
            __check(line, r1, c);
            a.shr(b.to_ulong());
            __check(line, a, c);
        }
        else if (oper == "<<")
        {
            shl(a, b.to_ulong(), r1);
            __check(line, r1, c);
            a.shl(b.to_ulong());
            __check(line, a, c);
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

        linestream.clear();
    }
}

void random_test()
{
    clock_t t0 = clock();
    __random_test();
    cout << "time: " << double(clock() - t0) / CLOCKS_PER_SEC << endl;
}
