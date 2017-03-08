import os, sys, glob

def call(cmd):
    print cmd
    os.system(cmd)

if len(sys.argv) == 2 and sys.argv[1] == 'clear':
    for f in glob.glob('*.dat'):
        os.remove(f)

elif len(sys.argv) == 2 and sys.argv[1] == 'small':
    call('python datgen.py -i 30000 -p > randomtest.dat')
    call('python divdatgen.py -i 30000 > divrandomtest.dat')
    call('python pomdatgen.py -i 30000 > pomrandomtest.dat')
    call('python muldatgen.py -i 30000 > mulrandomtest.dat')

else:
    call('python datgen.py -i 100000 > randomtest.dat')
    call('python datgen.py -i 3000 -p >> randomtest.dat')
    call('python datgen.py -i 100000 -d 200 >> randomtest.dat')
    call('python datgen.py -i 3000 -p >> randomtest.dat')
    call('python datgen.py -i 100000 > randomtest.dat')

    call('python divdatgen.py -i 300000 > divrandomtest.dat')
    call('python divdatgen.py -i 300000 -d 200 >> divrandomtest.dat')
    call('python divdatgen.py -i 300000 >> divrandomtest.dat')
    call('python divdatgen.py -i 300000 -d 200 >> divrandomtest.dat')

    call('python pomdatgen.py -i 100000 > pomrandomtest.dat')

    call('python muldatgen.py -i 300000 > mulrandomtest.dat')
    call('python muldatgen.py -i 300000 -d 300 >> mulrandomtest.dat')
    call('python muldatgen.py -i 300000 >> mulrandomtest.dat')
    call('python muldatgen.py -i 300000 -d 400 >> mulrandomtest.dat')
