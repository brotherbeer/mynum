MYNUM LIBARAY
-------------

This C++ library provides operations of big integers. Compared to the Java, python,
ruby and other well-known big integer library, mynum is more efficient!

operations:

 * [Initialization](#initialization)
 * [Addition](#addition)
 * [Subtraction](#subtraction)
 * [Multiplication](#multiplication)
 * [Division](#division)
 * [Modulo operation](#modulo-operation)
 * [Exponentiation](#exponentiation)
 * [Modular exponentiation](#modular-exponentiation)
 * [Root](#root)
 * [Comparison](#comparison)
 * [Bits operations](#bits-operations)
 * [String convertion](#string-convertion)

and other utilities:

 * [Absolute value](#absolute-value)
 * [Negative value](#negative-value)
 * [Number property](#number-property)
 * [String](#string)
 * [Other utils](#other-utils)

examples:

 * [Compute the greatest common divisor](#compute-the-greatest-common-divisor)
 * [Use Machin's formula to compute PI](#use-machin's-formula-to-compute-PI)
 * [Compute the decimal places of the natural logarithms base E](#compute-the-decimal-places-of-the-natural-logarithms-base-E)

No restrictions on the dissemination and modification of this source code, but should accord with socially beneficial purpose. 

This library is distributed in the hope that it will be useful, but without any warranty.

If you have any questions, please contact <brotherbeer@163.com>

`NOTICE!! This library is currently only available on the LITTLE-ENDIAN machines.`

##Installation
[mynum.h](https://github.com/brotherbeer/mynum/blob/master/mynum.h) and [mynum.cpp](https://github.com/brotherbeer/mynum/blob/master/mynum.cpp) are the essential files the library required, and other files are the expansion of core functions.

Include [mynum.h](https://github.com/brotherbeer/mynum/blob/master/mynum.h) and [mynum.cpp](https://github.com/brotherbeer/mynum/blob/master/mynum.cpp) into your project, and #include "mynum.h" in whichever file you need big integer operations.

[myoperators.h](https://github.com/brotherbeer/mynum/blob/master/operators.h) overloaded the associated C++ operators, if you want to use the operators to do the calculations, please #include "myoperators.h"

mynum can also be compiled into a dynamic library, for example:

`g++ -fPIC -shared -O2 -DNDEBUG mynum.cpp -o mynum.so`

The source code is currently applicable to g++, MSVC(2008 and above version).

##Initialization
The namespace is `'mynum'`, and the class name of big integer is `'number_t'`.
```C++
#include "mynum.h"
using namespace mynum;

number_t a = 123, b(789L);  // use basic integer type to initialize number_t object
number_t c("271828182845"); // use a string that represents decimal number
number_t d("abcdef", 16);   // use a string that represents hexadecimal number
number_t e("1111100", 2);   // use a string that represents binary number
number_t f("HIJKLMN", x);   // use a string that represents arbitrary based number, _x_ between [2, 36]
```
When using hexadecimal string, the efficiency is highest. The max base is 36.

In order to achieve a higher efficiency, mynum does not consider any prefix, such as 0x, 0b, and the constructor does not detect any wrong char in the string parameter. 

If the string parameter is wrong, then the value of object is wrong too, but will not crash.

If you want to test the correctness of the string, you can use the 'check' function, for example:
```C++
const char* s = "1234567890";
assert(check(s, 10) > 0); // if _s_ represents a decimal number, then _s_ is correct
assert(check(s, 8) == 0); // if _s_ represents an octal number, then _s_ is wrong
```
Function 'check' returns the string length if the string is right, or 0 if the string is wrong.

When the base is greater than 10, 'a' means 10, 'b' means 11, and so on, 'z' means 35, the chars are case insensitive.

The value of object defined by the default constructor is 0
```C++
number_t a;
assert(a == 0);   // == defined in myoperators.h
```

The number_t object required memory is allocated on the heap, call the destructor manually to deallocate the memory space
```C++
number_t a(123456LL);
a.~number_t();
assert(a == 0);
```

##Addition
```C++
number_t a = 123, b = 456, c;
add(a, b, c);  // add _a_ and _b_, _c_ is the result

c.add(a);      // add _a_ to _c_

c = add(a, b); // return object

c = a + b;     // overloaded operator +, need myoperators.h

c += a;        // overloaded operator +=

++c++;         // increase 1
```

##Subtraction
```C++
number_t a = 123, b = 456, c;
sub(a, b, c);  // _c_ is the result of _a_ - _b_

c.sub(a);      // the same to _c_ -= _a_

c = sub(a, b); // return object

c = a - b;     // overloaded operator -, need myoperators.h

c -= a;

--c--;         // decrease 1
```

##Multiplication
```C++
number_t a = 123, b = 456, c;
mul(a, b, c);  // _c_ is the result of _a_ * _b_

c.mul(a);      // the same to _c_ *= _a_

kmul(a, b, c); // using karatsuba algorithm

c = mul(a, b);

c = a * b;     // overloaded operator *, using kmul

c *= a;
```

##Division
```C++
number_t a = 123, b = 456, q, r;
div(a, b, q);     // _a_ is the dividend, _b_ is the divisor, _q_ is the quotient

div(a, b, q, r);  // _r_ is the remainder

a.div(b);

a.div(b, r);

q = div(a, b);   // return object

q = a / b;       // overloaded operator /, need myoperators.h

a /= b;
```

##Modulo operation
```C++
number_t a = 123, b = 456, q, r;

mod(a, b, r);    // _r_ is the remainder after division of _a_ by _b_

r = mod(a, b);   // return object

r = a % b;       // overloaded operator %, need myoperators.h

a %= b;
```

##Exponentiation
```C++
number_t a = 123, b = 456, c;

sqr(a, c);      // set _c_ to _a_ * _a_;

ksqr(a, c);     // using karatsuba algorithm

pow(a, b, c);   // set _c_ to _a_ raised to the power of _b_, it is required that _b_ > 0

c = pow(a, b);
```

##Modular exponentiation
```C++
number_t a = 123, b = 456, c = 678, d;

pom(a, b, c, d);  // set _d_ to _a_ raised to _b_ modulo _c_
```

##Root
```C++
number_t a = 123456, b;

sqrt(a, b);       // _b_ is the square root
```

##Comparison
```C++
number_t a = 123, b = 456;
cmp(a, b) < 0;   // _a_ < _b_
cmp(a, b) > 0;   // _a_ > _b_
cmp(a, b) == 0;  // _a_ == _b_

eq(a, b);        // equal
lt(a, b);        // less than
gt(a, b);        // greater than
elt(a, b);       // equal or less than
egt(a, b);       // equal or greater than

a < b;           // overloaded operators, need myoperators.h
a > b;
a == b;
a >= b;
a <= b;
```

##Bits operations
```C++
number_t a = 123, b = 456, c;
bit_and(a, b, c);  // bitwise and, _c_ is the result
bit_or(a, b, c);   // bitwise or
bit_xor(a, b, c);  // bitwise exclusive or
bit_not(a, c);     // bitwise not

a.bit_and(b);
a.bit_or(b);
a.bit_xor(b);
a.bit_not();

c = a & b;         // overloaded operators, need myoperators.h
c = a | b;
c = a ^ b;
c = ~a;

a &= b;
a |= b;
a ^= b;

shl(a, 5, c);     // left shift 5 bits
shr(a, 5, c);     // right shift 5 bits 

c = a << 5;       // overloaded operators, need myoperators.h
c = a >> 5;
a <<= 5;
a >>= 5;
```

##String convertion
```C++
number_t a = 123;

a.to_string(36);  // convert _a_ to base-36 string
a.to_string(19);  // convert _a_ to base-19 string
a.to_string(16);  // convert _a_ to base-16 string
a.to_string(10);  // convert _a_ to base-10 string
a.to_string(8);   // convert _a_ to base-8 string
a.to_string(2);   // convert _a_ to base-2 string
```
Use follow methods for higher efficiency:

 * to_bin_string() to get base-2 string
 * to_oct_string() to get base-8 string
 * to_dec_string() to get base-10 string
 * to_hex_string() to get base-16 string

in the above methods, the efficiency of to_hex_string() is the highest.

Currently, the max base supported is 36, it will be extended in the subsequent version,
you can use max_base() to obtain the max base supported

`NOTICE!! never use m.to_string(0), m.to_string(1) and a base larger than max_base() returned`

##Absolute value
```C++
number_t a = -123;
number_t b = abs(a);  // set _b_ to the absolute value of _a_

b = a.abs();    // equals to abs(_a_)

a.set_abs();    // set _a_ to its absolute value
```

##Negative value
```C++
number_t a = -123;
number_t b = neg(a);

b = a.neg();

a.set_neg();
```

##Number property
```C++
number_t a = 123, b = a;
a.is_even();   // return true if _a_ is an even number

a.is_not(b);   // return true if _a_ and _b_ are not same object

a.is_odd();    // return true if _a_ is an odd number

a.is_one();    // return true if _a_ is 1

a.is_power2(); // return true if _a_ is the _n_-th power of 2 (n >= 0)

a.is_zero();   // return true if _a_ is 0
```

##String
```C++
number_t a = 0xABCD;
string_t s;                // define a string object
s = a.to_hex_string();     // return the object, lower efficiency
assert(s == "abcd");             // lowercase is default
assert(s.to_upper() == "ABCD");  // to upper case

a.to_bin_string(s);  // to string based 2

a.to_oct_string(s);  // to string based 8

a.to_dec_string(s);  // to string based 10

a.to_hex_string(s);  // to string based 16

a.to_string(s);      // convert the value to string, base 10 is default 

a.to_string(s, x);   // convert the value to string based _x_ 
```

##Other utils
```C++
number_t a = 123, b = 456;

swap(a, b);      // swap _a_ and _b_, after this function called, _a_ is 456, _b_ is 123

a.steal(b);      // _a_ steals the data of _b_, so _a_ gets the memory of _b_, and _b_ is set 0

a.set_one();     // set _a_ to 1

a.set_zero();    // set _a_ to 0

a.set_sign(x);   // if _x_ is 1 then set _a_ to be positive, if _x_ is -1 then set _a_ to be negative

sign(a);         // if _a_ > 0 return 1 else return -1

same_sign(a, b); // if _a_ and _b_ have the same sign, return 1, else return 0
```

#Examples:

##Compute the greatest common divisor 
```C++
#include "myoperators.h"
using namespace mynum;

/*
 * compute the greatest common divisor with division algorithm
 */
void gcd_example()
{
    number_t m("3149916521386303663457"), n("97950481"), t;
    while (m % n != 0)
    {
        mod(m, n, t);
        m.steal(n);
        n.steal(t);
    }
    assert(n == 19937);
}
```

##Use Machin's formula to compute PI
```C++
#include "myoperators.h"
using namespace mynum;

/*
 * rearctan1 and rearctan2 have the same function,
 * they use Maclaurin expansion to get the product of arctan(1/_x_) and the _n_-th power of 10
 *
 * rearctan1 uses the overloaded operators, rearctan1 uses the normal APIs
 * operator overloading makes the source code briefer, but the efficiency is slightly lower
 * the two functions are used by PI_example() to compute the circumference ratio PI
 * the example obtaines 100 decimal places of PI, readers can set _n_ to their interested value,
 * and get the result
 */
void rearctan1(int x, int n, number_t& res);
void rearctan2(int x, int n, number_t& res);

/*
 *  Use Machin's formula to compute PI
 *  PI = 16 * arctan(1/5) - 4 * arctan(1/239)
 *  The last few decimal places may be wrong, increase _n_ to obtain higher accuracy
 */
void PI_example()
{
    int n = 100;
    number_t t0, t1;

    rearctan1(5, n, t0);
    t0 <<= 2;
    rearctan2(239, n, t1);
    t0 -= t1;
    t0 <<= 2;

    // the result
    assert (t0.to_dec_string() == "31415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170712");
}

void rearctan1(int x, int n, number_t& res)
{
    int k = 1;
    number_t u, v, xx(x * x);

    res = 10;
    res.pow(n).div(x);
    u = res;

    do
    {
        u /= xx;
        v = u / (2 * k + 1);
        v *= ((k & 1) * -1) | 1;
        res += v;
        k++;
    } while (v);
}

void rearctan2(int x, int n, number_t& res)
{
    int k = 1;
    number_t u, v, xx(x * x);

    res = 10;
    res.pow(n).div(x);
    u = res;

    do
    {
        u.div(xx);
        div(u, 2 * k + 1, v);
        v.set_sign(0 - (k & 1));
        res.add(v);
        k++;
    } while (v);
}
```

##Compute the decimal places of the natural logarithms base E
```C++
#include "myoperators.h"
using namespace mynum;

/*
 *  This example computes the decimal places of the natural logarithms base E
 *  The last few decimal places may be wrong, increase _n_ to obtain higher accuracy
 */
void E_example()
{
    // unit_t is the basic computing unit type of mynum
    // on 64bit systems, unit_t is unsigned int
    // on 32bit systems, unit_t is unsigned short
    // so the value of the unit_t type variable is between [0, 4294967295] on 64bit systems,
    // or [0, 65535] on 32bits systems
    // when the computing relates to small integers which can be hold by unit_t,
    // unit_t operations will be more efficient

    unit_t n = 100, i = 2;
    number_t x = 10, e;
    x.pow(n);

    while (x)
    {
        x.div_unit(i++);  // faster than x.div(i++)
        e.add(x);
    }

    // the result
    assert(e.to_string() == "7182818284590452353602874713526624977572470936999595749669676277240766303535475945713821785251664238");
}
```

other unit_t operations:
 * add_unit(unit_t x)
 * sub_unit(unit_t x)
 * mul_unit(unit_t x)
 * mod_unit(unit_t x)

the macro UNITBITS indcates how many bits are in the unit_t 

use precompiled instructions to determine the range of unit_t variable values
```C++
#if UNITBITS == 16
//max 65535
#elif UNITBITS == 32
//max 4294967295
#endif
```


more examples in [test.cpp](https://github.com/brotherbeer/mynum/blob/master/test.cpp)

