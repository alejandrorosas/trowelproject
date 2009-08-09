#!/usr/bin/env python

import oscillo, serial, re, sys
from PyQt4 import Qt
import numpy

class SerialReader(oscillo.Reader):
    ser_data = ''
    def __init__(self):
	oscillo.Reader.__init__(self)
	
	self.ser = serial.Serial("/dev/ttyUSB1", baudrate=115200, timeout=0)
	self.ptrn = re.compile("(?P<x>[- ][0-9]{3}), (?P<y>[- ][0-9]{3}), (?P<z>[- ][0-9]{3})\n")
	self.count = 0
	oscillo.REFRESH_RATE = 10
	oscillo.Y_SPAN = 256
	oscillo.X_SPAN = 4
	self.dt = 0.05
    
    def update(self):
	self.ser_data += self.ser.read(128)
	
	while '\n' in self.ser_data:
	    ix = self.ser_data.index('\n')+1
	    line = self.ser_data[:ix]
	    self.ser_data = self.ser_data[ix:]
	    
	    m = self.ptrn.match(line)
	    
	    if m is not None:
		self.t = numpy.append(self.t, self.count * self.dt)
		self.data = numpy.append(self.data, int(m.group('z')))
		self.count += 1
	
	if len(self.t) != 0:
	    return True
	
	return False


def make():
    reader = SerialReader()
    demo = oscillo.Scope(reader)
    demo.resize(500, 300)
    demo.show()
    return demo

# make()


def main(args):
    app = Qt.QApplication(args)
    demo = make()
    sys.exit(app.exec_())

# main()


# Admire
if __name__ == '__main__':
    main(sys.argv)
