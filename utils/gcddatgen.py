import sys, os, time, argparse
from gmpy2 import gcd, mpz
from random import *
from datgen import randNumStr

USAGE = '''
%s [-h] [-d MAX_DIGITS_COUNT] [-i ITEMS_COUNT]

This is a tool for generating GCD test data.
The test data is composed of lines, each line is
made up of two integers and their greatest common
divisor.
for example:

3b9cae3 ae3c08d9d 1

All the integers are in base 16.\n
''' % os.path.basename(sys.argv[0])

def genTestData(args):
   for i in range(args.items_count):
        if (i + 1) % 500000 == 0:
            print >> sys.stderr, i, 'items generated'
        x, b0 = randNumStr(args.max_digits_count / 2, 16)
        y, b1 = randNumStr(args.max_digits_count / 2, 16)
        z, b2 = randNumStr(args.max_digits_count / 2, 16)
        a = int(x, 16)
        b = int(y, 16)
        c = int(z, 16)
        a *= c
        b *= c
        g = int(gcd(mpz(a), mpz(b)))
        print '%x %x %x' % (a, b, g)

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
