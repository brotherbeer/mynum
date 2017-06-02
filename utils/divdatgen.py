import sys, os, argparse
from random import *
from datgen import randNumStr, notSameSign

USAGE = '''
%s [-h] [-d MAX_DIGITS_COUNT] [-i ITEMS_COUNT]

This is a tool for generating division test data.
The test data is composed of lines, each line is
made up of the dividend, dividend, quotient and
remainder.
for example:

-e6fb5e59 712f -20a70 -3c9

'-e6fb5e59' is the dividend, '712f' is the divisor,
'-20a70' is the quotient, '-3c9' is the remainder.
All the integers are in base 16.\n
''' % os.path.basename(sys.argv[0])

def mydivmod(a, b):
    q, r = divmod(abs(a), abs(b))
    if a < 0:
        r = -r
    if notSameSign(a, b):
        q = -q
    return q, r

def genTestData(args):
   for i in range(args.items_count):
        if (i + 1) % 500000 == 0:
            print >> sys.stderr, i, 'items generated'

        a = randNumStr(args.max_digits_count, 16, True)
        b = randNumStr(args.max_digits_count, 16, True)
        va = int(a, 16)
        vb = int(b, 16)
        if not vb:
            vb = 1
        q, r = mydivmod(va, vb)
        print '%x %x %x %x' % (va, vb, q, r)

def parseArgs():
    parser = argparse.ArgumentParser(usage = USAGE)
    parser.add_argument('-d', '--max-digits-count', type = int, help='The max digits of each integer', default = 64)
    parser.add_argument('-i', '--items-count', type = int, help = 'How many items should be generated', default = 32)
    args = parser.parse_args()
    if args.max_digits_count == 0:
        args.max_digits_count = 1
    return args
 
if __name__ == '__main__':
    args = parseArgs()
    genTestData(args)
