#include "myoperators.h"
#include "mytheory.h"

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
int b1, b2, b3, b4;

static void __check(const number_t& res, const number_t& exp)
{
    if (res != exp)
    {
        cout << "UNEXPECTED!!" << endl;
        if (oper != "") cout << oper << endl;
        if (i1 != "") cout << i1 << ", " << b1 << endl;
        if (i2 != "") cout << i2 << ", " << b2 << endl;
        if (i3 != "") cout << i3 << ", " << b3 << endl;
        cout << "exp: " << exp(10) << endl;
        cout << "res: " << res(10) << endl;
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

#define break_when_eof() \
    if (in.fail()) { \
        if (!in.eof()) { \
            cerr << "ITEM LOAD ERROR!! (" << n + 1 << ")" << endl; \
            abort(); \
        } \
        break; \
    }

#define __show_error_arg2(fun) { \
    cerr << #fun << " UNEXPECTED!!" << endl; \
    cerr << "op1:" << a(16) << endl; \
    cerr << "exp:" << exp(16) << endl; \
    cerr << "res:" << res(16) << endl; \
    abort(); \
}

#define __show_error_arg3(fun) { \
    cerr << #fun << " UNEXPECTED!!" << endl; \
    cerr << "op1:" << a(16) << endl; \
    cerr << "op2:" << b(16) << endl; \
    cerr << "exp:" << exp(16) << endl; \
    cerr << "res:" << res(16) << endl; \
    abort(); \
}

#define __show_error_arg4(fun) { \
    cerr << #fun << " UNEXPECTED!!" << endl; \
    cerr << "op1:" << a(16) << endl; \
    cerr << "op2:" << b(16) << endl; \
    cerr << "op3:" << c(16) << endl; \
    cerr << "exp:" << exp(16) << endl; \
    cerr << "res:" << res(16) << endl; \
    abort(); \
}

#define __check_fun_arg2(fun, a, res, exp) { \
    fun(a, res); \
    if (res != exp) { \
        cerr << #fun << " UNEXPECTED!! ln:" << __LINE__ << endl; \
        cerr << "op1:" << a(16) << endl; \
        cerr << "exp:" << exp(16) << endl; \
        cerr << "res:" << res(16) << endl; \
        abort(); \
    } \
}

#define __check_fun_arg3(fun, a, b, res, exp) { \
    fun(a, b, res); \
    if (res != exp) {\
        cerr << #fun << " UNEXPECTED!! " << __FUNCTION__ << "(" << __LINE__ << ")" <<endl; \
        cerr << "op1:" << a(16) << endl; \
        cerr << "op2:" << b(16) << endl; \
        cerr << "exp:" << exp(16) << endl; \
        cerr << "res:" << res(16) << endl; \
        abort(); \
    } \
}

#define __check_fun_arg4(fun, a, b, c, res, exp) {\
    fun(a, b, c, res); \
    if (res != exp) { \
        cerr << #fun << " UNEXPECTED!! " << __FUNCTION__ << "(" << __LINE__ << ")" <<endl; \
        cerr << "op1:" << a(16) << endl; \
        cerr << "op2:" << b(16) << endl; \
        cerr << "op3:" << c(16) << endl; \
        cerr << "exp:" << exp(16) << endl; \
        cerr << "res:" << res(16) << endl; \
        abort(); \
    } \
}

#define __check_fun_arg4_2(fun, a, b, res1, res2, exp1, exp2) {\
    fun(a, b, res1, res2); \
    if (res1 != exp1 || res2 != exp2) {\
        cerr << #fun << " UNEXPECTED!! " << __FUNCTION__ << "(" << __LINE__ << ")" <<endl; \
        cerr << "op1:"  << a(16) << endl; \
        cerr << "op2:"  << b(16) << endl; \
        cerr << "exp1:" << exp1(16) << endl; \
        cerr << "res1:" << res1(16) << endl; \
        cerr << "exp2:" << exp2(16) << endl; \
        cerr << "res2:" << res2(16) << endl; \
        abort(); \
    } \
}

size_t __random_test_div(ifstream& in)
{
    size_t n = 0;
    number_t a, b, exp1, exp2, res1, res2, aa, bb;
    number_t q, r;

    in.setf(istream::hex);
    while (1)
    {
        in >> a >> b >> exp1 >> exp2;
        break_when_eof();

        __check_fun_arg4_2(div, a, b, q, r, exp1, exp2);

        __check_fun_arg3(div, a, b, q, exp1);
        __check_fun_arg3(mod, a, b, r, exp2);

        aa.assign(a);
        bb.assign(b);
        __check_fun_arg3(div, a, bb, bb, exp1);
        __check_fun_arg3(div, aa, b, aa, exp1);

        aa.assign(a);
        bb.assign(b);
        __check_fun_arg3(mod, a, bb, bb, exp2);
        __check_fun_arg3(mod, aa, b, aa, exp2);

        aa.assign(a);
        bb.assign(b);
        __check_fun_arg4_2(div, a, bb, bb, r, exp1, exp2);
        __check_fun_arg4_2(div, aa, b, aa, r, exp1, exp2);

        aa.assign(a);
        bb.assign(b);
        __check_fun_arg4_2(div, a, bb, q, bb, exp1, exp2);
        __check_fun_arg4_2(div, aa, b, q, aa, exp1, exp2);
        __check_fun_arg4_2(div, b, b, q, r, ONE, ZERO);
        __check_fun_arg3(div, b, b, q, ONE);

        aa.assign(a);
        bb.assign(b);
        if (aa != 0) __check_fun_arg3(mod, aa, aa, aa, ZERO);
        __check_fun_arg3(div, bb, bb, bb, ONE);

        aa.assign(a);
        bb.assign(b);
        __check_fun_arg4_2(div, aa, aa, q, aa, ONE, ZERO);
        __check_fun_arg4_2(div, bb, bb, bb, r, ONE, ZERO);

        aa.assign(a);
        bb.assign(b);
        __check_fun_arg4_2(div, aa, bb, aa, bb, exp1, exp2);

        n++;
    }
    in.close();
    return n;
}

size_t __random_test_mul(ifstream& in)
{
    size_t n = 0;
    number_t a, b, exp, aa, bb, res;

    in.setf(istream::hex);
    while (1)
    {
        in >> a >> b >> exp;
        break_when_eof();

        __check_fun_arg3(mul, a, b, res, exp);
        __check_fun_arg3(kmul, a, b, res, exp);

        aa.assign(a);
        bb.assign(b);
        __check_fun_arg3(mul, a, bb, bb, exp);
        __check_fun_arg3(mul, aa, b, aa, exp);

        aa.assign(a);
        bb.assign(b);
        __check_fun_arg3(kmul, a, bb, bb, exp);
        __check_fun_arg3(kmul, aa, b, aa, exp);

        if (a == b)
        {
            __check_fun_arg2(sqr, a, res, exp);
            __check_fun_arg2(ksqr, a, res, exp);
            aa.assign(a);
            __check_fun_arg2(sqr, aa, aa, exp);
            aa.assign(a);
            __check_fun_arg2(ksqr, aa, aa, exp);
            aa.assign(a);
            __check_fun_arg3(mul, aa, aa, aa, exp);
            aa.assign(a);
            __check_fun_arg3(kmul, aa, aa, aa, exp);
        }
        n++;
    }
    in.close();
    return n;
}

size_t __random_test_pom(ifstream& in)
{
    size_t n = 0;
    number_t a, b, c, exp, res;

    in.setf(istream::hex);
    while (1)
    {
        in >> a >> b >> c >> exp;
        break_when_eof();
        __check_fun_arg4(pom, a, b, c, res, exp);
        n++;
    }
    in.close();
    return n;
}

size_t __random_test_gcd(ifstream& in)
{
    size_t n = 0;
    number_t a, b, exp, res;

    in.setf(istream::hex);
    while (1)
    {
        in >> a >> b >> exp;
        break_when_eof();
        __check_fun_arg3(gcd, a, b, res, exp);
        n++;
    }
    in.close();
    return n;
}

size_t __random_test_lcm(ifstream& in)
{
    size_t n = 0;
    number_t a, b, exp, res;

    in.setf(istream::hex);
    while (1)
    {
        in >> a >> b >> exp;
        break_when_eof();
        __check_fun_arg3(lcm, a, b, res, exp);
        n++;
    }
    in.close();
    return n;
}

size_t __random_test_gcd_ext(ifstream& in)
{
    size_t n = 0;
    number_t a, b, x, y, g;
    number_t u, v, w;

    in.setf(istream::hex);
    while (1)
    {
        in >> a >> b >> x >> y >> g;
        break_when_eof();
        gcd_ext(a, b, u, v, w);
        if (u != x || v != y || w != g)
        {
            cerr << "gcd_ext UNEXPECTED!! " << __FUNCTION__ << "(" << __LINE__ << ")" <<endl; \
            cerr << "op1:"  << a(16) << endl; \
            cerr << "op2:"  << b(16) << endl; \
            cerr << "exp1:" << x(16) << endl; \
            cerr << "res1:" << u(16) << endl; \
            cerr << "exp2:" << y(16) << endl; \
            cerr << "res2:" << v(16) << endl; \
            cerr << "exp3:" << g(16) << endl; \
            cerr << "res3:" << w(16) << endl; \
            abort(); \
        }
        n++;
    }
    in.close();
    return n;
}

size_t __random_test_prime(ifstream& in)
{
    size_t n = 0;
    number_t a;
    bool b, c;

    in.setf(istream::hex);
    while (1)
    {
        in >> a >> b;
        break_when_eof();
        if (b && (!(c = prime_test_roughly(a))))
        {
            cerr << "prime_test_roughly UNEXPECTED!!" << endl;
            goto ERR;
        }
        if ((c = MR_prime_test(a, 25)) != b)
        {
            cerr << "MR_prime_test UNEXPECTED!!" << endl;
            goto ERR;
        }
        n++;
    }
    in.close();
    return n;

ERR:
    cerr << "a:" << a(16) << endl;
    cerr << "exp:" << b << endl;
    cerr << "res:" << c << endl;
    abort();
    return 0;
}

size_t __random_test_jacobi(ifstream& in)
{
    size_t n = 0;
    number_t a, b;
    int exp, res;

    in.setf(istream::hex);
    while (1)
    {
        in >> a >> b >> exp;
        break_when_eof();
        res = jacobi(a, b);
        if (exp != res)
        {
            cerr << "Jacobi UNEXPECTED!!" << endl;
            cerr << "a:" << a(16) << endl;
            cerr << "b:" << b(16) << endl;
            cerr << "exp:" << exp << endl;
            cerr << "res:" << res << endl;           
            abort();
        }
        n++;
    }
    in.close();
    return n;
}

size_t __random_test_basic(ifstream& in)
{
    size_t n = 0;
    number_t a, b, c;
    number_t aa, bb, cc, res, q, r;
    oper = i1 = i2 = i3 = i4 = "";
    while (in.good() && !in.eof())
    {
        n++;
        in >> oper >> i1 >> b1 >> i2 >> b2 >> i3;

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

size_t __random_test_kmul(size_t N)
{
    size_t n = N;
    number_t a, b, r1, r2;

    while (n--)
    {
        rand(rand_word() % 80000, a);
        rand(rand_word() % 80000, b);
        mul(a, b, r1);
        kmul(a, b, r2);
        if (r1 != r2)
        {
            cerr << "KMUL ERROR!!" << endl;
            abort();
        }
    }
    return N;
}

size_t __random_test_ksqr(size_t N)
{
    size_t n = N;
    number_t a, r1, r2;

    while (n--)
    {
        rand(rand_word() % 80000, a);
        sqr(a, r1);
        ksqr(a, r2);
        if (r1 != r2)
        {
            cerr << "KSQR ERROR!!" << endl;
            abort();
        }
    }
    return N;
}

size_t __test_sqr_and_mul_performace(size_t N)
{
    clock_t t0, t1, t2;
    clock_t T0 = 0, T1 = 0;
    size_t n = N;
    number_t a, r1, r2;

    while (n--)
    {
        rand(1 + rand_word() % 32000, a);
        t0 = clock();
        sqr(a, r1);
        t1 = clock();
        mul(a, a, r2);
        t2 = clock();
        T0 += t1 - t0;
        T1 += t2 - t1;
        if (r1 != r2)
        {
            cerr << "SQR ERROR!!" << endl;
            abort();
        }
    }
    if (T0 >= T1)
    {
        cerr << "WARNING: sqr is not faster than mul" << endl;
    }
    cout << "sqr time: " << double(T0) / CLOCKS_PER_SEC
         << " mul time: " << double(T1) / CLOCKS_PER_SEC << endl;
    return N;
}

#define test_with_time(title, fun) {\
    cout << title << endl; \
    clock_t t0 = clock(); \
    size_t n = fun; \
    cout << "OK!" << endl; \
    cout << "time: " << double(clock() - t0) / CLOCKS_PER_SEC << endl; \
    cout << n << " items tested" << endl << endl; \
}

#define test_from_file(title, filename, fun) {\
    ifstream in(filename); \
    if (in) {\
        cout << title << endl; \
        clock_t t0 = clock(); \
        size_t n = fun(in); \
        cout << "OK!" << endl; \
        cout << "time: " << double(clock() - t0) / CLOCKS_PER_SEC << endl; \
        cout << n << " items tested" << endl << endl; \
    }\
}

void random_test()
{
    test_from_file("Testing basic operations", "randomtest.dat", __random_test_basic);
    test_from_file("Testing division", "divrandomtest.dat", __random_test_div);
    test_from_file("Testing multiplication", "mulrandomtest.dat", __random_test_mul);
    test_from_file("Testing modular exponentiation", "pomrandomtest.dat", __random_test_pom);
    test_from_file("Testing GCD", "gcdrandomtest.dat", __random_test_gcd);
    test_from_file("Testing GCD Ext", "gcdextrandomtest.dat", __random_test_gcd_ext);
    test_from_file("Testing LCM", "lcmrandomtest.dat", __random_test_lcm);
    test_from_file("Testing prime", "primerandomtest.dat", __random_test_prime);
    test_from_file("Testing Jacobi", "jacobirandomtest.dat", __random_test_jacobi);

    test_with_time("Testing sqr and mul", __test_sqr_and_mul_performace(3000));
    test_with_time("Testing kmul", __random_test_kmul(1000));
    test_with_time("Testing ksqr", __random_test_ksqr(1000));
}
