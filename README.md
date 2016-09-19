MYNUM LIBARAY
-------------

This C++ library provides operations of big integers, include:

 * [Initialization](#initialization)
 * [Addition](#addition)
 * [Subtraction](#subtraction)
 * [Multiplication](#multiplication)
 * [Division](#division)
 * [Modulo](#modulo)
 * [Power](#power)
 * [Root](#root)
 * [Comparison](#comparison)
 * [Bits operations](#bits-operations)
 * [String convertion](#string-convertion)

and other utilities:

 * [abs](#abs)
 * [neg](#neg)
 * [to_string](#to_string)

No restrictions on the dissemination and modification of this source code,
but should accord with socially beneficial purpose. 
This library is distributed in the hope that it will be useful, but without any warranty,
if you have any questions, please contact <brotherbeer@163.com>

`NOTICE!! This library is currently only available on the LITTLE-ENDIAN machines.`

##Addition
```C++
number_t a,b,c;
c = a + b;
c += a;
c = a + 6;
c += 6;
```
##Subtraction
```C++
number_t a,b,c;
c = a - b;
c -= a;
```
##Multiplication
```C++
number_t a,b,c;
c = a * b;
c *= a;
c = a * 6;
c *= 6;
```
##Bits operations
```C++
number_t a,b,c;
a << 3;
b >> 4;
```
