MYNUM LIBARAY
-------------

This C++ library provides operations of big integers. Compared to the Java, python,
ruby and other well-known big integer library, mynum has a higher efficiency!

operations:

 * [Initialization](#initialization)
 * [Addition](#addition)
 * [Subtraction](#subtraction)
 * [Multiplication](#multiplication)
 * [Division](#division)
 * [Modulo](#modulo)
 * [Power](#power)
 * [Power modulo](#power-modulo)
 * [Root](#root)
 * [Comparison](#comparison)
 * [Bits operations](#bits-operations)
 * [String convertion](#string-convertion)

and other utilities:

 * [absolute value](#absolute-value)
 * [negative value](#negative-value)
 * [number property](#number-property)

No restrictions on the dissemination and modification of this source code,but should accord with socially beneficial purpose. 

This library is distributed in the hope that it will be useful, but without any warranty,

if you have any questions, please contact <brotherbeer@163.com>

`NOTICE!! This library is currently only available on the LITTLE-ENDIAN machines.`

##Installation
[mynum.h](https://github.com/brotherbeer/mynum/blob/master/mynum.h) and [mynum.cpp](https://github.com/brotherbeer/mynum/blob/master/mynum.cpp) are the essential files the library required, and other files are the expansion of core functions.
Include [mynum.h](https://github.com/brotherbeer/mynum/blob/master/mynum.h) and [mynum.cpp](https://github.com/brotherbeer/mynum/blob/master/mynum.cpp) into your project, and #include "mynum.h" in whichever file you need big integer operations.

[myoperators.h](https://github.com/brotherbeer/mynum/blob/master/operators.h) overloaded the associated C++ operators, if you want to use the C++ operator to complete the calculation of large integers, please #include "myoperators.h"

mynum can also be compiled into a dynamic library, for example:

    `g++ -fPIC -shared -O2 -DNDEBUG mynum.cpp -o mynum.so`

The source code is currently applicable to g++, MSVC (MSVC please select 2008 and above version).

##Initialization
The namespace is `mynum`, and the class name of big integer is `number_t`.
```C++
number_t a = 123, b(789L);  // use basic integer type to initialize number_t object
number_t c("271828182845"); // use a string that represents decimal number
number_t d("abcdef", 16);   // use a string that represents hexadecimal number
number_t e("1111100", 2);   // use a string that represents binary number
number_t f("HIJKLMN", x);   // use a string that represents arbitrary based number, x between [2, 36]
```
The efficiency is highest, when using hexadecimal string, and the max base is 36.

In order to higher efficiency, mynum does not consider any prefix, such as 0x, 0b, and the constructor does not detect any wrong char in the string param. 

If the string parameter is wrong, then the value of object is wrong too, but will not crash.

If you want to test the correctness of the string, you can use the 'check' function, for example:
```C++
const char* s = "1234567890";
assert(check(s, 10) == true); // if s represents a decimal number, then s is correct
assert(check(s, 8) == false); // if s represents an octal number, then s is wrong
```
When the base is greater than 10, 'a' means 10, 'b' means 11, and so on, 'z' means 35, chars are case insensitive.

The value of object defined by the default constructor is 0
```C++
number_t a;
assert(a == 0);   // == defined in myoperators.h
```

The number_t object required memory is allocated on the heap, call the destructor manually to deallocate the memory space
```C++
number_t a(123456LL);
a.~number_t ();
assert (a == 0);
```
