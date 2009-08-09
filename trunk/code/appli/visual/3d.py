#!/usr/bin/env python

from visual import *
from serial import *
import re
import time

scene.forward = (-1,-1,-1)

axes = frame()
ax = arrow(frame=axes, pos=(0,0,0), axis=(1,0,0), color=(1,0,0), shaftwidth = 0.02)
ay = arrow(frame=axes, pos=(0,0,0), axis=(0,1,0), color=(0,1,0), shaftwidth = 0.02)
az = arrow(frame=axes, pos=(0,0,0), axis=(0,0,1), color=(0,0,1), shaftwidth = 0.02)

ax_text = label(frame=axes, pos=(1.1,0,0), text="x", color=(1,0,0))
ay_text = label(frame=axes, pos=(0,1.1,0), text="y", color=(0,1,0))
az_text = label(frame=axes, pos=(0,0,1.1), text="z", color=(0,0,1))

accel = arrow(pos=(0,0,0), axis=(0, 0.5, 0), color=(1,1,0))
#~ 
ser = Serial("/dev/ttyUSB1", 115200)
ptrn = re.compile("(?P<x>[- ][0-9]{3}), (?P<y>[- ][0-9]{3}), (?P<z>[- ][0-9]{3})\n")

data = ''

while (1):
  data += ser.read()
  
  while '\n' in data:
    ix = data.index('\n')+1
    line = data[:ix]
    data = data[ix:]
    m = ptrn.match(line)
    
    if m is not None:
      v = vector(int(m.group('x')), int(m.group('z')), -int(m.group('y')))
      v.x *= -1
      v.z *= -1
      v.mag = 1
      accel.axis = v
