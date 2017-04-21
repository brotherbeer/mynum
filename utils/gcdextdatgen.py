import sys, os, time, argparse
from gmpy2 import gcdext, mpz
from random import *
from datgen import randNumStr

USAGE = '''
%s [-h] [-d MAX_DIGITS_COUNT] [-i ITEMS_COUNT]

This is a tool for generating GCD extension test data.
The test data is composed of lines, each line is
made up of five integers.
for example:

c3 1ff1b05 53dfb -2 27

that means 0xc3 * 0x53dfb + 0x1ff1b05 * -2 = 0x27, and
0x27 is the GCD of 0xc3 and 0x1ff1b05.
All the integers are in base 16.\n
''' % os.path.basename(sys.argv[0])

def genTestData(args):
   for i in range(args.items_count):
        if (i + 1) % 500000 == 0:
            print >> sys.stderr, i, 'items generated'

        a = 0
        b = 0
        while a == 0 and b == 0:
            x = randNumStr(args.max_digits_count / 2, 16)
            y = randNumStr(args.max_digits_count / 2, 16)
            z = randNumStr(args.max_digits_count / 2, 16)
            a = int(x, 16)
            b = int(y, 16)
            c = int(z, 16)
            a *= c
            b *= c
        g, x, y = gcdext(mpz(a), mpz(b))
        print '%x %x %x %x %x' % (a, b, int(x), int(y), int(g))

def parseArgs():
    parser = argparse.ArgumentParser(usage = USAGE)
    parser.add_argument('-d', '--max-digits-count', type = int, help = 'The max digits of each integer', default = 64)
    parser.add_argument('-i', '--items-count', type = int, help = 'How many items should be generated', default = 32)
    args = parser.parse_args()
    if args.max_digits_count == 0:
        args.max_digits_count = 1
    return args
 
if __name__ == '__main__':
    args = parseArgs()
    genTestData(args)
