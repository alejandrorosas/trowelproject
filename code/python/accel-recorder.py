#!/usr/bin/env python
from sensor.wiimote import *
import sys,time

from sensor import wiimote

reader = wiimote.WiimoteReader()

c='a'
while c<>'q':
    print "Tapez q<ENTREE> pour quitter"
    c = sys.stdin.read(1)

reader.terminate()

data = reader.get_new_data()

f = open("result.csv",'w')
csv=u"t;raw ax;raw ay;raw az\n"
f.write(csv)

for i in range(len(data['t'])):
    #~ print "%f:"%t, d
    csv=("%f"%data['t'][i]).replace(".",",")
    csv+=u';'
    csv+=("%d"%data['x'][i]).replace(".",",")
    csv+=u';'
    csv+=("%d"%data['y'][i]).replace(".",",")
    csv+=u';'
    csv+=("%d"%data['z'][i]).replace(".",",")
    csv+=u"\n"
    f.write(csv)

f.close()
