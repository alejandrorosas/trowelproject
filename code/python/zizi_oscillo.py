#!/usr/bin/env python
from PyQt4 import Qt
import sys
from sensor import zizi
import oscillo

class ZiziReader(oscillo.SensorReader):
    def __init__(self):
        self.zizi = zizi.Zizi("/dev/ttyUSB1")
        self.zizi.start()
        
        oscillo.REFRESH_RATE = 10
        oscillo.Y_SPAN = 5
        oscillo.X_SPAN = 10
        
    def get_new_data(self):
        tu = self.zizi.get_data()
        return {'t':tu[0], 'x':tu[1], 'y':tu[2], 'z':tu[3]}
        
    def terminate(self):
        self.zizi.terminate()

if __name__ == '__main__':
    app = Qt.QApplication(sys.argv)
    
    reader = ZiziReader()
    demo = oscillo.Scope(reader)
    demo.resize(500, 300)
    demo.show()
    
    ret = app.exec_()
    reader.terminate()
    sys.exit(ret)
