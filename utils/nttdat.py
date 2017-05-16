import os, sys, math, traceback

def primeFactors(P):
    l = []
    t = P / 2 + 1
    while P % 2 == 0:
        l.append(2)
        P /= 2
    r = int(math.sqrt(P))
    for i in xrange(3, r + 1, 2):
        while P and P % i == 0:
            l.append(i)
            P /= i
    if P != 1:
        l.append(P)
    return l

def isPrimitiveRoot(P, g):
    for p in set(primeFactors(P - 1)):
        if pow(g, (P - 1) / p, P) == 1:
            return False
    return True

def gcdExt(a, b):
    if b == 0:
        return a, 1, 0
    d1, x1, y1 = gcdExt(b, a % b)
    d, x, y = d1, y1, x1 - (a / b) * y1
    return d, x, y

def modInv(a, n):
    d, x1, y1 = gcdExt(a, n)
    return (x1 + n / d) % n

def printArray(name, list):
    print 'const dunit_t %s[] = {' % name,
    for i in range(len(list)):
        if i % 4 == 0:
            print '\n   ',
        print '0x%x,' % list[i],
    print '\n};\n'

def printDat(P, n):
    g = 0
    W = []
    RW = []
    RN = []
    for i in range(2, 1024):
        if isPrimitiveRoot(P, i):
            g = i
            break
    if not g:
        print >> sys.stderr, 'Cannot find primitive root'
        quit()

    bits = len(bin(P)[2:])
    base = 1 << bits
    print 'const dunit_t P = 0x%x; // primitive root %d' % (P, g)
    print 'const dunit_t v = 0x%x;\n' % ((base ** 2 - 1) / P - base)
    for i in range(1, n):
        s = 1 << i
        w = pow(g, (P - 1) / s, P)
        rw = modInv(w, P)
        rn = modInv(s, P)
        W.append(w)
        RW.append(rw)
        RN.append(rn)

    printArray('W', W)
    printArray('RW', RW)
    printArray('RN', RN)

def argToInt(arg):
    try:
        if arg.startswith('0x'):
            return int(arg[2:], 16)
        else:
            return int(arg)
    except:
        print >> sys.stderr, traceback.format_exc()
        quit()

if __name__ == '__main__':
    #printDat(0xc0000001, 33)
    #printDat(0xf600000000000001, 33)

    if len(sys.argv) > 1:
        P = argToInt(sys.argv[1])
        if len(sys.argv) > 2:
            n = argToInt(sys.argv[2])
        else:
            n = 25
        printDat(P, n)
    else:
        print 'Usage: %s <prime number> [n = 33]' % os.path.basename(sys.argv[0])
