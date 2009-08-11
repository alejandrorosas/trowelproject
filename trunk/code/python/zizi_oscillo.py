#!/usr/bin/env python
from PyQt4 import Qt
from sensor import zizi
import sys, oscillo.ScopeWidget as scope , numpy

class ZiziReader(scope.SensorReader):
    
    def __init__(self):
        self.zizi = zizi.Zizi("/dev/ttyUSB1")
        self.zizi.start()
        self.data_reset()
        self.zizi.set_callback(self.new_data)
    
    def data_reset(self):
        self.data = {'t':numpy.array([]), 'x':numpy.array([]), 'y':numpy.array([]), 'z':numpy.array([])}
    
    def new_data(self, dict):
        self.data['t'] = numpy.append(self.data['t'], dict['t'])
        self.data['x'] = numpy.append(self.data['x'], dict['x'])
        self.data['y'] = numpy.append(self.data['y'], dict['y'])
        self.data['z'] = numpy.append(self.data['z'], dict['z'])
        
    def get_new_data(self):
        d = self.data
        self.data_reset()
        return (d['t'], d['x'])
        
    def terminate(self):
        self.zizi.terminate()

if __name__ == '__main__':
    app = Qt.QApplication(sys.argv)
    
    reader = ZiziReader()
    demo = scope.Scope(reader=reader)
    demo.resize(500, 300)
    demo.show()
    
    ret = app.exec_()
    reader.terminate()
    sys.exit(ret)
