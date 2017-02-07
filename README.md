![logo](https://github.com/brotherbeer/mydocument/blob/master/mynum/mynum-logo.png?raw=true)

Mynum is a portable library for big integer arithmetic, it aims to provide convenience for the research of number theory, cryptology and other fields. [\[中文版\]](https://github.com/brotherbeer/mydocument/blob/master/mynum/README-ch.md)

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
 * [Modular Exponentiation](https://github.com/brotherbeer/mydocument/blob/master/mynum/Modular-exponentiation.md)
 * [Comparison](https://github.com/brotherbeer/mydocument/blob/master/mynum/Comparison.md)
 * [Bitwise operation](https://github.com/brotherbeer/mydocument/blob/master/mynum/Bitwise-operation.md)
 * [Other-utils](https://github.com/brotherbeer/mydocument/blob/master/mynum/Other-utils.md)

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

Include [mynum.h][mynumheaderfile] and [mynum.cpp][mynumcppfile] into your project, and `#include "mynum.h"` in whichever file you need big integer arithmetic, and the namespace is `mynum`.

[myoperators.h][myoperatorheaderfile] overloaded the associated C++ operators, if you want to use the operators to do the calculations, please `#include "myoperators.h"`

[test.cpp][testcppfile] contained the examples and test cases

The source code is currently applicable to g++, MSVC(2008 and above versions), clang. see [\[the compilation\]](https://github.com/brotherbeer/mydocument/blob/master/mynum/compilation.md)

##Communication
 * If you **need help**, my email is <brotherbeer@163.com>
 * If you **found a bug**, *and can provide steps to reliably reproduce it*, open an issue.
 * If you **have a feature request**, open an issue.
 * If you **want to contribute**, submit a pull request.
