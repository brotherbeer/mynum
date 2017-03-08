import sys, os, argparse
from random import *
from datgen import randNumStr, notSameSign

USAGE = '''
%s [-h] [-d MAX_DIGITS_COUNT] [-i ITEMS_COUNT] [-b BASE]

This is a tool for generating modular exponentiation test data.
The test data is composed of lines, each line is 
made up of the base, exponent, divisor and the result.
for example:

51 1ddb 5 1

'51' is the base, '1ddb' is the exponent, '5' is the divisor,
'1' is the result.
All the integers are in base 16.\n
''' % os.path.basename(sys.argv[0])

def genTestData(args):
   for i in range(args.items_count):
        if (i + 1) % 500000 == 0:
            print >> sys.stderr, i, 'items generated'
        if args.base == 0:
            a, b0 = randNumStr(args.max_digits_count, 16, None)
        else:
            a, b0 = str(args.base), 10
        b, b1 = randNumStr(args.max_digits_count, 16, None)
        c, b2 = randNumStr(args.max_digits_count, 16, None)
        va = int(a, b0)
        vb = int(b, b1)
        vc = int(c, b1)
        if vc == 0:
            continue
        res = pow(va, vb, vc)
        print '%s %s %s %x' % (a, b, c, res)

def parseArgs():
    parser = argparse.ArgumentParser(usage = USAGE)
    parser.add_argument('-b', '--base', type = int, help = 'The base', default = 0)
    parser.add_argument('-d', '--max-digits-count', type = int, help='The max digits of each integer', default = 64)
    parser.add_argument('-i', '--items-count', type = int, help = 'How many items should be generated', default = 32)
    args = parser.parse_args()
    if args.max_digits_count == 0:
        args.max_digits_count = 1
    return args
 
if __name__ == '__main__':
    args = parseArgs()
    genTestData(args)
