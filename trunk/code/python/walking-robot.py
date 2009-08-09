#!/usr/bin/env python

from sensor.wiimote import MyWiimote
from vizual import Body
from visual import *

print 'Put Wiimote in discoverable mode now (press 1+2)...'
wiimote = MyWiimote()
print 'wiimote connected'

body = Body()

ball1 = sphere(pos=(0,0,0), radius=0.5)
ball2 = sphere(pos=(0,0,20), radius=0.5)
ball3 = sphere(pos=(0,0,-20), radius=0.5)

scene.forward=(-1,-1,-1)

while true:
    rate(100)
    if wiimote.get_accel_tuple()[2]<0.4:
        body.walk()


wiimote.close()
