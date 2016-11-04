import sys, argparse
from random import *

USAGE = '''
%s [-h] [-d MAX_DIGITS_COUNT] [-i ITEMS_COUNT]

This is a tool for generating test data for MYNUM.
The test data is composed of some test items, one item
is made up of an operator, two integers and a result.
for example:

+ 123 10 111001000 2 579

'+' is the operator, '123' is first integer based '10',
'111001000' is the second integer based '2', '579' is the
result of 123(10) + 111001000(2)\n
''' % sys.argv[0]

digits = '0123456789abcdefghijklmnopqrstuvwxyz'
opers = [
    '+', '-', '*', '/',
    '+', '-', '*', '/',
    '+', '-', '*', '/',
    '+', '-', '*', '/',
    '&', '|', '^',
    '&', '|', '^',
    '&', '|', '^',
    '==', '!=', '>=', '<=', '>', '<',
    '+s32', '+u32', '+s64', '+u64',
    '-s32', '-u32', '-s64', '-u64',
    '*s32', '*u32', '*s64', '*u64',
    '/s32', '/u32', '/s64', '/u64',
    '%s32', '%u32', '%s64', '%u64',
    '&s32', '&u32', '&s64', '&u64',
    '|s32', '|u32', '|s64', '|u64',
    '^s32', '^u32', '^s64', '^u64',
]

signs = ['-', '', ]

INT_MIN = (-2147483647 - 1)
INT_MAX = 2147483647
UINT_MAX = 0xffffffff
LLONG_MAX = 9223372036854775807
LLONG_MIN = (-9223372036854775807 - 1)
ULLONG_MAX = 0xffffffffffffffff

def randNumStr(dmax):
    b = randint(2, 36)
    n = randint(0, dmax)
    s = ['0'] * n if n > 0 else ['0']
    for i in range(n): s[i] = choice(digits[0: b])
    return choice(signs) + ''.join(s), b

def notSameSign(va, vb):
    sa = 1 if va >= 0 else -1
    sb = 1 if vb >= 0 else -1
    return sa != sb

def mydiv(va, vb):
    res = abs(va) / abs(vb)
    if notSameSign(va, vb): res *= -1
    return res

def mymod(va, vb):
    res = abs(va) / abs(vb)
    res = abs(va) - res * abs(vb)
    if va < 0:
        res *= -1
    return res

def myand(va, vb):
    res = abs(va) & abs(vb)
    if notSameSign(va, vb): res *= -1
    return res

def myor(va, vb):
    res = abs(va) | abs(vb)
    if notSameSign(va, vb): res *= -1
    return res

def myxor(va, vb):
    res = abs(va) ^ abs(vb)
    if notSameSign(va, vb): res *= -1
    return res

def myshl(va, vb):
    res = abs(va) << vb
    if va < 0: res *= -1
    return res

def myshr(va, vb):
    res = abs(va) >> vb
    if va < 0: res *= -1
    return res

def genTestData(args):
   for i in range(args.items_count):
        oper = choice(opers)
        a, b0 = randNumStr(args.max_digits_count)
        b, b1 = randNumStr(args.max_digits_count)
        va = int(a, b0)
        vb = int(b, b1)

        if oper == '+':
            res = va + vb

        elif oper == '-':
            res = va - vb

        elif oper == '*':
            res = va * vb

        elif oper == '/': 
            if not vb: continue
            res = mydiv(va, vb)

        elif oper == '%':
            if not vb: continue
            res = mymod(va, vb)

        elif oper == '&':
            res = myand(va, vb)

        elif oper == '|':
            res = myor(va, vb)

        elif oper == '^':
            res = myxor(va, vb)

        elif oper == '==':
            res = int(va == vb)

        elif oper == '!=':
            res = int(va != vb)

        elif oper == '>=':
            res = int(va >= vb)

        elif oper == '<=':
            res = int(va <= vb)

        elif oper == '<':
            res = int(va < vb)

        elif oper == '>':
            res = int(va > vb)

        elif oper == '+s32':
            vb = randint(INT_MIN, INT_MAX)
            b, b1 = str(vb), 10
            res = va + vb            

        elif oper == '+u32':
            vb = randint(0, UINT_MAX)
            b, b1 = str(vb), 10
            res = va + vb

        elif oper == '+s64':
            vb = randint(LLONG_MIN, LLONG_MAX)
            b, b1 = str(vb), 10
            res = va + vb   

        elif oper == '+u64':
            vb = randint(0, ULLONG_MAX)
            b, b1 = str(vb), 10
            res = va + vb   

        elif oper == '-s32':
            vb = randint(INT_MIN, INT_MAX)
            b, b1 = str(vb), 10
            res = va - vb   

        elif oper == '-u32':
            vb = randint(0, UINT_MAX)
            b, b1 = str(vb), 10
            res = va - vb

        elif oper == '-s64':
            vb = randint(LLONG_MIN, LLONG_MAX)
            b, b1 = str(vb), 10
            res = va - vb   

        elif oper == '-u64':
            vb = randint(0, ULLONG_MAX)
            b, b1 = str(vb), 10
            res = va - vb

        elif oper == '*s32':
            vb = randint(INT_MIN, INT_MAX)
            b, b1 = str(vb), 10
            res = va * vb

        elif oper == '*u32':
            vb = randint(0, UINT_MAX)
            b, b1 = str(vb), 10
            res = va * vb

        elif oper == '*s64':
            vb = randint(LLONG_MIN, LLONG_MAX)
            b, b1 = str(vb), 10
            res = va * vb

        elif oper == '*u64':
            vb = randint(0, ULLONG_MAX)
            b, b1 = str(vb), 10
            res = va * vb

        elif oper == '/s32':
            vb = randint(INT_MIN, INT_MAX)
            if not vb: continue
            b, b1 = str(vb), 10
            res = mydiv(va, vb)

        elif oper == '/u32':
            vb = randint(0, UINT_MAX)
            if not vb: continue
            b, b1 = str(vb), 10
            res = mydiv(va, vb)

        elif oper == '/s64':
            vb = randint(LLONG_MIN, LLONG_MAX)
            if not vb: continue
            b, b1 = str(vb), 10
            res = mydiv(va, vb)

        elif oper == '/u64':
            vb = randint(0, ULLONG_MAX)
            if not vb: continue
            b, b1 = str(vb), 10
            res = mydiv(va, vb)

        elif oper == '%s32':
            vb = randint(INT_MIN, INT_MAX)
            if not vb: continue
            b, b1 = str(vb), 10
            res = mymod(va, vb)

        elif oper == '%u32':
            vb = randint(0, UINT_MAX)
            if not vb: continue
            b, b1 = str(vb), 10
            res = mymod(va, vb)

        elif oper == '%s64':
            vb = randint(LLONG_MIN, LLONG_MAX)
            if not vb: continue
            b, b1 = str(vb), 10
            res = mymod(va, vb)

        elif oper == '%u64':
            vb = randint(0, ULLONG_MAX)
            if not vb: continue
            b, b1 = str(vb), 10
            res = mymod(va, vb)

        elif oper == '&s32':
            vb = randint(INT_MIN, INT_MAX)
            b, b1 = str(vb), 10
            res = myand(va, vb)

        elif oper == '&u32':
            vb = randint(0, UINT_MAX)
            b, b1 = str(vb), 10
            res = myand(va, vb)

        elif oper == '&s64':
            vb = randint(LLONG_MIN, LLONG_MAX)
            b, b1 = str(vb), 10
            res = myand(va, vb)

        elif oper == '&u64':
            vb = randint(0, ULLONG_MAX)
            b, b1 = str(vb), 10
            res = myand(va, vb)

        elif oper == '|s32':
            vb = randint(INT_MIN, INT_MAX)
            b, b1 = str(vb), 10
            res = myor(va, vb)

        elif oper == '|u32':
            vb = randint(0, UINT_MAX)
            b, b1 = str(vb), 10
            res = myor(va, vb)

        elif oper == '|s64':
            vb = randint(LLONG_MIN, LLONG_MAX)
            b, b1 = str(vb), 10
            res = myor(va, vb)

        elif oper == '|u64':
            vb = randint(0, ULLONG_MAX)
            b, b1 = str(vb), 10
            res = myor(va, vb)

        elif oper == '^s32':
            vb = randint(INT_MIN, INT_MAX)
            b, b1 = str(vb), 10
            res = myxor(va, vb)

        elif oper == '^u32':
            vb = randint(0, UINT_MAX)
            b, b1 = str(vb), 10
            res = myxor(va, vb)

        elif oper == '^s64':
            vb = randint(LLONG_MIN, LLONG_MAX)
            b, b1 = str(vb), 10
            res = myxor(va, vb)

        elif oper == '^u64':
            vb = randint(0, ULLONG_MAX)
            b, b1 = str(vb), 10
            res = myxor(va, vb)

        print '%s %s %s %s %s %s' % (oper, a, b0, b, b1, res)

def parseArgs():
    parser = argparse.ArgumentParser(usage = USAGE)
    parser.add_argument('-d', '--max-digits-count', type = int, help='The max digits of each integer', default = 64)
    parser.add_argument('-i', '--items-count', type = int, help = 'How many items should be generated', default = 32)
    args = parser.parse_args()
    return args
 
if __name__ == '__main__':
    genTestData(parseArgs())