MYNUM
-------------

Mynum is a portable library for big integer arithmetic, it aims to provide convenience for the research of number theory，cryptology and other fields.

mynum is currently only available on the LITTLE-ENDIAN machines.

In the latest release version, mynum provides:

 * [Initialization](#initialization)
 * [Addition](#addition)
 * [Subtraction](#subtraction)
 * [Multiplication](#multiplication)
 * [Division](#division)
 * [Modulo operation](#modulo-operation)
 * [Exponentiation](#exponentiation)
 * [Modular exponentiation](#modular-exponentiation)
 * [Comparison](#comparison)
 * [Bitwise operation](#bitwise-operation)
 * [String conversion](#string-conversion)
 * [Other utils](#other-utils)

The efficiency of mynum is not inferior to GMP and other well-known library too much, but the interfaces are much simpler.
Compared to the built-in big integer libaraies of java, python, etc., mynum is even more efficient.

No restrictions on the dissemination and modification of the source code. The author hopes mynum will be useful, but dose not make any warranty.

If you have any questions, please contact <brotherbeer@163.com>

[mynumheaderfile]: https://github.com/brotherbeer/mynum/blob/master/mynum.h
[mynumcppfile]: https://github.com/brotherbeer/mynum/blob/master/mynum.cpp
[myoperatorheaderfile]: https://github.com/brotherbeer/mynum/blob/master/operators.h
[testcppfile]: https://github.com/brotherbeer/mynum/blob/master/test.cpp

##Installation
[mynum.h][mynumheaderfile] and [mynum.cpp][mynumcppfile] are the essential files mynum required, and other files are the expansion of the core functions.

Include [mynum.h][mynumheaderfile] and [mynum.cpp][mynumcppfile] into your project, and #include "mynum.h" in whichever file you need big integer operations.

[myoperators.h][myoperatorheaderfile] overloaded the associated C++ operators, if you want to use the operators to do the calculations, please #include "myoperators.h"

[test.cpp][testcppfile] contained the examples and test cases

mynum can also be compiled into a dynamic library, for example:

`g++ -fPIC -shared -O2 -DNDEBUG mynum.cpp -o mynum.so`

The source code is currently applicable to g++, MSVC(2008 and above versions).

##Initialization
```C++
#include "mynum.h"
using namespace mynum;      // the namespace of mynum

number_t a = 123, b(789L);  // use basic integer type to initialize number_t objects
number_t c("27182818284");  // use a decimal string
number_t d("-abcdef", 16);  // use a hexadecimal number
number_t e("1234567", 8);   // use an octal string
number_t f("1111100", 2);   // use a binary string
```
When using a string denoting a hexadecimal intger, the efficiency is the highest.

In order to achieve higher efficiency, when constructing from string, mynum does not consider any prefixes, such as "0x", "0b", and all the constructors don't detect any wrong char in the string parameter.
If the string parameter is wrong, the value of the object is wrong too, but the program will not crash. For example:
```C++
number_t x("123456789", 8);   // in octal number, '8' and '9' are wrong, so the value of x is wrong
number_t y("1,234,567", 8);   // ',' and other punctuations are not acceptable
number_t z("1 234 567", 8);   // the blank spaces are not acceptable
```
In the later versions, mynum will optimize string related functions.

If you want to test the correctness of the string, you can use the 'check' function, for example:
```C++
const char* s = "1234567890";
assert(check(s, 10) > 0); // if s denotes a decimal number, then s is correct
assert(check(s, 8) == 0); // if s denotes an octal number, then s is wrong
```
The function 'check' returns the string length if the string is right, or 0 if the string is wrong.

When the base is greater than 10, 'a' means 10, 'b' means 11, and so on, 'z' means 35, the max base is 36, and the chars are case insensitive.

The value of the object constructed by the default constructor is 0
```C++
number_t a;
assert(a == 0);   // == defined in myoperators.h
```

The object memory is allocated on the heap, call the member function 'clear' to deallocate.
```C++
number_t a(123456LL);
a.clear();
assert(a == 0);
```

##Addition
```C++
number_t a = 123, b = 456, c;
add(a, b, c);  // add a and b, c is the result

c.add(a);      // add a to c

c = add(a, b); // return result as an object

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

##Bitwise operation
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

##String conversion
```C++
number_t a = 123;

a.to_string(36);  // convert to a string in base 36
a.to_string(19);  // in base 19
a.to_string(16);  // in base 16
a.to_string(10);  // in base 10
a.to_string(8);   // in base 8
a.to_string(2);   // in base 2
```
Use follow methods for higher efficiency:

 * to_bin_string() to get a string in base 2
 * to_oct_string() to get a string in base 8
 * to_dec_string() to get a string in base 10
 * to_hex_string() to get a string in base 16

in the above methods, to_hex_string() is the fastest.

Currently, the max base supported is 36, and the min base is 2, it will be extended in the subsequent version,
you can use max_base() to obtain the max base supported

`NOTICE!! never use a.to_string(0), a.to_string(1) and a base larger than max_base() returned`

##Other utils
####Absolute value
```C++
number_t a = -123;
number_t b = abs(a);  // set b to the absolute value of a

b = a.abs();    // equals to abs(a)

a.set_abs();    // set a to its absolute value
```

####Negative value
```C++
number_t a = -123;
number_t b = neg(a);

b = a.neg();

a.set_neg();
```

####Number property
```C++
number_t a = 123, b = a;
a.is_even();   // return true if a is an even number

a.is_not(b);   // return true if a and b are not same object

a.is_odd();    // return true if a is an odd number

a.is_one();    // return true if a is 1

a.is_po2();    // return true if a is the n-th power of 2 (n >= 0)

a.is_zero();   // return true if a is 0
```

####String
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

####Swap and sign
```C++
number_t a = 123, b = 456;

swap(a, b);      // swap a and b

a.steal(b);      // a steals the data of b, so a gets the memory of b, and b is set to 0

a.set_one();     // set a to 1

a.set_zero();    // set a to 0

a.set_sign(x);   // if x is 1 then set a to be positive, if x is -1 then set a to be negative

sign(a);         // if a >= 0 return 1 else return -1

same_sign(a, b); // if a and b have the same sign, return 1, else return 0
```

