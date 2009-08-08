#!/usr/bin/env python

from visual.graph import *
from serial import *
import re

ser = Serial("/dev/ttyUSB1", 115200)
ptrn = re.compile("(?P<x>[- ][0-9]{3}), (?P<y>[- ][0-9]{3}), (?P<z>[- ][0-9]{3})\n")

zgraph = gdisplay(title="Z accel")
zaccel = gcurve(gdisplay = zgraph, color=color.cyan)

data = ''
dt = 0.1
t = 0

while (1):
  data += ser.read()
  
  while '\n' in data:
    ix = data.index('\n')+1
    line = data[:ix]
    data = data[ix:]
    m = ptrn.match(line)
    
    if m is not None:
      z = int(m.group('z'))
      zaccel.plot(pos=(t, z))
      t += dt
