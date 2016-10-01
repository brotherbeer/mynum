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
 * [Use Machin's formula to compute PI](#use-machin's-formula-to-compute-pi)
 * [Compute the decimal places of the natural logarithms base E](#compute-the-decimal-places-of-the-natural-logarithms-base-e)

No restrictions on the dissemination and modification of this source code, but should accord with socially beneficial purpose. 

This library is distributed in the hope that it will be useful, but without any warranty.

If you have any questions, please contact <brotherbeer@163.com>

`NOTICE!! This library is currently only available on the LITTLE-ENDIAN machines.`

##Installation
[mynum.h](https://github.com/brotherbeer/mynum/blob/master/mynum.h) and [mynum.cpp](https://github.com/brotherbeer/mynum/blob/master/mynum.cpp) are the essential files the library required, and other files are the expansion of core functions.

Include [mynum.h](https://github.com/brotherbeer/mynum/blob/master/mynum.h) and [mynum.cpp](https://github.com/brotherbeer/mynum/blob/master/mynum.cpp) into your project, and #include "mynum.h" in whichever file you need big integer operations.

[myoperators.h](https://github.com/brotherbeer/mynum/blob/master/operators.h) overloaded the associated C++ operators, if you want to use the operators to do the calculations, please #include "myoperators.h"

[test.cpp](https://github.com/brotherbeer/mynum/blob/master/test.cpp) contained the examples and test cases

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
number_t f("HIJKLMN", x);   // use a string that represents arbitrary based number, x between [2, 36]
```
When using hexadecimal string, the efficiency is highest. The max base is 36.

In order to achieve a higher efficiency, mynum does not consider any prefix, such as 0x, 0b, and the constructor does not detect any wrong char in the string parameter. 

If the string parameter is wrong, then the value of object is wrong too, but will not crash.

If you want to test the correctness of the string, you can use the 'check' function, for example:
```C++
const char* s = "1234567890";
assert(check(s, 10) > 0); // if s represents a decimal number, then s is correct
assert(check(s, 8) == 0); // if s represents an octal number, then s is wrong
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
add(a, b, c);  // add a and b, c is the result

c.add(a);      // add a to c

c = add(a, b); // return object

c = a + b;     // overloaded operator +, need myoperators.h

c += a;        // overloaded operator +=

++c++;         // increase 1
```

##Subtraction
```C++
number_t a = 123, b = 456, c;
sub(a, b, c);  // c is the result of a - b

c.sub(a);      // the same to c -= a

c = sub(a, b); // return object

c = a - b;     // overloaded operator -, need myoperators.h

c -= a;

--c--;         // decrease 1
```

##Multiplication
```C++
number_t a = 123, b = 456, c;
mul(a, b, c);  // c is the result of a * b

c.mul(a);      // the same to c *= a

kmul(a, b, c); // using karatsuba algorithm

c = mul(a, b);

c = a * b;     // overloaded operator *, using kmul

c *= a;
```

##Division
```C++
number_t a = 123, b = 456, q, r;
div(a, b, q);     // a is the dividend, b is the divisor, q is the quotient

div(a, b, q, r);  // r is the remainder

a.div(b);

a.div(b, r);

q = div(a, b);   // return object

q = a / b;       // overloaded operator /, need myoperators.h

a /= b;
```

##Modulo operation
```C++
number_t a = 123, b = 456, q, r;

mod(a, b, r);    // r is the remainder after division of a by b

r = mod(a, b);   // return object

r = a % b;       // overloaded operator %, need myoperators.h

a %= b;
```

##Exponentiation
```C++
number_t a = 123, b = 456, c;

sqr(a, c);      // set c to a * a;

ksqr(a, c);     // using karatsuba algorithm

pow(a, b, c);   // set c to a raised to the power of b, it is required that b > 0

c = pow(a, b);
```

##Modular exponentiation
```C++
number_t a = 123, b = 456, c = 678, d;

pom(a, b, c, d);  // set d to a raised to b modulo c
```

##Root
```C++
number_t a = 123456, b;

sqrt(a, b);       // b is the square root
```

##Comparison
```C++
number_t a = 123, b = 456;
cmp(a, b) < 0;   // a < b
cmp(a, b) > 0;   // a > b
cmp(a, b) == 0;  // a == b 

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
bit_and(a, b, c);  // bitwise and, c is the result
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

a.to_string(36);  // convert a to base-36 string
a.to_string(19);  // convert a to base-19 string
a.to_string(16);  // convert a to base-16 string
a.to_string(10);  // convert a to base-10 string
a.to_string(8);   // convert a to base-8 string
a.to_string(2);   // convert a to base-2 string
```
Use follow methods for higher efficiency:

 * to_bin_string() to get base-2 string
 * to_oct_string() to get base-8 string
 * to_dec_string() to get base-10 string
 * to_hex_string() to get base-16 string

in the above methods, the efficiency of to_hex_string() is the highest.

Currently, the max base supported is 36, it will be extended in the subsequent version,
you can use max_base() to obtain the max base supported

`NOTICE!! never use a.to_string(0), a.to_string(1) and a base larger than max_base() returned`

##Absolute value
```C++
number_t a = -123;
number_t b = abs(a);  // set b to the absolute value of a

b = a.abs();    // equals to abs(a)

a.set_abs();    // set a to its absolute value
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
a.is_even();   // return true if a is an even number

a.is_not(b);   // return true if a and b are not same object

a.is_odd();    // return true if a is an odd number

a.is_one();    // return true if a is 1

a.is_power2(); // return true if a is the n-th power of 2 (n >= 0)

a.is_zero();   // return true if a is 0
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

a.to_string(s, x);   // convert the value to string based x 
```

##Other utils
```C++
number_t a = 123, b = 456;

swap(a, b);      // swap a and b, after this function called, a is 456, b is 123

a.steal(b);      // a steals the data of b, so a gets the memory of b, and b is set 0

a.set_one();     // set a 1

a.set_zero();    // set a 0

a.set_sign(x);   // if x is 1 then set a to be positive, if x is -1 then set a to be negative

sign(a);         // if a > 0 return 1 else return -1

same_sign(a, b); // if a and b have the same sign, return 1, else return 0
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
        m
