import sys, os, time, argparse
from gmpy2 import lcm, mpz
from random import *
from datgen import randNumStr

USAGE = '''
%s [-h] [-d MAX_DIGITS_COUNT] [-i ITEMS_COUNT]

This is a tool for generating LCM test data.
The test data is composed of lines, each line is
made up of two integers and their lowest common
multiple.
for example:

cc7de9c 3eb10 1235c66cc50

All the integers are in base 16.\n
''' % os.path.basename(sys.argv[0])

def genTestData(args):
   for i in range(args.items_count):
        if (i + 1) % 500000 == 0:
            print >> sys.stderr, i, 'items generated'

        x = randNumStr(args.max_digits_count / 2, 16)
        y = randNumStr(args.max_digits_count / 2, 16)
        z = randNumStr(args.max_digits_count / 2, 16)
        a = int(x, 16)
        b = int(y, 16)
        l = int(lcm(mpz(a), mpz(b)))
        print '%x %x %x' % (a, b, l)

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
