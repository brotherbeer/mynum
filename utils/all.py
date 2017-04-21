import os, sys, glob

def call(cmd):
    print cmd
    os.system(cmd)

if len(sys.argv) == 2 and sys.argv[1] == 'clear':
    for f in glob.glob('*.dat'):
        os.remove(f)

elif len(sys.argv) == 2 and sys.argv[1] == 'small':
    call('python datgen.py -i 30000 > randomtest.dat')
    call('python divdatgen.py -i 30000 > divrandomtest.dat')
    call('python pomdatgen.py -i 30000 > pomrandomtest.dat')
    call('python muldatgen.py -i 30000 > mulrandomtest.dat')
    call('python primedatgen.py -i 30000 > primerandomtest.dat')
    call('python gcddatgen.py -i 30000 > gcdrandomtest.dat')
    call('python gcdextdatgen.py -i 30000 > gcdextrandomtest.dat')
    call('python lcmdatgen.py -i 30000 > lcmrandomtest.dat')
    call('python jacobidatgen.py -i 30000 > jacobirandomtest.dat')

else:
    call('python datgen.py -i 100000 > randomtest.dat')
    call('python datgen.py -i 3000 -p >> randomtest.dat')
    call('python datgen.py -i 100000 -d 200 >> randomtest.dat')
    call('python datgen.py -i 3000 -p >> randomtest.dat')
    call('python datgen.py -i 100000 >> randomtest.dat')

    call('python divdatgen.py -i 300000 > divrandomtest.dat')
    call('python divdatgen.py -i 300000 -d 200 >> divrandomtest.dat')
    call('python divdatgen.py -i 300000 >> divrandomtest.dat')
    call('python divdatgen.py -i 300000 -d 200 >> divrandomtest.dat')

    call('python pomdatgen.py -i 300000 > pomrandomtest.dat')

    call('python muldatgen.py -i 300000 > mulrandomtest.dat')
    call('python muldatgen.py -i 300000 -d 400 >> mulrandomtest.dat')

    call('python jacobidatgen.py -i 300000 > jacobirandomtest.dat')
    call('python jacobidatgen.py -i 300000 -d 300 >> jacobirandomtest.dat')

    call('python gcddatgen.py -i 300000 > gcdrandomtest.dat')
    call('python gcddatgen.py -i 300000 -d 300 >> gcdrandomtest.dat')
    call('python gcdextdatgen.py -i 300000 > gcdextrandomtest.dat')
    call('python gcdextdatgen.py -i 300000 -d 300 >> gcdextrandomtest.dat')
    call('python lcmdatgen.py -i 300000 > lcmrandomtest.dat')
    call('python lcmdatgen.py -i 300000 -d 300 >> lcmrandomtest.dat')

    call('python primedatgen.py -i 50000 > primerandomtest.dat')
