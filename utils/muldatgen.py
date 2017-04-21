import sys, os, argparse
from random import *
from datgen import randNumStr, notSameSign

USAGE = '''
%s [-h] [-d MAX_DIGITS_COUNT] [-i ITEMS_COUNT]

This is a tool for generating multiplication test data.
The test data is composed of lines, each line is
made up of 2 factors and their product.
for example:

92f b2ae0 668ebf20

'92f' is the first factor, 'b2ae0' is the second factor,
'668ebf20' is the product. All the integers are in base 16.\n
''' % os.path.basename(sys.argv[0])

def genTestData(args):
   for i in range(args.items_count):
        if (i + 1) % 500000 == 0:
            print >> sys.stderr, i, 'items generated'

        a = randNumStr(args.max_digits_count, 16, True)
        b = randNumStr(args.max_digits_count, 16, True)
        va = int(a, 16)
        vb = int(b, 16)
        if randint(0, 50) == 1:
            b = a
            vb = va
        p = va * vb
        print '%s %s %x' % (a, b, p)

def parseArgs():
    parser = argparse.ArgumentParser(usage = USAGE)
    parser.add_argument('-d', '--max-digits-count', type = int, help='The max digits of each integer', default = 500)
    parser.add_argument('-i', '--items-count', type = int, help = 'How many items should be generated', default = 32)
    args = parser.parse_args()
    if args.max_digits_count == 0:
        args.max_digits_count = 1
    return args
 
if __name__ == '__main__':
    args = parseArgs()
    genTestData(args)
