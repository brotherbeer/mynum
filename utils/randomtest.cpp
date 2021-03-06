#include "myoperators.h"
#include "mytheory.h"

#include <ctime>
#include <cstdlib>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>


using namespace std;
using namespace mynum;

const number_t ZERO(0);
const number_t ONE(1);

#define show_unexpected_loc(name) \
    cerr << name << " UNEXPECTED!! in " << __FUNCTION__ << "(" << __LINE__ << ")" << endl \

#define break_when_eof() \
    if (in.fail()) { \
        if (!in.eof()) { \
            cerr << "ITEM LOAD ERROR!! (" << n + 1 << ")" << endl; \
            abort(); \
        } \
        break; \
    }

#define check_fun_arg2(fun, a, res, exp) { \
    fun(a, res); \
    if ((res) != (exp)) { \
        show_unexpected_loc(#fun); \
        cerr << "op1:" << a(16) << endl; \
        cerr << "exp:" << exp(16) << endl; \
        cerr << "res:" << res(16) << endl; \
        abort(); \
    } \
    if (chance(5)) (res).release();\
}

#define check_fun_arg3(fun, a, b, res, exp) { \
    fun(a, b, res); \
    if ((res) != (exp)) {\
        show_unexpected_loc(#fun); \
        cerr << "op1:" << a(16) << endl; \
        cerr << "op2:" << b(16) << endl; \
        cerr << "exp:" << exp(16) << endl; \
        cerr << "res:" << res(16) << endl; \
        abort(); \
    } \
    if (chance(5)) (res).release();\
}

#define check_fun_arg4(fun, a, b, c, res, exp) {\
    fun(a, b, c, res); \
    if ((res) != (exp)) { \
        show_unexpected_loc(#fun); \
        cerr << "op1:" << a(16) << endl; \
        cerr << "op2:" << b(16) << endl; \
        cerr << "op3:" << c(16) << endl; \
        cerr << "exp:" << exp(16) << endl; \
        cerr << "res:" << res(16) << endl; \
        abort(); \
    } \
    if (chance(5)) (res).release();\
}

#define check_fun_arg4_2(fun, a, b, res1, res2, exp1, exp2) {\
    fun(a, b, res1, res2); \
    if ((res1) != (exp1) || (res2) != (exp2)) {\
        show_unexpected_loc(#fun); \
        cerr << "op1:"  << a(16) << endl; \
        cerr << "op2:"  << b(16) << endl; \
        cerr << "exp1:" << exp1(16) << endl; \
        cerr << "res1:" << res1(16) << endl; \
        cerr << "exp2:" << exp2(16) << endl; \
        cerr << "res2:" << res2(16) << endl; \
        abort(); \
    } \
    if (chance(5)) (res1).release();\
    if (chance(5)) (res2).release();\
}

size_t random_test_div(ifstream& in)
{
    size_t n = 0;
    number_t a, b, exp1, exp2, res1, res2, aa, bb;
    number_t q, r;

    in.setf(istream::hex);
    while (1)
    {
        in >> a >> b >> exp1 >> exp2;

        break_when_eof();

        check_fun_arg4_2(div, a, b, q, r, exp1, exp2);

        check_fun_arg3(div, a, b, q, exp1);
        check_fun_arg3(mod, a, b, r, exp2);

        aa.assign(a);
        bb.assign(b);
        check_fun_arg3(div, a, bb, bb, exp1);
        check_fun_arg3(div, aa, b, aa, exp1);

        aa.assign(a);
        bb.assign(b);
        check_fun_arg3(mod, a, bb, bb, exp2);
        check_fun_arg3(mod, aa, b, aa, exp2);

        aa.assign(a);
        bb.assign(b);
        check_fun_arg4_2(div, a, bb, bb, r, exp1, exp2);
        check_fun_arg4_2(div, aa, b, aa, r, exp1, exp2);

        aa.assign(a);
        bb.assign(b);
        check_fun_arg4_2(div, a, bb, q, bb, exp1, exp2);
        check_fun_arg4_2(div, aa, b, q, aa, exp1, exp2);
        check_fun_arg4_2(div, b, b, q, r, ONE, ZERO);
        check_fun_arg3(div, b, b, q, ONE);

        aa.assign(a);
        bb.assign(b);
        if (aa != 0) check_fun_arg3(mod, aa, aa, aa, ZERO);
        check_fun_arg3(div, bb, bb, bb, ONE);

        aa.assign(a);
        bb.assign(b);
        if (aa != 0) check_fun_arg4_2(div, aa, aa, q, aa, ONE, ZERO);
        check_fun_arg4_2(div, bb, bb, bb, r, ONE, ZERO);

        aa.assign(a);
        bb.assign(b);
        check_fun_arg4_2(div, aa, bb, aa, bb, exp1, exp2);

        n++;

        if (chance(5)) a.release();
        if (chance(5)) b.release();
        if (chance(5)) res1.release();
        if (chance(5)) res2.release();
        if (chance(5)) exp1.release();
        if (chance(5)) exp2.release();
        if (chance(5)) aa.release();
        if (chance(5)) bb.release();
    }
    in.close();
    return n;
}

size_t random_test_mul(ifstream& in)
{
    size_t n = 0;
    number_t a, b, exp, aa, bb, res;

    in.setf(istream::hex);
    while (1)
    {
        in >> a >> b >> exp;
        break_when_eof();

        check_fun_arg3(mul, a, b, res, exp);
        check_fun_arg3(kmul, a, b, res, exp);

        aa.assign(a);
        bb.assign(b);
        check_fun_arg3(mul, a, bb, bb, exp);
        check_fun_arg3(mul, aa, b, aa, exp);

        aa.assign(a);
        bb.assign(b);
        check_fun_arg3(kmul, a, bb, bb, exp);
        check_fun_arg3(kmul, aa, b, aa, exp);

        if (a == b)
        {
            check_fun_arg2(sqr, a, res, exp);
            check_fun_arg2(ksqr, a, res, exp);
            aa.assign(a);
            check_fun_arg2(sqr, aa, aa, exp);
            aa.assign(a);
            check_fun_arg2(ksqr, aa, aa, exp);
            aa.assign(a);
            check_fun_arg3(mul, aa, aa, aa, exp);
            aa.assign(a);
            check_fun_arg3(kmul, aa, aa, aa, exp);
        }
        n++;

        if (chance(5)) a.release();
        if (chance(5)) b.release();
        if (chance(5)) res.release();
        if (chance(5)) exp.release();
        if (chance(5)) aa.release();
        if (chance(5)) bb.release();
    }
    in.close();
    return n;
}

size_t random_test_pom(ifstream& in)
{
    size_t n = 0;
    number_t a, b, c, exp, res;

    in.setf(istream::hex);
    while (1)
    {
        in >> a >> b >> c >> exp;
        break_when_eof();
        check_fun_arg4(pom, a, b, c, res, exp);
        n++;
        if (chance(5)) res.release();
    }
    in.close();
    return n;
}

size_t random_test_gcd(ifstream& in)
{
    size_t n = 0;
    number_t a, b, exp, res;

    in.setf(istream::hex);
    while (1)
    {
        in >> a >> b >> exp;
        break_when_eof();
        check_fun_arg3(gcd, a, b, res, exp);
        n++;
        if (chance(5)) res.release();
    }
    in.close();
    return n;
}

size_t random_test_lcm(ifstream& in)
{
    size_t n = 0;
    number_t a, b, exp, res;

    in.setf(istream::hex);
    while (1)
    {
        in >> a >> b >> exp;
        break_when_eof();
        check_fun_arg3(lcm, a, b, res, exp);
        n++;
        if (chance(5)) res.release();
    }
    in.close();
    return n;
}

size_t random_test_gcd_ext(ifstream& in)
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
        //if (u != x || v != y || w != g) TODO: if a and b are the same...
        if (a * u + b * v != w)
        {
            cerr << "gcd_ext UNEXPECTED!!" << endl;
            cerr << "op1:"  << a(16) << endl;
            cerr << "op2:"  << b(16) << endl;
            cerr << "exp1:" << x(16) << endl;
            cerr << "res1:" << u(16) << endl;
            cerr << "exp2:" << y(16) << endl;
            cerr << "res2:" << v(16) << endl;
            cerr << "exp3:" << g(16) << endl;
            cerr << "res3:" << w(16) << endl;
            abort();
        }
        n++;
        if (chance(5)) u.release();
        if (chance(5)) v.release();
        if (chance(5)) w.release();
    }
    in.close();
    return n;
}

size_t random_test_prime(ifstream& in)
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

size_t random_test_jacobi(ifstream& in)
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
            cerr << "jacobi UNEXPECTED!!" << endl;
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

#define check(res, exp) { \
    const number_t& rres = (res); \
    if (rres != exp) { \
        show_unexpected_loc(oper); \
        cout << "op1: " << s1 << "(" << b1 << ")" << endl; \
        cout << "op2: " << s2 << "(" << b2 << ")" << endl; \
        cout << "exp: " << exp(10) << endl; \
        cout << "res: " << rres(10) << endl; \
        abort(); \
    } \
    if (chance(5)) (res).release();\
}

#define check_bool(res, exp) { \
    if ((res) != (exp)) { \
        show_unexpected_loc(oper); \
        cout << "op1: " << s1 << "(" << b1 << ")" << endl; \
        cout << "op2: " << s2 << "(" << b2 << ")" << endl; \
        cout << "exp: " << (exp) << endl; \
        cout << "res: " << (res) << endl; \
        abort(); \
    } \
}

#define check_ordinary(fun, in_range_fun, to_ordinary_fun) { \
    if (!b.in_range_fun()) { \
        cout << b(10) << " "#in_range_fun" error" << endl; \
        abort(); \
    } \
    number_t aa(a), r; \
    aa.fun(b.to_ordinary_fun());\
    check(aa, exp); \
    mynum::fun(a, b.to_ordinary_fun(), r); \
    check(r, exp); \
    aa.assign(a); \
    mynum::fun(aa, b.to_ordinary_fun(), aa); \
    check(aa, exp); \
}

size_t random_test_basic(ifstream& in)
{
    size_t n = 0;
    number_t a, b, exp;
    number_t aa, bb, cc, res, q, r;
    int b1, b2;
    string_t oper, s1, s2, s3;

    while (in.good() && !in.eof())
    {
        n++;
        in >> oper >> s1 >> b1 >> s2 >> b2 >> s3;

        a.assign(s1, b1);
        b.assign(s2, b2);
        exp.assign(s3);

        if (oper == "+")
        {
            add(a, b, res);
            check(res, exp);

            aa.assign(a);
            bb.assign(b);

            add(a, bb, bb);
            check(bb, exp);
            add(aa, b, aa);
            check(aa, exp);

            aa.assign(a);
            aa.add(b);
            check(aa, exp);

            check(a + b, exp);
            check(a += b, exp);
        }
        else if (oper == "-")
        {
            sub(a, b, res);
            check(res, exp);

            aa.assign(a);
            bb.assign(b);

            sub(a, bb, bb);
            check(bb, exp);
            sub(aa, b, aa);
            check(aa, exp);

            aa.assign(a);
            aa.sub(b);
            check(aa, exp);

            check(a - b, exp);
            check(a -= b, exp);
        }
        else if (oper == "*")
        {
            aa.assign(a);
            bb.assign(b);

            mul(a, b, res);
            check(res, exp);

            kmul(a, b, res);
            check(res, exp);

            aa.mul(b);
            check(aa, exp);

            mul(a, bb, bb);
            check(bb, exp);

            fmul(a, b, res);
            check(res, exp);

            check(a * b, exp);
        }
        else if (oper == "/")
        {
            aa.assign(a);
            bb.assign(b);

            div(a, b, res);
            check(res, exp);

            div(a, b, q, r);
            check(q, exp);

            aa.div(b);
            check(aa, exp);

            div(a, bb, bb);
            check(bb, exp);

            check(a / b, exp);
        }
        else if (oper == "%")
        {
            aa.assign(a);
            bb.assign(b);

            mod(a, b, res);
            check(res, exp);

            aa.mod(b);
            check(aa, exp);

            mod(a, bb, bb);
            check(bb, exp);

            check(a % b, exp);

            div(a, b, aa, res);
            check(res, exp);
            
            bb.assign(b);
            div(a, bb, aa, bb);
            check(bb, exp);

            aa.assign(a);
            div(aa, b, bb, aa);
            check(aa, exp);
        }
        else if (oper == "&")
        {
            mynum::bit_and(a, b, res);
            check(res, exp);

            aa.assign(a);
            bb.assign(b);
            mynum::bit_and(a, bb, bb);
            check(bb, exp);
            mynum::bit_and(aa, b, aa);
            check(aa, exp);

            //aa.assign(a);
            //mynum::bit_and(aa, aa, aa);
            //check(aa, a);

            aa.assign(a);
            aa.bit_and(b);
            check(aa, exp);

            check(a & b, exp);
            check(a &= b, exp);
        }
        else if (oper == "|")
        {
            mynum::bit_or(a, b, res);
            check(res, exp);

            aa.assign(a);
            bb.assign(b);
            mynum::bit_or(a, bb, bb);
            check(bb, exp);
            mynum::bit_or(aa, b, aa);
            check(aa, exp);

            //aa.assign(a);
            //mynum::bit_or(aa, aa, aa);
            //check(aa, a);

            aa.assign(a);
            aa.bit_or(b);
            check(aa, exp);

            check(a | b, exp);
            check(a |= b, exp);
        }
        else if (oper == "^")
        {
            mynum::bit_xor(a, b, res);
            check(res, exp);

            aa.assign(a);
            bb.assign(b);
            mynum::bit_xor(a, bb, bb);
            check(bb, exp);
            mynum::bit_xor(aa, b, aa);
            check(aa, exp);

            aa.assign(a);
            mynum::bit_xor(aa, aa, aa);
            check(aa, ZERO);

            aa.assign(a);
            aa.bit_xor(b);
            check(aa, exp);

            check(a ^ b, exp);
            check(a ^= b, exp);
        }
        else if (oper == "<")
        {
            check_bool(lt(a, b), exp);
            check_bool(a < b, exp);
        }
        else if (oper == ">")
        {
            check_bool(gt(a, b), exp);
            check_bool(a > b, exp);
        }
        else if (oper == "<=")
        {
            check_bool(elt(a, b), exp);
            check_bool(a <= b, exp);
        }
        else if (oper == ">=")
        {
            check_bool(egt(a, b), exp);
            check_bool(a >= b, exp);
        }
        else if (oper == "==")
        {
            check_bool(eq(a, b), exp);
            check_bool(a == b, exp);
        }
        else if (oper == "!=")
        {
            check_bool(neq(a, b), exp);
            check_bool(a != b, exp);
        }
        else if (oper == "**")
        {
            sqr(a, res);
            check(res, exp);
            ksqr(a, res);
            check(res, exp);
            fsqr(a, res);
            check(res, exp);
            aa.assign(a);
            aa.ksqr();
            check(aa, exp);
            a.sqr();
            check(a, exp);
        }
        else if (oper == ">>")
        {
            aa.assign(a);
            check(aa >> b.to_ulong(), exp);

            shr(a, b.to_ulong(), res);
            check(res, exp);

            shr(aa, b.to_ulong(), aa);
            check(aa, exp);

            a.shr(b.to_ulong());
            check(a, exp);
        }
        else if (oper == "<<")
        {
            aa.assign(a);
            check(aa << b.to_ulong(), exp);

            shl(a, b.to_ulong(), res);
            check(res, exp);

            shl(aa, b.to_ulong(), aa);
            check(aa, exp);

            a.shl(b.to_ulong());
            check(a, exp);
        }
        else if (oper == "+s32")
        {
            check_ordinary(add, in_range_int, to_int);
            if (sizeof(long) == sizeof(int)) check_ordinary(add, in_range_long, to_long);
        }
        else if (oper == "-s32")
        {
            check_ordinary(sub, in_range_int, to_int);
            if (sizeof(long) == sizeof(int)) check_ordinary(sub, in_range_long, to_long);
        }
        else if (oper == "*s32")
        {
            check_ordinary(mul, in_range_int, to_int);
            if (sizeof(long) == sizeof(int)) check_ordinary(mul, in_range_long, to_long);
        }
        else if (oper == "/s32")
        {
            check_ordinary(div, in_range_int, to_int);
            if (sizeof(long) == sizeof(int)) check_ordinary(div, in_range_long, to_long);
        }
        else if (oper == "%s32")
        {
            check_ordinary(mod, in_range_int, to_int);
            if (sizeof(long) == sizeof(int)) check_ordinary(mod, in_range_long, to_long);
        }
        else if (oper == "&s32")
        {
            check_ordinary(bit_and, in_range_int, to_int);
            if (sizeof(long) == sizeof(int)) check_ordinary(bit_and, in_range_long, to_long);
        }
        else if (oper == "|s32")
        {
            check_ordinary(bit_or, in_range_int, to_int);
            if (sizeof(long) == sizeof(int)) check_ordinary(bit_or, in_range_long, to_long);
        }
        else if (oper == "^s32")
        {
            check_ordinary(bit_xor, in_range_int, to_int);
            if (sizeof(long) == sizeof(int)) check_ordinary(bit_xor, in_range_long, to_long);
        }
        else if (oper == "+u32")
        {
            check_ordinary(add, in_range_uint, to_uint);
            if (sizeof(long) == sizeof(int)) check_ordinary(add, in_range_ulong, to_ulong);
            if (sizeof(word_t) == 8) check_ordinary(add_unit, in_range_uint, to_uint);
        }
        else if (oper == "-u32")
        {
            check_ordinary(sub, in_range_uint, to_uint);
            if (sizeof(long) == sizeof(int)) check_ordinary(sub, in_range_ulong, to_ulong);
            if (sizeof(word_t) == 8) check_ordinary(sub_unit, in_range_uint, to_uint);
        }
        else if (oper == "*u32")
        {
            check_ordinary(mul, in_range_uint, to_uint);
            if (sizeof(long) == sizeof(int)) check_ordinary(mul, in_range_ulong, to_ulong);
            if (sizeof(word_t) == 8) check_ordinary(mul_unit, in_range_uint, to_uint);
        }
        else if (oper == "/u32")
        {
            check_ordinary(div, in_range_uint, to_uint);
            if (sizeof(long) == sizeof(int)) check_ordinary(div, in_range_ulong, to_ulong);
            if (sizeof(word_t) == 8)
            {
                check_ordinary(div_unit, in_range_uint, to_uint);
                aa.assign(a);
                UDM udm(b.to_uint());
                aa.div_unit(udm); check(aa, exp);
                div_unit(a, udm, cc); check(cc, exp);
            }
        }
        else if (oper == "%u32")
        {
            check_ordinary(mod, in_range_uint, to_uint);
            if (sizeof(long) == sizeof(int)) check_ordinary(mod, in_range_ulong, to_ulong);
            if (sizeof(word_t) == 8)
            {
                check_ordinary(mod_unit, in_range_uint, to_uint);
                aa.assign(a);
                UDM udm(b.to_uint());
                aa.mod_unit(udm); check(aa, exp);
                mod_unit(a, udm, cc); check(cc, exp);
            }
        }
        else if (oper == "&u32")
        {
            check_ordinary(bit_and, in_range_uint, to_uint);
            if (sizeof(long) == sizeof(int)) check_ordinary(bit_and, in_range_ulong, to_ulong);
            if (sizeof(word_t) == 8) check_ordinary(bit_and_unit, in_range_uint, to_uint);
        }
        else if (oper == "|u32")
        {
            check_ordinary(bit_or, in_range_uint, to_uint);
            if (sizeof(long) == sizeof(int)) check_ordinary(bit_or, in_range_ulong, to_ulong);
            if (sizeof(word_t) == 8) check_ordinary(bit_or_unit, in_range_uint, to_uint);
        }
        else if (oper == "^u32")
        {
            check_ordinary(bit_xor, in_range_uint, to_uint);
            if (sizeof(long) == sizeof(int)) check_ordinary(bit_xor, in_range_ulong, to_ulong);
            if (sizeof(word_t) == 8) check_ordinary(bit_xor_unit, in_range_uint, to_uint);
        }
        else if (oper == "+s64")
        {
            check_ordinary(add, in_range_longlong, to_longlong);
            if (sizeof(long) == sizeof(long long)) check_ordinary(add, in_range_long, to_long);
        }
        else if (oper == "-s64")
        {
            check_ordinary(sub, in_range_longlong, to_longlong);
            if (sizeof(long) == sizeof(long long)) check_ordinary(sub, in_range_long, to_long);
        }
        else if (oper == "*s64")
        {
            check_ordinary(mul, in_range_longlong, to_longlong);
            if (sizeof(long) == sizeof(long long)) check_ordinary(mul, in_range_long, to_long);
        }
        else if (oper == "/s64")
        {
            check_ordinary(div, in_range_longlong, to_longlong);
            if (sizeof(long) == sizeof(long long)) check_ordinary(div, in_range_long, to_long);
        }
        else if (oper == "%s64")
        {
            check_ordinary(mod, in_range_longlong, to_longlong);
            if (sizeof(long) == sizeof(long long)) check_ordinary(mod, in_range_long, to_long);
        }
        else if (oper == "&s64")
        {
            check_ordinary(bit_and, in_range_longlong, to_longlong);
            if (sizeof(long) == sizeof(long long)) check_ordinary(bit_and, in_range_long, to_long);
        }
        else if (oper == "|s64")
        {
            check_ordinary(bit_or, in_range_longlong, to_longlong);
            if (sizeof(long) == sizeof(long long)) check_ordinary(bit_or, in_range_long, to_long);
        }
        else if (oper == "^s64")
        {
            check_ordinary(bit_xor, in_range_longlong, to_longlong);
            if (sizeof(long) == sizeof(long long)) check_ordinary(bit_xor, in_range_long, to_long);
        }
        else if (oper == "+u64")
        {
            check_ordinary(add, in_range_ulonglong, to_ulonglong);
            if (sizeof(long) == sizeof(long long)) check_ordinary(add, in_range_ulong, to_ulong);
        }
        else if (oper == "-u64")
        {
            check_ordinary(sub, in_range_ulonglong, to_ulonglong);
            if (sizeof(long) == sizeof(long long)) check_ordinary(sub, in_range_ulong, to_ulong);
        }
        else if (oper == "*u64")
        {
            check_ordinary(mul, in_range_ulonglong, to_ulonglong);
            if (sizeof(long) == sizeof(long long)) check_ordinary(mul, in_range_ulong, to_ulong);
        }
        else if (oper == "/u64")
        {
            check_ordinary(div, in_range_ulonglong, to_ulonglong);
            if (sizeof(long) == sizeof(long long)) check_ordinary(div, in_range_ulong, to_ulong);
        }
        else if (oper == "%u64")
        {
            check_ordinary(mod, in_range_ulonglong, to_ulonglong);
            if (sizeof(long) == sizeof(long long)) check_ordinary(mod, in_range_ulong, to_ulong);
        }
        else if (oper == "&u64")
        {
            check_ordinary(bit_and, in_range_ulonglong, to_ulonglong);
            if (sizeof(long) == sizeof(long long)) check_ordinary(bit_and, in_range_ulong, to_ulong);
        }
        else if (oper == "|u64")
        {
            check_ordinary(bit_or, in_range_ulonglong, to_ulonglong);
            if (sizeof(long) == sizeof(long long)) check_ordinary(bit_or, in_range_ulong, to_ulong);
        }
        else if (oper == "^u64")
        {
            check_ordinary(bit_xor, in_range_ulonglong, to_ulonglong);
            if (sizeof(long) == sizeof(long long)) check_ordinary(bit_xor, in_range_ulong, to_ulong);
        }
        else if (oper == "po")
        {
            pow(a, b.to_int(), res);
            check(res, exp);
            a.pow(b.to_int());
            check(a, exp);
        }
        else if (oper == "+u16")
        {
            check_ordinary(add_unit, in_range_ushort, to_ushort);
        }
        else if (oper == "-u16")
        {
            check_ordinary(sub_unit, in_range_ushort, to_ushort);
        }
        else if (oper == "*u16")
        {
            check_ordinary(mul_unit, in_range_ushort, to_ushort);
        }
        else if (oper == "/u16")
        {
            check_ordinary(div_unit, in_range_ushort, to_ushort);

            UDM udm(b.to_ushort());
            aa.assign(a);
            aa.div_unit(udm); check(aa, exp);
            aa.assign(a);
            div_unit(aa, udm, cc); check(cc, exp);
        }
        else if (oper == "%u16")
        {
            check_ordinary(mod_unit, in_range_ushort, to_ushort);

            UDM udm(b.to_ushort());
            aa.assign(a);
            aa.mod_unit(udm); check(aa, exp);
            aa.assign(a);
            mod_unit(aa, udm, cc); check(cc, exp);
        }
        else if (oper == "&u16")
        {
            check_ordinary(bit_and_unit, in_range_ushort, to_ushort);
        }
        else if (oper == "|u16")
        {
            check_ordinary(bit_or_unit, in_range_ushort, to_ushort);
        }
        else if (oper == "^u16")
        {
            check_ordinary(bit_xor_unit, in_range_ushort, to_ushort);
        }

        if (chance(5)) a.release();
        if (chance(5)) b.release();
        if (chance(5)) res.release();
        if (chance(5)) exp.release();
        if (chance(5)) aa.release();
        if (chance(5)) bb.release();
        if (chance(5)) cc.release();
        if (chance(5)) q.release();
        if (chance(5)) r.release();
    }
    in.close();
    return n;
}

size_t random_test_kmul(size_t N)
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

size_t random_test_ksqr(size_t N)
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

size_t test_sqr_and_mul_performace(size_t N)
{
    clock_t t0, t1, t2;
    clock_t T0 = 0, T1 = 0;
    size_t n = N;
    number_t a, r1, r2;

    while (n--)
    {
        rand(1 + rand_word() % 36000, a);
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

size_t random_test_bit_remove(size_t N)
{
    size_t n = N;
    number_t a;
    string_t s, s0;

    while (n--)
    {
        size_t l, b, e;
        l = rand_unit() % 1024;
        rand(l, "01", s);

        b = rand_unit() % 1024;
        e = rand_unit() % 1024;
        if (b > e)
        {
            size_t t = b; b = e; e = t;
        }
        s0 = s;
        a.assign(s, 2);
        a.bit_remove(b, e);

        s.reverse();
        s.remove(b, e);
        s.reverse();
        s.strip_left("0");
        if (s == "")
        {
            s = "0";
        }
        if (a(2) != s)
        {
            cerr << "bit_remove ERROR!!" << endl;
            cerr << s0 << endl;
            cerr << b << " " << e << endl;
            abort();
        }
    }
    return N;
}

size_t random_test_bit_insert(size_t N)
{
    size_t n = N;
    number_t a;
    string_t s, s0;

    while (n--)
    {
        size_t l, bpos, size;
        l = rand_unit() % 1024;
        rand(l, "01", s);

        bpos = rand_unit() % 1024;
        size = rand_unit() % 1024;
        bool v = rand_unit() % 2? true: false;
        char cv = v? '1': '0';

        s0 = s;
        if (bpos > s.length())
        {
            s.prepend('0', bpos - s.length());
        }
        a.assign(s, 2);
        a.bit_insert(bpos, size, v);

        s.reverse();
        s.insert(bpos, cv, size);
        s.strip_right("0");
        s.reverse();

        if (s.empty())
        {
            s = "0";
        }
        if (a(2) != s)
        {
            cerr << "bit_insert ERROR!!" << endl;
            cerr << s0 << endl;
            cerr << bpos << " " << size << " " << v << endl;
            abort();
        }
    }
    return N;
}

void random_test_bit_set_pos()
{
    number_t a;
    string_t s, s0;
    size_t l, pos;
    l = rand_word() % 1024;
    rand(l, "01", s);

    pos = rand_word() % 1024;
    bool v = rand_word() % 2? true: false;
    char cv = v? '1': '0';

    s0 = s;
    a.assign(s, 2);
    a.bit_set(pos, v);
    if (pos >= s.length())
    {
        s.prepend('0', pos + 1 - s.length());
    }
    s.reverse();
    s[pos] = cv;
    s.reverse();
    s.strip_left("0");

    if (s.empty())
    {
        s = "0";
    }
    if (a(2) != s)
    {
        cerr << "bit_set pos ERROR!!" << endl;
        cerr << s0.strip_left("0") << endl;
        cerr << pos << " " << v << endl;
        cerr << a(2) << endl;
        cerr << s << endl;
        abort();
    }
}

void random_test_bit_set_range()
{
    number_t a;
    string_t s, s0;
    size_t l, bpos, epos;
    l = rand_word() % 32;
    rand(l, "01", s);

    bpos = rand_word() % 32;
    epos = rand_word() % 32;
    if (bpos > epos)
    {
        size_t t = bpos; bpos = epos; epos = t;
    }
    bool v = rand_word() % 2? true: false;
    char cv = v? '1': '0';

    s0 = s;
    a.assign(s, 2);
    a.bit_set(bpos, epos, v);

    if (epos > s.length())
    {
        s.prepend('0', epos - s.length());
    }

    s.reverse();
    if (chance(2))  // two ways to test
    {
        for (size_t pos = bpos; pos != epos; pos++)
        {
            s[pos] = cv;
        }
    }
    else
    {
        string_t s1, s2;
        s1.assign(s, 0, bpos);
        s2.assign(s, epos, s.end_pos());
        s1.append(cv, epos - bpos);
        s = s1 + s2;
    }
    s.reverse();
    s.strip_left("0");

    if (s.empty())
    {
        s = "0";
    }
    if (a(2) != s)
    {
        cerr << "bit_set range ERROR!!" << endl;
        cerr << s0.strip_left("0") << endl;
        cerr << bpos << " " << epos << " "  << v << endl;
        cerr << a(2) << endl;
        cerr << s << endl;
        abort();
    }
}

size_t random_test_bit_set(size_t N)
{
    size_t n = N;

    while (n--)
    {
        random_test_bit_set_pos();
        random_test_bit_set_range();
    }
    return N;
}

size_t random_test_bit_shift_xor(size_t N)
{
    size_t n = N;
    number_t a, aa, b, res1, res2;

    while (n--)
    {
        rand(rand_word() % 1024, a);
        rand(rand_word() % 1024, b);
        size_t shift = rand_word() % 1024;
        aa = a;
        res1 = a ^ (b << shift);

        bit_shift_xor(a, b, shift, res2);
        if (res2 != res1)
        {
            cerr << "bit_shift_xor ERROR!!" << endl;
            abort();
        }
        bit_shift_xor(a, b, shift, a);
        if (a != res1)
        {
            cerr << "bit_shift_xor(a, b, shift, a) ERROR!!" << endl;
            abort();
        }
        bit_shift_xor(aa, b, shift, b);
        if (b != res1)
        {
            cerr << "bit_shift_xor(a, b, shift, b) ERROR!!" << endl;
            abort();
        }
    }
    return N;
}

size_t random_test_bit_shift_or(size_t N)
{
    size_t n = N;
    number_t a, aa, b, res1, res2;

    while (n--)
    {
        rand(rand_word() % 1024, a);
        rand(rand_word() % 1024, b);
        size_t shift = rand_word() % 1024;
        aa = a;
        res1 = a | (b << shift);

        bit_shift_or(a, b, shift, res2);
        if (res2 != res1)
        {
            cerr << "bit_shift_or ERROR!!" << endl;
            abort();
        }
        bit_shift_or(a, b, shift, a);
        if (a != res1)
        {
            cerr << "bit_shift_or(a, b, shift, a) ERROR!!" << endl;
            abort();
        }
        bit_shift_or(aa, b, shift, b);
        if (b != res1)
        {
            cerr << "bit_shift_or(a, b, shift, b) ERROR!!" << endl;
            abort();
        }
    }
    return N;
}

size_t random_test_string_insert(size_t N)
{
    size_t n = N;
    string_t chars = " \tabcdefghijklmnopqrstuvwxyz";
    string_t a, b, s, t, u;
    while (n--)
    {
        rand(rand_word() % 128, chars, s);
        rand(rand_word() % 128, chars, t);
        rand(rand_word() % 128, chars, u);

        a = s + t;
        b = s + u + t;
        if (a.insert(s.length(), u) != b)
        {
            cerr << "string_t::insert ERROR!!" << endl;
            abort();
        }
        if (chance(5)) a.release();
        if (chance(5)) b.release();

        a = b = s;
        a.append(t);
        b.insert(b.end_pos(), t);
        if (a != b)
        {
            cerr << "string_t::append ERROR!!" << endl;
            abort();
        }
    }
    return N;
}

void replace_all(const string_t& s, const string_t& from, const string_t& to, string_t& res)
{
    res.clear();
    if (from.length())
    {
        size_t p0 = 0, p1;
        res.reserve(s.length());
        while ((p1 = s.find(p0, from)) != string_t::npos)
        {
            res.append(s, p0, p1);
            res.append(to);
            p0 = p1 + from.length();
        }
        res.append(s, p0, s.end_pos());
    }
    else
    {
        res.reserve(s.length() * to.length());
        res.append(to);
        char* p = s.dat;
        char* e = s.dat + s.length();
        while (p != e)
        {
            res.append(*p++);
            res.append(to);
        }
    }
}

size_t random_test_string_find(size_t N)
{
    size_t n = N;
    string_t chars = " \tabcdefghijklmnopqrstuvwxyz";
    string_t a, b, s, t, u;
    while (n--)
    {
        rand(rand_word() % 128, chars, s);
        rand(rand_word() % 128, chars, t);
        rand(rand_word() % 128, chars, u);

        a = s + t;
        b = s + u + t;
        if (a.insert(s.length(), u) != b)
        {
            cerr << "string_t::insert ERROR!!" << endl;
            abort();
        }
        if (chance(5)) a.release();
        if (chance(5)) b.release();

        a = b = s;
        a.append(t);
        b.insert(b.end_pos(), t);
        if (a != b)
        {
            cerr << "string_t::append ERROR!!" << endl;
            abort();
        }
    }
    return N;
}

#define test_with_time(title, fun) { \
    cout << title << endl; \
    clock_t t0 = clock(); \
    size_t n = fun; \
    cout << "OK!" << endl; \
    cout << "time: " << double(clock() - t0) / CLOCKS_PER_SEC << endl; \
    cout << n << " items tested" << endl << endl; \
}

#define test_from_file(title, filename, fun) { \
    ifstream in(filename); \
    if (in) { \
        cout << title << endl; \
        clock_t t0 = clock(); \
        size_t n = fun(in); \
        cout << "OK!" << endl; \
        cout << "time: " << double(clock() - t0) / CLOCKS_PER_SEC << endl; \
        cout << n << " items tested" << endl << endl; \
    } \
}

#define TIMES(n) for (int i = 0; i < n; i++)

size_t random_test_fmul(size_t N)
{
    number_t a, b, res1, res2;
    clock_t t1 = 0, t2 = 0;
    size_t n = N, units0, units1, bits0, bits1;

    t1 = clock();
    NTT::init_roots_pool();
    t2 = clock();
    cout << "init_roots_pool:" << double(t2 - t1) / CLOCKS_PER_SEC << endl;

    while (n--)
    {
        if (sizeof(size_t) == 4)
        {
            units0 = 1 + rand_word() % 16383;
            units1 = 1 + rand_word() % 16383;
        }
        else
        {
            units0 = 2 + rand_word() % 524288;
            units1 = 2 + rand_word() % 524288;
        }

        bits0 = units0 * sizeof(unit_t) * 8;
        bits1 = units1 * sizeof(unit_t) * 8;
        rand(bits0, default_RNG(), true, a);
        rand(bits1, default_RNG(), true, b);
        cout << bits0 << "*" << bits1 << endl;

        t1 = clock();
        kmul(a, b, res1);
        t2 = clock();
        cout << "kmul:" << double(t2 - t1) / CLOCKS_PER_SEC << endl;

        t1 = clock();
        fmul(a, b, res2);
        t2 = clock();
        cout << "fmul:" << double(t2 - t1) / CLOCKS_PER_SEC << endl;

        if (!eq(res1, res2))
        {
            cout << "NEQ!" << endl;
            break;
        }
        cout << endl;
    }
    return N;
}

size_t random_test_fsqr(size_t N)
{
    number_t a, res1, res2;
    clock_t t1 = 0, t2 = 0;
    size_t n = N, units, bits;

    t1 = clock();
    NTT::init_roots_pool();
    t2 = clock();
    cout << "init_roots_pool:" << double(t2 - t1) / CLOCKS_PER_SEC << endl;

    while (n--)
    {
        if (sizeof(size_t) == 4)
        {
            units = 1 + rand_word() % 16383;
        }
        else
        {
            units = 2 + rand_word() % 524288;
        }
        bits = units * sizeof(unit_t) * 8;
        rand(bits, default_RNG(), true, a);
        cout << bits << "*" << bits << endl;

        t1 = clock();
        ksqr(a, res1);
        t2 = clock();
        cout << double(t2 - t1) / CLOCKS_PER_SEC << endl;

        t1 = clock();
        fsqr(a, res2);
        t2 = clock();
        cout << double(t2 - t1) / CLOCKS_PER_SEC << endl;

        if (!eq(res1, res2))
        {
            cout << "NEQ!" << endl;
            break;
        }
        cout << endl;
    }
    return N;
}

int main()
{
    test_from_file("Testing basic operations", "randomtest.dat", random_test_basic);
    test_from_file("Testing division", "divrandomtest.dat", random_test_div);
    test_from_file("Testing multiplication", "mulrandomtest.dat", random_test_mul);
    test_from_file("Testing modular exponentiation", "pomrandomtest.dat", random_test_pom);
    test_from_file("Testing GCD", "gcdrandomtest.dat", random_test_gcd);
    test_from_file("Testing GCD Ext", "gcdextrandomtest.dat", random_test_gcd_ext);
    test_from_file("Testing LCM", "lcmrandomtest.dat", random_test_lcm);
    test_from_file("Testing prime", "primerandomtest.dat", random_test_prime);
    test_from_file("Testing Jacobi", "jacobirandomtest.dat", random_test_jacobi);
    test_with_time("Testing bit_remove", random_test_bit_remove(30000));
    test_with_time("Testing bit_insert", random_test_bit_insert(30000));
    test_with_time("Testing bit_set", random_test_bit_set(30000));
    test_with_time("Testing bit_shift_xor", random_test_bit_shift_or(300000));
    test_with_time("Testing bit_shift_or", random_test_bit_shift_xor(300000));
    test_with_time("Testing string insert", random_test_string_insert(300000));

    test_with_time("Testing sqr and mul", test_sqr_and_mul_performace(5000));
    test_with_time("Testing kmul", random_test_kmul(1000));
    test_with_time("Testing ksqr", random_test_ksqr(1000));
    test_with_time("Testing fmul", random_test_fmul(30));
    test_with_time("Testing fsqr", random_test_fsqr(30));

    return 0;
}
