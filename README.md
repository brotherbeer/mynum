![logo](https://github.com/brotherbeer/mydocument/blob/master/mynum/mynum-logo.png?raw=true)

What is mynum? Multiprecision binary number — mynum — is a portable C++ library for big integer arithmetic, it aims to provide convenience for the research of number theory, cryptology and other fields. [\[中文版\]](https://github.com/brotherbeer/mydocument/blob/master/mynum/README-ch.md)

Mynum is currently only available on the LITTLE-ENDIAN machines.

Documents:

 * [Compilation](https://github.com/brotherbeer/mydocument/blob/master/mynum/compilation.md)
 * [Data storage model](https://github.com/brotherbeer/mydocument/blob/master/mynum/Storage.md)
 * [Initialization](https://github.com/brotherbeer/mydocument/blob/master/mynum/Initialization.md)
 * [String conversion](https://github.com/brotherbeer/mydocument/blob/master/mynum/String-conversion.md)
 * [Big integer to ordinary integer](https://github.com/brotherbeer/mydocument/blob/master/mynum/To-basic-integer.md)
 * [Formatted string to big integer](https://github.com/brotherbeer/mydocument/blob/master/mynum/Formatted-input.md)
 * [Big integer to formatted string](https://github.com/brotherbeer/mydocument/blob/master/mynum/Formatted-output.md)
 * [Addition](https://github.com/brotherbeer/mydocument/blob/master/mynum/Addition.md)
 * [Subtraction](https://github.com/brotherbeer/mydocument/blob/master/mynum/Subtraction.md)
 * [Multiplication](https://github.com/brotherbeer/mydocument/blob/master/mynum/Multiplication.md)
 * [Division](https://github.com/brotherbeer/mydocument/blob/master/mynum/Division.md)
 * [Modulo operation](https://github.com/brotherbeer/mydocument/blob/master/mynum/Modulo-operation.md)
 * [Exponentiation](https://github.com/brotherbeer/mydocument/blob/master/mynum/Exponentiation.md)
 * [Modular exponentiation](https://github.com/brotherbeer/mydocument/blob/master/mynum/Modular-exponentiation.md)
 * [Number theoretic functions](https://github.com/brotherbeer/mydocument/blob/master/mynum/Number-theory.md)
 * [Random number functions](https://github.com/brotherbeer/mydocument/blob/master/mynum/Random-number.md)
 * [Comparison](https://github.com/brotherbeer/mydocument/blob/master/mynum/Comparison.md)
 * [Bitwise operation](https://github.com/brotherbeer/mydocument/blob/master/mynum/Bitwise-operation.md)
 * [Other utils](https://github.com/brotherbeer/mydocument/blob/master/mynum/Other-utils.md)

Mynum is a pure C++ library, coupled with the intrinsic technology of modern compiler, ensuring efficient computing and making the interface easy-using as the primary concept.

No restrictions on the dissemination and modification of the source code. The author hopes mynum will be useful, but dose not make any warranty.

[mynumheaderfile]: https://github.com/brotherbeer/mynum/blob/master/mynum.h
[mynumcppfile]: https://github.com/brotherbeer/mynum/blob/master/mynum.cpp
[mytheoryheaderfile]: https://github.com/brotherbeer/mynum/blob/master/mytheory.h
[mytheorycppfile]: https://github.com/brotherbeer/mynum/blob/master/mytheory.cpp
[myoperatorheaderfile]: https://github.com/brotherbeer/mynum/blob/master/myoperators.h
[testcppfile]: https://github.com/brotherbeer/mynum/blob/master/test.cpp

**Installation**

Mynum has 5 files:

 * [mynum.cpp][mynumcppfile] implements the elementary arithmetic and string formatting IO functions, the header file is [mynum.h][mynumheaderfile] 
 * [mytheory.cpp][mytheorycppfile] implements the number theoretic functions, such as GCD function, the header file is [mytheory.h][mytheoryheaderfile] 
 * [myoperators.h][myoperatorheaderfile] overloads the associated C++ operators

Copy these files into your porject, and include "mynum.h", "mytheory.h" or "myoperators.h" in whichever file you need big integer arithmetic, and the namespace is `mynum`.

 * [test.cpp][testcppfile] contains the examples and test cases

The source code is currently applicable to g++, MSVC(2008 and above versions), clang. see [\[the compilation\]](https://github.com/brotherbeer/mydocument/blob/master/mynum/compilation.md)

**Communication**

 * If you **need help**, the author's email is <brotherbeer@163.com>
 * If you **found a bug**, *and can provide steps to reliably reproduce it*, open an issue.
 * If you **have a feature request**, open an issue.
 * If you **want to contribute**, submit a pull request.
