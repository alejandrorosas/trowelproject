#!/usr/bin/env python
from wiimote import MyWiimote
import cwiid,sys,time

print 'Put Wiimote in discoverable mode now (press 1+2)...'
wiimote = MyWiimote()
print 'wiimote connected'

for a in range(3, -1, -1):
	time.sleep(1)
	print 'starting in %i s'%a

data=[]
time_init=time.time()

def callback(msgs):
	for msg in msgs:
		if msg[0]==cwiid.MESG_ACC:
			data.append((time.time()-time_init,msg[1]))
			#print "callback"

wiimote.set_msg_callback(callback)

c='a'
while c<>'q':
    print "Tapez q<ENTREE> pour quitter"
    c = sys.stdin.read(1)

print len(data)
wiimote.close()

f = open("result.csv",'w')
csv=u"t;raw ax;raw ay;raw az\n"
f.write(csv)

for t,r in data:
	#~ print "%f:"%t, d
	csv=("%f"%t).replace(".",",")
	csv+=u';'
	csv+=("%d"%r[0]).replace(".",",")
	csv+=u';'
	csv+=("%d"%r[1]).replace(".",",")
	csv+=u';'
	csv+=("%d"%r[2]).replace(".",",")
	csv+=u"\n"
	f.write(csv)

f.close()
