import sys, os, time, argparse
from gmpy2 import is_prime, next_prime, mpz
from random import *
from datgen import randNumStr

USAGE = '''
%s [-h] [-d MAX_DIGITS_COUNT] [-i ITEMS_COUNT]

This is a tool for generating prime test data.
The test data is composed of lines, each line is
made up of an integer and a '0' or '1' which indicates
whether the integer is a prime.
for example:

4de1 1
4de2 0

All the integers are in base 16.\n
''' % os.path.basename(sys.argv[0])

def genTestData(args):
   for i in range(args.items_count):
        if (i + 1) % 500000 == 0:
            print >> sys.stderr, i, 'items generated'
        x = randNumStr(args.max_digits_count, 16)
        a = mpz(int(x, 16))
        a = next_prime(a)
        if not randint(0, 4):
            a += 2
        b = is_prime(a)
        print '%x %x' % (int(a), b)

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
