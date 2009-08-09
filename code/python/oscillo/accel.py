#!/usr/bin/env python

import oscillo, serial, re, sys
from PyQt4 import Qt
import numpy

class SerialReader(oscillo.ScopeReader):
  
    count = 0
    dt = 0.05
    ser_data = ''
    
    def __init__(self):
        self.ser = serial.Serial("/dev/ttyUSB1", baudrate=115200, timeout=0)
        self.ptrn = re.compile("(?P<x>[- ][0-9]{3}), (?P<y>[- ][0-9]{3}), (?P<z>[- ][0-9]{3})\n")
        
        oscillo.REFRESH_RATE = 10
        oscillo.Y_SPAN = 5
        oscillo.X_SPAN = 10
    
    def refresh(self):
        t = numpy.array([])
        x = numpy.array([])
        y = numpy.array([])
        z = numpy.array([])
        
        self.ser_data += self.ser.read(128)
        #~ print self.ser_data
        
        while '\n' in self.ser_data:
            ix = self.ser_data.index('\n')+1
            line = self.ser_data[:ix]
            self.ser_data = self.ser_data[ix:]
            
            m = self.ptrn.match(line)
            
            if m is not None:
              t = numpy.append(t, self.count * self.dt)
              x = numpy.append(x, float(m.group('x'))/50)
              y = numpy.append(y, float(m.group('y'))/50)
              z = numpy.append(z, float(m.group('z'))/50)
              self.count += 1
        
        #~ print (t, x, y, z)
        return (t, x, y, z)
        


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
